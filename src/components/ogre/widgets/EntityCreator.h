//
// C++ Interface: EntityCreator
//
// Description:
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2008
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
#ifndef EMBEROGRE_GUIENTITYCREATOR_H
#define EMBEROGRE_GUIENTITYCREATOR_H

#include "components/ogre/authoring/EntityRecipe.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/widgets/Widget.h"
#include <wfmath/point.h>
#include <wfmath/quaternion.h>
#include <CEGUIWindow.h>
#include <sigc++/signal.h>
#include <sigc++/connection.h>
#include <OgreFrameListener.h>

namespace Eris
{
class TypeInfo;
class TypeService;
}

namespace EmberOgre
{

class World;

namespace Authoring
{
class DetachedEntity;
}
namespace Model
{
class ModelMount;
}
namespace Gui
{

class EntityCreatorInputAdapter;
class EntityCreatorMoveAdapter;
class EntityCreatorMovement;

/**
 * @brief Helper class for Entity Creator.
 *
 * Helper class for Entity Creator. Called from Lua to handle showing preview and creating the entity.
 *
 * @author Alexey Torkhov <atorkhov@gmail.com>
 */
class EntityCreator
{
public:
	/**
	 * Constructor.
	 */
	EntityCreator(World& world);

	/**
	 * Destructor.
	 */
	~EntityCreator();

	/**
	 * Sets recipe that would be used for entity creation in future.
	 */
	void setRecipe(Authoring::EntityRecipe& recipe);

	/**
	 * Toggles create mode and performs correspondent action.
	 */
	void toggleCreateMode();

	/**
	 * @brief Sets whether to randomize the orientation of all new entities.
	 * If set, all new entities will have their orientation randomized along the vertical axis.
	 * @param randomize Whether to randomize the orientation or not.
	 */
	void setRandomizeOrientation(bool randomize);

	/**
	 * Starts entity creation process.
	 */
	void startCreation();

	/**
	 * Stops entity creation process.
	 */
	void stopCreation();

	/**
	 * Composes entity
	 */
	void createEntity();

	/**
	 * Sends composed entity to server
	 */
	void finalizeCreation();

	/**
	 * Cleanup after creation of entity or cancel creation.
	 */
	void cleanupCreation();

	/**
	 * Sets preview model name
	 */
	void setModel(const std::string& modelName);

	/**
	 * Shows preview model part
	 */
	void showModelPart(const std::string& partName);

	/**
	 * Hide preview model part
	 */
	void hideModelPart(const std::string& partName);

	/**
	 * Entity creator widget. Set from Lua, so it is public.
	 */
	::EmberOgre::Gui::Widget* mWidget;

	/**
	 * @brief Emitted when all needed type info for the current recipe is loaded.
	 */
	sigc::signal<void> EventTypeInfoLoaded;

	/**
	 * @brief Makes sure that all types are loaded. This is needed for the type lookup we need to do in the recipes in order to get the default values.
	 */
	void loadAllTypes();

protected:
	/**
	 * Sets preview node properties basing on model. Code from EmberPhysicalEntity.
	 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
	 */
	void initFromModel();

	/**
	 * Applies correct scaling basing on model definition. Code from EmberPhysicalEntity.
	 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
	 */
	void scaleNode();

	/**
	 * @brief Called when the model is reloaded, which also happens if background loading is enabled and the model has been loaded in the background.
	 */
	void model_Reloaded();


	Model::Model* getModel();
	bool hasBBox();
	const WFMath::AxisBox<3> & getBBox();

	/**
	 * @brief The main world.
	 */
	World& mWorld;

	Eris::TypeService& mTypeService;

	/**
	 * Stores, are we in create mode.
	 */
	bool mCreateMode;

	/**
	 * Pointer to recipe that is currently selected in widget.
	 */
	Authoring::EntityRecipe* mRecipe;

	/**
	 * Connection to EntityRecipe::adapterValueChanged signal, when in create mode.
	 */
	sigc::connection mRecipeConnection;

	/**
	 * @brief Handler of EntityRecipe::adapterValueChanged signal.
	 *
	 * Called on EntityRecipe::adapterValueChanged signal and should update preview with new adapter values.
	 *
	 * TODO Unfortunately, this signal is never called. Need to find what happens with it in adapters class.
	 */
	void adapterValueChanged();

	/**
	 * @brief Checks whether the type info for the current recipe is fully bound.
	 * If so, the EventTypeInfoLoaded signal will be emitted right away, else it will be emitted later on when the type is bound through the typeService_BoundType method.
	 * Call this method right after a new entity recipe has been set, to make sure that the interface doesn't proceed until the type info is bound.
	 */
	void checkTypeInfoBound();

	/**
	 * @brief Listen for the bound signal for the type service, and if the type of our recipe has been bound emit the EventTypeInfoLoaded signal.
	 * @param typeInfo The type info that was bound.
	 */
	void typeService_BoundType(Eris::TypeInfo* typeInfo);

	/**
	 * Current position of preview in the world.
	 */
	WFMath::Point<3> mPos;

	/**
	 * Current orientation of preview in the world.
	 */
	WFMath::Quaternion mOrientation;

	/**
	 * Adapter that listens to changes of terrain cursor changes and positions preview accordingly.
	 */
	EntityCreatorMoveAdapter* mMoveAdapter;

	/**
	 * Detached entity that is used in process of creating preview.
	 */
	Authoring::DetachedEntity* mEntity;

	/**
	 * Preview scene node.
	 */
	Ogre::SceneNode* mEntityNode;

	Model::ModelMount* mModelMount;

	/**
	 * Preview model.
	 */
	Model::Model* mModel;

	/**
	 * Message that is composed from recipe entity spec with placeholders substitued with adapters values.
	 */
	Atlas::Message::MapType mEntityMessage;

	/**
	 * Parameters for blurb timing.
	 */
	Ogre::Real mTimeUntilShowBlurb, mTimeBlurbShown, mTimeToShowBlurb;

	/**
	 @brief If set to true, all new entities will have their orientation randomized around the vertical axis.
	 */
	bool mRandomizeOrientation;

	/**
	 * @brief The connection for the TypeInfo bound signal, used for making sure that the UI doesn't proceed until the type info has been correctly bound.
	 */
	// 	sigc::connection mTypeInfoBoundConnection;

	EntityCreatorMovement* mMovement;


	Ogre::Entity* mAxisMarker;
};

/**
 * Adapter for position entity preview.
 */
class EntityCreatorMoveAdapter: public Ogre::FrameListener
{
public:
	EntityCreatorMoveAdapter(EntityCreator& adapter);
	~EntityCreatorMoveAdapter();
	/**
	 * Registers adapter in input service.
	 */
	void addAdapter();
	/**
	 * Unregisters adapter in input service.
	 */
	void removeAdapter();
	/**
	 * Called each frame to update position of preview.
	 */
	bool frameStarted(const Ogre::FrameEvent& event);
protected:
	EntityCreator& mEntityCreator;
};

}

}

#endif
