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

#ifndef SMARTBODYMANAGER_H
#define SMARTBODYMANAGER_H

	#include "SmartBodyAnimation.h"

	#include <string>
	#include <vector>

namespace SmartBody
{

class SBScene;
class SBCharacter;
class SBAssetManager;
class SBSimulationManager;
class SBBmlProcessor;

}

namespace Ogre
{
class Entity;
}

namespace Ember
{

class SmartBodyBehaviors;
class SmartBodyRepresentation;
class SmartBodyAnimation;

/**
 @brief This class is here to set up the SmartBody library.

 Its main function is to bound the Ogre skeletons to SmartBody's ones, in order to animate the humanoid characters like real persons. Every call to
 SmartBody has to be done threw this class.

 @author Céline NOEL <celine.noel.7294@gmail.com>

 */
class SmartBodyManager
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyManager();

	/**
	 * @brief Dtor.
	 */
	~SmartBodyManager();

	/**
	 * @brief Initializes the object, and consequently, the SmartBody library.
	 */
	void initialize(double startTime);

	/**
	 * @brief Sets sbName to the name of the SmartBody skeleton corresponding to ogreName (returns false if not possible).
	 */
	bool setCorrespondingSkeletonName(std::string& sbName, const std::string& ogreName);

	/**
	 * @brief Returns true if this Ogre skeleton possesses its equivalent in SmartBody by checking through the assets if it has been loaded.
	 */
	bool hasSkeleton(const std::string& ogreName);

	/**
	 * @brief Creates a new SmartBody character and initialize him (skeleton, behaviors, etc.).
	 */
	SmartBodyRepresentation* createCharacter(const Ogre::Entity& entity, const std::string& group, const std::string& sbSkName);

	/**
	 * @brief Removes a character from the scene.
	 */
	void removeCharacter(SmartBodyRepresentation *character);

	/**
	 * @brief Updates the scene.
	 */
	void updateAnimations(double timeSlice);

	/**
	 * @brief Launches an animation on a character.
	 */
	void animate(SmartBodyRepresentation& character, SmartBodyAnimation::Name animation);

	/**
	 * @brief Freezes all animations on this character and toggles to Ogre animated mode. 
	 */
	void freeze(SmartBodyRepresentation& character) const;


private:

	/**
	 * @brief The SmartBody scene.
	 */
	SmartBody::SBScene& mScene;

	/**
	 * @brief SmartBody asset manager.
	 */
	SmartBody::SBAssetManager& mAssetManager;

	/**
	 * @brief SmartBody simulation object.
	 */
	SmartBody::SBSimulationManager& mSimulation;

	/**
	 * @brief SmartBody BML Processor.
	 */
	SmartBody::SBBmlProcessor& mProcessor;

	/**
	 * @brief A vector containing all the characters created by this manager.
	 */
	std::vector<SmartBodyRepresentation*> mCharacters;

	/**
	 * @brief The collection of all the supported behaviors.
	 */
	std::vector<SmartBodyBehaviors*> mBehaviors;

	/**
	 * @brief The different possible animations.
	 */
	std::vector<SmartBodyAnimation> mAnimations;

	/**
	 * @brief An enum to locate each behavior in mBehaviors.
	 */
	enum BehaviorType
	{
		BHV_LOCOMOTION, BHV_SIZE
	};

	/**
	 * @brief Boolean stating that the manager has been initialized correctly.
	 */
	bool mIsInit;


	/**
	 * @brief Load all the skeletons situated in motions/skeletons.
	 */
	void loadSkeletonAssets();

	/**
	 * @brief Loads and set up all the behaviors.
	 */
	void loadAllBehaviors();

};

}

#endif