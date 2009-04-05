//
// C++ Implementation: PositionAdapter
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "PositionAdapter.h"
#include <wfmath/vector.h>
#include <wfmath/atlasconv.h>

namespace CEGUI {
class PushButton;
}

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

PositionAdapter::PositionAdapter(const ::Atlas::Message::Element& element, CEGUI::Window* xWindow, CEGUI::Window* yWindow, CEGUI::Window* zWindow, CEGUI::PushButton* moveButton)
: AdapterBase(element), mXWindow(xWindow), mYWindow(yWindow), mZWindow(zWindow)
{
	if (mXWindow) {
		addGuiEventConnection(mXWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&PositionAdapter::window_TextChanged, this))); 
	}
	if (mYWindow) {
		addGuiEventConnection(mYWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&PositionAdapter::window_TextChanged, this))); 
	}
	if (mZWindow) {
		addGuiEventConnection(mZWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&PositionAdapter::window_TextChanged, this))); 
	}
	
	if (moveButton) {
		addGuiEventConnection(moveButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PositionAdapter::moveButton_Clicked, this))); 
	}
	
	updateGui(mOriginalElement);
}


PositionAdapter::~PositionAdapter()
{
}

void PositionAdapter::updateGui(const ::Atlas::Message::Element& element)
{
	AdapterSelfUpdateContext context(*this);
	WFMath::Vector<3> vector(element);
// 	axisBox.fromAtlas(element.asList());
	if (mXWindow) {
		mXWindow->setText(toString(vector.x())); 
	}
	if (mYWindow) {
		mYWindow->setText(toString(vector.y())); 
	}
	if (mZWindow) {
		mZWindow->setText(toString(vector.z())); 
	}
}

bool PositionAdapter::window_TextChanged(const CEGUI::EventArgs& e)
{
	if (!mSelfUpdate) {
		EventValueChanged.emit();
	}
	return true;
}

bool PositionAdapter::moveButton_Clicked(const CEGUI::EventArgs& e)
{
	EventMoveClicked.emit();
	return true;
}

void PositionAdapter::fillElementFromGui()
{
	WFMath::Vector<3> vector;
	if (mXWindow) {
		vector.x() = atof(mXWindow->getText().c_str()); 
	}
	if (mYWindow) {
		vector.y() = atof(mYWindow->getText().c_str()); 
	}
	if (mZWindow) {
		vector.z() = atof(mZWindow->getText().c_str()); 
	}
	mEditedElement = vector.toAtlas();
}

bool PositionAdapter::_hasChanges()
{
	WFMath::Vector<3> originalValue;
	originalValue.fromAtlas(mOriginalElement);
	WFMath::Vector<3> newValue;
	newValue.fromAtlas(getValue());
	return originalValue != newValue;
}
}

}

}

}
