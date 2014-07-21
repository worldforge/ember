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

#ifndef SMARTBODYBEHAVIORS_H
#define SMARTBODYBEHAVIORS_H

	#include <string>
	#include <vector>


namespace SmartBody
{

class SBCharacter;
class SBAssetManager;
class SBAnimationBlendManager;
class SBRetargetManager;
class SBMotion;
class SBJointMapManager;

}

namespace Ember
{

/**
 * @brief A behavior set is a group of motions.
 *
 * To load a behavior set, you have to create a SmartBodyBehaviors object, then use setup(). In order to use these animations on another character.
 * use retarget() on its skeleton.
 *
 * This class is abstract. Right now, only SmartBodyLocomotion and SmartBodyGestures derived from it. 
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 *
 */
class SmartBodyBehaviors
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyBehaviors(const std::string& motionPath, const std::string& skeletonRef, SmartBody::SBAssetManager& assetMng,
		SmartBody::SBAnimationBlendManager& blendMng, SmartBody::SBRetargetManager& retargetMng);

	/**
	 * @brief Dtor.
	 */
	virtual ~SmartBodyBehaviors();

	/**
	 * @brief Check that all assets have been correctly loaded and setup the behavior set.
	 */
	virtual bool setup(SmartBody::SBJointMapManager& jointMapManager) = 0;

	/**
	 * @brief Retarget the behavior set from the skeleton reference to another character (setup must have been called first).
	 */
	void retarget(const std::string& skName);

	/**
	 * @brief If necessary, add some contraints on the character to enable the retargeting.
	 */
	virtual void applyRetargeting(SmartBody::SBCharacter& character) = 0;


protected:

	/**
	 * @brief Name of the skeleton this motion set is made for.
	 */
	std::string mSkelRefName;

	/**
	 * @brief Reference to the asset manager.
	 */
	SmartBody::SBAssetManager& mAssetManager;

	/**
	 * @brief Reference to the animation blend manager.
	 */
	SmartBody::SBAnimationBlendManager& mBlendManager;

	/**
	 * @brief Reference to the retarget manager.
	 */
	SmartBody::SBRetargetManager& mRetargetManager;

	/**
	 * @brief Stating that setup has been effectuated.
	 */
	bool mSetup;


	/**
	 * @brief Checks that the assets have been correctly loaded and map the skeleton reference.
	 */
	bool setupAssets(SmartBody::SBJointMapManager& jointMapMng);

	/**
	 * @brief Sets up the behaviors.
	 */
	void setupBehaviors();

	/**
	 * @brief Gets all the motions constituing this behavior set.
	 */
	virtual std::vector<std::string> getMotions() = 0;

	/**
	 * @brief Checks that each asset has been correctly loaded (skeleton + motions).
	 */
	bool assetsExist();
};

}

#endif
