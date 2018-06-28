/*
 Copyright (C) 2018 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef EMBER_SCALEADAPTER_H
#define EMBER_SCALEADAPTER_H

#include "AdapterBase.h"

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {
class ScaleAdapter : public AdapterBase {
public:

	struct Widgets {
		CEGUI::Window* x;
		CEGUI::Window* y;
		CEGUI::Window* z;
		CEGUI::ToggleButton* uniformToggle;
		CEGUI::Slider* scaler;
	};

	ScaleAdapter(const ::Atlas::Message::Element& element, Widgets widgets);

	/**
	Updates the gui with new values.
	*/
	void updateGui(const ::Atlas::Message::Element& element) override;

protected:

	Widgets mWidgets;

	bool window_TextChanged(const CEGUI::EventArgs& e);

	void fillElementFromGui() override;

	bool _hasChanges() override;

};

}
}
}
}
}

#endif //EMBER_SCALEADAPTER_H
