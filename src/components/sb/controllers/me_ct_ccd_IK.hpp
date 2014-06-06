#pragma once
#include "controllers/me_ct_jacobian_IK.hpp"

// feng : this class is basically a re-write for MeCtReachIK ( a CCD based IK solver derived from MeCtIK, with more shared code 
// and better readibility. By using the tree structure by default ( since IK-chain is just a special case of the tree ), this provides 
// potential extension to handling & blending multiple IK chains in the solver.

class MeCtCCDIK : public MeCtIKInterface	
{	
protected:	
	MeCtIKTreeScenario* ikScenario;
public:
	MeCtCCDIK(void);
	~MeCtCCDIK(void);
public:
	virtual void update(MeCtIKTreeScenario* scenario);	
	static gwiz::vector_t quat2SwingTwist(gwiz::quat_t& quat);
	static gwiz::quat_t   swingTwist2Quat(gwiz::vector_t& quat);
protected:
	void CCDUpdate(MeCtIKTreeNode* startNode, EffectorConstraint* con);
	// perform a single rotation based on startNode and endEffector
	void CCDIteration(MeCtIKTreeNode* startNode, EffectorConstraint* con);
	// sort constraint list according to the end effector node level, so we process the higher level effector first.
	void sortConstraint(ConstraintMap* conMap, VecOfConstraintPtr& sortConList);
};
