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
#include "components/ogre/input/IInputAdapter.h"
#include <Eris/Connection.h>
#include <wfmath/point.h>
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
	void createEntity(EntityRecipe& recipe, WFMath::Point<3> pos);

protected:
	void connectedToServer(Eris::Connection* conn);
	Eris::Connection* mConn;
};

/**
 * Class for intercepting mouse click.
 */
class EntityCreateAdapter : public IInputAdapter
{
public:
	EntityCreateAdapter(EntityCreator& entityCreator, EntityRecipe& entityRecipe);
	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse);
	virtual bool injectMouseButtonUp(const Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Input::MouseButton& button);
	virtual bool injectChar(char character);
	virtual bool injectKeyDown(const SDLKey& key);
	virtual bool injectKeyUp(const SDLKey& key);
private:
	EntityCreator& mEntityCreator;
	EntityRecipe& mEntityRecipe;
};

}

}

#endif
