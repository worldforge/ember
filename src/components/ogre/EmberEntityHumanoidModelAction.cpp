#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberEntityHumanoidModelAction.h"

#include "components/ogre/model/ModelRepresentationHumanoid.h"
#include "components/ogre/EmberOgre.h"


namespace Ember
{

namespace OgreView
{

EmberEntityHumanoidModelAction::EmberEntityHumanoidModelAction(EmberEntity& entity, const std::string& modelName, Scene& scene, EntityMapping::EntityMapping& modelMapping)
	: EmberEntityModelAction::EmberEntityModelAction(entity, modelName, scene, modelMapping)
{
}

EmberEntityHumanoidModelAction::~EmberEntityHumanoidModelAction()
{
}

Model::ModelRepresentation* EmberEntityHumanoidModelAction::createModelRepresentation(Model::Model& model)
{
	SmartBodyManager& sbManager = OgreView::EmberOgre::getSingleton().getSmartBodyManager();
	Model::ModelRepresentationHumanoid *representation = new Model::ModelRepresentationHumanoid(mEntity, model, mScene, mMapping, sbManager);
	
	return representation;
}

}
}