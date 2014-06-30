#pragma once
#include "controllers/me_ct_data_driven_reach.hpp"
#include "controllers/me_ct_motion_example.hpp"
#include <sb/SBSkeleton.h>
#include <sb/SBJoint.h>

class MotionParameter
{
public:
	SmartBody::SBSkeleton* skeletonRef;
	std::vector<SmartBody::SBJoint*> affectedJoints;
public:
	MotionParameter(SmartBody::SBSkeleton* skel, std::vector<SmartBody::SBJoint*>& joints);
	virtual ~MotionParameter(void);

	virtual void getPoseParameter(const BodyMotionFrame& frame, dVector& outPara) = 0;
	virtual void getMotionFrameParameter(BodyMotionInterface* motion, float refTime, dVector& outPara) = 0;
	virtual void getMotionParameter(BodyMotionInterface* motion, dVector& outPara) = 0;
	virtual SkJoint* getMotionFrameJoint(const BodyMotionFrame& frame, const char* jointName);
};


class ReachMotionParameter : public MotionParameter
{
protected:
	SmartBody::SBJoint* reachJoint;
	SmartBody::SBJoint* rootJoint;
public:
	ReachMotionParameter(SmartBody::SBSkeleton* skel, std::vector<SmartBody::SBJoint*>& joints, SmartBody::SBJoint* rjoint, SmartBody::SBJoint* rootJoint);
	virtual ~ReachMotionParameter();

	virtual void getPoseParameter(const BodyMotionFrame& frame, dVector& outPara);
	virtual void getMotionParameter(BodyMotionInterface* motion, dVector& outPara);
	virtual void getMotionFrameParameter(BodyMotionInterface* motion, float refTime, dVector& outPara);	
};

class LocomotionParameter : public MotionParameter
{
protected:
	std::string baseJointName;	
public:
	LocomotionParameter(SmartBody::SBSkeleton* skel, std::vector<SmartBody::SBJoint*>& joints, const std::string& baseName);
	~LocomotionParameter();

	virtual void getPoseParameter(const BodyMotionFrame& frame, dVector& outPara);
	virtual void getMotionFrameParameter(BodyMotionInterface* motion, float refTime, dVector& outPara);
	virtual void getMotionParameter(BodyMotionInterface* motion, dVector& outPara);	

protected:
	float getMotionSpeed( BodyMotionInterface* motion, const std::string& jointName);
	float getMotionAngularSpeed( BodyMotionInterface* motion, const std::string& jointName);
	float getMotionSpeedAxis( BodyMotionInterface* motion, const std::string& axis, const std::string& jointName);

};

class JumpParameter : public MotionParameter
{
protected:
	std::string baseJointName;	
public:
	JumpParameter(SmartBody::SBSkeleton* skel, std::vector<SmartBody::SBJoint*>& joints, const std::string& baseName);
	~JumpParameter();

	virtual void getPoseParameter(const BodyMotionFrame& frame, dVector& outPara);
	virtual void getMotionFrameParameter(BodyMotionInterface* motion, float refTime, dVector& outPara);
	virtual void getMotionParameter(BodyMotionInterface* motion, dVector& outPara);	

protected:
	float getMotionHeight( BodyMotionInterface* motion, const std::string& jointName);
	float getMotionDirection( BodyMotionInterface* motion, const std::string& jointName);
	float getMotionXZDistance( BodyMotionInterface* motion, const std::string& jointName);

};

