#include "controllers/me_ct_physics_controller.h"
#include <sr/sr_euler.h>

#include <sb/SBSkeleton.h>
#include <sb/SBEvent.h>
#include <sb/SBScene.h>
#include <sb/SBSceneListener.h>

std::string MeCtPhysicsController::CONTROLLER_TYPE = "PhysicsController";

MeCtPhysicsController::MeCtPhysicsController(SbmCharacter* character) : SmartBody::SBController()
{
	_character = character;
	_skeletonCopy = new SmartBody::SBSkeleton();
	_skeletonCopy->copy(_character->getSkeleton());
	_valid = true;
	_prevTime = 0.0;
	_duration = 0.0;
}

MeCtPhysicsController::~MeCtPhysicsController()
{
	if (_skeletonCopy)
		delete _skeletonCopy;	
}


void MeCtPhysicsController::getJointChannelValues( const std::string& jointName, MeFrameData& frame, SrQuat& outQuat, SrVec& outPos )
{
	bool hasRotation = true;
	int channelId = _context->channels().search(jointName, SkChannel::Quat);
	if (channelId < 0)	hasRotation = false;
	int bufferId = frame.toBufferIndex(channelId);
	if (bufferId < 0)	hasRotation = false;	

	bool hasTranslation = true;
	int positionChannelID = _context->channels().search(jointName, SkChannel::XPos);
	if (positionChannelID < 0) hasTranslation = false;
	int posBufferID = frame.toBufferIndex(positionChannelID);
	if (posBufferID < 0) hasTranslation = false;		
	// input reference pose
	if (hasRotation)
	{
		outQuat.w = frame.buffer()[bufferId + 0];
		outQuat.x = frame.buffer()[bufferId + 1];
		outQuat.y = frame.buffer()[bufferId + 2];
		outQuat.z = frame.buffer()[bufferId + 3];
	}
	if (hasTranslation)
	{
		outPos.x = frame.buffer()[posBufferID + 0];
		outPos.y = frame.buffer()[posBufferID + 1];
		outPos.z = frame.buffer()[posBufferID + 2];				
	}
}

bool MeCtPhysicsController::controller_evaluate(double t, MeFrameData& frame)
{
#ifndef SB_NO_ODE_PHYSICS	
	if (_prevTime == 0)
		_dt = 0.016;
	else
	{
		_dt = t - _prevTime;
		_prevTime = t;
	}	
	float invDt = 1.f/0.016;
	if (_dt > 1e-6)
		invDt = 1.f/(float)_dt;	
#if 1
	if (_valid && _context )
	{
		SmartBody::SBPhysicsSim* phyEngine = SmartBody::SBPhysicsSim::getPhysicsEngine();
		std::string charName = _character->getName();
		SmartBody::SBPhysicsCharacter* phyChar = phyEngine->getPhysicsCharacter(charName);//_character->getPhysicsCharacter();
		if (!phyChar) return true;

		bool hasPhy = (phyEngine->getBoolAttribute("enable") && phyChar->getBoolAttribute("enable"));		
		std::vector<SmartBody::SbmJointObj*> jointObjList = phyChar->getJointObjList();

		std::vector<SmartBody::CollisionRecord>& colRecords = phyChar->getCollisionRecords();
		static bool hasGaze = false;
		static bool hasPD = true;
		static float fadeOutTime = 0.f;
		if (colRecords.size() > 0 && !hasGaze)
		{
			SmartBody::CollisionRecord& col = colRecords[0];
			SrVec hitPos = col.collisionPt;
			char eventMsg[256];
			
			//start collision event
			//sprintf(eventMsg,"start %s %f %f %f",_character->getName().c_str(),hitPos[0],hitPos[1],hitPos[2]);
			SmartBody::SBJoint* hitJoint = col.hitJointObj->getSBJoint();
			sprintf(eventMsg,"start %s %s %s %f %f %f %f",_character->getName().c_str(),col.collider->getName().c_str(),hitJoint->jointName().c_str(),hitPos[0],hitPos[1],hitPos[2], col.momentum.len());			
			LOG("eventMsg = %s",eventMsg);
			std::string cmd = eventMsg;
			SmartBody::SBMotionEvent motionEvent;
			std::string eventType = "collision";
			motionEvent.setType(eventType);			
			motionEvent.setParameters(cmd);
			SmartBody::SBEventManager* manager = SmartBody::SBScene::getScene()->getEventManager();		
			manager->handleEvent(&motionEvent,t);
			colRecords.clear();
			hasGaze = true;
			fadeOutTime = (float)t+2.f;
		}
		else if (hasGaze && t > fadeOutTime)
		{
			// end collision event
			char eventMsg[256];
			sprintf(eventMsg,"end %s",_character->getName().c_str());
// 			if (!hasPD)
// 				phyChar->setBoolAttribute("usePD",false);
			std::string cmd = eventMsg;
			SmartBody::SBMotionEvent motionEvent;
			std::string eventType = "collision";
			motionEvent.setType(eventType);						
			motionEvent.setParameters(cmd);
			SmartBody::SBEventManager* manager = SmartBody::SBScene::getScene()->getEventManager();	
			manager->handleEvent(&motionEvent,t);
			hasGaze = false;
			hasPD = true;
			fadeOutTime = 0.f;
		}
		else
		{
			colRecords.clear();
		}

		// update world-offset
		std::string wname = "world_offset";
		SrQuat wquat;
		SrVec wpos;
		getJointChannelValues(wname,frame,wquat,wpos);

		SmartBody::SBJoint* wjoint = _skeletonCopy->getJointByName(wname);
		if (wjoint)
		{
			if (wjoint->pos())
			{					
				wjoint->pos()->value(0,wpos[0]);
				wjoint->pos()->value(1,wpos[1]);
				wjoint->pos()->value(2,wpos[2]);
			}
			if (wjoint->quat())
			{					
				wjoint->quat()->value(wquat);					
			}
		}		

		// recompute joint torque 
		// update physics simulation results to character channel arrays
		for (unsigned int i=0;i<jointObjList.size();i++)
		{
			SmartBody::SbmJointObj* obj = jointObjList[i];
			SmartBody::SBPhysicsJoint* phyJoint = obj->getPhyJoint();
			SmartBody::SBJoint* joint = obj->getPhyJoint()->getSBJoint();
			if (!joint)	continue;
			
			SrMat tran = obj->getRelativeOrientation();				
			std::string jname = joint->getMappedJointName();
			SrQuat inQuat;
			SrVec inPos;
			getJointChannelValues(jname,frame,inQuat,inPos);
			
			
			SmartBody::SBJoint* jointCopy = _skeletonCopy->getJointByName(jname);
			if (jointCopy)
			{
				if (jointCopy->pos())
				{					
					jointCopy->pos()->value(0,inPos[0]);
					jointCopy->pos()->value(1,inPos[1]);
					jointCopy->pos()->value(2,inPos[2]);
				}
				if (jointCopy->quat())
				{					
					jointCopy->quat()->value(inQuat);					
				}
			}

			bool kinematicRoot = (joint->getMappedJointName() == "base" || joint->getMappedJointName() == "JtPelvis") && phyChar->getBoolAttribute("kinematicRoot");
			if (kinematicRoot)
				continue;

			
			SrMat preRot;
			joint->quat()->prerot().inverse().get_mat(preRot);

			SrQuat oldPhyQuat = SrQuat(tran);
			SrQuat phyQuat = SrQuat(tran*preRot);
			
			
			SrQuat newQuat = (joint->quat()->prerot()*inQuat);

			
			SrMat newMat; newQuat.get_mat(newMat);						
			SrQuat pQuat;			
			if (joint->getParent())
			{
				pQuat = SrQuat(joint->getParent()->getMatrixGlobal());
				newMat = newMat*joint->getParent()->getMatrixGlobal();
			}
			
			SrQuat prevQuat = phyJoint->getRefQuat();
 			SrQuat quatDiff = (inQuat*prevQuat.inverse());
			quatDiff.normalize();
			SrVec angDiff = quatDiff.axisAngle();
 			angDiff = angDiff*prevQuat*pQuat;
 			angDiff = angDiff*invDt;	


			SrQuat pQuatPhy;			
			if (obj->getParentObj())
				pQuatPhy = obj->getParentObj()->getGlobalTransform().rot;

			SrQuat newGQuat = SrQuat(newMat);			
			//LOG("joint name = %s",joint->getName().c_str());
			//sr_out << "gmat Quat = " << SrQuat(joint->getMatrixGlobal()) << srnl;
			//sr_out << "old phyQuat = " << pQuatPhy*oldPhyQuat << "  , newQuat = " << newQuat*pQuat << srnl;
			//sr_out << "new GQuat = " << newGQuat << srnl;
			
			//LOG("joint name = %s",joint->getName().c_str());
			//sr_out << "angVel = " << angDiff << srnl;
 			//phyJoint->setRefAngularVel(angDiff);			
			phyJoint->setRefQuat(inQuat);
			if (hasPhy)
			{				
				int channelId = _context->channels().search(jname, SkChannel::Quat);
				if (channelId < 0)	continue;
				int bufferId = frame.toBufferIndex(channelId);
				if (bufferId < 0)	continue;	

				frame.buffer()[bufferId + 0] = phyQuat.w;
				frame.buffer()[bufferId + 1] = phyQuat.x;
				frame.buffer()[bufferId + 2] = phyQuat.y;
				frame.buffer()[bufferId + 3] = phyQuat.z;

				int positionChannelID = _context->channels().search(jname, SkChannel::XPos);
				if (positionChannelID < 0) continue;
				int posBufferID = frame.toBufferIndex(positionChannelID);
				if (posBufferID < 0)	continue;
				SrVec pos = tran.get_translation() - joint->offset(); // remove the joint offset in local space to get actual Pos channel values
				if (!obj->getParentObj())
				{
					frame.buffer()[posBufferID + 0] = pos.x;
					frame.buffer()[posBufferID + 1] = pos.y;
					frame.buffer()[posBufferID + 2] = pos.z;	
				}
				//LOG("joint name = %s",joint->getName().c_str());
				//sr_out << "Pos = " << pos << srnl;				
			}
			else
			{
				SBTransform diffSRT = SBTransform::diff(obj->getGlobalTransform(),obj->getRefTransform());
				SrVec refAngVel = diffSRT.rot.axisAngle()*obj->getGlobalTransform().rot;
				refAngVel = refAngVel*invDt;
				SrVec refLinearVel = diffSRT.tran*invDt;
				obj->setVec3Attribute("refLinearVelocity",refLinearVel[0],refLinearVel[1],refLinearVel[2]);
				obj->setVec3Attribute("refAngularVelocity",refAngVel[0],refAngVel[1],refAngVel[2]);
				obj->setLinearVel(refLinearVel);
				obj->setAngularVel(refAngVel);
			}
		}

		_skeletonCopy->update_global_matrices();
		for (unsigned int i=0;i<jointObjList.size();i++)
		{
			SmartBody::SbmJointObj* obj = jointObjList[i];
			SmartBody::SBPhysicsJoint* phyJoint = obj->getPhyJoint();
			SmartBody::SBJoint* sbJoint = phyJoint->getSBJoint();
			SmartBody::SBJoint* joint = NULL; 
			if (sbJoint)
				joint = _skeletonCopy->getJointByName(sbJoint->getMappedJointName());
			if (joint)
			{
				obj->setRefTransform(joint->gmat());
			}
		}
	}	
#endif
#endif
	return true;
}

