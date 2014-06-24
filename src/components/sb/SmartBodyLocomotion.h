#ifndef SMARTBODYLOCOMOTION_H
#define SMARTBODYLOCOMOTION_H

	#include <string>
	#include <vector>
	#include "SmartBodyBehaviors.h"

namespace Ember
{

class SmartBodyManager;

class SmartBodyLocomotion : public SmartBodyBehaviors
{

public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyLocomotion(std::string const &motionPath, std::string const &skeletonRef, SmartBodyManager& manager);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyLocomotion(void);


	/**
	 * @brief Setup the locomotion behavior set.
	 */
	bool setup(bool check = true);

	/**
	 * @brief Retarget the locomotion set to another character.
	 */
	void retarget(SmartBody::SBCharacter& character);


private:

	/**
	 * @brief Create the build joint trajectories for this motion.
	 */
	void setupMotion(SmartBody::SBMotion& motion);

	/**
	 * @brief Gets the name of all the motions contained in the locomotion set.
	 */
	std::vector<std::string> getMotions(void);

	/**
	 * @brief Gets the name of all the motions involved in locomotionSetup.
	 */
	std::vector<std::string> getMotionsForLocomotionSetup(void);

	void locomotionSetup();
	void startingSetup();
	void idleTurnSetup();
	void stepSetup();
	void transitionSetup();
};

}

#endif