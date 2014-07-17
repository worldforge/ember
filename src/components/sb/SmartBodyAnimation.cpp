#include "SmartBodyAnimation.h"


namespace Ember
{

SmartBodyAnimation::SmartBodyAnimation(SmartBodyAnimation::Name name)
{
	switch (name)
	{
		case Name::IDLE:
			mBmlRequest = std::string("<body posture=\"ChrUtah_Idle001\"/>");
			break;

		case Name::WALK:
			mBmlRequest = std::string("<body posture=\"ChrUtah_Walk001\"/>");
			break;

		case Name::RUN:
			mBmlRequest = std::string("<body posture=\"ChrUtah_Run001\"/>");
			break;

		case Name::NOD:
			mBmlRequest = std::string("<head type=\"NOD\" repeats=\"2\"/>");
			break;
	}
}

SmartBodyAnimation::~SmartBodyAnimation()
{
}


const std::string& SmartBodyAnimation::getBmlRequest() const
{
	return mBmlRequest;
}

}