//
// C++ Interface: AdapterBase
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
#ifndef EMBEROGRE_GUI_ADAPTERS_ATLASADAPTERBASE_H
#define EMBEROGRE_GUI_ADAPTERS_ATLASADAPTERBASE_H

#include <CEGUI.h>
#include <Atlas/Message/Element.h>
#include <sigc++/signal.h>
// namespace CEGUI
// {
// 	class Window;
// }


namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

static std::string toString(float number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

class AdapterBase;

struct AdapterWrapper
{
	AdapterBase* Adapter;
	CEGUI::Window* ContainerWindow;
};

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class AdapterBase
{
public:
    AdapterBase(const ::Atlas::Message::Element& element);

    virtual ~AdapterBase();
	
	
	/**
	Sets the value, thus also updating the gui.
	*/
	void setValue(::Atlas::Message::Element& element);

	::Atlas::Message::Element& getValue();
	const ::Atlas::Message::Element& getOriginalValue() const;
	
	/**
	Emitted when the value has been changed from a gui update.
	*/
	sigc::signal<void> EventValueChanged;
	
	/**
	Updates the gui with new values.
	*/
	virtual void updateGui(const ::Atlas::Message::Element& element) = 0;
	
	bool hasChanges();
	::Atlas::Message::Element getChangedElement();
	
	void remove();
	
	bool isRemoved() const;
	
protected:

	const ::Atlas::Message::Element& mOriginalElement;
	::Atlas::Message::Element mEditedElement;
	
	bool mSelfUpdate;
	bool mRemoved;
	
	virtual void fillElementFromGui() = 0;
	virtual bool _hasChanges() = 0;
	virtual ::Atlas::Message::Element _getChangedElement();

};

}

}

}

}

#endif
