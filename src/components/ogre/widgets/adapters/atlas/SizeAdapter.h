//
// C++ Interface: SizeAdapter
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
#ifndef EMBEROGRE_GUI_ADAPTERS_ATLASSIZEADAPTER_H
#define EMBEROGRE_GUI_ADAPTERS_ATLASSIZEADAPTER_H

#include "AdapterBase.h"

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {

/**
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class SizeAdapter : public AdapterBase
{
public:

	struct Widgets {
		CEGUI::Window& lowerXWindow;
		CEGUI::Window& lowerYWindow;
		CEGUI::Window& lowerZWindow;
		CEGUI::Window& upperXWindow;
		CEGUI::Window& upperYWindow;
		CEGUI::Window& upperZWindow;
		CEGUI::Slider& scaler;
		CEGUI::Window& infoWindow;
		CEGUI::ToggleButton& editable;
	};

    SizeAdapter(const ::Atlas::Message::Element& element, const Widgets& widgets);

	~SizeAdapter() override;
	
	/**
	Updates the gui with new values.
	*/
	void updateGui(const ::Atlas::Message::Element& element) override;

protected:
	Widgets mWidgets;
	
	/**
	Updates the info window with measurement information.
	*/
	void updateInfo();
	
	bool window_TextChanged(const CEGUI::EventArgs& e);
	bool slider_ValueChanged(const CEGUI::EventArgs& e);

	void fillElementFromGui() override;

	bool _hasChanges() override;
};

}

}

}

}

}

#endif
