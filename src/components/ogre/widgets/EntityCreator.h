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

#include "components/ogre/manipulation/EntityRecipe.h"
#include <Eris/Connection.h>
#include <CEGUIWindow.h>

namespace EmberOgre {

namespace Gui {

/**
 * Helper class for Entity Creator.
 */
class EntityCreator {
public:
	/**
	 * Constructor.
	 */
	EntityCreator();

	/**
	 * Destructor.
	 */
	~EntityCreator();

	/**
	 * Show adapters of specific recipe in container
	 */
	void showRecipe(EntityRecipe& recipe, CEGUI::Window* container);

	/**
	 * Sends composed entity to server
	 */
	void createEntity(EntityRecipe& recipe);

protected:
	void connectedToServer(Eris::Connection* conn);
	Eris::Connection* mConn;
};

}

}

#endif
