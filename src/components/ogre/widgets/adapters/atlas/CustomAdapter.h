//
// C++ Interface: CustomAdapter
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
#ifndef EMBEROGRE_GUI_ADAPTERS_ATLASCUSTOMADAPTER_H
#define EMBEROGRE_GUI_ADAPTERS_ATLASCUSTOMADAPTER_H

#include "AdapterBase.h"


namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class CustomAdapter : public AdapterBase, public sigc::trackable
{
public:
    CustomAdapter(const ::Atlas::Message::Element& element);

    virtual ~CustomAdapter();
    
	/**
	Updates the gui with new values.
	*/
	virtual void updateGui(const ::Atlas::Message::Element& element);
    
    sigc::signal<void, bool&> QueryHasChanges;
    sigc::signal<void, const ::Atlas::Message::Element*> QueryUpdateGui;
    sigc::signal<void, ::Atlas::Message::Element*> QueryFillElementFromGui;
    sigc::signal<void, ::Atlas::Message::Element*> QueryGetChangedElement;
    
protected:
	
	virtual void fillElementFromGui();
	virtual bool _hasChanges();
	virtual ::Atlas::Message::Element _getChangedElement();


};

}

}

}

}

#endif
