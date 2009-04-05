//
// C++ Interface: MapAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRE_GUI_ADAPTERS_ATLASMAPADAPTER_H
#define EMBEROGRE_GUI_ADAPTERS_ATLASMAPADAPTER_H

#include "AdapterBase.h"
#include "components/ogre/widgets/StackableContainer.h"


namespace CEGUI {
	class Window;
}

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {




/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class MapAdapter : public AdapterBase
{
public:
    
    MapAdapter(const ::Atlas::Message::Element& element, CEGUI::Window* childContainer);

    virtual ~MapAdapter();

    std::vector<std::string> getAttributeNames();

	const ::Atlas::Message::Element& valueOfAttr(const std::string& attr) const;
	bool hasAttr(const std::string& attr) const;

	/**
	Updates the gui with new values.
	*/
	virtual void updateGui(const ::Atlas::Message::Element& element);

    void addAttributeAdapter(const std::string& attributeName, Adapters::Atlas::AdapterBase* adapter, CEGUI::Window* containerWindow);
    void removeAdapters();
    
	::Atlas::Message::Element getSelectedChangedElements();
	
	/**
	 * @brief Returns true if the map adapter already contains an adapter with the specified name.
	 * This works very much like hasAttr(), with the exception that this also will take into considerations those adapters that have been added but not yet applied to the underlying element.
	 * @param attr The name of the attribute to look for an adapter for.
	 * @return True if an adapter for the named attribute exists.
	 */
	bool hasAdapter(const std::string& attr) const;

protected:
	typedef std::map<std::string, AdapterWrapper> AdapterStore;
	
	CEGUI::Window* mChildContainer;
// 	StackableContainer mStackableContainer;
	const ::Atlas::Message::MapType& mAttributes;
	
	AdapterStore mAdapters;
	
	virtual void fillElementFromGui();
	virtual bool _hasChanges();
	virtual ::Atlas::Message::Element _getChangedElement();

};

}

}

}

}

#endif
