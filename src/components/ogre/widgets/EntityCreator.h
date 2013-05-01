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

#include <wfmath/quaternion.h>

#include <sigc++/signal.h>
#include <sigc++/connection.h>
#include <sigc++/trackable.h>
#include <sigc++/slot.h>

namespace Eris
{
class TypeInfo;
class TypeService;
}

namespace Ember
{
namespace OgreView
{

class World;

namespace Authoring
{
class DetachedEntity;
class EntityRecipe;
}
namespace Model
{
class ModelMount;
}
namespace Gui
{

class EntityCreatorInputAdapter;
class EntityCreatorMovement;
class EntityCreatorCreationInstance;

/**
 * @brief Helper class for Entity Creator.
 *
 * Helper class for Entity Creator. Called from Lua to handle showing preview and creating the entity.
 *
 * @author Alexey Torkhov <atorkhov@gmail.com>
 */
class EntityCreator : public virtual sigc::trackable
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
	 * Sends composed entity to server
	 */
	void finalizeCreation();

	/**
	 * @brief Emitted when all needed type info for the current recipe is loaded.
	 */
	sigc::signal<void> EventTypeInfoLoaded;

	/**
	 * @brief Emitted when creation has started.
	 */
	sigc::signal<void> EventCreationStarted;

	/**
	 * @brief Emitted when creation has ended.
	 */
	sigc::signal<void> EventCreationEnded;

	/**
	 * @brief Makes sure that all types are loaded. This is needed for the type lookup we need to do in the recipes in order to get the default values.
	 */
	void loadAllTypes();

protected:


	/**
	 * @brief The main world.
	 */
	World& mWorld;

	/**
	 * @brief The main type service.
	 */
	Eris::TypeService& mTypeService;

	/**
	 * @brief The currently selected recipe.
	 */
	Authoring::EntityRecipe* mRecipe;

	/**
	 * @brief A creation instance, which represents a preview of the entity, before it's created on the server.
	 */
	EntityCreatorCreationInstance* mCreationInstance;

	/**
	 * @brief The last orientation used for creation.
	 *
	 * This is sent to any new creation instance, thus making sure that any new entity starts out with the same orientation as the last one.
	 * (That is unless we mRandomizeOrientation is set to True.)
	 */
	WFMath::Quaternion mLastOrientation;

	/**
	 * @brief If set to true, all new entities will have their orientation randomized around the vertical axis.
	 */
	bool mRandomizeOrientation;

	/**
	 * @brief A slot used for listening for changes to the recipe's adapter values.
	 */
	sigc::slot<void> mAdapterValueChangedSlot;

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
	 * @brief Listens for requests from the creationInstance to abort creation.
	 */
	void creationInstance_AbortRequested();

	/**
	 * @brief Listens for requests from the creationInstance to finalize creation.
	 */
	void creationInstance_FinalizeRequested();

	/**
	 * @brief Handler of EntityRecipe::adapterValueChanged signal.
	 *
	 * Called on EntityRecipe::adapterValueChanged signal and should update preview with new adapter values.
	 *
	 * TODO Unfortunately, this signal is never called. Need to find what happens with it in adapters class.
	 */
	void adapterValueChanged();

	/**
	 * @brief Creates a new EntityCreatorCreationInstance.
	 *
	 * Note that any existing such instance will first be destroyed.
	 */
	void createNewCreationInstance();




};

}

}

}

#endif
