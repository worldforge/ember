#ifndef _MOTION_ANALYSIS_H_
#define _MOTION_ANALYSIS_H_

#include <sb/SBMotion.h>
#include <sb/SBSkeleton.h>
#include <sb/SBCharacter.h>
#include <sb/SBAnimationState.h>
#include "me_ct_param_animation_utilities.h"
#include "me_ct_motion_example.hpp"
#include "me_ct_ccd_IK.hpp"

struct LegInfo
{
	std::string base, hip;
	std::vector<std::string> supportJoints;
	std::vector<SrVec> supportOffset;
	ConstraintMap ikConstraint;
};

struct LegCycleSample
{	
	std::vector<SrVec> supportPos;	
};

struct LegCycleState
{
	std::vector<SrVec> curSupportPos;
	std::vector<SrVec> globalSupportPos;
	std::vector<SrVec> stanceSupportPos;
	float heightOffset;
	float cycleTime;
	float flightTime;
	float timeToNextCycle;	
	int prevCycle;	
	bool newCycle;
	SrVec curStep, nextStep;
	std::vector<int> prevMotionCycle;
};

struct LocomotionLegCycle
{	
	int cycleIdx;
	float motionStartTime, motionEndTime;
	float cycleStartTime, cycleEndTime;
	float stanceTime;
	float liftTime;
	float liftoffTime;
	float strikeTime;
	float landTime;	

	float normlizedStanceTime;
	float normalizedLiftTime;
	float normalizeLiftoffTime;
	float normalizeStrikeTime;
	float normalizedLandTime;	

	float cycleDuration;	
	std::vector<SrVec> stanceSupportPos;	
	std::vector<LegCycleSample> samples;
public:	
	SrVec getSupportPos(float motionTime, int idx);	
	SrVec getSupportPosNormalize(float normalizeTime, int idx);
	SrVec getStanceSupportPos(int idx);	
	float getTransitionWeight(float motionTime, int& phase);
	float getNormalizedCycleTime(float motionTime);
	float getNormalizedFlightTime(float motionTime);
};


typedef std::vector<LocomotionLegCycle> LegCycleVec;

class LocomotionAnalyzer
{
protected:
	//SmartBody::SBAnimationBlend* locomotionBlend;	
	std::string motionName;
	float startTime, endTime;
	std::map<int,LegCycleVec> legCycleMap;
public:
	std::vector<LegInfo*> legInfos;	
public:
	LocomotionAnalyzer();
	~LocomotionAnalyzer();
	std::string getMotionName();
	void initLegCycles(const std::string& name, SmartBody::SBAnimationBlend* locoBlend, KeyTagMap& keyTag, SmartBody::SBSkeleton* skelCopy);
	LocomotionLegCycle* getLegCycle(int iLeg, float motionTime);	
	LocomotionLegCycle* getLegCycleByIndex(int iLeg, int cycleIdx);
protected:
	double getKeyTagTime(const std::string& key, int iCycle, KeyTag& tag);
	void sampleLegCycle(LegInfo* legInfo, LocomotionLegCycle& legCycle, SmartBody::SBMotion* motion, SmartBody::SBSkeleton* skel, int nSample);
};

class MotionAnalysis
{
protected:		
	std::vector<std::string> motionNames;
	MeCtCCDIK             ikCCD;	
	std::vector<LegInfo*>  legInfos;
	std::vector<LegCycleState> legStates;
	std::vector<LocomotionAnalyzer*> locoAnalyzers;
	SmartBody::SBSkeleton* skelCopy;	
	float skelBaseHeight;	
public:
	MotionAnalysis(void);
	~MotionAnalysis(void);			
	void init(std::string skeletonName, std::string baseJoint, SmartBody::SBAnimationBlend* locomotionBlend, const std::vector<std::string>& motions, std::string motionPrefix);
	void initLegInfos();
#ifdef EMBER_SB_STEER
	void applyIKFix(MeCtIKTreeScenario& ikScenario, SmartBody::SBCharacter* sbChar, std::vector<double>& weights, PATimeManager* timeManager, SrMat worldOffsetMat, SrVec velocity, float angSpeed, BodyMotionFrame& inputFrame, BodyMotionFrame& outFrame);
#endif
protected:
	#ifdef EMBER_SB_HEIGHTFIELD
	float getTerrainYOffset(LegCycleState& state, float flightTime);
	#endif
};

#endif