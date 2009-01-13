//
// C++ Interface: EntityEditor
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
#ifndef EMBEROGRE_GUIENTITYEDITOR_H
#define EMBEROGRE_GUIENTITYEDITOR_H

#include <map>
#include <vector>
#include <Atlas/Message/Element.h>

namespace Eris
{
	class Entity;
}

namespace CEGUI
{
class Window;
}

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

class AdapterBase;
class MapAdapter;

}

}

struct AdapterWrapper
{
	Adapters::Atlas::AdapterBase* Adapter;
	CEGUI::Window* ContainerWindow;
};

/**
@brief Helper class for the entity editor widget.
The main logic of the entity editor is in the lua file. This class acts like a helper class, implementing C++ functionality that might be cumbersome to code in Lua.
Normally an instance of this is created by the lua widget for each entity that's being edited.

@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class EntityEditor {
public:
    /**
     * @brief Ctor.
     * @param entity The entity which should be edited.
     * @param rootAdapter The root adapter for the attributes.
     */
    EntityEditor(Eris::Entity* entity, Adapters::Atlas::MapAdapter* rootAdapter);

    virtual ~EntityEditor();
    
    void submitChanges();

	Atlas::Message::Element createMapElement();
	Atlas::Message::Element createListElement();
	Atlas::Message::Element createStringElement();
	Atlas::Message::Element createIntElement();
	Atlas::Message::Element createFloatElement();
	Atlas::Message::Element createPosition2dElement();
protected:

	Adapters::Atlas::MapAdapter* mRootAdapter;

	Eris::Entity* mEntity;
	
	

};

}

}

#endif
