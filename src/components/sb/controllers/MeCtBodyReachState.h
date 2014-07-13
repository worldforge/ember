#pragma once
#include "controllers/me_ct_example_body_reach.hpp"

namespace SmartBody {
	class SBPawn;
}

class ReachTarget
{
public:
	SRT      targetState;
	SRT      paraTargetState;
	//SbmPawn* targetPawn;
	std::string targetPawnName;
	SkJoint* targetJoint;
	bool     useTargetPawn, useTargetJoint;
public:
	ReachTarget();
	~ReachTarget() {}
	ReachTarget& operator= (const ReachTarget& rt);
	bool targetIsPawn();
	bool targetHasGeometry();
	bool targetIsJoint();
	void setTargetState(SRT& ts);
	void setTargetPawnName(std::string pawnName);
	void setTargetJoint(SkJoint* tjoint);
	SRT getTargetState(); // the return state is based on target's state
	SRT getParaTargetState(); // the return state is based on target's state
	SRT getGrabTargetState(SRT& naturalState, float offset = 0.f); 
	SmartBody::SBPawn* getTargetPawn();
	std::string getTargetPawnName() { return targetPawnName; }
};

class EffectorState 
{
public:
	std::string  effectorName;
	SRT startTargetState, curBlendState; 
	SRT curIKTargetState, ikTargetState;
	SRT curState; 
	SRT grabStateError;
	SrVec paraTarget;
	SrMat gmatZero;

	//SbmPawn* attachedPawn;	
	std::string attachedPawnName;
	SrMat    attachMat;
public:
	EffectorState();
	~EffectorState() {}
	SmartBody::SBPawn* getAttachedPawn();
	void setAttachedPawn(ReachStateData* rd);
	void removeAttachedPawn(ReachStateData* rd);	
};

class ReachHandAction // dafault hand behavior for "Touch"
{
public:	
	virtual void reachPreCompleteAction(ReachStateData* rd) ;
	virtual void reachCompleteAction(ReachStateData* rd) ;	
	virtual void reachPreReturnAction(ReachStateData* rd);
	virtual void reachNewTargetAction(ReachStateData* rd) ;
	virtual void reachReturnAction(ReachStateData* rd) ;
	virtual SRT getHandTargetStateOffset(ReachStateData* rd, SRT& naturalState);
	virtual bool isPickingUpNewPawn(ReachStateData* rd) ;

	void sendReachEvent(const std::string& etype, const std::string& cmd, float time = 0.0);	
public:
	void pickUpAttachedPawn(ReachStateData* rd);
	void putDownAttachedPawn(ReachStateData* rd);
	virtual int getType();
protected:
	std::string generateGrabCmd(const std::string& charName, const std::string& targetName, const std::string& grabState, int type, float grabSpeed);
	std::string generateAttachCmd(const std::string& charName, const std::string& targetName, int type, float grabSpeed);
};

class ReachHandPickUpAction : public ReachHandAction 
{
public:		
	virtual void reachCompleteAction(ReachStateData* rd);		
	virtual void reachNewTargetAction(ReachStateData* rd);
	virtual void reachPreReturnAction(ReachStateData* rd) {} ;
	virtual void reachReturnAction(ReachStateData* rd); // do nothing when return
	virtual int getType();
};

class ReachHandPutDownAction : public ReachHandAction
{
public:	
	virtual void reachPreCompleteAction(ReachStateData* rd) {}; // do nothing when moving toward target
	virtual void reachCompleteAction(ReachStateData* rd);	
	virtual void reachReturnAction(ReachStateData* rd);	
	virtual SRT getHandTargetStateOffset(ReachStateData* rd, SRT& naturalState);
	virtual int getType();
};

class ReachHandPointAction : public ReachHandAction // dafault hand behavior for "Touch"
{
public:	
	virtual void reachPreCompleteAction(ReachStateData* rd);
	virtual void reachCompleteAction(ReachStateData* rd) {};	
	virtual void reachPreReturnAction(ReachStateData* rd) {};
	virtual void reachNewTargetAction(ReachStateData* rd) {};	
	virtual SRT getHandTargetStateOffset(ReachStateData* rd, SRT& naturalState);
	virtual bool isPickingUpNewPawn(ReachStateData* rd);
	virtual int getType();
};

class ReachStateData
{
public:	
	std::string     charName; // character name
	int             reachType;
	float           curTime, dt;
	float           stateTime; // how much time has been in a state 
	float           curRefTime, du;	
	float           blendWeight;
	float           retimingFactor;
	SrMat           gmat;
	SrVec           centerOffset;
	SrVec           shoulderPosition;
	// reference motion frames ( as motion interpolation output, or IK reference pose )
	BodyMotionFrame idleRefFrame, startRefFrame, targetRefFrame, currentRefFrame;		
	BodyMotionFrame curMotionFrame; 
	EffectorState   effectorState;
	ReachTarget     reachTarget;
	SRT             desireHandState;
	// flags for state transition
	float           autoReturnTime;
	float           characterHeight;
	bool            startReach, endReach;
	bool            ikReachTarget;
	bool            useExample;	
	bool            locomotionComplete;
	bool            hasSteering;
	bool            useProfileInterpolation;
	bool            useRetiming;
	bool            newTarget;

	// for pick-up/put-down action
	ReachHandAction* curHandAction;

	// for motion interpolation
	ResampleMotion* interpMotion;
	MotionParameter* motionParameter;
	DataInterpolator* dataInterpolator;
	MeCtExampleBodyReach* reachControl;
public:
	float linearVel;
	float grabSpeed;
	float reachRegion;
public:	
	ReachStateData();
	~ReachStateData();
	void updateReachState(const SrMat& worldOffset, BodyMotionFrame& motionFrame); // update corresponding parameters according to current body frame
	void updateBlendWeight(SrVec paraPos);
	void getInterpFrame(float refTime, BodyMotionFrame& outFrame);
	SRT getBlendPoseState(SrVec paraPos, float refTime);
	SRT getPoseState(BodyMotionFrame& frame);
	bool useInterpolation();	
	float XZDistanceToTarget(SrVec& pos);
	bool hasAttachedPawn();
};

class ReachStateInterface
{	
public:
	virtual ~ReachStateInterface() {}
	virtual void update(ReachStateData* rd) = 0;
	virtual void updateEffectorTargetState(ReachStateData* rd) = 0;	
	virtual float curStatePercentTime(ReachStateData* rd, float refTime)  { return 0.f; };
	virtual std::string nextState(ReachStateData* rd) = 0;
	virtual std::string curStateName() = 0;
protected:
	void updateReturnToIdle(ReachStateData* rd);
	void updateReachToTarget(ReachStateData* rd);
	void updateMotionIK(ReachStateData* rd);
	void updateMotionPoseInterpolation(ReachStateData* rd);
	void updateMotionInterp(ReachStateData* rd);
	bool ikTargetReached(ReachStateData* rd, float ratio = 0.1f);
	bool poseTargetReached(ReachStateData* rd, float ratio = 0.1f);
	bool interpTargetReached(ReachStateData* rd);	
};

class ReachStateIdle : public ReachStateInterface
{
public:
	virtual void update(ReachStateData* rd);
	virtual void updateEffectorTargetState(ReachStateData* rd);	
	virtual std::string nextState(ReachStateData* rd);	
	virtual std::string curStateName() { return "Idle"; }
};

class ReachStateStart : public ReachStateInterface
{
public:
	virtual void update(ReachStateData* rd);
	virtual void updateEffectorTargetState(ReachStateData* rd);	
	virtual float curStatePercentTime(ReachStateData* rd, float refTime);
	virtual std::string nextState(ReachStateData* rd);	
	virtual std::string curStateName() { return "Start"; }
};

class ReachStateComplete : public ReachStateInterface
{
protected:
	float completeTime;
public:
	ReachStateComplete() { completeTime = 0.f; }
	virtual void update(ReachStateData* rd);
	virtual void updateEffectorTargetState(ReachStateData* rd);	
	virtual std::string nextState(ReachStateData* rd);	
	virtual std::string curStateName() { return "Complete"; }
};

class ReachStateNewTarget : public ReachStateInterface
{
public:
	virtual void update(ReachStateData* rd);
	virtual void updateEffectorTargetState(ReachStateData* rd);	
	virtual std::string nextState(ReachStateData* rd);
	virtual std::string curStateName() { return "NewTarget"; }
};

class ReachStatePreReturn : public ReachStateComplete
{
public:
	ReachStatePreReturn() : ReachStateComplete() {}
	virtual void update(ReachStateData* rd) {}
	virtual void updateEffectorTargetState(ReachStateData* rd) {} // do nothing
	virtual std::string nextState(ReachStateData* rd);	
	virtual std::string curStateName() { return "PreReturn"; }
};

class ReachStateReturn : public ReachStateInterface
{
public:
	virtual void update(ReachStateData* rd);
	virtual void updateEffectorTargetState(ReachStateData* rd);	
	virtual float curStatePercentTime(ReachStateData* rd, float refTime);
	virtual std::string nextState(ReachStateData* rd);
	virtual std::string curStateName() { return "Return"; }
};


