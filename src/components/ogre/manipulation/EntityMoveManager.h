//
// C++ Interface: EntityMoveManager
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#include "EntityMoveAdapter.h"

namespace EmberOgre {

class EmberEntity;

class EntityMoveManager : public Ember::ConsoleObject
{
public:
	EntityMoveManager();

	const Ember::ConsoleCommandWrapper Move;

	void startMove(EmberEntity* entity);
	
	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command 
	 * @param args 
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);

protected:
	EntityMoveAdapter mMoveAdapter;

	void GuiManager_EntityAction(const std::string& action, EmberEntity* entity);

};

}

#endif
