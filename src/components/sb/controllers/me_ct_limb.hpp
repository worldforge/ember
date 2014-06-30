#pragma once
#include "me_ct_IK_scenario.hpp"
#include "sbm/gwiz_math.h"
#include <sk/sk_motion.h>

// Contains the joint chains and rotation info for IK
using namespace std;

class MeCtLimb
{		
	
public:
	SkSkeleton* skeleton;
	MeCtIKScenario ik;	
	vector<SkJoint*> joint_chain;
	vector<MeCtIKJointLimit> joint_chain_limit;
	vector<int>      buf_index;
	vector<SrQuat>   joint_quat; // temp buffer for joint quats from previous frame
public:
	MeCtLimb(void);
	~MeCtLimb(void);

	SkJoint* getChainRoot();
	SkJoint* getChainEndEffector();
	bool hasJoint(SkJoint* joint);
	void init(SkSkeleton* skel);
	// To-Do : build a tree traversal to find directed path between root node and effector node
	bool buildJointChain(const char* root_name, const char* effector_name) { return true; }

	// User provide the joint list and all joint limits. Joint[0] = root, Joint[Size-1] = end effector.
	bool buildJointChain(SrArray<const char*>& joint_names, SrArray<MeCtIKJointLimit>& joint_limit);
	float computeLimbLength();
	void updateQuat(MeFrameData& frame, bool bRead = true);
	void updateQuatToJointChain(SrArray<SrQuat>& joint_quat);
	void updateMotionFrameToJointChain(SkMotion* motion, int frame);
};

template <class T>
void VecToSrArray(const std::vector<T>& inVec, SrArray<T>& outArray)
{
	if (inVec.size() == 0)
		return;

	outArray.capacity(inVec.size());
	outArray.size(inVec.size());
	for (unsigned int i=0;i<inVec.size();i++)
		outArray[i] = inVec[i];
}

template <class T>
void SrArrayToVec(const SrArray<T>& inArray, std::vector<T>& outVec)
{
	if (inArray.size() == 0)
		return;

	outVec.resize(inArray.size());
	for (unsigned int i=0;i<outVec.size();i++)
		outVec[i] = inArray[i];
}

