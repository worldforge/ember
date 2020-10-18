/*
 Copyright (C) 2011 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef ENTITYCREATORCREATIONINSTANCE_H_
#define ENTITYCREATORCREATIONINSTANCE_H_

#include "components/ogre/OgreIncludes.h"
#include <Atlas/Message/Element.h>
#include <wfmath/point.h>
#include <wfmath/quaternion.h>

#include <sigc++/trackable.h>
#include <sigc++/signal.h>
#include <sigc++/connection.h>
#include <Eris/Entity.h>
#include "components/ogre/authoring/EntityRecipe.h"

namespace Eris {
class TypeService;
}

namespace Ember {
namespace OgreView {

class World;

namespace Authoring {
class DetachedEntity;

class EntityRecipe;
}
namespace Model {
class ModelMount;

class Model;
}
namespace Gui {
class EntityCreatorInputAdapter;

class EntityCreatorMovement;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Represents one instance of a creation of an entity from a recipe.
 *
 * Once startCreation() has been called, a preview of the entity will be displayed in the world, and the user can position this preview.
 * Calling finalizeCreation() will submit data to the server, trying to create the entity. Note that the server might not allow this, for varying reasons.
 */
class EntityCreatorCreationInstance : public virtual sigc::trackable {
public:

	/**
	 * @brief Ctor.
	 *
	 * @param world The world in which we want to create an entity.
	 * @param typeService The type service used by the world.
	 * @param recipe The recipe which we want to create an entity from.
	 * @param randomizeOrientation Whether to randomize orientation.
	 * @param adapterValueChangedSlot A slot which should be attached to the EntityRecipe::EventValueChanged event.
	 */
	EntityCreatorCreationInstance(World& world,
								  Eris::TypeService& typeService,
								  std::vector<Atlas::Message::MapType> entityMaps,
								  bool randomizeOrientation);

	/**
	 * @brief Dtor.
	 */
	virtual ~EntityCreatorCreationInstance();

	/**
	 * @brief Initiates creation.
	 */
	void startCreation();


	/**
	 * @brief Sends composed entity to server
	 */
	void finalizeCreation();

	EntityCreatorMovement* getMovement();



	/**
	 * @brief Gets the current orientation of preview in the world.
	 */
	WFMath::Quaternion getOrientation() const;

	/**
	 * @brief Sets the orientation of the preview in the world.
	 *
	 * @param orientation The new orientation.
	 */
	void setOrientation(const WFMath::Quaternion& orientation);

	/**
	 * @brief Emitted when a stop of the creation process has been requested.
	 *
	 * This means that the creation process should be aborted.
	 */
	sigc::signal<void> EventAbortRequested;

	/**
	 * @brief Emitted when a finalization of the creation process has been requested.
	 */
	sigc::signal<void> EventFinalizeRequested;

	sigc::signal<void, Eris::Entity*, const WFMath::Point<3>&> EventMoved;


protected:

	/**
	 * @brief The world in which an entity should be created.
	 */
	World& mWorld;

	/**
	 * @brief The main type service.
	 */
	Eris::TypeService& mTypeService;

	struct EntityPreview {
		/**
		 * @brief Message that is composed from recipe entity spec with placeholders substituted with adapters values.
		 */
	 	Atlas::Message::MapType entityMap;

		/**
		 * @brief Detached entity that is used in process of creating preview.
		 */
		std::unique_ptr<Authoring::DetachedEntity> mEntity;

		/**
		 * @brief A model mount, to which the model is attached.
		 */
		std::unique_ptr<Model::ModelMount> mModelMount;

		/**
		 * @brief Preview model.
		 */
		std::unique_ptr<Model::Model> mModel;
	};

	std::vector<EntityPreview> mEntityPreviews;

	/**
	 * @brief Preview scene node.
	 */
	Ogre::SceneNode* mEntityNode;

	/**
	 * @brief Handles movement of the entity preview.
	 */
	std::unique_ptr<EntityCreatorMovement> mMovement;


	/**
	 * @brief Current position of preview in the world.
	 */
	WFMath::Point<3> mPos;

	/**
	* @brief The initial orientation to use, if no entity has been created yet.
	*/
	WFMath::Quaternion mInitialOrientation;

	/**
	 * @brief An entity which shows an axis, to mark the preview entity.
	 */
	Ogre::Entity* mAxisMarker;

	/**
	 * @brief A connection for the EntityRecipe::EventValueChanged event.
	 */
	sigc::connection mConnection;

	/**
	 * @brief Composes entity.
	 */
	void createEntity();

	/**
	 * @brief Sets preview model name
	 */
	void setModel(EntityPreview& entry,const std::string& modelName);

	/**
	 * Shows preview model part
	 */
	void showModelPart(EntityPreview& entry,const std::string& partName);

	/**
	 * Hide preview model part
	 */
	void hideModelPart(EntityPreview& entry,const std::string& partName);

};
}
}
}

#endif /* ENTITYCREATORCREATIONINSTANCE_H_ */
