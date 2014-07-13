#ifndef SMARTBODYLOCOMOTION_H
#define SMARTBODYLOCOMOTION_H

	#include <string>
	#include <vector>
	#include "SmartBodyBehaviors.h"

namespace Ember
{

class SmartBodyLocomotion : public SmartBodyBehaviors
{

public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyLocomotion(std::string const &motionPath, std::string const &skeletonRef, SmartBody::SBAssetManager& assetMng,
		SmartBody::SBAnimationBlendManager& blendMng, SmartBody::SBRetargetManager& retargetMng);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyLocomotion(void);


	/**
	 * @brief Setup the locomotion behavior set.
	 */
	bool setup(SmartBody::SBJointMapManager& jointMapManager);

	/**
	 * @brief Add the given contraints to retarget locomotion on another character.
	 */
	void applyRetargeting(SmartBody::SBCharacter& character);


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

	/**
	 * @brief Setup the locomotion behavior one step at a time.
	 */
	void locomotionSetup();
	void startingSetup();
	void idleTurnSetup();
	void stepSetup();
	void transitionSetup();
};

}

#endif