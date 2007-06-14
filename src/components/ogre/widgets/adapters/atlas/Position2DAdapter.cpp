//
// C++ Implementation: Position2DAdapter
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
#include "Position2DAdapter.h"
#include <wfmath/vector.h>
#include <wfmath/atlasconv.h>

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

Position2DAdapter::Position2DAdapter(const ::Atlas::Message::Element& element, CEGUI::Window* xWindow, CEGUI::Window* yWindow)
: AdapterBase(element), mXWindow(xWindow), mYWindow(yWindow)
{
	if (mXWindow) {
		mXWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&Position2DAdapter::window_TextChanged, this)); 
	}
	if (mYWindow) {
		mYWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&Position2DAdapter::window_TextChanged, this)); 
	}
	updateGui(mOriginalElement);
}


Position2DAdapter::~Position2DAdapter()
{
}

void Position2DAdapter::updateGui(const ::Atlas::Message::Element& element)
{
	mSelfUpdate = true;
	WFMath::Vector<2> vector(element);
// 	axisBox.fromAtlas(element.asList());
	if (mXWindow) {
		mXWindow->setText(toString(vector.x())); 
	}
	if (mYWindow) {
		mYWindow->setText(toString(vector.y())); 
	}
	mSelfUpdate = false;
}

bool Position2DAdapter::window_TextChanged(const CEGUI::EventArgs& e)
{
	if (!mSelfUpdate) {
		EventValueChanged.emit();
	}
	return true;
}

void Position2DAdapter::fillElementFromGui()
{
	WFMath::Vector<2> vector;
	if (mXWindow) {
		vector.x() = atof(mXWindow->getText().c_str()); 
	}
	if (mYWindow) {
		vector.y() = atof(mYWindow->getText().c_str()); 
	}
	mEditedElement = vector.toAtlas();
}

bool Position2DAdapter::_hasChanges()
{
	WFMath::Vector<2> originalValue;
	originalValue.fromAtlas(mOriginalElement);
	WFMath::Vector<2> newValue;
	newValue.fromAtlas(getValue());
	return originalValue != newValue;
}
}

}

}

}
