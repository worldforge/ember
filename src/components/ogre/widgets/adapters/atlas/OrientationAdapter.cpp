//
// C++ Implementation: OrientationAdapter
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OrientationAdapter.h"
#include <wfmath/quaternion.h>
#include <wfmath/atlasconv.h>

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

OrientationAdapter::OrientationAdapter(const ::Atlas::Message::Element& element, CEGUI::Window* xWindow, CEGUI::Window* yWindow, CEGUI::Window* zWindow, CEGUI::Window* scalarWindow)
: AdapterBase(element), mXWindow(xWindow), mYWindow(yWindow), mZWindow(zWindow), mScalarWindow(scalarWindow)
{
	if (mXWindow) {
		addGuiEventConnection(mXWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&OrientationAdapter::window_TextChanged, this))); 
	}
	if (mYWindow) {
		addGuiEventConnection(mYWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&OrientationAdapter::window_TextChanged, this))); 
	}
	if (mZWindow) {
		addGuiEventConnection(mZWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&OrientationAdapter::window_TextChanged, this))); 
	}
	if (mScalarWindow) {
		addGuiEventConnection(mScalarWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&OrientationAdapter::window_TextChanged, this))); 
	}

	
	updateGui(mOriginalElement);
}


OrientationAdapter::~OrientationAdapter()
{
}

void OrientationAdapter::updateGui(const ::Atlas::Message::Element& element)
{
	AdapterSelfUpdateContext context(*this);
	WFMath::Quaternion orientation(element);
// 	axisBox.fromAtlas(element.asList());
	if (mXWindow) {
		mXWindow->setText(toString(orientation.vector().x())); 
	}
	if (mYWindow) {
		mYWindow->setText(toString(orientation.vector().y())); 
	}
	if (mZWindow) {
		mZWindow->setText(toString(orientation.vector().z())); 
	}
	if (mScalarWindow) {
		mScalarWindow->setText(toString(orientation.scalar())); 
	}

}

bool OrientationAdapter::window_TextChanged(const CEGUI::EventArgs& e)
{
	if (!mSelfUpdate) {
		EventValueChanged.emit();
	}
	return true;
}

void OrientationAdapter::fillElementFromGui()
{
	float x(0), y(0), z(0), scalar(0);
	if (mXWindow) {
		x = atof(mXWindow->getText().c_str()); 
	}
	if (mYWindow) {
		y = atof(mYWindow->getText().c_str()); 
	}
	if (mZWindow) {
		z = atof(mZWindow->getText().c_str()); 
	}
	if (mScalarWindow) {
		scalar = atof(mScalarWindow->getText().c_str()); 
	}
	WFMath::Quaternion orientation(scalar, x, y, z);
	mEditedElement = orientation.toAtlas();
}

bool OrientationAdapter::_hasChanges()
{
	WFMath::Quaternion originalOrientation(mOriginalElement);
	WFMath::Quaternion newOrientation(getValue());
	return originalOrientation != newOrientation;
}

}

}

}

}
