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

#include "ModelRepresentationHumanoid.h"
#include "Model.h"
#include "ModelAction.h"

#include "components/ogre/MotionManager.h"

#include "components/sb/SmartBodyManager.h"
#include "components/sb/SmartBodyRepresentation.h"
#include "components/sb/SmartBodyAnimation.h"

#include <OgreSkeletonInstance.h>
#include <string>

namespace Ember
{

namespace OgreView
{

namespace Model
{

std::string ModelRepresentationHumanoid::sTypeName("ModelRepresentationHumanoid");


ModelRepresentationHumanoid::ModelRepresentationHumanoid(EmberEntity& entity, Model& model, Scene& scene, EntityMapping::EntityMapping& mapping, SmartBodyManager& sbManager)
	: ModelRepresentation::ModelRepresentation(entity, model, scene, mapping), mCharacter(nullptr), mSbManager(sbManager)
{
}

ModelRepresentationHumanoid::~ModelRepresentationHumanoid()
{
	if (mCharacter)
	{
		//Destroy the SBCharacter.
		mSbManager.removeCharacter(mCharacter);
	}
}

void ModelRepresentationHumanoid::model_Reloaded_der()
{
	//When the model has finally been completely created, we need to create the SmartBody character.
	createSmartBodyCharacter();
}

const std::string& ModelRepresentationHumanoid::getType() const
{
	return sTypeName;
}

const std::string& ModelRepresentationHumanoid::getTypeNameForClass()
{
	return sTypeName;
}

const Ogre::Vector3& ModelRepresentationHumanoid::getTranslation() const
{
	return mCharacter->getTranslation();
}

const Ogre::Quaternion& ModelRepresentationHumanoid::getRotation() const
{
	return mCharacter->getRotation();
}

void ModelRepresentationHumanoid::reinitializeTransformation()
{
	mCharacter->reinitializeTransformation();
}

bool ModelRepresentationHumanoid::isOgreAnimated() const
{
	if (mCharacter)
	{
		return !mCharacter->isAnimated();
	}

	return true;
}

bool ModelRepresentationHumanoid::isMoving() const
{
	return mCharacter ? mCharacter->isMoving() : false;
}

bool ModelRepresentationHumanoid::isStatic() const
{
	return mCharacter ? mCharacter->isStatic() : false;
}

void ModelRepresentationHumanoid::createSmartBodyCharacter()
{
	//Get the Ogre skeleton.	
	Ogre::SkeletonInstance *skeleton = mModel.getSkeleton();

	//Get the name that the SmartBody skeleton should have.
	std::string sbSkName("");
	mSbManager.setCorrespondingSkeletonName(sbSkName, skeleton->getName());

	//If the skeleton exists for SmartBody, then create the corresponding character.
	if (mSbManager.hasSkeleton(sbSkName))
	{
		//Create the model representation set for SmartBody humanoid character.
		mCharacter = mSbManager.createCharacter(*mModel.getSkeletonOwnerEntity(), mModel.getDefinition()->getName(), sbSkName);
	}

	else
	{
		//TODO: Do something to say that there is an error, and if possible change the ModelRepresentationHumanoid to a normal ModelRepresentation.
	}
}

void ModelRepresentationHumanoid::setIdleAnimationTime()
{
}


void ModelRepresentationHumanoid::setAnimation(const std::string& actionName, Action *newAction)
{
	if (mCharacter)
	{
		if (newAction)
		{
			//Remove this from the Ogre Animated entities.
			MotionManager::getSingleton().removeAnimated(mEntity.getId());

			//If the action is supported by SmartBody,
			if (actionName == ACTION_STAND)
			{
				resetAnimations();

				//start the idling animation on this character.
				mSbManager.animate(*mCharacter, SmartBodyAnimation::Name::STANDING);
			}

			else if (actionName == ACTION_WALK)
			{
				resetAnimations();

				//start the walking animation on this character.
				mSbManager.animate(*mCharacter, SmartBodyAnimation::Name::WALKING);
			}

			else if (actionName == ACTION_RUN)
			{
				resetAnimations();

				//start the walking animation on this character.
				mSbManager.animate(*mCharacter, SmartBodyAnimation::Name::RUNNING);
			}

			//Else, stop the SmartBody animations for this character and use the original method.
			else
			{
				mSbManager.freeze(*mCharacter);
				ModelRepresentation::setAnimation(actionName, newAction);
			}
		}

		else
		{
			mSbManager.freeze(*mCharacter);
			ModelRepresentation::setAnimation(actionName, newAction);
		}
	}

	else
	{
		ModelRepresentation::setAnimation(actionName, newAction);
	}
}

}
}
}

			