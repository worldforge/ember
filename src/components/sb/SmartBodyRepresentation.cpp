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

#include "sb/SBScene.h"
#include "sb/SBCharacter.h"
#include "sb/SBSkeleton.h"
#include "sr/sr_quat.h"

#include <OgreSkeletonInstance.h>
#include <OgreEntity.h>
#include <OgreBone.h>

namespace Ember
{

//Constructor that create the SmartBody character.
SmartBodyRepresentation::SmartBodyRepresentation(SmartBody::SBScene& scene, const Ogre::Entity& entity, const std::string& group,
	const std::string& skName, const std::vector<SmartBodyBehaviors*>& behaviors)
:	mScene(scene),
	mEntity(entity), mOgreSkeleton(*entity.getSkeleton()), 
	mCharacter(createCharacter(entity.getName(), group)), mSkeleton(*scene.createSkeleton(skName)),
	mBaseJoint(-1),
	mTranslation(0, 0, 0), mRotation(1, 0, 0, 0), mIsTransformationInit(false), mIsMoving(true),
	mManualMode(false), mIsAnimated(false)
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

	//Initially, the character is set in Ogre animated state.
	setManualControl(false);
}

SmartBodyRepresentation::~SmartBodyRepresentation()
{
	//Free the memory.
//	mScene.removeCharacter(getName());
	//When relogging, the motion manager call SmartBodyManager::updateAnimations, into which SBScene::update will fail, because
	//the bml requests send for this character have not been removed. As long as we cannot properly free them, we should not remove
	//characters from the scene.
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

void SmartBodyRepresentation::setManualControl(bool mode /*= true */)
{
	Ogre::Skeleton::BoneIterator it = mOgreSkeleton.getBoneIterator(); 

	while (it.hasMoreElements()) 
	{ 
		it.getNext()->setManuallyControlled(mode);
	}

	mManualMode = mode;
}

void SmartBodyRepresentation::updateBonePositions()
{
	//Check that mManuallyControl is set to true.
	if (!mManualMode)
		setManualControl(true);
	
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
			if (i == mBaseJoint)
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
	//If mPrvPosition/Orientation have already been initialized, we then will be able to calculate the tranformations applied
	//since the last frame.
	if (mIsTransformationInit)
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

		//If the translation is nul, then we can consider that the representation is static.
		mIsMoving = translation == Ogre::Vector3(0, 0, 0) ? false : true;
	}

	else
	{
		reinitializeTransformation();
	}

	mPrvPosition = position;
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
	mIsTransformationInit = true;
	mTranslation = Ogre::Vector3(0, 0, 0);
	mRotation = Ogre::Quaternion(1, 0, 0, 0);
}

bool SmartBodyRepresentation::isManuallyControlled() const
{
	return mManualMode;
}

void SmartBodyRepresentation::setAnimation()
{
	mIsAnimated = true;
}

void SmartBodyRepresentation::freezeAnimation()
{
	mIsAnimated = false;

	if (mManualMode)
	{
		setManualControl(false);
	}

	//From now on, the character is controlled with Ogre skeletal animations. It may be moving without SmartBody being informed, 
	//we will then have to reinitialized prvPosition the next time a SmartBody animation is launched.
	mIsTransformationInit = false;
}

bool SmartBodyRepresentation::isAnimated() const
{
	return mIsAnimated;
}

bool SmartBodyRepresentation::isTransformationInitialized() const
{
	return mIsTransformationInit;
}

bool SmartBodyRepresentation::isMoving() const
{
	return mIsMoving;
}

const std::string& SmartBodyRepresentation::getName() const
{
	return mCharacter.getName();
}

}