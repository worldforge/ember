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

#ifndef SMARTBODYREPRESENTATION_H
#define SMARTBODYREPRESENTATION_H

	#include <string>
	#include <vector>
	#include <OgreVector3.h>
	#include <OgreQuaternion.h>

namespace Ogre
{
class Entity;
class SkeletonInstance;
}

namespace SmartBody
{
class SBCharacter;
class SBSkeleton;
class SBScene;
}

namespace Ember
{

class SmartBodyBehaviors;

/**
 * @brief This class gives an interface between SmartBody and Ogre respective representation for characters.
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 *
 */
class SmartBodyRepresentation
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyRepresentation(SmartBody::SBScene& scene, const Ogre::Entity& entity, const std::string& group, const std::string& skName,
		const std::vector<SmartBodyBehaviors*>& behaviors);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyRepresentation();

	/**
	 * @brief Starts a new animation on this character.
	 */
	void setAnimation();

	/**
	 * @brief Stops the animation.
	 */
	void freezeAnimation();

	/**
	 * @brief Returns the value of mIsAnimated.
	 */
	bool isAnimated() const;

	/**
	 * @brief Tranfers the position of the bones from SmartBody to Ogre.
	 */
	void updateBonePositions();

	/**
	 * @brief Gets the translation of the base joint since the since the last time the node this representation is attached to has 
	 * been updated.
	 * @return mTranslation.
	 */
	const Ogre::Vector3& getTranslation() const;

	/**
	 * @brief Same but with the rotation.
	 * @return mRotation.
	 */	
	const Ogre::Quaternion& getRotation() const;

	/**
	 * @brief Reinitialize mTranslation and mRotation to identity, and sets mIsTransformationInit to true.
	 */	
	void reinitializeTransformation();

	/**
	 * @brief Returns true if the ModelHumanoidAttachment can use the values of translation and rotation.
	 */
	bool isTransformationInitialized() const;

	/**
	 * @brief Allows to know if the character is moving (globally, like walking, running, not idling nor noding for example), to eventually
	 * update or not the position of the scene node.
	 */
	bool isMoving() const;

	/**
	 * @brief Returns the name of mCharacter.
	 */
	const std::string& getName() const;


private:

	/**
	 * @brief Reference over the scene (to destruct the character).
	 */
	SmartBody::SBScene& mScene;

	/**
	 * @brief The SmartBody character.
	 */
	SmartBody::SBCharacter& mCharacter;

	/**
	 * @brief The SmartBody skeleton.
	 */
	SmartBody::SBSkeleton& mSkeleton;

	/**
	 * @brief The Ogre entity.
	 */
	const Ogre::Entity& mEntity;

	/**
	 * @brief The Ogre skeleton.
	 */
	Ogre::SkeletonInstance& mOgreSkeleton;

	/**
	 * @brief Contains the transformations (translation, and rotation) applied on the base bone since the last time the scene node
	 * this representation is attached to has been updated.
	 */
	Ogre::Vector3 mTranslation;
	Ogre::Quaternion mRotation;
	Ogre::Vector3 mLastTranslation; //To calculate mTranslation when the loop of the motion has been executed one time.

	/**
	 * @brief The last position taken by the base joint to calculate mTranslation.
	 */
	Ogre::Vector3 mPrvPosition;

	/**
	 * @brief States that the previous has been initialized.
	 */
	bool mIsTransformationInit;

	/**
	 * @brief States that the character is moving (rather being shifted).
	 */
	bool mIsMoving;

	/**
	 * @brief Indicates if the manual control is set or not.
	 */
	bool mManualMode;

	/**
	 * @brief Indicates if the character is currently animated through SmartBody.
	 */
	bool mIsAnimated;

	/**
	 * @brief Calculates the transformations applied on the representation since the last frame to know where to put the scene node.
	 */
	void calculateTranformations(const Ogre::Vector3& position, const Ogre::Quaternion& orientation);

	/**
	 * @brief Sets or unsets manual control over the skeleton of the Ogre entity.
	 */
	void setManualControl(bool mode = true);

	/**
	 * @brief Returns the value of mManualMode.
	 */
	bool isManuallyControlled() const;

};

}

#endif