//
// C++ Interface: EntityMoveManager
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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

#ifndef EMBEROGREENTITYMOVEMANAGER_H
#define EMBEROGREENTITYMOVEMANAGER_H

#include "MovementAdapter.h"
#include "EntityMoveAdjuster.h"
#include "framework/ConsoleCommandWrapper.h"
#include "components/ogre/EmberOgrePrerequisites.h"
#include "components/ogre/EntityObserverBase.h"
#include "framework/ConsoleObject.h"
#include <Eris/ActiveMarker.h>

namespace Ember
{
class EmberEntity;
namespace OgreView
{

class World;

namespace Authoring
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief An instance of this is used to keep track off the currently moved entity.
 * It's main purpose is to make sure that all structures are correctly cleaned up if the entity is deleted while being moved.
 *
 */
class EntityMoveInstance: public EntityObserverBase
{
public:
	EntityMoveInstance(EmberEntity& entity, MovementAdapter& moveAdapter, sigc::signal<void()>& eventFinishedMoving, sigc::signal<void()>& eventCancelledMoving);
	~EntityMoveInstance() override = default;
private:
	void cleanup() override;

	MovementAdapter& mMoveAdapter;
};

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Responsible for allowing movement of entities in the world by the user.
 * While entities normally get moved as a result of data sent from the server, we also want to allow the user to move entities independently.
 * The two main situations for this is either when the user is rearranging entities in the world, or when the user is a world author and needs to change the world.
 */
class EntityMoveManager: public ConsoleObject, public virtual sigc::trackable
{
public:
	explicit EntityMoveManager(World& world);

	/**
	 * @brief Console command for starting the movement operation.
	 */
	const ConsoleCommandWrapper Move;
	const ConsoleCommandWrapper Place;

	/**
	 * @brief Starts moving of an entity.
	 * @param entity
	 */
	void startMove(EmberEntity& entity);

	/**
	 * @brief Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	void runCommand(const std::string &command, const std::string &args) override;

	/**
	 * @brief Accessor for the World instance that this manager operates within.
	 * @returns The World instance to which this manager belongs.
	 */
	World& getWorld() const;

	/**
	 * Posts a one second delayed call to update the position of the attachment.
	 * This is useful when an entity has been moved, and we want to make sure that it actually got moved.
	 * @param entityId
	 */
	void delayedUpdatePositionForEntity(std::string entityId);

	void place(const std::string &args);

	/**
	 * @brief Emitted when the movement of an entity starts
	 */
	sigc::signal<void(EmberEntity&, EntityMover&)> EventStartMoving;

	/**
	 * @brief Emitted when the movement of an entity has finished.
	 */
	sigc::signal<void()> EventFinishedMoving;

	/**
	 * Emitted when the movement of an entity has been canceled.
	 */
	sigc::signal<void()> EventCancelledMoving;

protected:

	World& mWorld;

	/**
	 * @brief Main adapter which will intercept mouse and keyboard input to allow for movement of an entity.
	 */
	MovementAdapter mMoveAdapter;

	/**
	 * @brief Listen for entityActions from the gui ("move").
	 * @param action
	 * @param entity
	 */
	void GuiManager_EntityAction(const std::string& action, EmberEntity* entity);

	/**
	 * @brief Responsible for making sure that entities that cannot be moved are returned to their correct place.
	 */
	EntityMoveAdjuster mAdjuster;

	Eris::ActiveMarker mActiveMarker;

};

}
}

}

#endif
