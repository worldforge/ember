#include "controllers/me_ct_hand.hpp"
#include <sb/SBScene.h>
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <boost/foreach.hpp>
#include <sb/sbm_character.hpp>
#include <sb/SBReach.h>

#include "controllers/MeCtReachEngine.h"
using namespace boost;



/************************************************************************/
/* FingerChain                                                          */
/************************************************************************/

void FingerChain::init( MeCtIKTreeNode* figTip )
{
	fingerTip = figTip;
	MeCtIKTreeNode* node = fingerTip;
	fingerNodes.clear();
	while (node->parent != NULL)
	{
		fingerNodes.push_back(node);
		node = node->parent;
	}
	//fingerQuats.resize(fingerNodes.size());	
}

void FingerChain::unlockChain()
{
	for (unsigned int i=0;i<fingerNodes.size();i++)
	{
		fingerNodes[i]->lock = false;
	}	
}


void FingerChain::getLineSeg( std::vector<SrVec>& lineSeg )
{
	lineSeg.clear();
	for (unsigned int i=0;i<fingerNodes.size();i++)
	{
		lineSeg.push_back(fingerNodes[i]->gmat.get_translation());		
	}
}

void FingerChain::testCollision( SBGeomObject* colObj )
{
	std::vector<SrVec> lineSeg;
	getLineSeg(lineSeg);
	SBGeomNullObject* colNullObj = dynamic_cast<SBGeomNullObject*>(colObj);
	if (lineSeg.size() < 2 || !colObj || colNullObj)
	{
		isLock = true;
		return; // no line
	}

	//printf("line seg size = %d\n",lineSeg.size());
	for (unsigned int i=1;i<lineSeg.size();i++)
	{
		bool isIntersect = colObj->isIntersect(lineSeg[i-1],lineSeg[i],0.001f);
		if (isIntersect)
		{			
			fingerNodes[i]->lock = true;
			isLock = true;
		}
	}
}
/************************************************************************/
/* Hand Controller                                                      */
/************************************************************************/

std::string MeCtHand::CONTROLLER_TYPE = "Hand";

MeCtHand::MeCtHand( SmartBody::SBSkeleton* sk, SmartBody::SBJoint* wrist)
{		
	skeletonRef  = sk;	
	// work on the copy of skeleton to avoid problems
	skeletonCopy = new SmartBody::SBSkeleton(sk); 	
	if (wrist)
	{
		wristJoint = wrist;//skeletonCopy->search_joint(wrist->name().get_string());
	}	
	grabTargetName = "";
	attachedPawnName = "";
	currentGrabState = GRAB_RETURN;
	grabSpeed = 50.f;
	jointSpeed = 20.f;
	_duration = -1.f;	
}

MeCtHand::~MeCtHand( void )
{
	if (skeletonCopy)
		delete skeletonCopy;
}

SbmPawn* MeCtHand::getAttachedPawn()
{
	
	SbmPawn* attachedPawn =  SmartBody::SBScene::getScene()->getPawn(attachedPawnName);	
	if (attachedPawnName != "" && !attachedPawn) // pawn is removed
	{
		releasePawn();
		setGrabState(GRAB_RETURN);
	}
	return attachedPawn;
}

SbmPawn* MeCtHand::getTargetObject()
{
	
	SbmPawn* targetObject =  SmartBody::SBScene::getScene()->getPawn(grabTargetName);
	if (grabTargetName != "" && !targetObject) // pawn is removed
		grabTargetName = "";
	return targetObject;
}

void MeCtHand::attachPawnTarget( SbmPawn* pawn, std::string jointName )
{	
	SkJoint* attachJoint = skeletonRef->search_joint(jointName.c_str());
	if (!attachJoint)
		return;
	//printf("attach pawn\n");
	attachJointName = jointName;
	attachedPawnName = pawn->getName();
	SbmPawn* attachedPawn = getAttachedPawn();
	if (!attachedPawn) // if the pawn does not exist
	{
		releasePawn();		
		return;
	}
	//attachMat = attachedPawn->get_world_offset_joint()->gmat()*attachJoint->gmat().inverse();	
	skeletonRef->update_global_matrices();
	attachMat = attachedPawn->get_world_offset()*attachJoint->gmat().inverse();	
}

void MeCtHand::releasePawn()
{
	attachedPawnName = "";
	attachMat = SrMat();
	attachJointName = "";
}

void MeCtHand::updateAttachedPawn()
{
	SkJoint* attachJoint = skeletonRef->search_joint(attachJointName.c_str());
	SbmPawn* attachedPawn = getAttachedPawn();
	if (!attachJoint || !attachedPawn)
		return;
	//printf("update pawn\n");
	SrMat effectorWorld = attachJoint->gmat();// motionParameter->getMotionFrameJoint(ikMotionFrame,reachEndEffector->name().get_string())->gmat();
	SrMat newWorld = attachMat*effectorWorld;
	SrVec pos = newWorld.get_translation();
	//LOG("attached pawn world pos = %f %f %f",pos.x,pos.y,pos.z);
	attachedPawn->setWorldOffset(newWorld);
	attachedPawn->updateToColObject();
}

void MeCtHand::setGrabState( GrabState state )
{
	if (currentGrabState != state)
	{
		for (int i=0;i<fingerChains.size();i++)
		{
			FingerChain& fig = fingerChains[i];
			fig.isLock = false;		
			fig.unlockChain();
		}
	}
	currentGrabState = state;
	//LOG("current grab state = %d",currentGrabState);
}

void MeCtHand::setGrabTargetObject( SbmPawn* targetObj )
{	
	grabTargetName = targetObj->getName();
	//setGrabState(GRAB_START);	
}



void MeCtHand::init(std::string grabType, const MotionDataSet& reachPose, const MotionDataSet& grabPose, const MotionDataSet& releasePose, const MotionDataSet& pointPose)
{
	int type = MeCtReachEngine::getReachType(grabType);
	if (type == -1)
		type = MeCtReachEngine::RIGHT_ARM;

	this->grabType = type;
	std::vector<std::string> stopJoints;
	ikScenario.buildIKTreeFromJointRoot(wristJoint,stopJoints);
	fingerChains.resize(MeCtHand::F_NUM_FINGERS);
	for (unsigned int i=0;i<ikScenario.ikTreeNodes.size();i++)
	{
		MeCtIKTreeNode* node = ikScenario.ikTreeNodes[i];
		if (!node->child)
		{
			FingerID fID = findFingerID(node->getNodeName().c_str());
			//LOG("finger name = %s\n",node->nodeName.c_str());
			FingerChain& fchain = fingerChains[fID];
			fchain.init(node);			
			// add constraint
			EffectorConstantConstraint* cons = new EffectorConstantConstraint();
			cons->efffectorName = node->getNodeName();
			cons->rootName = wristJoint->jointName();//"r_shoulder";//rootJoint->name().get_string();		
			handPosConstraint[cons->efffectorName] = cons;
		}
	}

	const IKTreeNodeList& nodeList = ikScenario.ikTreeNodes;		
	releaseFrame.jointQuat.resize(nodeList.size());
	currentFrame = releaseFrame;
	reachFrame = releaseFrame;
	grabFrame  = releaseFrame;
	tempFrame    = releaseFrame;
	inputFrame = releaseFrame;
	pointFrame = releaseFrame;
	affectedJoints.clear();	
	for (unsigned int i=0;i<nodeList.size();i++)
	{
		MeCtIKTreeNode* node = nodeList[i];
		SmartBody::SBJoint* joint = skeletonCopy->getJointByName(node->getNodeName());
		SkJointQuat* skQuat = joint->quat();		
		affectedJoints.push_back(joint);	
		_channels.add(joint->getMappedJointName(), SkChannel::Quat);		
	}	
	SmartBody::SBMotion *releaseHand, *grabHand, *reachHand, *pointHand;
	releaseHand = dynamic_cast<SmartBody::SBMotion*>(SmartBody::SBReach::findTagMotion(type,releasePose));
	grabHand = dynamic_cast<SmartBody::SBMotion*>(SmartBody::SBReach::findTagMotion(type,grabPose));
	reachHand = dynamic_cast<SmartBody::SBMotion*>(SmartBody::SBReach::findTagMotion(type,reachPose));
	pointHand = dynamic_cast<SmartBody::SBMotion*>(SmartBody::SBReach::findTagMotion(type, pointPose));	

	bool useRetarget = true;
	if (releaseHand)
		releaseFrame.setMotionPose((float)releaseHand->time_stroke_emphasis(),skeletonCopy,affectedJoints,releaseHand,useRetarget);
	if (grabHand)
		grabFrame.setMotionPose((float)grabHand->time_stroke_emphasis(),skeletonCopy,affectedJoints,grabHand,useRetarget);
	if (reachHand)
		reachFrame.setMotionPose((float)reachHand->time_stroke_emphasis(),skeletonCopy,affectedJoints,reachHand,useRetarget);
	if (pointHand)
		pointFrame.setMotionPose((float)pointHand->time_stroke_emphasis(),skeletonCopy,affectedJoints,pointHand,useRetarget);
//	if (releaseHand && grabHand && reachHand)
//	{
		//printf("set example hand pose\n");		
		//releaseFrame.setMotionPose((float)releaseHand->time_stroke_emphasis(),skeletonCopy,affectedJoints,releaseHand);
		//grabFrame.setMotionPose((float)grabHand->time_stroke_emphasis(),skeletonCopy,affectedJoints,grabHand);
		//reachFrame.setMotionPose((float)reachHand->time_stroke_emphasis(),skeletonCopy,affectedJoints,reachHand);
		//pointFrame.setMotionPose((float)pointHand->time_stroke_emphasis(),skeletonCopy,affectedJoints,pointHand);

		
//	}
// 	else
// 	{
// 		SrVec vec(-8,-6,0);
// 		getPinchFrame(grabFrame,vec);
// 	}
	grabTargetName = "";//NULL;//new ColSphere(); // hard coded to sphere for now		
}

void MeCtHand::getPinchFrame( BodyMotionFrame& pinchFrame, SrVec& wristOffset )
{
	ikScenario.updateNodeGlobalMat(ikScenario.ikTreeRoot,QUAT_INIT);	
	SrVec wristPos = ikScenario.ikTreeRoot->gmat.get_translation();
	SrVec ikTarget = wristPos + wristOffset*ikScenario.ikTreeRoot->gmat.get_rotation();
	for (int i=0;i<fingerChains.size();i++)
	{		
		FingerChain& fig = fingerChains[i];
		MeCtIKTreeNode* node = fig.fingerTip;
		if (!node) continue;
		EffectorConstantConstraint* cons = dynamic_cast<EffectorConstantConstraint*>(handPosConstraint[node->getNodeName()]);
		cons->targetPos = ikTarget;		
	}
	for (int i=0;i<100;i++)
		solveIK(1.f);
	ikScenario.getTreeNodeQuat(pinchFrame.jointQuat,QUAT_CUR);
}

void MeCtHand::solveIK(float dt)
{
	ikScenario.setTreeNodeQuat(releaseFrame.jointQuat,QUAT_REF);								
	ikScenario.ikPosEffectors = &handPosConstraint;
	ikScenario.ikRotEffectors = &handRotConstraint;
	ik.maxOffset = grabSpeed*dt;
	ik.dampJ = 0.5f;
	ik.refDampRatio = 0.1;
	for (int i=0;i<1;i++)
	{
		ik.update(&ikScenario);		
		ikScenario.copyTreeNodeQuat(QUAT_CUR,QUAT_INIT);		
	}
}

BodyMotionFrame& MeCtHand::findTargetFrame( GrabState state )
{
	if (state == GRAB_START)
	{
		//LOG("grabStartFrame");
		return grabFrame;
	}
	else if (state == GRAB_REACH)
	{
		//LOG("grabReachFrame");
		return grabFrame;
	}
	else if (state == GRAB_FINISH)
	{
		return reachFrame;
	}
	else if (state == GRAB_RETURN)
	{
		//return releaseFrame;
		return inputFrame;
	}
	else if (state == GRAB_POINT)
	{
		return pointFrame;
	}
	return reachFrame;
}

bool MeCtHand::controller_evaluate( double t, MeFrameData& frame )
{	
	//float dt = 0.001f;
	//LOG("MeCtHand::controller_evaluate");
	float du = 0.0;
	if (prev_time == -1.f) // first start
	{
		//dt = 0.001f;
		updateChannelBuffer(frame,currentFrame,true);
	}

	updateChannelBuffer(frame,inputFrame,true);
	updateDt((float)t);

	static bool bInit = false;
	if (!bInit)
	{			
		ikScenario.setTreeNodeQuat(currentFrame.jointQuat,QUAT_INIT);
		ikScenario.setTreeNodeQuat(currentFrame.jointQuat,QUAT_CUR);		
		bInit = true;
	}		

	//LOG("hand controller, t = %f",t);
	
	updateChannelBuffer(frame,tempFrame,true);
	currentFrame.jointQuat[0] = tempFrame.jointQuat[0];
	BodyMotionFrame& curTargetFrame = findTargetFrame(currentGrabState);//currentGrabState == GRAB_START ? grabFrame : reachFrame;

	ikScenario.setTreeNodeQuat(currentFrame.jointQuat,QUAT_CUR);
	ikScenario.ikTreeRoot->lock = true;

	float maxAngleDelta = sr_torad(grabSpeed*dt);		
	
	//if (maxAngleDelta > 0.01)
	//LOG("maxAngleDelta = %f",maxAngleDelta);
	//LOG("handControl grabSpeed = %f",grabSpeed);
	updateFingerChains(curTargetFrame,maxAngleDelta);
	//updateFingerChains(curTargetFrame,0.01f);

	skeletonRef->invalidate_global_matrices();
	skeletonRef->update_global_matrices();
	updateAttachedPawn();	
	std::string rootName = ikScenario.ikTreeRoot->joint->parent()->jointName();
	ikScenario.ikGlobalMat = skeletonRef->search_joint(rootName.c_str())->gmat();
	ikScenario.updateNodeGlobalMat(ikScenario.ikTreeRoot,QUAT_CUR);


	SbmPawn* grabTarget = getTargetObject();
	if (grabTarget && grabTarget->getGeomObject())
	{
		SBGeomObject* geomObj = grabTarget->getGeomObject();
		for (int i=0;i<fingerChains.size();i++)
		{
			FingerChain& fig = fingerChains[i];
			MeCtIKTreeNode* node = fig.fingerTip;				
			//SrVec curPos = node->gmat.get_translation();	
			if (currentGrabState == GRAB_REACH || currentGrabState == GRAB_START)
			{
				fig.testCollision(geomObj); // test collision
			}
	// 		std::vector<SrVec> chainSeg;
	// 		fig.getLineSeg(chainSeg);
	// 		if (!fig.isLock && currentGrabState == GRAB_START && grabTarget->isCollided(chainSeg))		
	// 		{			
	// 			fig.isLock = true;				
	// 		}			
		}
	}
	
	
	//BodyMotionFrame outMotionFrame = curTargetFrame;
	ikScenario.getTreeNodeQuat(currentFrame.jointQuat,QUAT_CUR);
	updateChannelBuffer(frame,currentFrame);
	return true;
}

void MeCtHand::updateFingerChains( BodyMotionFrame& targetMotionFrame, float maxAngDelta )
{	
	for (unsigned int i=0;i<fingerChains.size();i++)
	{
		FingerChain& fig = fingerChains[i];		
		bool bStop = false;
// 		if (fig.isLock)
// 			continue;
		for (unsigned int k=0;k<fig.fingerNodes.size() && !bStop;k++)
		{			
			MeCtIKTreeNode* node = fig.fingerNodes[k];
			if (node->lock) 
			{				
				bStop = true;
				continue;
			}
			SrQuat q = node->getQuat();
			SrQuat qT = targetMotionFrame.jointQuat[node->nodeIdx];
			SrQuat diff = qT*q.inverse();
			diff.normalize();
			float angle = diff.angle();
			if (angle > maxAngDelta)
				angle = maxAngDelta;
			SrQuat newQ;
			if (fabs(angle) < maxAngDelta)
				newQ = qT;
			else
				newQ = SrQuat(diff.axis(),angle)*q;
			//newQ.normalize();
			node->setQuat(newQ);
		}
	}	
}
void MeCtHand::updateChannelBuffer( MeFrameData& frame, BodyMotionFrame& handMotionFrame, bool bRead /*= false*/ )
{
	SrBuffer<float>& buffer = frame.buffer();
	int count = 0;
	// update root translation
	if (handMotionFrame.jointQuat.size() != affectedJoints.size())
		handMotionFrame.jointQuat.resize(affectedJoints.size());
	//BOOST_FOREACH(SrQuat& quat, motionFrame.jointQuat)
	for (unsigned int i=0;i<handMotionFrame.jointQuat.size();i++)
	{
		SrQuat& quat = handMotionFrame.jointQuat[i];		
		int index = frame.toBufferIndex(_toContextCh[count++]);	
		//printf("buffer index = %d\n",index);	
		if (index == -1)
		{
			if (bRead)
			{
				quat = SrQuat();
			}
		}
		else
		{			
			if (bRead)
			{
				quat.w = buffer[index] ;
				quat.x = buffer[index + 1] ;
				quat.y = buffer[index + 2] ;
				quat.z = buffer[index + 3] ;			
			}
			else 
			{
				buffer[index] = quat.w;
				buffer[index + 1] = quat.x;
				buffer[index + 2] = quat.y;
				buffer[index + 3] = quat.z;			
			}			
		}				
	}
}


void MeCtHand::print_state( int tabs )
{

}

void MeCtHand::controller_start()
{
	controlRestart();
}

void MeCtHand::controller_map_updated()
{

}

MeCtHand::FingerID MeCtHand::findFingerID( const char* jointName )
{
	const char fingerName[][20] = {"thumb", "index", "middle", "ring", "pinky"};
	std::string jointStr = jointName;
	for (int i=0;i<5;i++)
	{
		size_t found = jointStr.find(fingerName[i]);
		if (found != std::string::npos)
			return (MeCtHand::FingerID)i;
	}
	return MeCtHand::F_THUMB;
}
