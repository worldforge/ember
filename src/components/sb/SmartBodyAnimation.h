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
		IDLE, WALK, RUN, ANIMATIONS_COUNT
	};

	SmartBodyAnimation(SmartBodyAnimation::Name name);
	~SmartBodyAnimation(void);

	const std::string& getBmlRequest(void) const;


private:

	std::string mBmlRequest;
	
};

}

#endif
