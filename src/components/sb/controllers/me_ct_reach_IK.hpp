#pragma once
#include "controllers/me_ct_IK.hpp"
#include "sbm/gwiz_math.h"

class MeCtReachIK :
	public MeCtIK
{
public:
	MeCtReachIK(void);
	~MeCtReachIK(void);
public:	
	virtual void update(MeCtIKScenario* scenario);

public:
	static gwiz::vector_t quat2SwingTwist(gwiz::quat_t& quat);
	static gwiz::quat_t   swingTwist2Quat(gwiz::vector_t& quat);
	float getDt() const { return dt; }
	void setDt(float val) { dt = val; }

	static SrQuat dampQuat(const SrQuat& prevQuat, const SrQuat& nextQuat, float damping_angle);
protected:
	virtual void adjust();		
	virtual void calc_target(SrVec& orientation, SrVec& offset);	
	int check_joint_limit(SrQuat* quat, int index); // new routine for most detailed joint angle limit constraints
	void ccdRotate(SrVec& src, int start_index); // use cyclic coordinate descend to find local minimum.	
	void ccdRotatePositionAndOrientation(SrVec& src, int start_index);
	void particleIK();
	
	void matchSkeleton(SrArray<SrVec>& particleList);

protected: // a hack to get rotation axis re-align	
	float dt; // for constraining rotation speed
	SrArray<SrQuat> cur_quatList;
};
