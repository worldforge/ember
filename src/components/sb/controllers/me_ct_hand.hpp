#pragma once
#include "me_ct_motion_parameter.h"
#include "me_ct_jacobian_IK.hpp"
#include "me_ct_ccd_IK.hpp"
#include "me_ct_constraint.hpp"

#include <sr/planner/sk_pos_cfg.h> // ?
#include <sb/SBSkeleton.h>
#include <sb/SBJoint.h>
#include <sb/SBMotion.h>

using namespace std;



class FingerChain
{
public:
	MeCtIKTreeNode* fingerTip;
	std::vector<MeCtIKTreeNode*> fingerNodes;	
	SrVec           fingerTarget;
	bool            isLock;
public:
	FingerChain() { isLock = false; fingerTip = NULL; fingerTarget = SrVec(); }
	~FingerChain() {}
	void init(MeCtIKTreeNode* figTip);	
	void unlockChain();
	void getLineSeg(std::vector<SrVec>& lineSeg);
	void testCollision(SBGeomObject* colObj);
};


class MeCtHand :
	public MeController, public FadingControl
{
private:
	static std::string CONTROLLER_TYPE;
public:
	enum FingerID { F_THUMB = 0, F_INDEX, F_MIDDLE, F_RING, F_PINKY, F_NUM_FINGERS };
	enum GrabState { GRAB_START, GRAB_REACH, GRAB_FINISH, GRAB_RETURN, GRAB_POINT };
protected:
	SmartBody::SBSkeleton*     skeletonRef;
	SmartBody::SBSkeleton*     skeletonCopy;
	SmartBody::SBJoint*        wristJoint;
	float 			_duration;
	SkChannelArray	_channels;	
	GrabState             currentGrabState;
	int                   grabType;
	BodyMotionFrame       releaseFrame, grabFrame, reachFrame, currentFrame, pointFrame, tempFrame, inputFrame;	
	
	vector<FingerChain>   fingerChains;
	vector<SmartBody::SBJoint*>      affectedJoints;
	
	ConstraintMap         handPosConstraint;
	ConstraintMap         handRotConstraint;

	MeCtIKTreeScenario    ikScenario;
	MeCtJacobianIK        ik;
	//SbmPawn*              grabTarget; 
	std::string           grabTargetName;

	std::string           attachJointName;
	//SbmPawn*              attachedPawn;
	std::string           attachedPawnName;
	SrMat                 attachMat;

public:
	float                 grabSpeed;
	float                 jointSpeed;

public:
	MeCtHand(SmartBody::SBSkeleton* sk, SmartBody::SBJoint* wrist);
	~MeCtHand(void);	

	void init(std::string grabType, const MotionDataSet& reachPose, const MotionDataSet& grabPose, const MotionDataSet& releasePose, const MotionDataSet& pointPose);

public:
	virtual void controller_map_updated();
	virtual void controller_start();	
	virtual bool controller_evaluate( double t, MeFrameData& frame );

	virtual SkChannelArray& controller_channels()	{ return( _channels ); }
	virtual double controller_duration()			{ return( (double)_duration ); }
	void set_duration(float duration) { _duration = duration; }
	virtual const std::string& controller_type() const		{ return( CONTROLLER_TYPE ); }
	virtual void print_state( int tabs );
	int  getGrabType() { return grabType; }
	void setGrabState(GrabState state);
	void setGrabTargetObject(SbmPawn* targetObj);

	void attachPawnTarget(SbmPawn* pawn, std::string jointName);
	void releasePawn();
	void updateAttachedPawn();
protected:
	void solveIK(float dt);
	void updateChannelBuffer(MeFrameData& frame, BodyMotionFrame& handMotionFrame, bool bRead = false);
	void getPinchFrame(BodyMotionFrame& pinchFrame, SrVec& wristOffset);
	FingerID findFingerID(const char* jointName);
	void updateFingerChains( BodyMotionFrame& targetMotionFrame, float maxAngDelta);
	BodyMotionFrame& findTargetFrame(GrabState state);	
	SbmPawn* getAttachedPawn();
	SbmPawn* getTargetObject();
};







