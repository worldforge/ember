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

 	#include "SmartBodyMovingAnimation.h"
 
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
class SmartBodyAnimationInstance;
class SmartBodyGestureAnimationInstance;

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
	 * @brief Returns the name of mCharacter.
	 */
	const std::string& getName() const;

	/**
	 * @brief States that the character is animated using SmartBody engine.
	 * @return mIsAnimated.
	 *
	 * We use it to know if we have to update the skeleton manually.
	 * This is also used to know if the ModelRepresentation linked to this SmartBodyRepresentation should be placed in the world directly,
	 * or if the position determined by the server should be adjusted to fit humanoid movements. 
	 */
	bool isAnimated() const;

	/**
	 * @brief Tranfers the position of the bones from SmartBody to Ogre.
	 * Called by the SmartBodyManager once a frame.
	 */
	void updateBonePositions();

	/**
	 * @brief Returns true if the ModelHumanoidAttachment can use the values of translation and rotation.
	 */
	bool isTransformationInitialized() const;

	/**
	 * @brief Gets the translation of the base joint since the since the last time the node this representation is attached to has 
	 * been updated.
	 * @return mTranslation.
	 */
	const Ogre::Vector3& getTranslation() const;

	/**
	 * @brief Returns the rotation that must be applied on the character.
	 * @return mRotation.
	 */	
	const Ogre::Quaternion& getRotation() const;

	/**
	 * @brief Reinitialize mTranslation and mRotation to identity, and sets mIsTransformationInit to true.
	 */	
	void reinitializeTransformation();

	/**
	 * @brief Allows to know if the character is moving (globally, like walking, running, not idling nor noding for example), to eventually
	 * update or not the position of the scene node.
	 */
	bool isMoving() const;

	/**
	 * @brief If the character is in a static posture, this returns true.
	 */
	bool isStatic() const;

	/**
	 * @brief Sets a new posture for this character.
	 * @param staticPosture: true if posture refers to a SmartBodyStaticAnimation, false if it is to a SmartBodyMovingAnimation.
	 * @return A pointer to the previous posture (needs to be destroyed).
	 * Called by the SmartBodyAnimationManager.
	 *
	 * In static mode: the posture is updated randomly by the SmartBodyAnimationManager. The character then changes his position to
	 * give a more natural effect.
	 *
	 * In moving mode: the character can use four (actually, only three as we don't have backward moving motions yet) different motions
	 * depending on the orientation of the character when moving (~direction). To change from one to another, use setDirection() on this
	 * representation.
	 */
	void setPosture(SmartBodyAnimationInstance *posture, bool staticPosture);

	/**
	 * @brief Gets the posture of the character, in order for the SmartBodyAnimationManager to treat the requests, or destroy the object.
	 * @return mPosture.
	 * Called by the SmartBodyAnimationManager.
	 */
	SmartBodyAnimationInstance* getPosture();

	/**
	 * @brief Adds a new gesture for this character.
	 * Called by the SmartBodyAnimationManager.
	 */
	void addGesture(SmartBodyGestureAnimationInstance *gesture);

	/**
	 * @brief Passes the character in Ogre animated mode. 
	 * @reutrn A pointer to the previous posture, that will be destroyed by the SmartBodyAnimationManager.
	 * Called by the SmartBodyAnimationManager.
	 */
	void leaveAnimatedState();

	/**
	 * @brief If the character is in a moving animation, you can set alterate his orientation with this method.
	 * @see SmartBodyMovingAnimation.h: SmartBodyMovingAnimation::Direction.
	 */
	void setDirection(SmartBodyMovingAnimation::Direction direction);


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
	 * @brief Creates a SmartBody character to fit this representation.
	 * This insures that the character is well created.
	 */
	SmartBody::SBCharacter& createCharacter(const std::string& name, const std::string& group);

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
	 * @brief The index of the base joint in the skeleton.
	 */
	int mBaseJoint;

	/**
	 * @brief Initializes mBaseJoint.
	 *
	 * When updating the skeleton, we must not change the position of the hips (= base joint) as their motion should not be echoed in
	 * the skeleton, but in the scene. Consequently, we have to identify the base joint, and to store its index once for all.
	 */
	void setBaseJointIndex();

	/**
	 * @brief Indicates if the character is currently animated through SmartBody.
	 */
	bool mIsAnimated;

	/**
	 * @brief Sets or unsets manual control over the skeleton of the Ogre entity.
	 */
	void setManualControl(bool mode = true);

	/**
	 * @brief This boolean is set to true if a movement of the base joint has been detected during the last calculateTransformations().
	 */
	bool mIsInMovement;

	/**
	 * @brief Contains the transformations (translation, and rotation) applied on the base bone since the last time the scene node
	 * this representation is attached to has been updated.
	 */
	Ogre::Vector3 mTranslation;
	Ogre::Quaternion mRotation;
	Ogre::Vector3 mLastTranslation; //To calculate mTranslation when the motion duration has elapsed and it is reexecuted from the beginnig.

	/**
	 * @brief The last position taken by the base joint to calculate mTranslation.
	 */
	Ogre::Vector3 mPrvPosition;

	/**
	 * @brief Calculates the transformations applied on the representation since the last frame to know where to put the scene node.
	 */
	void calculateTranformations(const Ogre::Vector3& position, const Ogre::Quaternion& orientation);

	/**
	 * @brief Initializes mPrvPosition to the position of the base joint. 
	 * This must be done everytime we toggle from Ogre skeletal animated mode to SmartBody one.
	 */
	void initializePrvPosition();

	/**
	 * @brief If the character is moving, then, mPosture points on a SmartBodyMovingAnimationInstance. Otherwise, it points on a
	 * SmartBodyStaticAnimationInstance. To finish of, if mIsAnimated is false, it is just null.
	 */
	SmartBodyAnimationInstance *mPosture;

	/**
	 * @brief mIsStatic is true only if mPosture is a SmartBodyStaticAnimationInstance.
	 * Careful, to know if the character has really stopped moving (when going back to an idling posture, there is a blend between the 
	 * previous movement and the idling animation), we must test that (mIsStatic && !mIsInMovement)) is true.
	 */
	bool mIsStatic;

	/**
	 * @brief If the character is animated for the first time, the default behavior would be to blend from the original posture (= T_POSE) to the
	 * new one. To avoid that, we set this variable to true during the construction. Then, when we add an animation for the first time, we specify
	 * that it will not be blended with the previous posture by setting the ready time to 0.
	 */
	bool mIsAnimatedForFirstTime;
};

}

#endif