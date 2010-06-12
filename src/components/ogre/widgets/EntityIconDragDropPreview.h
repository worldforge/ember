//
// C++ Interface: EntityIconDragDropTarget
//
// Description:
//
//
// Author: Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGRE_GUIENTITYICONDRAGDROPPREVIEW_H
#define EMBEROGRE_GUIENTITYICONDRAGDROPPREVIEW_H

#include "components/ogre/authoring/EntityRecipe.h"
#include "components/entitymapping/IActionCreator.h"
#include "components/ogre/authoring/MovementAdapter.h"
#include "components/ogre/authoring/IMovementBridge.h"
#include "components/ogre/authoring/EntityMoverBase.h"
#include "components/entitymapping/Actions/Action.h"
#include <wfmath/point.h>
#include <wfmath/quaternion.h>

namespace Eris {
class Entity;
class Connection;
class TypeInfo;
}

namespace EmberOgre {

namespace Model {
class Model;
class ModelMount;
}

namespace Authoring {
class DetachedEntity;
}

namespace Gui {

class EntityIcon;
class ModelPreviewWorkerMovement;
class ModelPreviewWorker;

/**
@author Tiberiu Paunescu <tpa12@sfu.ca>

@brief Creates a temporary preview model to allow the user to visualise their actions.

An instance of this class can be used to generate a preview of a model visible to the user, based on an existing entity.
A user can cancel the current action or complete it, the action performed for completion can be signaled using EventIconFinalized.

@note Implementation is very similar to that of EntityCreator, may wish to consolidate similarities.
*/
class EntityIconDragDropPreview {
public:
	/**
	 * @brief Constructor
	 * @param conn Connection between the server and the client.
	 */
	EntityIconDragDropPreview();

	/**
	 * @brief Destructor
	 */
	virtual ~EntityIconDragDropPreview();

	/**
	 * @brief Begins the creation of preview model.
	 * @param icon The icon of the entity for which the preview is generated.
	 * @note Will be changed to take EmberEntity as input.
	 */
	void createPreview(EntityIcon* icon);

	/**
	 * @brief Clean up after finalizing or cancelling the user action.
	 */
	void cleanupCreation();

	/**
	 * @brief Notifies of action commitment.
	 */
	void finalizeCreation();

	/**
	 * @brief Returns the offset between the preview model and the avatar.
	 * @return Vector of the offset between the preview model and the avatar.
	 */
	WFMath::Vector<3> getDropOffset();



	/**
	 * @brief Emitted when we finalize our intended action
	 */
	sigc::signal<void, EmberEntity*> EventEntityFinalized;

private:
	/**
	 * @brief The entity we are generating a preview model for.
	 */
	EmberEntity* mIconEntity;

	/**
	 * @brief The location of the preview model to the avatar
	 */
	WFMath::Vector<3> mDropOffset;

	/**
	 * @brief Manages the detached entity / preview model used
	 */
	ModelPreviewWorker *mModelPreviewWorker;

};

class ModelPreviewWorker
{
public:
	ModelPreviewWorker(Eris::Entity* entity);
	~ModelPreviewWorker();
	/**
	 * @brief Sets model information based on the entity type.
	 * @param modelName The name of the model.
	 */
	void setModel(const std::string& modelName);

	/**
	 * @brief Shows preview model part.
	 */
	void showModelPart(const std::string& partName);

	/**
	 * @brief Hide preview model part.
	 */
	void hideModelPart(const std::string& partName);

	/**
	 * @brief Get the current position of the world
	 */
	const WFMath::Point<3> getPosition();

	sigc::signal<void> EventFinalizeCreation;
	sigc::signal<void> EventCleanupCreation;

protected:
	/**
	 * @brief Sets preview node properties basing on model. Code from EmberPhysicalEntity.
	 */
	void initFromModel();

	/**
	 * @brief Applies correct scaling basing on model definition. Code from EmberPhysicalEntity.
	 */
	void scaleNode();

	/**
	 * @brief Called when the model is reloaded, which also happens if background loading is enabled and the model has been loaded in the background.
	 */
	void model_Reloaded();

	/**
	 * @brief Returns true if our detached entity has a bounding box.
	 * @return Does the detached entity have a bounding box.
	 */
	bool hasBBox();

	/**
	 * @brief Returns the bounding box our detached entity.
	 * @return The bounding box.
	 */
	const WFMath::AxisBox<3> & getBBox();

private:

	/**
	 * @brief Detached entity that is used in process of creating preview.
	 */
	Authoring::DetachedEntity* mEntity;

	/**
	* @brief Message that is composed from attributes of the entity we're creating a preview for.
	*/
	Atlas::Message::MapType mEntityMessage;

	/**
	 * @brief Preview scene node.
	 */
	Ogre::SceneNode* mEntityNode;

	/**
	 * @brief Preview model.
	 */
	Model::Model* mModel;

	/**
	 * @brief Preview model mount.
	 */
	Model::ModelMount* mModelMount;

	/**
	 * @brief Current position of preview in the world.
	 */
	WFMath::Point<3> mPos;

	/**
	 * @brief Current orientation of preview in the world.
	 * @note We don't change the orientation.
	 */
	WFMath::Quaternion mOrientation;

	/**
	 * @brief Movement adapter for our preview model
	 */
	ModelPreviewWorkerMovement* mMovement;
};

/**
 * @brief Shows or hides specific model part in entity creator preview.
 */
class ModelPreviewWorkerPartAction : public Ember::EntityMapping::Actions::Action
{
public:
	/**
	 * @brief Constructor
	 * @param entityIconDragDropPreview The preview model class that holds the model
	 * @param partName The specific model part to monitor
	 */
	ModelPreviewWorkerPartAction(ModelPreviewWorker& modelPreviewWorker, std::string partName);

	/**
	 * @brief Desctructor
	 */
	~ModelPreviewWorkerPartAction();

	/**
	 * @brief Shows specific model part. Called by model mapping framework.
	 */
	virtual void activate(Ember::EntityMapping::ChangeContext& context);

	/**
	 * @brief Hides specific model part. Called by model mapping framework.
	 */
	virtual void deactivate(Ember::EntityMapping::ChangeContext& context);
protected:

	/**
	 * @brief The preview model class that holds the model
	 */

	ModelPreviewWorker& mModelPreviewWorker;
	/**
	 * @brief The specific model part to hide/show
	 */
	std::string mPartName;
};

/**
 * @brief Hides specific model in entity creator preview.
 */
class ModelPreviewWorkerHideModelAction : public Ember::EntityMapping::Actions::Action
{
public:
	/**
	 * @brief Constructor
	 * @param entityIconDragDropPreview The preview model class that holds the model
	 */
	ModelPreviewWorkerHideModelAction(ModelPreviewWorker& modelPreviewWorker);

	/**
	 * @brief Destructor
	 */
	virtual ~ModelPreviewWorkerHideModelAction();

	/**
	 * @brief Hides model. Called by model mapping framework.
	 */
	virtual void activate(Ember::EntityMapping::ChangeContext& context);

	/**
	 * @brief Does nothing. Called by model mapping framework.
	 */
	virtual void deactivate(Ember::EntityMapping::ChangeContext& context);
protected:

	/**
	 * @brief The preview model class that holds the model
	 */
	ModelPreviewWorker& mModelPreviewWorker;
};

/**
 * @brief Shows the specific model in entity creator preview.
 */
class ModelPreviewWorkerModelAction : public Ember::EntityMapping::Actions::Action
{
public:
	/**
	 * @brief Constructor
	 * @param entityIconDragDropPreview The preview model class that holds the model
	 * @param partName The specific model to monitor
	 */
	ModelPreviewWorkerModelAction(ModelPreviewWorker& modelPreviewWorker, std::string modelName);

	/**
	 * @brief Destructor
	 */
	~ModelPreviewWorkerModelAction();

	/**
	 * @brief Shows specific model. Called by model mapping framework.
	 */
	virtual void activate(Ember::EntityMapping::ChangeContext& context);

	/**
	 * @brief Hides model. Called by model mapping framework.
	 */
	virtual void deactivate(Ember::EntityMapping::ChangeContext& context);
protected:
	/**
	 * @brief The preview model class that holds the model
	 */
	ModelPreviewWorker& mModelPreviewWorker;

	/**
	 * @brief The specific model to hide/show
	 */
	std::string mModelName;
};

	/**
	 * @brief Class that controls the visibility of the preview model
	 */
class ModelPreviewWorkerActionCreator  : public Ember::EntityMapping::IActionCreator {
public:
	/**
	 * @brief Constructor
	 * @param entityIconDragDrop Preview model class that holds the model
	 */
	ModelPreviewWorkerActionCreator(ModelPreviewWorker& modelPreviewWorker);

	/**
	 * @brief Destructor
	 */
	~ModelPreviewWorkerActionCreator();

	/**
	 * @brief Creates the actions we can perform on the model
	 * @param modelMapping
	 * @param aCase
	 * @param caseDefinition
	 */
	virtual void createActions(Ember::EntityMapping::EntityMapping& modelMapping, Ember::EntityMapping::Cases::CaseBase* aCase, Ember::EntityMapping::Definitions::CaseDefinition& caseDefinition);
protected:

	/**
	 * @brief Preview model class that holds the model
	 */
	ModelPreviewWorker& mModelPreviewWorker;
};

class ModelPreviewWorkerMovementBridge : public ::EmberOgre::Authoring::EntityMoverBase
{
public:

	ModelPreviewWorkerMovementBridge(ModelPreviewWorker& creator, Authoring::DetachedEntity& entity, Ogre::SceneNode* node);
	virtual ~ModelPreviewWorkerMovementBridge();

	virtual void finalizeMovement();
	virtual void cancelMovement();

private:
	ModelPreviewWorker& mModelPreviewWorker;

};

class ModelPreviewWorkerMovement
{
public:

	ModelPreviewWorkerMovement(ModelPreviewWorker& mModelPreviewWorker, const Camera::MainCamera& camera, Authoring::DetachedEntity& entity, Ogre::SceneNode* node);
	~ModelPreviewWorkerMovement();

protected:
	Authoring::MovementAdapter mMoveAdapter;
};

}
}

#endif
