#pragma once
#include "controllers/me_ct_data_driven_reach.hpp"
#include "controllers/me_ct_motion_timewarp.hpp"
#include "controllers/me_ct_ublas.hpp"
#include <sb/SBMotion.h>
#include <sb/SBSkeleton.h>
#include <sb/SBJoint.h>

using namespace std;

class InterpolationExample;
class BodyMotionInterface;
class MotionExample;
class MotionParameter;
class MotionProfile;
typedef std::vector<InterpolationExample*> VecOfInterpExample;
typedef std::vector<BodyMotionInterface*> VecOfBodyMotionPtr;
typedef std::vector<MotionExample*> VecOfMotionExamplePtr;
typedef std::pair<int,float> InterpWeight;
typedef std::vector<InterpWeight> VecOfInterpWeight;

/************************************************************************/
/* Motion Interface                                                     */
/************************************************************************/

class BodyMotionFrame
{
public:
	SrVec       rootPos;	
	VecOfSrQuat jointQuat;

	BodyMotionFrame() {};
	virtual ~BodyMotionFrame() {};
	BodyMotionFrame& operator=(const BodyMotionFrame& rhs);
	void setMotionPose(float time, SmartBody::SBSkeleton* skel, const vector<SmartBody::SBJoint*>& affectedJoints, SmartBody::SBMotion* motion, bool retarget = false);
};

class BodyMotionInterface
{
public:
	enum DurationType { DURATION_ACTUAL = 0, DURATION_REF };
	MotionParameter* motionParameterFunc;
	MotionProfile*   motionProfile;
public:
	virtual ~BodyMotionInterface() {}
public:	
	virtual void getMotionParameter(dVector& outPara);	
	virtual double getMotionFrame(float time, SmartBody::SBSkeleton* skel, const vector<SmartBody::SBJoint*>& affectedJoints, BodyMotionFrame& outMotionFrame) = 0;
	virtual SrVec getMotionBaseTranslation(float time, const std::string& baseName) { return SrVec(); }
	virtual SrQuat getMotionBaseRotation(float time, const std::string& baseName) { return SrQuat(); }
	
	virtual double strokeEmphasisTime() = 0;
	virtual double motionDuration(DurationType durType) = 0;
	virtual double motionPercent(float time) = 0; // compute how much percentage of time has elapsed until the motion reach its end
	virtual double getRefDeltaTime(float u, float dt) = 0; // return the du in reference time frame based on current ref time & current acutual dt
};

class BodyMotion : public BodyMotionInterface
{
public:
	SmartBody::SBMotion* motion;
	MotionTimeWarpFunc* timeWarp; // time warp function for the motion
	SrVec rootOffset;
	SrQuat quatP;
public:
	BodyMotion();
	virtual ~BodyMotion();
	virtual double strokeEmphasisTime();
	virtual double getMotionFrame(float time, SmartBody::SBSkeleton* skel, const vector<SmartBody::SBJoint*>& affectedJoints, BodyMotionFrame& outMotionFrame);
	virtual SrVec getMotionBaseTranslation(float time, const std::string& baseName);
	virtual SrQuat getMotionBaseRotation(float time, const std::string& baseName);
	virtual double motionDuration(DurationType durType);	
	virtual double motionPercent(float time);
	virtual double getRefDeltaTime(float u, float dt);	

	void updateRootOffset(SmartBody::SBSkeleton* skel, SmartBody::SBJoint* rootJoint);
};

/************************************************************************/
/* Motion Example                                                       */
/************************************************************************/
class InterpolationExample
{
public:	
	VecOfInterpWeight weight;
	dVector           parameter;
public:
	InterpolationExample() {}
	virtual ~InterpolationExample() {}	
	virtual void getExampleParameter(dVector& outPara) = 0;
};

// axis aligned bounding box of any dimension
class ParameterBoundingBox
{
public:	
	bool        isInit;
	int         numDim;
	
	dVector     minParameter, maxParameter;
public:
	ParameterBoundingBox();
	ParameterBoundingBox(int nD);
	ParameterBoundingBox(ParameterBoundingBox& bBox);
	~ParameterBoundingBox() {}
	void scaleBBox(double scaleRatio);
	void extendBBox(const dVector& inPara);
	void randomPointInBox(dVector& outPara);
	// return the integer hash value based on input parameter value
	int  gridHashing(const dVector& inPara, double cellSize, VecOfInt& adjHash );
protected:
	void initBBox(int nD);
};

class ResampleMotion : public InterpolationExample, public BodyMotionInterface
{
public:
	VecOfBodyMotionPtr* motionDataRef;	
public:
	ResampleMotion(VecOfBodyMotionPtr* motionRef);
	virtual ~ResampleMotion() {}

	virtual double strokeEmphasisTime();
	virtual double getMotionFrame(float time, SmartBody::SBSkeleton* skel, const vector<SmartBody::SBJoint*>& affectedJoints, BodyMotionFrame& outMotionFrame);
	virtual SrVec getMotionBaseTranslation(float time, const std::string& baseName);
	virtual SrQuat getMotionBaseRotation(float time, const std::string& baseName);
	virtual double motionDuration(DurationType durType);	
	virtual void getExampleParameter(dVector& outPara);
	virtual double motionPercent(float time);
	virtual double getRefDeltaTime(float u, float dt);
	MotionProfile* getValidMotionProfile();
};

class MotionExample : public InterpolationExample, public BodyMotion
{
public:	
	MotionExample() {}
	virtual ~MotionExample() {}	
	virtual void getExampleParameter(dVector& outPara);		
};

/************************************************************************/
/* Example Set                                                          */
/************************************************************************/

class ExampleSet
{
protected:
	vector<InterpolationExample*> interpExamples;
	ParameterBoundingBox          parameterBBox;
	int                           paraSize;
public:
	ExampleSet() { paraSize = 0; }
	~ExampleSet() {}
	
	int parameterDim() { return paraSize; }
	void addExample(InterpolationExample* ex);
	vector<InterpolationExample*>& getExamples() { return interpExamples; }
	ParameterBoundingBox& getParameterBBox() { return parameterBBox; }	
	virtual InterpolationExample* createPseudoExample() = 0; // create the interpolation example 
};


class MotionExampleSet : public ExampleSet
{
protected:	
	SmartBody::SBSkeleton*        skeletonRef; // reference to the connected skeleton
	vector<SmartBody::SBJoint*>   affectedJoints; // joints that will be affected by this motion set
	VecOfBodyMotionPtr motionData;	
	vector<MotionExample*> motionExamples;
	vector<InterpolationExample*> resampleExamples;
	MotionParameter*       motionParameterFunc;
public:
	MotionExampleSet() {}
	~MotionExampleSet();

	VecOfBodyMotionPtr* getMotionData() { return &motionData; }
	void initMotionExampleSet(MotionParameter* parameterFunc);
	bool addMotionExample(MotionExample* ex);
	// blend multiple motion examples according to the blending weights. A set of joint quats ( based on affected joints ) are returned at the given ref time 
	double blendMotion(float time, const VecOfInterpWeight& blendWeight, BodyMotionFrame& outMotionFrame);
	virtual InterpolationExample* createPseudoExample();
	MotionExample* getMotionExample(const std::string& motionName);
	
public:
	static double blendMotionFunc(float time, SmartBody::SBSkeleton* skel, const vector<SmartBody::SBJoint*>& joints, const VecOfBodyMotionPtr& motions, 
		                        const VecOfInterpWeight& blendWeight, BodyMotionFrame& outMotionFrame);
	static double blendMotionFuncFast(float time, SmartBody::SBSkeleton* skel, const vector<SmartBody::SBJoint*>& joints, const VecOfBodyMotionPtr& motions, 
		const VecOfInterpWeight& blendWeight, BodyMotionFrame& outMotionFrame);
	// blend start frame to end frame based on blend weight
	static void   blendMotionFrame( BodyMotionFrame& startFrame,  BodyMotionFrame& endFrame, float weight, BodyMotionFrame& outFrame);	
	static void   blendMotionFrameProfile( ResampleMotion* motion, BodyMotionFrame& startFrame,  BodyMotionFrame& endFrame, float weight, BodyMotionFrame& outFrame);
	static float   blendMotionFrameEulerProfile( ResampleMotion* motion, BodyMotionFrame& startFrame,  BodyMotionFrame& endFrame, float scaleFactor, float weight, BodyMotionFrame& outFrame);
};









