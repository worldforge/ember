#include "ModelRepresentationHumanoid.h"
#include "Model.h"

#include "components/sb/SmartBodyManager.h"

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
	//When the model has finally been completely created, we need to create the SmartBody character.
	model.Reloaded.connect(sigc::mem_fun(*this, &ModelRepresentationHumanoid::createSmartBodyCharacter));
}

ModelRepresentationHumanoid::~ModelRepresentationHumanoid(void)
{
	if (mCharacter)
	{
		//Destroy the SBCharacter.
		mSbManager.removeCharacter(mCharacter);
	}
}

const std::string& ModelRepresentationHumanoid::getType(void) const
{
	return sTypeName;
}

const std::string& ModelRepresentationHumanoid::getTypeNameForClass(void)
{
	return sTypeName;
}

void ModelRepresentationHumanoid::createSmartBodyCharacter(void)
{
	//Get the Ogre skeleton.
	Ogre::SkeletonInstance *skeleton = mModel.getSkeleton();

	//Get the name that the SmartBody skeleton should have.
	std::string sbSkName("");
	mSbManager.setCorrespondingSkeletonName(sbSkName, skeleton->getName());

	//If the skeleton exists for SmartBody, then create the corresponding character.
	if (mSbManager.hasSkeleton(sbSkName, true))
	{
		//Create the model representation set for SmartBody humanoid character.
		mCharacter = mSbManager.createCharacter(*mModel.getSkeletonOwnerEntity(), mModel.getDefinition()->getName(), sbSkName);
	}

	else
	{
		//TODO: Do something to say that there is an error, and if possible change the ModelRepresentationHumanoid to a normal ModelRepresentation.
	}
}


}
}
}

			