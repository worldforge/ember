//
// C++ Implementation: EntityCreator
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityCreator.h"
#include "../GUIManager.h"
#include "components/ogre/widgets/adapters/atlas/AdapterFactory.h"
#include <iostream>

namespace EmberOgre {

namespace Gui {

EntityCreator::EntityCreator()
{

}

EntityCreator::~EntityCreator()
{

}

void EntityCreator::showRecipe(EmberOgre::EntityRecipe& recipe, CEGUI::Window* container)
{
	EmberOgre::GUIManager& mGuiManager = EmberOgre::GUIManager::getSingleton();

	GUIAdaptersStore::iterator I = recipe.getGUIAdaptersIteratorBegin();
	GUIAdaptersStore::iterator end = recipe.getGUIAdaptersIteratorEnd();
	for (; I != end; I++)
	{
		S_LOG_VERBOSE("Creating adapter " << I->first << " of type " << I->second->getType());

		CEGUI::Window* window = mGuiManager.createWindow("DefaultGUISheet");
		container->addChildWindow(window);

		I->second->attach(window);
	}
}

}

}
