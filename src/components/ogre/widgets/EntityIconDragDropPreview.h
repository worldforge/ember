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
class EntityIconDragDropPreviewMovement;

/**
@author Tiberiu Paunescu <tpa12@sfu.ca>

@brief Creates a temporary preview model to allow the user to visualise their actions.

An instance of this class can be used to generate a preview of a model visible to the user, based on an existing entity.
A user can cancel the current action or complete it, the action performed for completion can be signaled using EventIconFinalized

@note implementation is very similar to that of EntityCreator, may wish to consolidate similarities
*/
class EntityIconDragDropPreview {
public:
	/**
	 * @brief Constructor
	 * @param conn Connection between the server and the client
	 */
	EntityIconDragDropPreview(Eris::Connection& conn);

	/**
	 * @brief Desctructor
	 */
	virtual ~EntityIconDragDropPreview();

	/**
	 * @brief Begins the creation of preview model
	 * @param icon the icon of the entity for which the preview is generated
	 * @note will be changed take EmberEntity as input
	 */
	void createPreview(EntityIcon* icon);

	/**
	 * @brief Sets model information based on the entity type
	 * @param modelName the name of the model
	 */
	void setModel(const std::string& modelName);

	/**
	 * @brief Clean up after finalizing or cancelling the user action
	 */
	void cleanupCreation();

	/**
	 * @brief Notifies of action commitment
	 */
	void finalizeCreation();

	/**
	 * @brief Returns the offset between the preview model and the avatar
	 * @return vector of the offset between the preview model and the avatar
	 */
	const WFMath::Vector<3> & getDropOffset();

	/**
	 * @brief Emitted when we finalize our intended action
	 */
	sigc::signal<void, EmberEntity*> EventEntityFinalized;
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
	 * @brief Returns the model for the preview
	 * @return model for the preview
	 */
	Model::Model* getModel();

	/**
	 * @brief Returns true if our detached entity has a bounding box
	 * @return Does the detached entity have a bounding box
	 */
	bool hasBBox();

	/**
	 * @brief Returns the bounding box our detached entity
	 * @return the bounding box
	 */
	const WFMath::AxisBox<3> & getBBox();

private:
	/**
	 * @brief The entity we are generating a preview model for
	 */
	EmberEntity* mIconEntity;

	/**
	 * @brief Eris connection
	 */
	Eris::Connection& mConn;

	/**
	 * @brief Detached entity that is used in process of creating preview.
	 */
	Authoring::DetachedEntity* mEntity;

	/**
	* @brief Message that is composed from attributes of the entity we're creating a preview for
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
	 * @brief Preview model mount
	 */
	Model::ModelMount* mModelMount;

	/**
	 * @brief Current position of preview in the world.
	 */
	WFMath::Point<3> mPos;

	/**
	 * @brief Current orientation of preview in the world.
	 * @note we don't change the orientation
	 */
	WFMath::Quaternion mOrientation;

	/**
	 * @brief The location of the preview model to the avatar
	 */
	WFMath::Vector<3> mDropOffset;

	/**
	 * @brief Movement adapter for our preview model
	 */
	EntityIconDragDropPreviewMovement* mMovement;

	/**
	 * @brief Keeps track of whether or not we are generating a preview
	 */
	bool mActiveIcon;
};

class EntityIconDragDropPreviewActionCreator  : public Ember::EntityMapping::IActionCreator {
public:
	EntityIconDragDropPreviewActionCreator(EntityIconDragDropPreview& entityIconDragDrop);
	~EntityIconDragDropPreviewActionCreator();
	virtual void createActions(Ember::EntityMapping::EntityMapping& modelMapping, Ember::EntityMapping::Cases::CaseBase* aCase, Ember::EntityMapping::Definitions::CaseDefinition& caseDefinition);
protected:
	EntityIconDragDropPreview& mEntityIconDragDropPreview;
};

class EntityIconDragDropPreviewMovementBridge : public ::EmberOgre::Authoring::EntityMoverBase
{
public:

	EntityIconDragDropPreviewMovementBridge(EntityIconDragDropPreview& creator, Authoring::DetachedEntity& entity, Ogre::SceneNode* node);
	virtual ~EntityIconDragDropPreviewMovementBridge();

	virtual void finalizeMovement();
	virtual void cancelMovement();

private:
	EntityIconDragDropPreview& mCreator;

};

class EntityIconDragDropPreviewMovement
{
public:

	EntityIconDragDropPreviewMovement(EntityIconDragDropPreview& mEntityIconDragDropPreview, const Camera::MainCamera& camera, Authoring::DetachedEntity& entity, Ogre::SceneNode* node);
	~EntityIconDragDropPreviewMovement();

protected:
	Authoring::MovementAdapter mMoveAdapter;
};

}
}

#endif
