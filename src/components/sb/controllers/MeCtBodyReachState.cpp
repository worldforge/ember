#include "MeCtBodyReachState.h"
#include <sb/SBEvent.h>
#include <sb/SBScene.h>
#include <sb/SBPawn.h>

#include <boost/lexical_cast.hpp>

/************************************************************************/
/* Effector State                                                       */
/************************************************************************/
EffectorState::EffectorState()
{
	//attachedPawn = NULL;
	attachedPawnName = "";
}

SmartBody::SBPawn* EffectorState::getAttachedPawn()
{
	
	SmartBody::SBPawn* attachedPawn =  SmartBody::SBScene::getScene()->getPawn(attachedPawnName);
	if (attachedPawnName != "" && !attachedPawn) // the pawn is no longer exist...
	{
		attachedPawnName = "";
	}
	return attachedPawn;
}

void EffectorState::setAttachedPawn( ReachStateData* rd )
{
	ReachTarget& target = rd->reachTarget;
	if (target.targetHasGeometry())
	{
		SmartBody::SBPawn* attachedPawn = target.getTargetPawn();
		attachedPawnName = target.getTargetPawnName();
		//attachMat = attachedPawn->get_world_offset_joint()->gmat()*curState.gmat().inverse();	
		attachMat = attachedPawn->get_world_offset()*curState.gmat().inverse();
		//SRT state = target.getTargetState();
		//target.setTargetState(state);
	}
}

void EffectorState::removeAttachedPawn(ReachStateData* rd)
{
	attachedPawnName = "";
	attachMat = SrMat();
}

/************************************************************************/
/* Reach Target                                                         */
/************************************************************************/
SRT ReachTarget::getTargetState()
{
	SRT st = targetState;
	if (targetIsPawn())
	{
		//LOG("target is pawn");
		SbmPawn* targetPawn = getTargetPawn();
		
//  		SkJoint* worldJoint = const_cast<SkJoint*>(targetPawn->get_world_offset_joint());
//  		worldJoint->set_lmat_changed();
//  		worldJoint->update_gmat();
//  		st.gmat(targetPawn->get_world_offset_joint()->gmat());
		
		st.gmat(targetPawn->get_world_offset());
	}
	else if (targetIsJoint())
	{
		//LOG("target is joint");
		targetJoint->update_gmat();
		st.gmat(targetJoint->gmat());
	}
	return st;	
}


SRT ReachTarget::getParaTargetState()
{
	if (targetIsPawn() || targetIsJoint())
	{
		return getTargetState();
	}
	else
	{
		return paraTargetState;
	}
}

bool ReachTarget::targetIsPawn()
{
	return (useTargetPawn && getTargetPawn());
}

bool ReachTarget::targetHasGeometry()
{
	if (!targetIsPawn()) return false;
	//LOG("target pawn name = %s",targetPawn->getName().c_str());
	SbmPawn* targetPawn = getTargetPawn();
	if (!targetPawn) return false;
	if (targetPawn->getGeomObject() == NULL) return false;
	SBGeomNullObject* nullObject = dynamic_cast<SBGeomNullObject*>(targetPawn->getGeomObject());
	if (nullObject) return false;
	return true;
}

bool ReachTarget::targetIsJoint()
{
	return (useTargetJoint && targetJoint);
}

SmartBody::SBPawn* ReachTarget::getTargetPawn()
{
	SmartBody::SBPawn* targetPawn = SmartBody::SBScene::getScene()->getPawn(targetPawnName);
	if (targetPawnName != "" && !targetPawn) // the pawn is no longer exist...
	{
		targetPawnName = "";
	}
	if (useTargetPawn && targetPawn)// && targetPawn->colObj_p)
	{
		return targetPawn;
	}
	return NULL;
}


SRT ReachTarget::getGrabTargetState( SRT& naturalState, float offset )
{
	SRT st = naturalState;
	//st.tran = getTargetState().tran;
	// if there is a collider object, estimate the correct hand position & orientation
	SmartBody::SBPawn* targetPawn = getTargetPawn();
	if (useTargetPawn && targetPawn && targetPawn->getGeomObject())
	{
		targetPawn->getGeomObject()->estimateHandPosture(naturalState.rot,st.tran,st.rot, offset);				
	}
	return st;
}

ReachTarget::ReachTarget()
{
	targetPawnName ="";
	targetJoint = NULL;
	useTargetPawn = false;
	useTargetJoint = false;
}

void ReachTarget::setTargetState( SRT& ts )
{
	targetState = ts;
	paraTargetState = ts;
	targetPawnName = "";
	targetJoint = NULL;
	useTargetPawn = false;
	useTargetJoint = false;
}

void ReachTarget::setTargetPawnName( std::string pawnName )
{
	targetPawnName = pawnName;
	SmartBody::SBPawn* tpawn = getTargetPawn();
	useTargetPawn = true;	
	useTargetJoint = false;
	targetJoint = NULL;
}

void ReachTarget::setTargetJoint( SkJoint* tjoint )
{
	targetJoint = tjoint;
	useTargetJoint = true;
	useTargetPawn = false;
	targetPawnName = "";
}

ReachTarget& ReachTarget::operator=( const ReachTarget& rt )
{
	targetJoint = rt.targetJoint;
	targetPawnName  = rt.targetPawnName;
	targetState = rt.targetState;
	useTargetJoint = rt.useTargetJoint;
	useTargetPawn  = rt.useTargetPawn;
	return *this;
}

/************************************************************************/
/* Reach Hand Action                                                    */
/************************************************************************/
void ReachHandAction::sendReachEvent(const std::string& etype, const std::string& cmd, float time /*= 0.0*/ )
{
	std::string eventType = etype;		
	SmartBody::SBMotionEvent motionEvent;
	motionEvent.setType(eventType);			
	motionEvent.setParameters(cmd);
	SmartBody::SBEventManager* manager = SmartBody::SBScene::getScene()->getEventManager();		
	manager->handleEvent(&motionEvent,time);
}

SRT ReachHandAction::getHandTargetStateOffset( ReachStateData* rd, SRT& naturalState )
{	
	if (rd->reachTarget.targetHasGeometry())
	{
		float grabOffset = rd->characterHeight*0.01f;
		SRT handState = rd->reachTarget.getGrabTargetState(naturalState,grabOffset);	
		rd->desireHandState = handState;
		return SRT::diff(naturalState,handState);
	}	
	else
	{
		return SRT();//rd->effectorState.grabStateError;
	}
}

void ReachHandAction::reachPreCompleteAction( ReachStateData* rd )
{
	ReachTarget& rtarget = rd->reachTarget;
	EffectorState& estate = rd->effectorState;	
	SmartBody::SBPawn* targetPawn = rtarget.getTargetPawn();
	if (!targetPawn || !targetPawn->getGeomObject())
		return;
	std::string cmd;
	std::string charName = rd->charName;
	std::string targetName = targetPawn->getName();	
// 	std::string wristName = "r_wrist";
// 	std::string reachType = "right";
// 	if (rd->reachType == MeCtReach::REACH_LEFT_ARM)
// 	{
// 		wristName = "l_wrist";
// 		reachType = "left";
// 	}	
// 	cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + reachType + "_gc\" sbm:wrist=\"" + wristName + "\"sbm:grab-type=\"" + reachType + "\" sbm:grab-state=\"start\" target=\"" + targetName  + "\"/>";

	cmd = generateGrabCmd(charName,targetName,"start",rd->reachType, rd->grabSpeed);
	sendReachEvent("reach",cmd);

	cmd = "bml char " + charName + " reach-preComplete";
	sendReachEvent("reachNotifier",cmd);
	//LOG("Reach Pre Complete Action");
}

void ReachHandAction::reachCompleteAction( ReachStateData* rd )
{
	ReachTarget& rtarget = rd->reachTarget;
	SmartBody::SBPawn* targetPawn = rtarget.getTargetPawn();
	if (!targetPawn || !targetPawn->getGeomObject())
		return;

	std::string cmd;
	std::string charName = rd->charName;
	std::string targetName = targetPawn->getName();
// 	std::string wristName = "r_wrist";
 	std::string reachType = "right";
 	if (rd->reachType == MeCtReach::REACH_LEFT_ARM)
 	{
// 		wristName = "l_wrist";
 		reachType = "left";
 	}	
	cmd = generateGrabCmd(charName,targetName,"reach",rd->reachType, rd->grabSpeed);
	//cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + "_gc\" sbm:wrist=\"r_wrist\" sbm:grab-state=\"reach\" target=\"" + targetName  + "\"/>";
	//cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + reachType + "_gc\" sbm:wrist=\"" + wristName + "\"sbm:grab-type=\"" + reachType + "\" sbm:grab-state=\"reach\" target=\"" + targetName  + "\"/>";
	//LOG("send out grab command = %s",cmd.c_str());
	sendReachEvent("reach",cmd);

	cmd = "bml char " + charName + " reach-complete: " + reachType;
	sendReachEvent("reachNotifier",cmd);
	//LOG("Reach Complete Action");
}

void ReachHandAction::reachNewTargetAction( ReachStateData* rd )
{	
	std::string cmd;
	std::string charName = rd->charName;	

	EffectorState& estate = rd->effectorState;	
	if (estate.getAttachedPawn())
		return; // don't do any hand animation if there is a pawn attached
	//cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + "_gc\" sbm:grab-state=\"finish\"/>";
	cmd = generateGrabCmd(charName,"","finish",rd->reachType, rd->grabSpeed);
	ReachTarget& rtarget = rd->reachTarget;
	if (rtarget.targetHasGeometry())
	{
		sendReachEvent("reach",cmd);

		cmd = "bml char " + charName + " reach-newTarget";
		sendReachEvent("reachNotifier",cmd);
	}
	//rd->effectorState.removeAttachedPawn(rd);
	//LOG("Reach New Target Action");	
}

void ReachHandAction::reachReturnAction( ReachStateData* rd )
{
	std::string cmd;
	std::string charName = rd->charName;

	EffectorState& estate = rd->effectorState;	
	if (estate.getAttachedPawn())
		return; // don't do any hand animation if there is a pawn attached
	//cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + "_gc\" sbm:grab-state=\"return\"/>";
	
	cmd = generateGrabCmd(charName,"","return",rd->reachType, rd->grabSpeed);
	sendReachEvent("reach",cmd);

 	std::string reachType = "right";
 	if (rd->reachType == MeCtReach::REACH_LEFT_ARM)
 		reachType = "left";
	cmd = "bml char " + charName + " reach-return: " + reachType;
	sendReachEvent("reachNotifier",cmd);
	
	cmd = "char " + charName + " gazefade out 0.5";
	sendReachEvent("reach",cmd);
	//LOG("Reach Return Action");	
	//rd->effectorState.removeAttachedPawn(rd);
}

bool ReachHandAction::isPickingUpNewPawn( ReachStateData* rd )
{
	ReachTarget& rtarget = rd->reachTarget;
	EffectorState& estate = rd->effectorState;
	if (rd->startReach && rtarget.targetIsPawn()) // a new target pawn
	{
		return (estate.getAttachedPawn() != rtarget.getTargetPawn());
	}
	else if (rd->startReach && !rtarget.targetIsPawn() && !rtarget.targetIsJoint()) // a new target position
	{
		return (estate.getAttachedPawn() != NULL);
	}

	return false;
}

void ReachHandAction::pickUpAttachedPawn( ReachStateData* rd )
{
	// send attachment to hand controller
	rd->effectorState.setAttachedPawn(rd);
	SbmPawn* attachedPawn = rd->effectorState.getAttachedPawn();
	if (!attachedPawn || !attachedPawn->getGeomObject())
		return;	
	std::string charName = rd->charName;
	std::string targetName = attachedPawn->getName();
	std::string cmd;
	//cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + "_gc\" sbm:source-joint=\"" + "r_wrist" + "\" sbm:attach-pawn=\"" + targetName + "\"/>";
	cmd = generateAttachCmd(charName,targetName,rd->reachType, rd->grabSpeed);
	LOG("   pawn attached: %s",targetName.c_str());
	rd->curHandAction->sendReachEvent("reach",cmd);

	cmd = "bml char " + charName + " pawn-attached";
	EffectorState& estate = rd->effectorState;
	if(estate.getAttachedPawn())
		cmd = cmd + ": " + estate.attachedPawnName;
	rd->curHandAction->sendReachEvent("reachNotifier",cmd);
	
	cmd = "pawn " + targetName + " physics off";
	rd->curHandAction->sendReachEvent("reach",cmd);	
	SRT prevParaState = rd->reachTarget.getParaTargetState();
	rd->reachTarget.setTargetState(rd->effectorState.ikTargetState);	
	rd->reachTarget.paraTargetState = prevParaState;
	//rd->reachTarget.setTargetState(rd->reachTarget.getTargetState());
	//rd->effectorState.ikTargetState = rd->effectorState.curIKTargetState;
}

void ReachHandAction::putDownAttachedPawn( ReachStateData* rd )
{
	SbmPawn* attachedPawn = rd->effectorState.getAttachedPawn();
	if (!attachedPawn || !attachedPawn->getGeomObject())
		return;

	std::string charName = rd->charName;	
	std::string cmd;
	cmd = generateAttachCmd(charName,"",rd->reachType, rd->grabSpeed);
	//cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + "_gc\" sbm:release-pawn=\"true\"/>";	
	rd->curHandAction->sendReachEvent("reach",cmd);

	cmd = "bml char " + charName + " pawn-released";
	EffectorState& estate = rd->effectorState;
	if(estate.getAttachedPawn())
		cmd = cmd + ": " + estate.attachedPawnName;
	rd->curHandAction->sendReachEvent("reachNotifier",cmd);

	std::string targetName = "";	
	if (attachedPawn)
		targetName = attachedPawn->getName();
	LOG("   pawn released: %s",targetName.c_str());
	cmd = "pawn " + targetName + " physics on";
	rd->curHandAction->sendReachEvent("reach",cmd);
	rd->effectorState.removeAttachedPawn(rd);
}

void ReachHandAction::reachPreReturnAction( ReachStateData* rd )
{
	ReachHandAction::reachNewTargetAction(rd);	
}

std::string ReachHandAction::generateGrabCmd( const std::string& charName, const std::string& targetName, const std::string& grabState, int type, float grabSpeed )
{
	std::string wristName = "r_wrist";
	std::string reachType = "right";
	if (type == MeCtReachEngine::LEFT_ARM || type == MeCtReachEngine::LEFT_JUMP)
	{
		wristName = "l_wrist";
		reachType = "left";
	}
	std::string targetStr = "";
	if (targetName != "")
	{
		targetStr = " target=\"" + targetName + "\"";
	}	
	std::string grabStr = "";
	if (grabSpeed > 0.0)
	{
		grabStr = " sbm:grab-speed=\"" + boost::lexical_cast<std::string>(grabSpeed) + "\"";
	}
	std::string cmd;
	//cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + "_gc\" sbm:wrist=\"r_wrist\" sbm:grab-state=\"reach\" target=\"" + targetName  + "\"/>";
	cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + reachType + "_gc\" sbm:wrist=\"" + wristName + "\" sbm:grab-type=\"" + reachType + "\" sbm:grab-state=\""+ grabState + "\"" + targetStr + grabStr + "/>";
	return cmd;
}

std::string ReachHandAction::generateAttachCmd( const std::string& charName, const std::string& targetName, int type, float grabSpeed )
{
	std::string wristName = "r_wrist";
	std::string reachType = "right";
	if (type == MeCtReachEngine::LEFT_ARM || type == MeCtReachEngine::LEFT_JUMP)
	{
		wristName = "l_wrist";
		reachType = "left";
	}
	std::string targetStr = "";
	if (targetName != "")
	{
		targetStr = " sbm:attach-pawn=\"" + targetName + "\"";
	}	
	else
		targetStr = " sbm:release-pawn=\"true\"";

	std::string grabStr = "";
	if (grabSpeed > 0.0)
	{
		grabStr = " sbm:grab-speed=\"" + boost::lexical_cast<std::string>(grabSpeed) + "\"";
	}

	std::string cmd;
	//cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + "_gc\" sbm:wrist=\"r_wrist\" sbm:grab-state=\"reach\" target=\"" + targetName  + "\"/>";
	cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + reachType + "_gc\" sbm:source-joint=\"" + wristName + "\" sbm:grab-type=\"" + reachType + "\"" + targetStr + grabStr + "/>";
	return cmd;
}

int ReachHandAction::getType()
{
	return MeCtReachEngine::TOUCH_OBJECT;
}

/************************************************************************/
/* Reach Hand Pick-Up Action                                            */
/************************************************************************/
void ReachHandPickUpAction::reachCompleteAction( ReachStateData* rd )
{
	if (!rd->ikReachTarget)
	{
		ReachHandAction::reachPreReturnAction(rd);
		return;
	}
	ReachHandAction::reachCompleteAction(rd);	
	pickUpAttachedPawn(rd);
}

void ReachHandPickUpAction::reachNewTargetAction( ReachStateData* rd )
{	
	//putDownAttachedPawn(rd);
	//rd->effectorState.removeAttachedPawn(rd);
	ReachHandAction::reachNewTargetAction(rd);
}

void ReachHandPickUpAction::reachReturnAction( ReachStateData* rd )
{
	
	std::string cmd;
	std::string charName = rd->charName;	
	cmd = "char " + charName + " gazefade out 0.5";
	//sendReachEvent(cmd);
}

int ReachHandPickUpAction::getType()
{
	return MeCtReachEngine::PICK_UP_OBJECT;
}
/************************************************************************/
/* Reach Hand Put-Down Action                                           */
/************************************************************************/
void ReachHandPutDownAction::reachCompleteAction( ReachStateData* rd )
{
	putDownAttachedPawn(rd);
	//rd->effectorState.removeAttachedPawn(rd);
	ReachHandAction::reachNewTargetAction(rd);	
}

void ReachHandPutDownAction::reachReturnAction( ReachStateData* rd )
{	
	/*
	std::string cmd;
	std::string charName = rd->charName;
	//cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + "_gc\" sbm:grab-state=\"return\"/>";
	generateGrabCmd(charName,"","return",rd->reachType);
	sendReachEvent("reach",cmd);	

	cmd = "char " + charName + " gazefade out 0.5";
	//sendReachEvent(cmd);	
	rd->effectorState.removeAttachedPawn(rd);
	*/
	ReachHandAction::reachReturnAction(rd);

	std::string charName = rd->charName;
 	std::string reachType = "right";
 	if (rd->reachType == MeCtReach::REACH_LEFT_ARM)
 		reachType = "left";
	std::string cmd = "bml char " + charName + " reach-returned: " + reachType;
	sendReachEvent("reachNotifier",cmd);
}

SRT ReachHandPutDownAction::getHandTargetStateOffset( ReachStateData* rd, SRT& naturalState )
{
	return rd->effectorState.grabStateError;
}

int ReachHandPutDownAction::getType()
{
	return MeCtReachEngine::PUT_DOWN_OBJECT;
}


SRT ReachHandPointAction::getHandTargetStateOffset( ReachStateData* rd, SRT& naturalState )
{
	return SRT();	
}

bool ReachHandPointAction::isPickingUpNewPawn( ReachStateData* rd )
{
	return false;	
}

int ReachHandPointAction::getType()
{
	return MeCtReachEngine::POINT_AT_OBJECT;
}

void ReachHandPointAction::reachPreCompleteAction( ReachStateData* rd )
{
	std::string cmd;
	std::string charName = rd->charName;

	EffectorState& estate = rd->effectorState;	
	if (estate.getAttachedPawn())
		return; // don't do any hand animation if there is a pawn attached
	//cmd = "bml char " + charName + " <sbm:grab sbm:handle=\"" + charName + "_gc\" sbm:grab-state=\"return\"/>";

	cmd = generateGrabCmd(charName,"","point",rd->reachType, rd->grabSpeed);
	sendReachEvent("reach",cmd);

	cmd = "bml char " + charName + " reach-preComplete";
	sendReachEvent("reachNotifier",cmd);
}
/************************************************************************/
/* Reach State Data                                                     */
/************************************************************************/

ReachStateData::ReachStateData()
{
	curTime = 0.f;
	curRefTime = 0.f;
	stateTime = 0.f;
	blendWeight = 0.f;
	retimingFactor = 1.f;
	dt = du = 0.f;
	startReach = endReach = useExample = locomotionComplete = newTarget = hasSteering = ikReachTarget = false;
	useProfileInterpolation = false;
	useRetiming = false;
	autoReturnTime = -1.f;
	reachType = MeCtReachEngine::RIGHT_ARM;

	interpMotion = NULL;
	motionParameter = NULL; 
	dataInterpolator = NULL;
	curHandAction = NULL;

	linearVel = 70.f;
	grabSpeed = 80.f;
}

ReachStateData::~ReachStateData()
{
	if (interpMotion)
	{
		delete interpMotion;
		interpMotion = NULL;
	}
	if (motionParameter)
	{
		delete motionParameter; 
		motionParameter = NULL;
	}
	if (dataInterpolator)
	{
		delete dataInterpolator;
		dataInterpolator = NULL;
	}
	if (curHandAction)
	{
		delete curHandAction;
		curHandAction = NULL;
	}
}


void ReachStateData::updateReachState(const SrMat& worldOffset, BodyMotionFrame& motionFrame )
{
	gmat = worldOffset;
	curMotionFrame = motionFrame;
	effectorState.curState = getPoseState(motionFrame);
	//effectorState.updateAttachedPawn();
}

void ReachStateData::updateBlendWeight( SrVec paraPos )
{
	SrVec localTarget = paraPos*gmat.inverse();	
	dVector para; para.resize(3);
	for (int i=0;i<3;i++)
		para[i] = localTarget[i];
	dataInterpolator->predictInterpWeights(para,interpMotion->weight);	
}

SRT ReachStateData::getBlendPoseState( SrVec paraPos, float refTime )
{
	BodyMotionFrame outFrame;
	updateBlendWeight(paraPos);
	getInterpFrame(refTime,outFrame);
	return getPoseState(outFrame);
}


SRT ReachStateData::getPoseState( BodyMotionFrame& frame )
{
	SkJoint* targetJoint = motionParameter->getMotionFrameJoint(frame,effectorState.effectorName.c_str());	
	SRT poseState;
	if (!targetJoint)
		return poseState;
	poseState.tran = targetJoint->gmat().get_translation();
	poseState.rot  = SrQuat(targetJoint->gmat());
	return poseState;
}

void ReachStateData::getInterpFrame( float refTime, BodyMotionFrame& outFrame )
{
	interpMotion->getMotionFrame(refTime,motionParameter->skeletonRef,motionParameter->affectedJoints,outFrame);	
}

bool ReachStateData::useInterpolation()
{
	return (interpMotion && useExample);
}

bool ReachStateData::hasAttachedPawn()
{
	return (effectorState.getAttachedPawn() != NULL);
}

float ReachStateData::XZDistanceToTarget(SrVec& pos)
{
	SrVec targetXZ = reachTarget.getTargetState().tran; targetXZ.y = 0.f;
	SrVec curXZ = pos; curXZ.y = 0.f;
	float dist = (targetXZ - curXZ).norm();
	return dist;
}


/************************************************************************/
/* Reach State Interface                                                */
/************************************************************************/

void ReachStateInterface::updateReturnToIdle( ReachStateData* rd )
{
	// only change effector target state, no change on para blend weight	
	EffectorState& estate = rd->effectorState;	
	if (rd->useInterpolation())
	{
		float rtime = (float)rd->interpMotion->motionDuration(BodyMotionInterface::DURATION_REF)*0.999f;
		BodyMotionFrame targetRefFrame;
		//rd->getInterpFrame(rtime,rd->targetRefFrame);		
		rd->targetRefFrame = rd->idleRefFrame;		
	}	
	estate.ikTargetState = rd->getPoseState(rd->idleRefFrame);
}

void ReachStateInterface::updateReachToTarget( ReachStateData* rd )
{	
	EffectorState& estate = rd->effectorState;
	ReachTarget& rtarget = rd->reachTarget;
	SRT ts = rtarget.getTargetState();
	
	if (rd->curHandAction->getType() == MeCtReachEngine::POINT_AT_OBJECT)
	{
		SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
		SmartBody::SBCharacter* sbChar = scene->getCharacter(rd->charName);
		
		SrVec sbCenter = rd->shoulderPosition;//(rd->gmat.get_translation()+rd->centerOffset);
		SrVec newTargetDir = ts.tran - sbCenter;
		newTargetDir.normalize(); 
		SrVec newTargetPos = sbCenter + newTargetDir*rd->characterHeight*0.4f;
		ts.tran = newTargetPos;
	}	

	SRT tsBlend = rd->getPoseState(rd->targetRefFrame);
	SRT tsBlendGlobal;

	if (rd->useInterpolation())
	{
		SRT paraTarget = rtarget.getParaTargetState();
		float stime = (float)rd->interpMotion->strokeEmphasisTime();
		estate.paraTarget = paraTarget.tran;			
		rd->updateBlendWeight(estate.paraTarget);
		tsBlend = rd->getBlendPoseState(estate.paraTarget,stime);		
		rd->getInterpFrame(stime,rd->targetRefFrame);
	}			
	//LOG("reach target before offset = %f %f %f\n",ts.tran[0],ts.tran[1],ts.tran[2]);
	tsBlend.tran = ts.tran;
	tsBlendGlobal = tsBlend;
	SrQuat newRot = SrQuat(rd->effectorState.gmatZero.inverse()*tsBlendGlobal.gmat());
	tsBlendGlobal.rot = newRot;
	SRT offset = rd->curHandAction->getHandTargetStateOffset(rd,tsBlendGlobal);	
	SrQuat quatZero = SrQuat(rd->effectorState.gmatZero);
	SrQuat newOffsetRot = quatZero*offset.rot*quatZero.inverse();
	offset.rot = newOffsetRot;
	tsBlend.add(offset);		
	//LOG("reach target after offset = %f %f %f\n",tsBlend.tran[0],tsBlend.tran[1],tsBlend.tran[2]);
	estate.ikTargetState = tsBlend;
	estate.grabStateError = offset;
	//LOG("reach target position = %f %f %f\n",
}


void ReachStateInterface::updateMotionPoseInterpolation( ReachStateData* rd )
{
	EffectorState& estate = rd->effectorState;

	// scale factor depends only on traveling distance
	float reachDistance = (estate.startTargetState.tran - estate.ikTargetState.tran).norm();
	float scaleFactor = (reachDistance)*3.f/rd->characterHeight;
	float timeScale = (rd->useRetiming && rd->useProfileInterpolation) ? (scaleFactor*(1.f - rd->retimingFactor) + rd->retimingFactor) : 1.f;//(pow(scaleFactor,rd->retimingFactor)) : 1.f;	
	float reachStep = rd->linearVel*rd->dt*timeScale;
	SRT diff = SRT::diff(estate.curBlendState,estate.ikTargetState);		
	SrVec stepVec = diff.tran;		
	if (stepVec.norm() > reachStep)
	{
		stepVec.normalize();
		stepVec = stepVec*reachStep;
	}

	BodyMotionFrame interpFrame;	
	SRT stateOffset = SRT::diff(rd->getPoseState(rd->currentRefFrame),estate.curIKTargetState);	
	
	SrVec newCurTarget = estate.curBlendState.tran + stepVec;
	
	float startLength = (newCurTarget - estate.startTargetState.tran).norm();
	float endLength   = (estate.ikTargetState.tran - newCurTarget).norm();
	float morphWeight = endLength > 0.f ? (startLength+reachStep)/(endLength+startLength+reachStep) : 1.f;
	BodyMotionFrame morphFrame;			

	if (scaleFactor > 1.f) scaleFactor = 1.f;
	if (rd->useProfileInterpolation)
	{
		//MotionExampleSet::blendMotionFrameProfile(rd->interpMotion,rd->startRefFrame,rd->targetRefFrame,morphWeight,morphFrame);		
		float timeFactor = MotionExampleSet::blendMotionFrameEulerProfile(rd->interpMotion,rd->startRefFrame,rd->targetRefFrame,scaleFactor,morphWeight,morphFrame);
		rd->retimingFactor = pow(timeFactor,2.f);//timeFactor*timeFactor;	
		//LOG("retiming factor = %f\n",rd->retimingFactor);
	}
	else
	{
		MotionExampleSet::blendMotionFrame(rd->startRefFrame,rd->targetRefFrame,morphWeight,morphFrame);	
		rd->retimingFactor = 0.f;
	}

	SRT interpState = rd->getPoseState(morphFrame);	
	SRT stateError = SRT::diff(rd->getPoseState(rd->targetRefFrame),estate.ikTargetState);	

	float weight = 1.f;
	if (reachStep < endLength && endLength != 0.f)
		weight = reachStep /endLength;
	SRT delta = SRT::blend(stateOffset,stateError,weight);
	interpState.add(delta);
	estate.curBlendState = SRT::blend(estate.startTargetState,estate.ikTargetState,morphWeight);
	estate.curIKTargetState = interpState;	
	rd->currentRefFrame = morphFrame;	
}

void ReachStateInterface::updateMotionIK( ReachStateData* rd )
{
	EffectorState& estate = rd->effectorState;
	float reachStep = rd->linearVel*rd->dt;
    //LOG("liner vel = %f, reachStep = %f",rd->linearVel, reachStep);	
	SRT diff = SRT::diff(estate.curIKTargetState,estate.ikTargetState);
	SrVec offset = diff.tran;

	float offsetLength = offset.norm();								
	if (offset.norm() > reachStep)
	{
		offset.normalize();
		offset = offset*reachStep;
	}
	float morphWeight = offsetLength > 0.f ? (offset.norm()+reachStep)/(offsetLength+reachStep) : 1.f;	

	BodyMotionFrame morphFrame;				
	MotionExampleSet::blendMotionFrame(rd->currentRefFrame,rd->targetRefFrame,morphWeight,morphFrame);						
	estate.curIKTargetState = SRT::blend(estate.curIKTargetState,estate.ikTargetState,morphWeight);
	rd->currentRefFrame = morphFrame;	
}

void ReachStateInterface::updateMotionInterp( ReachStateData* rd )
{
	ResampleMotion* interpMotion = rd->interpMotion;

	if (!interpMotion || !rd->useExample)
	{
		updateMotionIK(rd);
		return;
	}
	EffectorState& estate = rd->effectorState;
	SRT stateOffset = SRT::diff(rd->getPoseState(rd->currentRefFrame),estate.curIKTargetState);
	BodyMotionFrame outFrame,interpFrame;
	rd->getInterpFrame(rd->curRefTime,interpFrame);
	MotionExampleSet::blendMotionFrame(interpFrame,rd->idleRefFrame,rd->blendWeight,outFrame);
	SRT interpState = rd->getPoseState(outFrame);	
	SRT stateError = SRT::diff(rd->getPoseState(rd->targetRefFrame),estate.ikTargetState);	

	float percentTime = curStatePercentTime(rd,rd->curRefTime);		
	//LOG("percent time = %f, dt = %f, ref time = %f, du = %f",percentTime,rd->dt, rd->curRefTime, rd->du);
	float deltaPercent = percentTime - curStatePercentTime(rd,rd->curRefTime-rd->du);
	float remain = 1.f - percentTime;
	float weight = 1.f;
	if (deltaPercent < remain && remain != 0.f)
		weight = deltaPercent /remain;

	SRT delta = SRT::blend(stateOffset,stateError,weight);
	interpState.add(delta);
	estate.curIKTargetState = interpState;
	rd->currentRefFrame = outFrame;	

	rd->du = (float)interpMotion->getRefDeltaTime(rd->curRefTime,rd->dt);
	rd->curRefTime += rd->du;
}

bool ReachStateInterface::ikTargetReached( ReachStateData* rd, float ratio )
{
	EffectorState& estate = rd->effectorState;
	float dist = SRT::dist(estate.curState,estate.ikTargetState);
	return (dist < rd->reachRegion*ratio);
}

bool ReachStateInterface::interpTargetReached( ReachStateData* rd )
{
	if (rd->useInterpolation())
		return (curStatePercentTime(rd,rd->curRefTime) >= 1.f);
	else
		return true;//ikTargetReached(rd);
}

bool ReachStateInterface::poseTargetReached( ReachStateData* rd, float ratio /*= 0.1f*/ )
{
	EffectorState& estate = rd->effectorState;
	float dist = SRT::dist(estate.curIKTargetState,estate.ikTargetState);
	return (dist < rd->reachRegion*ratio);
}
/************************************************************************/
/* Reach State Idle                                                       */
/************************************************************************/
void ReachStateIdle::updateEffectorTargetState( ReachStateData* rd )
{
	ReachStateInterface::updateReturnToIdle(rd);	
	rd->effectorState.curIKTargetState = rd->effectorState.ikTargetState;
}

void ReachStateIdle::update( ReachStateData* rd )
{
	ReachStateInterface::updateMotionIK(rd);	
}

std::string ReachStateIdle::nextState( ReachStateData* rd )
{
	std::string nextStateName = "Idle";
	if (rd->startReach)
	{		
		rd->curRefTime = 0.f;
		rd->ikReachTarget = false;
		// test the distance to the target	
		{
			//printf("idle to start\n");
			rd->reachControl->setFadeIn(0.5f);
			nextStateName = "Start";
		}		
	}
	return nextStateName;
}

/************************************************************************/
/* Reach State Start                                                    */
/************************************************************************/
void ReachStateStart::updateEffectorTargetState( ReachStateData* rd )
{
	ReachStateInterface::updateReachToTarget(rd);
}

float ReachStateStart::curStatePercentTime( ReachStateData* rd, float refTime)
{
	if (!rd->useInterpolation())
		return ReachStateInterface::curStatePercentTime(rd,refTime);

	ResampleMotion* interpMotion = rd->interpMotion;
	float percent = refTime/ (float)interpMotion->strokeEmphasisTime();
	return min(max(0.f,percent),1.f);
}

void ReachStateStart::update( ReachStateData* rd )
{
	ReachStateInterface::updateMotionInterp(rd);
	if (curStatePercentTime(rd,rd->curRefTime) > 0.8f && rd->startReach)
	{
		rd->curHandAction->reachPreCompleteAction(rd); 
		rd->startReach = false;
	}
	rd->blendWeight = max(0.f,(1.f - curStatePercentTime(rd,rd->curRefTime)*4.f));
}

std::string ReachStateStart::nextState( ReachStateData* rd )
{
	std::string nextStateName = "Start";
	if (interpTargetReached(rd))
	{
		//if (ikTargetReached(rd,10.f))
		rd->ikReachTarget = ikTargetReached(rd,2.f);
		{
			rd->curHandAction->reachCompleteAction(rd);
			rd->curHandAction->sendReachEvent("reachstate","complete");
			nextStateName = "Complete";
			//rd->startReach = false;		
		}
		
		// 		else
		// 		{
		// 			rd->startReach = false;
		// 			nextStateName = "Return";
		// 		}		
	}
	return nextStateName;
}

/************************************************************************/
/* Reach State Complete                                                 */
/************************************************************************/
void ReachStateComplete::updateEffectorTargetState( ReachStateData* rd )
{
	ReachStateInterface::updateReachToTarget(rd);
	if (rd->useInterpolation())
		rd->curRefTime = (float)rd->interpMotion->strokeEmphasisTime();
}

void ReachStateComplete::update( ReachStateData* rd )
{
	ReachStateInterface::updateMotionIK(rd);
	//completeTime += rd->dt;
}

std::string ReachStateComplete::nextState( ReachStateData* rd )
{
	std::string nextStateName = "Complete";	
	bool toNextState = rd->autoReturnTime > 0.f ? rd->autoReturnTime < rd->stateTime : rd->endReach;
	//LOG("ReachComplete:rd->stateTime = %f",rd->stateTime);
	if (rd->endReach)
		toNextState = true;

	if (rd->curHandAction->isPickingUpNewPawn(rd))
	{
		rd->curHandAction->reachNewTargetAction(rd);
		rd->newTarget = true;	
		rd->retimingFactor = 0.f;
		//completeTime = 0.f;		
		nextStateName = "PreReturn";//"NewTarget";
	}
	else if (toNextState)
	{
		//rd->curHandAction->reachNewTargetAction(rd);
		rd->curHandAction->reachPreReturnAction(rd);
		rd->endReach = false;		
		//completeTime = 0.f; // reset complete time
		nextStateName = "PreReturn";
	}	
	return nextStateName;
}
/************************************************************************/
/* Reach State New Target                                               */
/************************************************************************/
void ReachStateNewTarget::updateEffectorTargetState( ReachStateData* rd )
{
	ReachStateInterface::updateReachToTarget(rd);
	if (rd->useInterpolation())
		rd->curRefTime = (float)rd->interpMotion->strokeEmphasisTime();
}

void ReachStateNewTarget::update( ReachStateData* rd )
{
	ReachStateInterface::updateMotionIK(rd);	
	//ReachStateInterface::updateMotionPoseInterpolation(rd);
}
std::string ReachStateNewTarget::nextState( ReachStateData* rd )
{
	std::string nextStateName = "NewTarget";
	//if (ikTargetReached(rd))
	if (poseTargetReached(rd))
	{
		rd->curHandAction->reachCompleteAction(rd);
		rd->startReach = false;
		rd->curHandAction->sendReachEvent("reachstate","newtarget");
		nextStateName = "Complete";
	}
// 	else if (rd->endReach)
// 	{
// 		rd->startReach = false;
// 		nextStateName = "PreReturn";
// 	}
	return nextStateName;
}

/************************************************************************/
/* Reach State Pre-Return                                               */
/************************************************************************/
std::string ReachStatePreReturn::nextState( ReachStateData* rd )
{
	std::string nextStateName = "PreReturn";
	bool toNextState = rd->stateTime > 0.0f;
	if (toNextState)
	{
		completeTime = 0.f; // reset complete time
		if (rd->newTarget)
		{
			rd->newTarget = false;
			nextStateName = "NewTarget";			
		}
		else // return to rest pose
		{
			rd->curHandAction->reachReturnAction(rd);	

			// fade out any gaze
			std::string cmd;
			std::string charName = rd->charName;
			cmd = "char " + charName + " gazefade out 0.5";
			rd->curHandAction->sendReachEvent("reach",cmd);

			
			cmd = "bml char " + charName + " reach-return";
			rd->curHandAction->sendReachEvent("reachNotifier",cmd);

			nextStateName = "Return";
		}		
	}
	return nextStateName;
}


/************************************************************************/
/* Reach State Return                                                   */
/************************************************************************/
void ReachStateReturn::updateEffectorTargetState( ReachStateData* rd )
{
	ReachStateInterface::updateReturnToIdle(rd);	
}

float ReachStateReturn::curStatePercentTime( ReachStateData* rd, float refTime )
{
	if (!rd->useInterpolation())
		return ReachStateInterface::curStatePercentTime(rd,refTime);

	ResampleMotion* interpMotion = rd->interpMotion;
	float percent = (refTime - (float)interpMotion->strokeEmphasisTime())/(float)(interpMotion->motionDuration(BodyMotionInterface::DURATION_REF)-interpMotion->strokeEmphasisTime());	
	return min(max(0.f,percent),1.f);
}

void ReachStateReturn::update( ReachStateData* rd )
{
	//LOG("curRefTime = %f\n",rd->curRefTime);
	ReachStateInterface::updateMotionInterp(rd);
	rd->blendWeight = curStatePercentTime(rd,rd->curRefTime);	
	//LOG("Blend weight = %f\n",rd->blendWeight);
}

std::string ReachStateReturn::nextState( ReachStateData* rd )
{
	std::string nextStateName = "Return";

	if (interpTargetReached(rd))
	{
		rd->startReach = false;
		rd->endReach = false;
		rd->reachControl->setFadeOut(0.5f);
		rd->blendWeight = 0.f;
		rd->curHandAction->sendReachEvent("reachstate","return");

		std::string charName = rd->charName;
		std::string _state = "bml char " + charName + " reach-stateCurrent: " + nextStateName;
		
		nextStateName = "Idle";

		_state = _state + " reach-stateNew: " + nextStateName;
		EffectorState& estate = rd->effectorState;	
		if(estate.getAttachedPawn())
			_state = _state + " pawn-attached: " + estate.attachedPawnName;
		rd->curHandAction->sendReachEvent("reachNotifier", _state);
	}


	//// send attachment to hand controller
	//rd->effectorState.setAttachedPawn(rd);
	//SbmPawn* attachedPawn = rd->effectorState.getAttachedPawn();
	//if (!attachedPawn || !attachedPawn->getGeomObject())
	//	return;	
	//std::string charName = rd->charName;
	//std::string targetName = attachedPawn->getName();


	return nextStateName;
}

