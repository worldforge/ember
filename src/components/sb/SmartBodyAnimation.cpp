#include "SmartBodyAnimation.h"


namespace Ember
{

SmartBodyAnimation::SmartBodyAnimation(SmartBodyAnimation::Name name)
{
	switch (name)
	{
		case IDLE:
			mBmlRequest = std::string("<body posture=\"ChrUtah_Idle001\"/>");
			break;

		case WALK:
			mBmlRequest = std::string("<body posture=\"ChrUtah_Walk001\"/>");
			break;

		case RUN:
			mBmlRequest = std::string("<body posture=\"ChrUtah_Run001\"/>");
			break;

		case NOD:
			mBmlRequest = std::string("<head type=\"NOD\" repeats=\"2\"/>");
			break;
	}
}

SmartBodyAnimation::~SmartBodyAnimation(void)
{
}


const std::string& SmartBodyAnimation::getBmlRequest(void) const
{
	return mBmlRequest;
}

}