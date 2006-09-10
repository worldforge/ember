//
// C++ Implementation: EntityMoveManager
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

#include "EntityMoveManager.h"
#include "EntityMoveAdapter.h"
#include "EntityMover.h"
#include "../GUIManager.h"
#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "../EmberOgre.h"



namespace EmberOgre {


EntityMoveManager::EntityMoveManager() :
Move("move", this, "Moves an entity."), mMoveAdapter(this), mAdjuster(this)
{
	GUIManager::getSingleton().EventEntityAction.connect(sigc::mem_fun(*this, &EntityMoveManager::GuiManager_EntityAction));
}



void EntityMoveManager::GuiManager_EntityAction(const std::string& action, EmberEntity* entity) {

	if (action == "move") {
		startMove(entity);
	}
}

void EntityMoveManager::startMove(EmberEntity* entity)
{
	EntityMover* mover = new EntityMover(entity);
	mMoveAdapter.attachToBridge(mover);
	EventStartMoving.emit(entity);
}

void EntityMoveManager::runCommand(const std::string &command, const std::string &args)
{
	if(Move == command)
	{
		//the first argument must be a valid entity id
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string entityId = tokeniser.nextToken();
		if (entityId != "") {
			EmberEntity* entity = EmberOgre::getSingleton().getEntity(entityId);
			if (entity != 0) {
				startMove(entity);
			}
		} else {
			Ember::ConsoleBackend::getMainConsole()->pushMessage("You must specifify a valid entity id to move.");
		}
		
	}
}

}
