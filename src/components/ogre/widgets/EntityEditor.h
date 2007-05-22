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
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class EntityEditor{
public:
    EntityEditor(Eris::Entity* entity, Adapters::Atlas::MapAdapter* rootAdapter);

    virtual ~EntityEditor();
    
    void submitChanges();

	Atlas::Message::Element createMapElement();
	Atlas::Message::Element createListElement();
	Atlas::Message::Element createStringElement();
	Atlas::Message::Element createIntElement();
	Atlas::Message::Element createFloatElement();
protected:

	Adapters::Atlas::MapAdapter* mRootAdapter;

	Eris::Entity* mEntity;
	
	

};

}

}

#endif
