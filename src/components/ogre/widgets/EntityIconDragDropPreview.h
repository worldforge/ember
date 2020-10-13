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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_GUIENTITYICONDRAGDROPPREVIEW_H
#define EMBEROGRE_GUIENTITYICONDRAGDROPPREVIEW_H

#include "components/entitymapping/IActionCreator.h"
#include "components/ogre/authoring/MovementAdapter.h"
#include "components/ogre/authoring/IMovementBridge.h"
#include "components/ogre/authoring/EntityMoverBase.h"
#include "components/entitymapping/Actions/Action.h"
#include <wfmath/point.h>
#include <wfmath/quaternion.h>
#include <Atlas/Message/Element.h>
#include <sigc++/connection.h>

namespace Eris {
class ViewEntity;
class Entity;
class Connection;
class TypeInfo;
}

namespace Ember {
namespace OgreView {

class World;

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
	 * @param world The world.
	 */
	explicit EntityIconDragDropPreview(World& world);

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
	 * Returns the position where the preview was dropped in the world.
	 * @return A position.
	 */
	WFMath::Point<3> getDropPosition() const;

	/**
	 * @brief Returns the orientation of the preview model.
	 * @return The orientation of the preview model.
	 */
	WFMath::Quaternion getDropOrientation() const;

	/**
	 * @brief Emitted when we finalize our intended action
	 */
	sigc::signal<void, EmberEntity*> EventEntityFinalized;

private:

	/**
	 * @brief The world.
	 */
	World& mWorld;

	/**
	 * @brief The entity we are generating a preview model for.
	 */
	EmberEntity* mIconEntity;

	/**
	 * @brief The location of the preview model in the world.
	 */
	WFMath::Point<3> mDropPosition;

	/**
	 * @brief The orientation of the preview model.
	 */
	WFMath::Quaternion mDropOrientation;

	/**
	 * @brief Manages the detached entity / preview model used
	 */
	std::unique_ptr<ModelPreviewWorker> mModelPreviewWorker;

    sigc::connection mEntityDeleteConnection;
    sigc::connection mEntityLocationChangeConnection;

};

class ModelPreviewWorker
{
public:
	/**
	 * @brief Ctor.
	 * @param world The world.
	 * @param entity The entity being moved.
	 */
	ModelPreviewWorker(World& world, Eris::ViewEntity& entity);
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
	 * @returns The current position in the world.
	 */
	WFMath::Point<3> getPosition() const;

	/**
	 * @brief Gets the current orientation.
	 * @returns The current orientation.
	 */
	WFMath::Quaternion getOrientation() const;

	const Eris::Entity& getEntity() const;

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
	 * @brief The world.
	 */
	World& mWorld;

	Eris::Entity& mEntity;

	/**
	 * @brief Preview scene node.
	 */
	Ogre::SceneNode* mEntityNode;

	/**
	 * @brief Preview model.
	 */
	std::unique_ptr<Model::Model> mModel;

	/**
	 * @brief Preview model mount.
	 */
	std::unique_ptr<Model::ModelMount> mModelMount;

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
	std::unique_ptr<ModelPreviewWorkerMovement> mMovement;
};

class ModelPreviewWorkerMovementBridge : public Authoring::EntityMoverBase
{
public:

	ModelPreviewWorkerMovementBridge(ModelPreviewWorker& creator, Eris::Entity& entity, Ogre::SceneNode* node);

	~ModelPreviewWorkerMovementBridge() override = default;

	void finalizeMovement() override;

	void cancelMovement() override;

private:
	ModelPreviewWorker& mModelPreviewWorker;

};

class ModelPreviewWorkerMovement
{
public:

	ModelPreviewWorkerMovement(ModelPreviewWorker& mModelPreviewWorker, Camera::MainCamera& camera, Eris::Entity& entity, Ogre::SceneNode* node);
	~ModelPreviewWorkerMovement();

protected:
	Authoring::MovementAdapter mMoveAdapter;
};

}
}

}

#endif
