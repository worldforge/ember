#include <sr/sr_euler.h>
#include "controllers/me_ct_motion_parameter.h"

MotionParameter::MotionParameter(SmartBody::SBSkeleton* skel, std::vector<SmartBody::SBJoint*>& joints)
{
	skeletonRef = skel;
	affectedJoints = joints;
}

MotionParameter::~MotionParameter(void)
{

}

SkJoint* MotionParameter::getMotionFrameJoint( const BodyMotionFrame& frame, const char* jointName )
{
	SkJoint* outJoint = NULL;	
	for (int i=0;i<3;i++)
	{		
		//skeletonRef->->pos()->value(i,frame.rootPos[i]);
		affectedJoints[0]->pos()->value(i,frame.rootPos[i]);
	}			
	for (unsigned int i=0;i<affectedJoints.size();i++)
	{
		SkJoint* joint = affectedJoints[i];				
		joint->quat()->value(frame.jointQuat[i]);
		joint->update_lmat();
		if (strcmp(joint->getMappedJointName().c_str(),jointName) == 0)
			outJoint = joint;
	}			
	skeletonRef->invalidate_global_matrices();
	skeletonRef->update_global_matrices();
	return outJoint;
}

ReachMotionParameter::ReachMotionParameter( SmartBody::SBSkeleton* skel, std::vector<SmartBody::SBJoint*>& joints, SmartBody::SBJoint* rjoint, SmartBody::SBJoint* root ) : MotionParameter(skel,joints)
{
	reachJoint = rjoint;	
	rootJoint = root;
}

ReachMotionParameter::~ReachMotionParameter()
{

}

void ReachMotionParameter::getPoseParameter( const BodyMotionFrame& frame, dVector& outPara )
{
	// set root 		
	SkJoint* rJoint = getMotionFrameJoint(frame,reachJoint->getMappedJointName().c_str());	
	rJoint->update_lmat();
	rJoint->update_gmat_up();
	skeletonRef->update_global_matrices();
	//printf("reach joint name = %s\n",reachJoint->name().get_string());
	SrMat ginv = skeletonRef->search_joint(rootJoint->getMappedJointName().c_str())->gmat().inverse();
	SrVec endPos = rJoint->gmat().get_translation()*ginv;
	outPara.resize(3);
	for (int i=0;i<3;i++)
		outPara[i] = endPos[i];	
}

void ReachMotionParameter::getMotionParameter(BodyMotionInterface* motion, dVector& outPara )
{
	double timeRef = motion->strokeEmphasisTime();//motion->motionDuration(BodyMotionInterface::DURATION_REF)*0.999;
	//printf("timeRef = %f\n",timeRef);
	getMotionFrameParameter(motion,(float)timeRef,outPara);	
}

void ReachMotionParameter::getMotionFrameParameter( BodyMotionInterface* motion, float refTime, dVector& outPara )
{
	BodyMotionFrame tempFrame;
	motion->getMotionFrame((float)refTime,skeletonRef,affectedJoints,tempFrame);
// 	int noffset = 30;
// 	for (int i=noffset;i<5+noffset;i++)
// 		sr_out << "motion frame quat " << i+noffset << " : " << tempFrame.jointQuat[i+noffset] << srnl;
	getPoseParameter(tempFrame,outPara);	
}

LocomotionParameter::LocomotionParameter( SmartBody::SBSkeleton* skel, std::vector<SmartBody::SBJoint*>& joints, const std::string& baseName ): MotionParameter(skel,joints)
{
	baseJointName = baseName;
}

LocomotionParameter::~LocomotionParameter()
{

}

void LocomotionParameter::getMotionParameter( BodyMotionInterface* motion, dVector& outPara )
{
	std::string targetJointName = "base";
	float speed = getMotionSpeed(motion,targetJointName);
	float angVel = getMotionAngularSpeed(motion,targetJointName);
	float strafeVel = getMotionSpeedAxis(motion, "X", targetJointName);
	outPara.resize(3);
	outPara(0) = speed;
	outPara(1) = angVel;
	outPara(2) = strafeVel;
	//LOG("para = %f %f %f", outPara(0),outPara(1),outPara(2));
}

void LocomotionParameter::getMotionFrameParameter( BodyMotionInterface* motion, float refTime, dVector& outPara )
{
	// do nothing
}

void LocomotionParameter::getPoseParameter( const BodyMotionFrame& frame, dVector& outPara )
{
	// do nothing
}

float LocomotionParameter::getMotionSpeed( BodyMotionInterface* motion, const std::string& jointName )
{
	float dt = 0.1f;	
	SrVec prevPos;
	BodyMotionFrame tempFrame;
	float distance = 0.f;
	float du = 0.03f;
#if 1
	int nSegment = 5;
	du = float(motion->motionDuration(BodyMotionInterface::DURATION_REF)/nSegment);
	//du = 0.03f;
	for (float refTime = 0.f ; refTime <= motion->motionDuration(BodyMotionInterface::DURATION_REF); refTime+= du)
	{	
		// 		motion->getMotionFrame((float)refTime,skeletonRef,affectedJoints,tempFrame);
		// 		SkJoint* rJoint = getMotionFrameJoint(tempFrame,jointName.c_str());	
		// 		rJoint->update_lmat();
		// 		rJoint->update_gmat_up();		
		// 		//printf("reach joint name = %s\n",reachJoint->name().get_string());
		// 		SrVec destPt = rJoint->gmat().get_translation();
		SrVec destPt = motion->getMotionBaseTranslation(refTime, jointName);
		if (refTime > 0.f)
		{
			distance += (destPt - prevPos).norm();
		}
		prevPos = destPt;
		// 		if (refTime > 0.f)
		// 		{
		// 			distance += (destPt - prevPos).norm();
		// 		}
		// 		prevPos = destPt;
		// 		du = (float)motion->getRefDeltaTime(refTime, dt);
	}
#endif
	// 	SrVec startPos = motion->getMotionBaseTranslation(0.f, jointName);
	// 	SrVec endPos = motion->getMotionBaseTranslation((float)motion->motionDuration(BodyMotionInterface::DURATION_REF), jointName);
	// 	distance = (endPos - startPos).norm();
	float speed = distance/(float)motion->motionDuration(BodyMotionInterface::DURATION_ACTUAL);
	return speed;
}



float LocomotionParameter::getMotionSpeedAxis( BodyMotionInterface* motion, const std::string& axis, const std::string& jointName )
{
	float dt = 0.1f;	
	SrVec prevPos;
	BodyMotionFrame tempFrame;
	float distance = 0.f;
	float du = 0.03f;

	int axisIndex = 0;
	if (axis == "X" || axis == "x")
		axisIndex = 0;
	else if (axis == "Y" || axis == "y")
		axisIndex = 1;
	else if (axis == "Z" || axis == "z")
		axisIndex = 2;
	else
	{
		LOG("Bad axis specified '%s', defaulting to use the X-axis.", axis.c_str());
	}
#if 1
	int nSegment = 5;
	du = float(motion->motionDuration(BodyMotionInterface::DURATION_REF)/nSegment);
	//du = 0.03f;
	for (float refTime = 0.f ; refTime <= motion->motionDuration(BodyMotionInterface::DURATION_REF); refTime+= du)
	{	
		// 		motion->getMotionFrame((float)refTime,skeletonRef,affectedJoints,tempFrame);
		// 		SkJoint* rJoint = getMotionFrameJoint(tempFrame,jointName.c_str());	
		// 		rJoint->update_lmat();
		// 		rJoint->update_gmat_up();		
		// 		//printf("reach joint name = %s\n",reachJoint->name().get_string());
		SrVec destPt = motion->getMotionBaseTranslation(refTime, jointName);
		if (refTime > 0.f)
		{
			distance += destPt[axisIndex] - prevPos[axisIndex];
		}
		prevPos = destPt;
		// 		du = (float)motion->getRefDeltaTime(refTime, dt);
	}
#endif
	// 	SrVec startPos = motion->getMotionBaseTranslation(0.f, jointName);
	// 	SrVec endPos = motion->getMotionBaseTranslation((float)motion->motionDuration(BodyMotionInterface::DURATION_REF), jointName);
	// 	distance = (endPos[axisIndex] - startPos[axisIndex]);
	float speed = distance/(float)motion->motionDuration(BodyMotionInterface::DURATION_ACTUAL);
	return speed;
}

float LocomotionParameter::getMotionAngularSpeed( BodyMotionInterface* motion, const std::string& jointName )
{
	float dt = 0.1f;	
	float prevRotY = 0.f;
	BodyMotionFrame tempFrame;
	float rotYDiff = 0.f;
	float du = 0.1f;
	int nSegment = 5;
	const int rotType = 132;
	du = float(motion->motionDuration(BodyMotionInterface::DURATION_REF)/nSegment);
	//du = 0.03f;
#if 1
	for (float refTime = 0.f ; refTime <= motion->motionDuration(BodyMotionInterface::DURATION_REF); refTime+= du)
	{	
		//motion->getMotionFrame((float)refTime,skeletonRef,affectedJoints,tempFrame);
		//SkJoint* rJoint = getMotionFrameJoint(tempFrame,jointName.c_str());	
		//rJoint->update_lmat();
		//rJoint->update_gmat_up();		
		SrQuat q = motion->getMotionBaseRotation((float)refTime, jointName);
		//printf("reach joint name = %s\n",reachJoint->name().get_string());
		float rx, ry, rz;
		SrMat rotMat;
		rotMat = q.get_mat(rotMat);
		sr_euler_angles(rotType, rotMat, rx, ry, rz);		
		if (refTime > 0.f)
		{
			float diff;
			float destRotY = ry;		
			if (destRotY * prevRotY < 0 && fabs(destRotY) > 1.0f)
				diff = - destRotY - prevRotY;
			else
				diff = destRotY - prevRotY;
			rotYDiff += diff;
		}		
		prevRotY = ry;				
	}
#endif	

	float speed = rotYDiff/(float)motion->motionDuration(BodyMotionInterface::DURATION_ACTUAL);
	speed *= (180.0f/ float(M_PI));
	return speed;
}

/************************************************************************/
/* Jump Parameter                                                       */
/************************************************************************/
JumpParameter::JumpParameter( SmartBody::SBSkeleton* skel, std::vector<SmartBody::SBJoint*>& joints, const std::string& baseName ): MotionParameter(skel,joints)
{
	baseJointName = baseName;
}

JumpParameter::~JumpParameter()
{

}

void JumpParameter::getPoseParameter( const BodyMotionFrame& frame, dVector& outPara )
{
	// do nothing
}

void JumpParameter::getMotionFrameParameter( BodyMotionInterface* motion, float refTime, dVector& outPara )
{
	// do nothing
}

void JumpParameter::getMotionParameter( BodyMotionInterface* motion, dVector& outPara )
{
	std::string targetJointName = "base";
	float distance = getMotionXZDistance(motion,targetJointName);
	float height = getMotionHeight(motion,targetJointName);
	float angle = getMotionDirection(motion, targetJointName);
	outPara.resize(3);
	outPara(0) = distance;
	outPara(1) = height;
	outPara(2) = angle;
	//LOG("distance = %f, height = %f, angle = %f",distance,height, angle);
}

float JumpParameter::getMotionHeight( BodyMotionInterface* motion, const std::string& jointName )
{
	double timeRef = motion->strokeEmphasisTime();//motion->motionDuration(BodyMotionInterface::DURATION_REF)*0.999;
	//printf("timeRef = %f\n",timeRef);
	//getMotionFrameParameter(motion,(float)timeRef,outPara);	
	SrVec apexPos = motion->getMotionBaseTranslation((float)timeRef, jointName);
	return apexPos[1];
}

float JumpParameter::getMotionDirection( BodyMotionInterface* motion, const std::string& jointName )
{
	double timeStart = 0.0;
	double timeEnd = motion->motionDuration(BodyMotionInterface::DURATION_REF);
	SrVec startPos = motion->getMotionBaseTranslation((float)timeStart, jointName);
	SrVec endPos = motion->getMotionBaseTranslation((float)timeEnd, jointName);
	SrVec dir = endPos - startPos; dir.y = 0.f; dir.normalize();
	SrVec ang = cross(SrVec(0,0,1), dir);
	//LOG("cross = %f %f %f",ang[0],ang[1],ang[2]);
	float rotAngle = asinf(ang.y);	
	return rotAngle;
}

float JumpParameter::getMotionXZDistance( BodyMotionInterface* motion, const std::string& jointName )
{
	double timeStart = 0.0;
	double timeEnd = motion->motionDuration(BodyMotionInterface::DURATION_REF);
	SrVec startPos = motion->getMotionBaseTranslation((float)timeStart, jointName);
	SrVec endPos = motion->getMotionBaseTranslation((float)timeEnd, jointName);
	SrVec dir = endPos - startPos; dir.y = 0.f;	
	return dir.norm();
}