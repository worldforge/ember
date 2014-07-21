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
	mCharacter(*scene.createCharacter(entity.getName(), group)), mSkeleton(*scene.createSkeleton(skName)),
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

	//Initially, the character is set in Ogre animated state.
	setManualControl(false);
}

SmartBodyRepresentation::~SmartBodyRepresentation()
{
	//Free the memory.
	mScene.removeCharacter(getName());
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

			//We update the bone positions in Ogre.
			bone->setPosition(bone->getInitialPosition() + position);
			bone->setOrientation(quaternion);
		}
	}
}

bool SmartBodyRepresentation::isManuallyControlled() const
{
	return mManualMode;
}

void SmartBodyRepresentation::setAnimatedState(bool isAnimated)
{
	mIsAnimated = isAnimated;

	if (!isAnimated && mManualMode)
		setManualControl(false);
}

bool SmartBodyRepresentation::isAnimated() const
{
	return mIsAnimated;
}

const std::string& SmartBodyRepresentation::getName() const
{
	return mCharacter.getName();
}

}