//
// C++ Interface: NumberAdapter
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_GUI_ADAPTERS_ATLASNUMBERADAPTER_H
#define EMBEROGRE_GUI_ADAPTERS_ATLASNUMBERADAPTER_H

#include "AdapterBase.h"

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {

/**
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class NumberAdapter : public AdapterBase
{
public:
    NumberAdapter(const ::Atlas::Message::Element& element, CEGUI::Combobox* textWindow);

    virtual ~NumberAdapter();
    
	/**
	Updates the gui with new values.
	*/
	virtual void updateGui(const ::Atlas::Message::Element& element);
	
	virtual void addSuggestion(const std::string& suggestedValue);
   
protected:
	CEGUI::Combobox* mTextWindow;
	bool window_TextChanged(const CEGUI::EventArgs& e);

	virtual void fillElementFromGui();
	virtual bool _hasChanges();

};

}

}

}

}

}

#endif
