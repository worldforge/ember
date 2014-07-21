/*
 Copyright (C) 2014 Céline NOËL <celine.noel.7294@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef SMARTBODYLOCOMOTION_H
#define SMARTBODYLOCOMOTION_H

	#include <string>
	#include <vector>
	#include "SmartBodyBehaviors.h"

namespace Ember
{

/**
 * @brief The locomotion behavior. It handles different animations such as idlying, walking, running, but also turning, etc.
 *
 * This class derived from SmartBodyBehaviors.
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 *
 */
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
	~SmartBodyLocomotion();


	/**
	 * @brief Setup the locomotion behavior set.
	 */
	bool setup(SmartBody::SBJointMapManager& jointMapManager);

	/**
	 * @brief Overrides SmartBodyBehaviors::applyRetargeting().
	 * This method does nothing as the retargeting of the locomotion doesn't imply modifying the SBCharacter instance.
	 */
	void applyRetargeting(SmartBody::SBCharacter& character);


private:

	/**
	 * @brief Overrides SmartBodyBehaviors::getMotions().
	 * Gets the name of all the motions contained in the locomotion set.
	 */
	std::vector<std::string> getMotions();

	/**
	 * @brief Gets the name of all the motions involved in locomotionSetup.
	 */
	std::vector<std::string> getMotionsForLocomotionSetup();

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