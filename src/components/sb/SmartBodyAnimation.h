#ifndef SMARTBODYANIMATION_H
#define SMARTBODYANIMATION_H

	#include <string>

namespace Ember
{

/**
 * @brief This class is used as a mapping between the name of an animation and the corresponding bml request.
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 *
 */
class SmartBodyAnimation
{


public:

	enum Name 
	{
		IDLE, WALK, RUN, NOD, ANIMATIONS_COUNT
	};

	SmartBodyAnimation(SmartBodyAnimation::Name name);
	~SmartBodyAnimation();

	const std::string& getBmlRequest() const;


private:

	std::string mBmlRequest;
	
};

}

#endif
