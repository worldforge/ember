#ifndef EMBEROGREEMBERENTITYHUMANOIDMODELACTION_H
#define EMBEROGREEMBERENTITYHUMANOIDMODELACTION_H

#include "EmberEntityModelAction.h"

namespace Ember
{
class EmberEntity;

namespace EntityMapping
{
class EntityMapping;
}

namespace OgreView
{
class Scene;

namespace Model
{
class Model;
class ModelRepresentation;
}

/**
 * @brief Show a certain humanoid Model.
 * @author Céline Noël <celine.noel.7294@gmail.com>
 */
class EmberEntityHumanoidModelAction: public EmberEntityModelAction
{
public:

	/**
	 * @brief Ctor.
	 */
	EmberEntityHumanoidModelAction(EmberEntity& entity, const std::string& modelName, Scene& scene, EntityMapping::EntityMapping& modelMapping);

	/**
	 * @brief Dtor.
	 */
	virtual ~EmberEntityHumanoidModelAction();


protected:
	
	/**
	 * @brief Creates the corresponding ModelRepresentationHumanoid.
	 */
	Model::ModelRepresentation* createModelRepresentation(Model::Model& model);
};

}

}

#endif