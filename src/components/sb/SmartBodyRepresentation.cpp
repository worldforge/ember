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

#include "SmartBodyRepresentation.h"
#include "SmartBodyBehaviors.h"
#include "SmartBodyStaticAnimation.h"
#include "SmartBodyMovingAnimation.h"
#include "SmartBodyGestureAnimation.h"
#include "SmartBodyIntuitiveAnimation.h"


#include "sb/SBScene.h"
#include "sb/SBCharacter.h"
#include "sb/SBSkeleton.h"
#include "sr/sr_quat.h"

#include <OgreSkeletonInstance.h>
#include <OgreEntity.h>
#include <OgreBone.h>


namespace Ember
{

SmartBodyRepresentation::SmartBodyRepresentation(SmartBody::SBScene& scene, const Ogre::Entity& entity, const std::string& group,
	const std::string& skName, const std::vector<SmartBodyBehaviors*>& behaviors)
:	mScene(scene), mCharacter(createCharacter(entity.getName(), group)), mSkeleton(*scene.createSkeleton(skName)),
	mEntity(entity), mOgreSkeleton(*entity.getSkeleton()), mIsAnimated(false), mIsInMovement(false), mPosture(nullptr), mIsAnimatedForFirstTime(true)
{
	//Associate the skeleton to the character.
	mCharacter.setSkeleton(&mSkeleton);

	//Add controllers.
	mCharacter.createStandardControllers();

	//Set retargeting parameters.
	for (int i = 0, n = behaviors.size(); i < n; i ++)
	{
		behaviors[i]->applyRetargeting(mCharacter);
	} 

	//Find the index into the skeleton for the base joint.
	setBaseJointIndex();
}

SmartBodyRepresentation::~SmartBodyRepresentation()
{
	//Free the memory.
	delete mPosture;
	mScene.removeCharacter(getName());
}

SmartBody::SBCharacter& SmartBodyRepresentation::createCharacter(const std::string& name, const std::string& group)
{
	SmartBody::SBCharacter* character = mScene.getCharacter(name);
	std::string newName(name);
	int num = 0;

	//If the name of the character has already been used, mScene.createCharacter() will return NULL.
	while (character)
	{
		newName = name + std::to_string(num);
		character = mScene.getCharacter(newName);
		num ++;
	}

	return *mScene.createCharacter(newName, group);
}

void SmartBodyRepresentation::setBaseJointIndex() 
{
	for (int i = 0, n = mSkeleton.getNumJoints(); i < n; i++)
	{
		//We get the joint by its index. 
		SmartBody::SBJoint *joint = mSkeleton.getJoint(i);

		if (joint->getMappedJointName() == "base")
		{
			mBaseJoint = i;	
			return;
		}
	}
}

const std::string& SmartBodyRepresentation::getName() const
{
	return mCharacter.getName();
}

bool SmartBodyRepresentation::isAnimated() const
{
	return mIsAnimated;
}

void SmartBodyRepresentation::setManualControl(bool mode /*= true */)
{
	Ogre::Skeleton::BoneIterator it = mOgreSkeleton.getBoneIterator(); 

	while (it.hasMoreElements()) 
	{ 
		it.getNext()->setManuallyControlled(mode);
	}
}

void SmartBodyRepresentation::updateBonePositions()
{
	//Each bone must be set at the same position than its reference in SmartBody.
	for (int i = 0, n = mSkeleton.getNumJoints(); i < n; i++)
	{
		//We get the joint by its index. 
		SmartBody::SBJoint *joint = mSkeleton.getJoint(i);

		//If it exists, we get the corresponding bone by the name of the joint (which is the one in Ogre, due to the jointMap).
		if (mOgreSkeleton.hasBone(joint->getName()))
		{	
			Ogre::Bone *bone = mOgreSkeleton.getBone(joint->getName());

			//We get the position of the joint.
			SrQuat orientation = joint->quat()->value();

			//The position vector are relative to the original position.
			Ogre::Vector3 position(joint->getPosition().x, joint->getPosition().y, joint->getPosition().z);
			Ogre::Quaternion quaternion(orientation.w, orientation.x, orientation.y, orientation.z);
				quaternion.normalise();

			//If we are looking at the base joint, we shall not change the coordinates of the position vector (it reflects the global
			//movement of the body, and this must be handled through Ember::Model::ModelHumanoidAttachment, not here).
			if (i == mBaseJoint && isMoving())
			{
				calculateTranformations(position, quaternion);	
				bone->setOrientation(quaternion);		
			}

			else
			{
				//We update the bone positions in Ogre.
				bone->setPosition(bone->getInitialPosition() + position);
				bone->setOrientation(quaternion);
			}
		}
	}
}

void SmartBodyRepresentation::calculateTranformations(const Ogre::Vector3& position, const Ogre::Quaternion& orientation)
{
	Ogre::Quaternion rotation;
	Ogre::Vector3 translation;

	if (mPrvPosition.z > position.z) 
	{
		//We suppose that the movement is uniform.
		translation = mLastTranslation;
	}

	else
	{	
		translation = position - mPrvPosition;
		mLastTranslation = translation;
	}

	//For the rotation, we just need to consider the orientation of the joint, relatively to his previous orientation.
	//Right now, the rotation does not render well, it has to be improved before being used.
	rotation = orientation;

	mTranslation += translation;
	mRotation = orientation;
	mPrvPosition = position;

	//If the translation is nul, then we can consider that the representation stopped moving.
	mIsInMovement = translation == Ogre::Vector3(0, 0, 0) ? false : true;
}

const Ogre::Vector3& SmartBodyRepresentation::getTranslation() const
{
	return mTranslation;
}

const Ogre::Quaternion& SmartBodyRepresentation::getRotation() const
{
	return mRotation;
}

void SmartBodyRepresentation::reinitializeTransformation()
{
	mTranslation = Ogre::Vector3(0, 0, 0);
	mRotation = Ogre::Quaternion(1, 0, 0, 0);
}

void SmartBodyRepresentation::initializePrvPosition()
{
	//Gets the current position of the base joint.
	SmartBody::SBJoint *baseJoint = mSkeleton.getJoint(mBaseJoint);
	mPrvPosition = Ogre::Vector3(baseJoint->getPosition().x, baseJoint->getPosition().y, baseJoint->getPosition().z);
}

bool SmartBodyRepresentation::isMoving() const
{
	return (mIsInMovement || !mIsStatic);
}

bool SmartBodyRepresentation::isStatic() const
{
	return mIsStatic;
}

void SmartBodyRepresentation::setPosture(SmartBodyAnimationInstance *posture, bool staticPosture)
{
	if (mIsAnimatedForFirstTime)
	{
		posture->specifyStartTime(true, 0.0f);
		posture->specifyReadyTime(true, 0.0f);
		mIsAnimatedForFirstTime = false;
	}

	mIsStatic = staticPosture;
	mPosture = posture;

	//If we were in Ogre skeletal animated mode, we have to set the manual control and to reinitialize the transformations.
	if (!mIsAnimated)
	{
		setManualControl();
		reinitializeTransformation();
		initializePrvPosition();

		mIsAnimated = true;
	}
}

void SmartBodyRepresentation::setIntuitivePosture(SmartBodyIntuitiveAnimationInstance *posture)
{
	setPosture(posture, false);
	posture->initializePositionAndOrientation(mWorldPosition, mWorldOrientation);
}

SmartBodyAnimationInstance* SmartBodyRepresentation::getPosture()
{
	return mPosture;
}

void SmartBodyRepresentation::addGesture(SmartBodyGestureAnimationInstance *gesture)
{
	//TODO.
}

void SmartBodyRepresentation::leaveAnimatedState()
{
	if (mIsAnimated)
	{
		setManualControl(false);
		mPosture = nullptr;

		mIsAnimated = false;
	}
}

void SmartBodyRepresentation::setDirection(SmartBodyMovingAnimation::Direction direction)
{
	if (mIsAnimated && !mIsStatic)
	{
		SmartBodyMovingAnimationInstance *animation = dynamic_cast<SmartBodyMovingAnimationInstance*>(mPosture);
		animation->setDirection(direction);
	}
}

void SmartBodyRepresentation::setPositionAndOrientation(const Ogre::Vector3& position, const Ogre::Quaternion& orientation)
{
	mWorldPosition = position;
	mWorldOrientation = orientation;
}

const Ogre::Vector3& SmartBodyRepresentation::getWorldPosition() const
{
	return mWorldPosition;
}

const Ogre::Quaternion& SmartBodyRepresentation::getWorldOrientation() const
{
	return mWorldOrientation;
}


}