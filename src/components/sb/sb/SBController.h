#ifndef _SBCONTROLLER_
#define _SBCONTROLLER_

#include <sb/SBTypes.h>
#include "controllers/me_controller.h"

namespace SmartBody {

class SBController : public MeController
{
	public:
		SBAPI SBController();
		SBAPI SBController(const SBController& controller);
		SBAPI ~SBController();

		SBAPI const std::string& getType();

		SBAPI SBController* getParent();			// how to get parent?
		SBAPI SBController* getChild(int index);
		SBAPI int getNumChildren();
		SBAPI void addChannel(std::string jointName, std::string channelName);

		SBAPI const std::string& getCharacterName();

		SBAPI void setIgnore(bool val);
		SBAPI bool isIgnore();
		SBAPI void setDebug(bool val);
		SBAPI bool isDebug();

		SBAPI void printInfo();
		SBAPI void startRecordSkm(int maxFrame);
		SBAPI void startRecordBvh(int maxFrame);
		SBAPI void writeRecord(std::string prefix);
		SBAPI void stopRecord();
		SBAPI void clearRecord();
		SBAPI double getDuration();

		SBAPI void setTiming(float indt, float outdt, float empht);

		SBAPI void getJointChannelValues(const std::string& jointName, MeFrameData& frame, SrMat& outMat);
		SBAPI void getJointChannelValues(const std::string& jointName, MeFrameData& frame, SrQuat& outQuat, SrVec& outPos);
		SBAPI void setJointChannelQuat(const std::string& jointName, MeFrameData& frame, SrQuat& outQuat);
		SBAPI void setJointChannelPos(const std::string& jointName, MeFrameData& frame, SrVec& outPos);

		SBAPI virtual bool controller_evaluate ( double t, MeFrameData& frame ) { return true;}
		SBAPI virtual double controller_duration () { return 0.0; }
		SBAPI virtual SkChannelArray& controller_channels () { return channelArray; }
		SBAPI virtual const std::string& controller_type () const { return controllerType; }

	private:
		SkChannelArray channelArray;
		std::string controllerType;

};

}

#endif