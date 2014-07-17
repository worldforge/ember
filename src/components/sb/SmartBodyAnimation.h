#ifndef SMARTBODYANIMATION_H
#define SMARTBODYANIMATION_H

	#include <string>

namespace Ember
{

/**
 * @brief This class is used as a mapping between the name of an animation and the corresponding bml request.
 *			(not finished).
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 *
 */
class SmartBodyAnimation
{

public:

	/**
	 * @brief The different name an animation can take.
	 */
	enum class Name 
	{
		IDLE, WALK, RUN,			//Body postures.
		NOD, 						//Head animations.

		ANIMATIONS_COUNT			//Gives the number of supported animations.
	};

	/**
	 * @brief Ctor.
	 */
	SmartBodyAnimation(SmartBodyAnimation::Name name);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyAnimation();


	/**
	 * @brief Accessor for the request that is to be sent to the BMLProcessor to execute an animation on a character.
	 * @return mBmlRequest.
	 */
	const std::string& getBmlRequest() const;


private:

	/**
	 * @brief The corresponding request to send to the BMLProcessor to execute an animation on a character.
	 */
	std::string mBmlRequest;
	
};

}

#endif
