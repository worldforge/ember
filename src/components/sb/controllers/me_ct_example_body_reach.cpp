#include <assert.h>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include "components/sb/sr/sr_timer.h"

#include "components/sb/sb/SBScene.h"
#include "components/sb/sb/SBCommandManager.h"
#include "components/sb/sb/SBEvent.h"
#include "components/sb/sb/sbm_character.hpp"
#include "components/sb/sb/SBCharacter.h"
#include "me_ct_example_body_reach.hpp"
#include "me_ct_barycentric_interpolation.h"

#include "MeCtBodyReachState.h"
#include "me_ct_example_body_reach.hpp"

using namespace boost;


/************************************************************************/
/* Exampled-Based Reach Controller                                      */
/************************************************************************/

std::string MeCtExampleBodyReach::CONTROLLER_TYPE = "BodyReach";

MeCtExampleBodyReach::MeCtExampleBodyReach( std::map<int,MeCtReachEngine*>& reMap, int reachType)  : SmartBody::SBController()
{
	currentReachData = NULL;
	currentReachEngine = NULL;

	_duration = -1.f;	
	footIKFix = true;
	useProfileInterpolation = false;
	useLocomotion = false;
	useRetiming = false;
	isMoving = false;
	startReach = false;
	endReach = false;
	autoReturnDuration = -1.f;
	defaultReachType = reachType;
	reachVelocityScale = 1.f; 
	desireLinearVel = -1.f;
	desireGrabSpeed = -1.f;
	setDefaultAttributeGroupPriority("Reaching", 220);
	addDefaultAttributeFloat("reach.autoReturnDuration",-1.f, "Reaching", &autoReturnDuration);
	addDefaultAttributeFloat("reach.velocityScale",1.f,"Reaching", &reachVelocityScale);
	addDefaultAttributeBool("reach.footIK",true, "Reaching", &footIKFix);
	addDefaultAttributeBool("reach.useProfileInterpolation",false, "Reaching", &useProfileInterpolation);
	addDefaultAttributeBool("reach.useRetiming",false, "Reaching", &useRetiming);

	reachEngineMap = reMap;
	ReachEngineMap::iterator mi;
	for (mi  = reachEngineMap.begin();
		mi != reachEngineMap.end();
		mi++)
	{
		MeCtReachEngine* re = mi->second;
		if (re)
		{
			re->getReachData()->reachControl = this;
		}
	}
	//if (reachEngineMap.size() > 0)
	if (defaultReachType != -1 && reachEngineMap.find(defaultReachType) != reachEngineMap.end())
	{

		currentReachEngine = reachEngineMap[defaultReachType];
		currentReachData = currentReachEngine->getReachData();
	}	
	else if (reachEngineMap.size() > 0)
	{
		currentReachEngine = reachEngineMap[MeCtReachEngine::RIGHT_ARM];
		currentReachData = currentReachEngine->getReachData();
	}
}

MeCtExampleBodyReach::~MeCtExampleBodyReach( void )
{
	// remove any registered subjects
	if (currentReachData)
	{
		ReachTarget& t = currentReachData->reachTarget;
		SbmPawn* oldPawn = t.getTargetPawn();
		/*
		if (oldPawn)
		{
			// remove any notifications for existing target pawns
			oldPawn->unregisterObserver(this);
			t.setTargetPawn(NULL);
			t.useTargetPawn = false;
		}
		*/
	}
}

void MeCtExampleBodyReach::setDefaultReachType( const std::string& reachTypeName )
{
	defaultReachType = MeCtReachEngine::getReachType(reachTypeName);	
//	LOG("default reach type name = %s, id =  %d",reachTypeName.c_str(), defaultReachType);
}

void MeCtExampleBodyReach::setHandActionState( MeCtReachEngine::HandActionState newState )
{
	//curHandActionState = newState;	
	currentReachEngine->curHandActionState = newState;
}

MeCtReachEngine::HandActionState MeCtExampleBodyReach::getHandActionState()
{
	return currentReachEngine->curHandActionState;
}


void MeCtExampleBodyReach::setReachCompleteDuration( float duration )
{
	//reachData->autoReturnTime = duration;
	autoReturnDuration = duration;
}

void MeCtExampleBodyReach::setFinishReaching( bool isFinish )
{
	//currentReachData->endReach = isFinish;
	endReach = isFinish;
}

void MeCtExampleBodyReach::setFootIK( bool useIK )
{
	//reachEngine->footIKFix = useIK;
	footIKFix = useIK;
}

void MeCtExampleBodyReach::setUseLocomotion( bool useLoco )
{
	useLocomotion = useLoco;
}

void MeCtExampleBodyReach::setLinearVelocity( float vel )
{
	//currentReachData->linearVel = vel;
	desireLinearVel = vel;
}

void MeCtExampleBodyReach::setGrabSpeed( float vel )
{
	//currentReachData->linearVel = vel;
	desireGrabSpeed = vel;
}

bool MeCtExampleBodyReach::addHandConstraint( SkJoint* targetJoint, const char* effectorName )
{
	return currentReachEngine->addHandConstraint(dynamic_cast<SmartBody::SBJoint*>(targetJoint),effectorName);
}

SbmPawn* MeCtExampleBodyReach::getAttachedPawn()
{
	if (currentReachData->hasAttachedPawn())
		return currentReachData->effectorState.getAttachedPawn();
	else
		return NULL;
}

void MeCtExampleBodyReach::setReachTargetPawn( SbmPawn* targetPawn )
{
	//reachTargetPawn = targetPawn;	
	ReachTarget& t = currentReachData->reachTarget;
	/*
	SbmPawn* oldPawn = t.getTargetPawn();
	if (oldPawn)
	{
		// remove any notifications for existing target pawns
		oldPawn->unregisterObserver(this);
	}
	*/
	EffectorState& estate = currentReachData->effectorState;

	t.setTargetPawnName(targetPawn->getName());
	/*
	if (targetPawn)
	{
		// add a notifications for existing target pawn
		targetPawn->registerObserver(this);
	}
	*/
	//currentReachData->startReach = true;	
	startReach = true;
}

void MeCtExampleBodyReach::setReachTargetJoint( SkJoint* targetJoint )
{
	ReachTarget& t = currentReachData->reachTarget;
	t.setTargetJoint(targetJoint);
	startReach = true;
	//currentReachData->startReach = true;	
}

void MeCtExampleBodyReach::setReachTargetPos( SrVec& targetPos )
{
	ReachTarget& t = currentReachData->reachTarget;
	SRT ts;
	ts.tran = targetPos;
	ts.rot = t.targetState.rot;
	t.setTargetState(ts);
	//currentReachData->startReach = true;	
	startReach = true;
}

bool MeCtExampleBodyReach::updateLocomotion()
{	
	// we only move the character when it is idle, and no need to move if we are just point at the object
	float x,y,z,h,p,r;
	SbmCharacter* character = currentReachEngine->getCharacter();
	character->get_world_offset(x,y,z,h,p,r);	
	SrVec curPos = SrVec(x,y,z);

	SrVec targetXZ = currentReachData->reachTarget.getTargetState().tran; targetXZ.y = 0.f;
	SrVec targetPos = currentReachData->reachTarget.getTargetState().tran;
	SrVec distanceVec(x, y, z);
	float dist = currentReachData->XZDistanceToTarget(distanceVec);	

	
	if (currentReachEngine->curHandActionState == MeCtReachEngine::POINT_AT_OBJECT || !useLocomotion)
	{
		if (currentReachEngine->getCurrentState()->curStateName() == "Idle" && startReach)
			updateReachType(targetPos);
		return true;
	}
#ifdef EMBER_SB_STEER
	else if (dist > character->getHeight()*0.35f && !isMoving && startReach && SmartBody::SBScene::getScene()->getSteerManager()->getEngineDriver()->isInitialized() )//currentReachData->startReach) 
	{	
		// if the target is far away, move the character first
		//printf("idle to walk\n");
		std::string cmd;
		std::string charName = character->getName();		
		SrVec curXZ = curPos; curXZ.y = 0.f;
		SrVec targetDir = targetXZ - curXZ; targetDir.normalize();					
		SrVec steerTarget = curXZ + targetDir*(dist);// - character->getHeight()*0.2f);
		float facing = ((float)acos(dot(targetDir,SrVec(0,0,1))))*180.f/(float)M_PI;
		if (dot(cross(targetDir,SrVec(0,0,1)),SrVec(0,1,0)) > 0.f)
			facing = -facing;
		//LOG("facing = %f\n",facing);
		cmd = "bml char " + charName + " <locomotion target=\"" + boost::lexical_cast<std::string>(steerTarget.x) + " " + 
			boost::lexical_cast<std::string>(steerTarget.z) + "\"/>";//+ "\" facing=\"" + boost::lexical_cast<std::string>(facing) +"\"/>";//"\" proximity=\"" +  boost::lexical_cast<std::string>(rd->characterHeight*0.8f*0.01f) +"\"/>";
		//rd->curHandAction->sendReachEvent(cmd);			
		SmartBody::SBScene::getScene()->getCommandManager()->execute(const_cast<char*>(cmd.c_str()));
		isMoving = true;
		//currentReachData->startReach = false;
		startReach = false;
		return false;
	}
#endif
	else if (!isMoving && startReach)//currentReachData->startReach) // the object is already close to character, no need to move
	{		
		LOG("reach in place\n");
		updateReachType(targetPos);
		//setFadeIn(0.5f);
		return true;
	}

	if (isMoving && character->_reachTarget && !character->_lastReachStatus) // character is moving and has reached the target
	{		
		if (dist < character->getHeight()*0.35f)
		{			
			// choose the correct hand
			LOG("reach after locomotion\n");
			updateReachType(targetPos);
			//currentReachData->startReach = true;
			startReach = true;
			isMoving = false;
			//setFadeIn(0.5f);
			return true;
		}
		else
		{
			LOG("[Reach Controller] Warning : Locomotion can not reach the target, dist = %f\n",dist);
			//currentReachData->startReach = false;
			startReach = false;
			isMoving = false;
			return true;
		}					
	}
	return false;
}
int  MeCtExampleBodyReach::determineReachType(SrVec& targetPos)
{
	// current reach hand
	int reachType = currentReachEngine->getReachType(currentReachEngine->getReachTypeTag());
	float x,y,z,h,p,r;
	SbmCharacter* character = currentReachEngine->getCharacter();
	character->get_world_offset(x,y,z,h,p,r);
	SrVec targetDir = SrVec(targetPos.x - x, 0, targetPos.z - z); targetDir.normalize();
	SrVec charDir = character->getFacingDirection(); 

	MeCtReachEngine* newEngine = currentReachEngine;	
	SrVec crossDir = cross(targetDir,charDir);

#define USE_JUMP_MOTION 0

	if (dot(crossDir,SrVec(0,1,0)) > 0 && isValidReachEngine(MeCtReachEngine::RIGHT_ARM)) // right hand
	{
#if USE_JUMP_MOTION
		if (targetPos.y < character->getHeight()*1.1)
#else
		if (1)
#endif
		{
			reachType = MeCtReachEngine::RIGHT_ARM;
		}
		else
			reachType = MeCtReachEngine::RIGHT_JUMP;
	}	
	else if (isValidReachEngine(MeCtReachEngine::LEFT_ARM))
	{
#if USE_JUMP_MOTION
		if (targetPos.y < character->getHeight()*1.1)
#else
		if (1)
#endif
		{
			reachType = MeCtReachEngine::LEFT_ARM;
		}
		else
			reachType = MeCtReachEngine::LEFT_JUMP;
	}
	return reachType;
}
int  MeCtExampleBodyReach::getReachTypeWithAttachedPawn()
{
	int reachType = -1; // return -1 if there is no pawn attached
	ReachEngineMap::iterator mi;
	for (mi  = reachEngineMap.begin();
		mi != reachEngineMap.end();
		mi++)
	{
		MeCtReachEngine* re = mi->second;
		if (re)
		{
			if (re->getReachData()->hasAttachedPawn())
				return re->getReachTypeID();
		}
	}
	return reachType;
}

void MeCtExampleBodyReach::updateReachType(SrVec& targetPos)
{	
	MeCtReachEngine* newEngine = currentReachEngine;		

	int reachType = determineReachType(targetPos); // the best hand according to target position
	
	// if we are putting down an object
	int reachTypeWithAttachedPawn = getReachTypeWithAttachedPawn();
	// or if we specify a default hand
	if (defaultReachType != -1 && reachEngineMap.find(defaultReachType) != reachEngineMap.end())
	{		
		reachType = defaultReachType;
	}
	else if (currentReachEngine->curHandActionState == MeCtReachEngine::PUT_DOWN_OBJECT && reachTypeWithAttachedPawn != -1)
	{
		reachType = reachTypeWithAttachedPawn;
	}	
	MeCtReachEngine* re = reachEngineMap[reachType];		
	newEngine = reachEngineMap[reachType];	
	setNewReachEngine(newEngine);
	newEngine->getCharacter()->setCurrentReachType(reachType); // update the character reach type
}


void MeCtExampleBodyReach::setNewReachEngine( MeCtReachEngine* newEngine )
{
	//if (newEngine == currentReachEngine) // no need to change if it is already the current engine
	//	return;

	ReachStateData* newData = newEngine->getReachData();
	newData->reachTarget = currentReachData->reachTarget;
	newData->startReach  = currentReachData->startReach;
	newData->endReach = currentReachData->endReach;
	newEngine->curHandActionState = currentReachEngine->curHandActionState;	
	newEngine->fadingWeight       = currentReachEngine->fadingWeight;
	currentReachEngine = newEngine;
	currentReachData   = newData;	
}

bool MeCtExampleBodyReach::controller_evaluate( double t, MeFrameData& frame )
{	
	//updateDefaultVariables(frame);	
	
	
	updateDt((float)t);	
	updateChannelBuffer(frame,inputMotionFrame,true);	

	// add logic to steer the character if it is too far away

	bool canReach = updateLocomotion();
	
	// update control parameters
	currentReachEngine->fadingWeight = blendWeight;
	currentReachData->autoReturnTime = autoReturnDuration;	
	currentReachData->useProfileInterpolation = useProfileInterpolation;
	currentReachData->useRetiming = useRetiming;
	if (desireLinearVel > 0)
		currentReachData->linearVel = desireLinearVel;
	else
		currentReachData->linearVel = currentReachEngine->ikDefaultVelocity*reachVelocityScale;
	if (desireGrabSpeed > 0)
		currentReachData->grabSpeed = desireGrabSpeed;
	else
		currentReachData->grabSpeed = 50.f;
		
	currentReachEngine->footIKFix    = footIKFix;
	//LOG("curReachData linearVel = %f",currentReachData->linearVel);
	//if (canReach)
	if (startReach)
	{
		currentReachData->startReach     = startReach;
		startReach = false;
	}
	if (endReach)
	{
		currentReachData->endReach = endReach;
		endReach = false;
	}
	

	//startReach = currentReachData->startReach;

	// blending the input frame with ikFrame based on current fading
	bool finishFadeOut = updateFading(dt);
	
	SmartBody::SBCharacter* curCharacter = dynamic_cast<SmartBody::SBCharacter*>(currentReachEngine->getCharacter());
	if (currentReachEngine->getCurrentState()->curStateName() != "Idle" || currentReachData->startReach )
	{		
		//LOG("update reach");
		currentReachEngine->updateReach((float)t,dt,inputMotionFrame,blendWeight);	
		curCharacter->setBoolAttribute("isReaching", true);		
	}
	else
	{
		curCharacter->setBoolAttribute("isReaching",false);
	}
	
	//printf("blend weight = %f\n",blendWeight);
	BodyMotionFrame outMotionFrame;
	MotionExampleSet::blendMotionFrame(inputMotionFrame,currentReachEngine->outputMotion(),blendWeight,outMotionFrame);	

	ConstraintMap& handConstraint = currentReachEngine->getHandConstraint();
	ConstraintMap::iterator si;
	for ( si  = handConstraint.begin();
		si != handConstraint.end();
		si++)
	{	
		EffectorJointConstraint* cons = dynamic_cast<EffectorJointConstraint*>(si->second);//rotConstraint[i];
		SrMat gmat = currentReachEngine->getMotionParameter()->getMotionFrameJoint(outMotionFrame,cons->efffectorName.c_str())->gmat();
		SrVec targetPos = gmat.get_translation();
		SrQuat targetRot = SrQuat(gmat.get_rotation());		
		for (int k=0;k<3;k++)
			cons->targetJoint->pos()->value(k,targetPos[k]);		
		cons->targetJoint->quat()->value(targetRot);
		cons->targetJoint->update_gmat();
	}
	updateChannelBuffer(frame,outMotionFrame);
	return true;
}

void MeCtExampleBodyReach::init(SbmPawn* pawn)
{	
	IKTreeNodeList& nodeList = currentReachEngine->ikTreeNodes();
	MeCtIKTreeNode* rootNode = nodeList[0];
	for (int i=0;i<3;i++)
		_channels.add(rootNode->joint->getMappedJointName(), (SkChannel::Type)(SkChannel::XPos+i));
	affectedJoints.clear();
	for (unsigned int i=0;i<nodeList.size();i++)
	{
		MeCtIKTreeNode* node = nodeList[i];
		SkJoint* joint = node->joint;
		SkJointQuat* skQuat = joint->quat();				
		affectedJoints.push_back(joint);
		_channels.add(joint->getMappedJointName(), SkChannel::Quat);		
	}		
	blendWeight = currentReachEngine->fadingWeight;
	//LOG("init blend weight = %f\n",blendWeight);	
	MeController::init(pawn);	
}

void MeCtExampleBodyReach::updateChannelBuffer( MeFrameData& frame, BodyMotionFrame& motionFrame, bool bRead /*= false*/ )
{
	SrBuffer<float>& buffer = frame.buffer();
	int count = 0;
	// update root translation
	for (int i=0;i<3;i++)
	{
		int index = frame.toBufferIndex(_toContextCh[count++]);
		if (bRead)
		{
			motionFrame.rootPos[i] = buffer[index] ;
		}
		else
		{
			buffer[index] = motionFrame.rootPos[i];			
		}
	}

	if (motionFrame.jointQuat.size() != affectedJoints.size())
		motionFrame.jointQuat.resize(affectedJoints.size());

	for (unsigned int i=0;i<motionFrame.jointQuat.size();i++)
	{
		SrQuat& quat = motionFrame.jointQuat[i];		
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

void MeCtExampleBodyReach::print_state( int tabs )
{

}

void MeCtExampleBodyReach::controller_start()
{
	//restart = true;
	controlRestart();
}

void MeCtExampleBodyReach::controller_map_updated()
{

}

bool MeCtExampleBodyReach::isValidReachEngine( int reachType )
{
	if (reachEngineMap.find(reachType) != reachEngineMap.end())
		return true;

	//MeCtReachEngine* re = reachEngineMap[reachType];
	//return re->isValid();
	return false;
}

void MeCtExampleBodyReach::notify(SBSubject* subject)
{
// 	SbmPawn* pawn = dynamic_cast<SbmPawn*>(subject);
// 	if (pawn)
// 	{
// 		ReachTarget& t = currentReachData->reachTarget;
// 		SbmPawn* targetPawn = t.getTargetPawn();
// 		if (targetPawn == pawn)
// 		{
// 			// pawn is being removed, let go of it
// 			t.setTargetPawnName("");
// 			currentReachData->endReach = true;
// 		}
// 	}
}
