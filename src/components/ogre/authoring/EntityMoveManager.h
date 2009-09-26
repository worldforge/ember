//
// C++ Interface: EntityMoveManager
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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

#ifndef EMBEROGREENTITYMOVEMANAGER_H
#define EMBEROGREENTITYMOVEMANAGER_H

#include "../EmberOgrePrerequisites.h"
#include "framework/ConsoleObject.h"
#include "MovementAdapter.h"
#include "EntityMoveAdjuster.h"

namespace EmberOgre
{

class EmberEntity;
namespace Authoring
{

/**
 Responsible for allowing movement of entities in the world by the user.
 */
class EntityMoveManager: public Ember::ConsoleObject, public sigc::trackable
{
public:
	EntityMoveManager();

	const Ember::ConsoleCommandWrapper Move;

	/**
	 *    Starts moving of an entity.
	 * @param entity
	 */
	void startMove(EmberEntity* entity);

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	virtual void runCommand(const std::string &command, const std::string &args);

	/**
	 * Emitted when the movement of an entity starts
	 */
	sigc::signal<void, EmberEntity*> EventStartMoving;

	/**
	 * Emitted when the movement of an entity has finished.
	 */
	sigc::signal<void> EventFinishedMoving;

	/**
	 * Emitted when the movement of an entity has been cancelled.
	 */
	sigc::signal<void> EventCancelledMoving;

protected:
	/**
	 Main adapter which will intercept mouse and keyboard input to allow for movement of an entity.
	 */
	MovementAdapter mMoveAdapter;

	/**
	 *    Listen for entityActions from the gui ("move").
	 * @param action
	 * @param entity
	 */
	void GuiManager_EntityAction(const std::string& action, EmberEntity* entity);

	/**
	 Responsible for making sure that entities that cannot be moved are returned to their correct place.
	 */
	EntityMoveAdjuster mAdjuster;

};

}
}

#endif
