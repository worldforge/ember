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

bool ModelRepresentationHumanoid::isOgreAnimated() const
{
	return !mCharacter->isAnimated();
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
				mSbManager.animate(*mCharacter, SmartBodyAnimation::Name::IDLE);
			}

			else if (actionName == ACTION_WALK)
			{
				resetAnimations();

				//start the walking animation on this character.
				mSbManager.animate(*mCharacter, SmartBodyAnimation::Name::WALK);
			}

			else if (actionName == ACTION_RUN)
			{
				resetAnimations();

				//start the walking animation on this character.
				mSbManager.animate(*mCharacter, SmartBodyAnimation::Name::RUN);
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

			