#pragma once
#include "controllers/me_ct_data_interpolation.h"
#include "controllers/me_ct_barycentric_interpolation.h"
#include "controllers/me_ct_motion_parameter.h"
#include "controllers/me_ct_motion_profile.h"
#include "controllers/me_ct_jacobian_IK.hpp"
#include "controllers/me_ct_ccd_IK.hpp"
#include "controllers/me_ct_constraint.hpp"
#include <sb/sbm_pawn.hpp>

class SbmCharacter;
class ReachStateData;
class ReachStateInterface;
class ReachHandAction;

class MeCtReachEngine
{
public:
	enum HandActionState { PICK_UP_OBJECT = 0, TOUCH_OBJECT, PUT_DOWN_OBJECT, POINT_AT_OBJECT };
	enum { RIGHT_ARM = 0, LEFT_ARM, RIGHT_JUMP, LEFT_JUMP, REACH_TYPE_SIZE };
	static std::string ReachTypeTag[REACH_TYPE_SIZE];
protected:
	bool          valid;
	bool          ikInit;
	int           reachType;
	SbmCharacter* character;
	SmartBody::SBSkeleton*   skeletonCopy, *skeletonRef;
	MotionDataSet         motionData;
	SmartBody::SBMotion*             refMotion;   // reference motion for time warping
	MotionExampleSet      motionExamples;

	// for motion interpolation
	MotionParameter*      motionParameter;		
	DataInterpolator*     dataInterpolator;	
	ResampleMotion*       interpMotion; // pointer to motion interface for generating motion example

	vector<InterpolationExample*>* interpExampleData;
	vector<InterpolationExample*>* resampleData;
	vector<SmartBody::SBJoint*>      affectedJoints; // list of joints that are affected by motion interpolation & IK. 
	// set to the full skeleton by default ( excluding fingers & face bones ).
	std::map<std::string,ReachStateInterface*> stateTable;
	std::map<HandActionState,ReachHandAction*> handActionTable;
	ReachStateInterface*  curReachState;	


	SmartBody::SBJoint*              reachEndEffector;
	ConstraintMap         reachPosConstraint;
	ConstraintMap         reachRotConstraint;
	ConstraintMap         reachNoRotConstraint;

	ConstraintMap         leftFootConstraint, rightFootConstraint;	
	ConstraintMap         handConstraint;

	BodyMotionFrame       inputMotionFrame,ikMotionFrame, idleMotionFrame;

	bool                  initStart;
	double                ikDamp;
	float                 ikReachRegion, ikMaxOffset;	

	MeCtJacobianIK        ik;
	MeCtCCDIK             ikCCD;	
	ReachStateData*       reachData;	
public:
	vector<SrVec>         examplePts,resamplePts;
	HandActionState       curHandActionState;
	float                 fadingWeight, ikDefaultVelocity;
	float                 reachCompleteDuration;
	bool                  footIKFix;
	MeCtIKTreeScenario    ikScenario, ikCCDScenario;

public:
	MeCtReachEngine(SbmCharacter* sbmChar, SmartBody::SBSkeleton* sk);
	virtual ~MeCtReachEngine(void);
	bool isValid() { return valid; }
	std::string     getReachTypeTag();
	int             getReachTypeID();
	ReachStateInterface* getCurrentState() { return curReachState; }
	SbmCharacter*   getCharacter() { return character; }
	ReachStateData* getReachData() { return reachData; }
	MotionParameter* getMotionParameter() { return motionParameter; }
	BodyMotionFrame& outputMotion() { return ikMotionFrame; }
	IKTreeNodeList& ikTreeNodes() { return ikScenario.ikTreeNodes; }
	ConstraintMap&  getHandConstraint() { return handConstraint; }


	bool addHandConstraint(SmartBody::SBJoint* targetJoint, const char* effectorName);
	void updateReach(float t, float dt, BodyMotionFrame& inputFrame, float blendWeight);
	void init(int rtype, SmartBody::SBJoint* effectorJoint);
	void updateMotionExamples(const MotionDataSet& inMotionSet, std::string interpolatorType);
	void solveIK(ReachStateData* rd, BodyMotionFrame& outFrame );
	static int getReachType(const std::string& tag);
protected:
	void updateSkeletonCopy();
	ReachStateInterface* getState(const std::string& stateName);
	SmartBody::SBJoint* findRootJoint(SmartBody::SBSkeleton* sk);
	DataInterpolator* createInterpolator(std::string interpolatorType);
	ResampleMotion*   createInterpMotion();	
	bool hasEffectorRotConstraint(ReachStateData* rd);
};

