#ifndef _ME_CT_MOTION_RECORDER_H
#define _ME_CT_MOTION_RECORDER_H

#include <sb/SBController.h>
#include <sb/SBCharacter.h>
#include <string>

class MeCtMotionRecorder : public SmartBody::SBController
{
public:
	static std::string CONTROLLER_TYPE;
	
public:
	MeCtMotionRecorder(SmartBody::SBCharacter* c);
	~MeCtMotionRecorder();

	void startRecording(double frameRate);
	void stopRecording(const std::string& motionName, const std::string& type);
	void writeRecording(const std::string& motionName, const std::string& type);
	void init(SbmPawn* pawn);		
	virtual void controller_map_updated();
    virtual SkChannelArray& controller_channels();
    virtual double controller_duration();
	virtual const std::string& controller_type() const {return CONTROLLER_TYPE;}
	virtual bool controller_evaluate( double t, MeFrameData& frame );

protected:
	SkJoint* findRootJoint( SmartBody::SBSkeleton* skel );

	void getJointChannelValues(const std::string& jointName, MeFrameData& frame, SrQuat& outQuat, SrVec& outPos);
	void setJointChannelQuat(const std::string& jointName, MeFrameData& frame, SrQuat& outQuat);
	void setJointChannelPos(const std::string& jointName, MeFrameData& frame, SrVec& outPos);

protected:
	SmartBody::SBCharacter*		  character;	
	SkChannelArray				  channels;
	bool		                  recordStart;
	std::string			          rootJointName;
	double                        prevTime, startTime;
	double                        recordDt;
};

#endif
