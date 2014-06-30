#pragma once
#include "me_ct_reach.hpp"
#include "sr/sr_box.h"
#include <assert.h>




class PoseExample;
typedef std::vector<PoseExample> VecOfPoseExample;
typedef std::vector<SrQuat> VecOfSrQuat;
typedef std::vector<double> VecOfDouble;
typedef std::vector<int>    VecOfInt;


#if 0 

using namespace std;

class PoseExample
{
public:
	VecOfSrQuat jointQuat; // joint configuration
	VecOfDouble poseParameter; // parameter for searching KNN, use end effector position by default
public:
	PoseExample() {};
	~PoseExample() {};
	PoseExample& operator=(const PoseExample& rhs);
};

class PoseExampleSet
{
protected:	
	VecOfPoseExample poseData;		
	ANNpointArray dataPts; // data points	
	ANNkd_tree* kdTree; // search structure
public:
	PoseExampleSet();
	~PoseExampleSet();	

	const VecOfPoseExample& PoseData() const { return poseData; }

	void buildKDTree();
	void kdTreeKNN(const vector<double>& parameter, vector<float>& dists, vector<PoseExample*>& KNN, int nK);
	int linearKNN(const vector<double>& parameter, vector<float>& dists, vector<PoseExample*>& KNN, int nK);	

	bool addPose(const PoseExample& poseEx, float fMinDist = -1.f);
	SrBox computeBBox();
	void clearData();
};

// MeCtDataDrivenReach improves the IK result by interpolating the example motions 
// Based on end-effector position ( or any other designated pose parameters ), a pose is predicted from examples
// The predicted pose is then used to guide the traditional IK ( Use as starting guess for CCD, or as the reference pose for other IK solver. )

class MeCtDataDrivenReach :
	public MeCtReach
{
protected:
	MotionDataSet motionData; // training motions to improve IK results
	PoseExampleSet examplePoseData; // extracted example poses from training motions			
	PoseExampleSet resampledPosedata;		
public:
	// a hack for debugging only
	bool useDataDriven;
	bool useIK;
public:
	MeCtDataDrivenReach(SkSkeleton* sk);
	~MeCtDataDrivenReach(void);
	const PoseExampleSet& ResampledPosedata() const { return resampledPosedata; }
	const PoseExampleSet& ExamplePoseData() const { return examplePoseData; }	

	void updateExamplesFromMotions(const MotionDataSet& inMotionSet, bool rebuild = false, float minDist = 5.0);	
	void buildResamplePoseData(int nExamples, float fMinDist = 1.0);
	virtual bool controller_evaluate( double t, MeFrameData& frame );

	static float Random(float r_min, float r_max);
private:
	void getPoseParameter(vector<double>& para, SkSkeleton* skeleton);
	void getPoseExampleFromSkeleton(PoseExample& pose);		
	
	static void blendPose(vector<SrQuat>& blendPoses, vector<float>& KNNweights, vector<PoseExample*>& KNNPoses);		
	static void computeWeightFromDists(vector<float>& dists, vector<float>& outWeights);
	static void generateRandomWeight(int nK, vector<float>& outWeights);	
	static SrVec randomPointInBox(SrBox& box);	
	static SrVec getWorldPos(SkJoint* joint);
};

#endif

template <class T> void Plus(const vector<T>& A, const vector<T>& B, vector<T>& Out, double ratio)
{
	assert(A.size() == B.size());
	Out.resize(A.size());
	for (unsigned int i=0;i<A.size();i++)
	{
		Out[i] = A[i] + B[i]*ratio;		
	}
}

template <class T, class S> void pairToVec(const vector<pair<T,S> >& A, vector<T>& outT, vector<S>& outS)
{
	outT.resize(A.size());
	outS.resize(A.size());
	for (unsigned int i=0;i<A.size();i++)
	{
		outT[i] = A[i].first;
		outS[i] = A[i].second;
	}
}
template <class T, class S> void vecToPair(vector<T>& vecT, vector<S>& vecS, vector<pair<T,S> >& outVec)
{
	int numElem = vecT.size() < vecS.size() ? vecT.size() : vecS.size();
	outVec.resize(numElem);
	for (unsigned int i=0;i<outVec.size();i++)
	{
		outVec[i].first = vecT[i];
		outVec[i].second = vecS[i];
	}
}


template <class T>
T Norm2(vector<T>& Out)
{
	T norm = 0.0;
	for (unsigned int i=0;i<Out.size();i++)
		norm += Out[i]*Out[i];
	return norm;
};

template <class T>
T Dist(const vector<T>& A, const vector<T>& B)
{
	vector<T> diff;
	Plus(A,B,diff,-1.0);
	return Norm2(diff);
}








