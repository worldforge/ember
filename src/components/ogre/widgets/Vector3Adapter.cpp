//
// C++ Implementation: Vector3Adapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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

#include "Vector3Adapter.h"
#include "Widget.h"

#include <CEGUIWindow.h>


namespace EmberOgre {

namespace Gui {

Vector3Adapter::Vector3Adapter(CEGUI::Window *xWindow, CEGUI::Window *yWindow, CEGUI::Window *zWindow, const Ogre::Vector3& vector)
: mVector(vector), mOriginalVector(vector), mXWindow(xWindow), mYWindow(yWindow), mZWindow(zWindow), mSelfUpdate(false)

{
	if (xWindow) {
		BIND_CEGUI_EVENT(xWindow, CEGUI::Window::EventTextChanged, Vector3Adapter::window_TextChanged);
	}
	if (yWindow) {
		BIND_CEGUI_EVENT(yWindow, CEGUI::Window::EventTextChanged, Vector3Adapter::window_TextChanged);
	}
	if (zWindow) {
		BIND_CEGUI_EVENT(zWindow, CEGUI::Window::EventTextChanged, Vector3Adapter::window_TextChanged);
	}
}


Vector3Adapter::~Vector3Adapter()
{
}

void Vector3Adapter::setValue(const Ogre::Vector3& vector)
{
	updateGui(vector);
	EventValueChanged.emit();
}
    
const Ogre::Vector3& Vector3Adapter::getValue() const
{
	if (mXWindow) {
		mVector.x = Ogre::StringConverter::parseReal( mXWindow->getText().c_str());
	}
	if (mXWindow) {
		mVector.y = Ogre::StringConverter::parseReal( mYWindow->getText().c_str());
	}
	if (mZWindow) {
		mVector.z = Ogre::StringConverter::parseReal( mZWindow->getText().c_str());
	}
	return mVector;
}

const Ogre::Vector3& Vector3Adapter::getOriginalValue() const
{
	return mOriginalVector;
}
    
void Vector3Adapter::updateGui(const Ogre::Vector3& vector)
{
	mSelfUpdate = true;
	if (&vector) {
		if (mXWindow) {
			mXWindow->setText(Ogre::StringConverter::toString(vector.x));
		}
		if (mYWindow) {
			mYWindow->setText(Ogre::StringConverter::toString(vector.y));
		}
		if (mZWindow) {
			mZWindow->setText(Ogre::StringConverter::toString(vector.z));
		}
	} else {
		if (mXWindow) {
			mXWindow->setText("");
		}
		if (mYWindow) {
			mYWindow->setText("");
		}
		if (mZWindow) {
			mZWindow->setText("");
		}
	}
	mSelfUpdate = false;
}

bool Vector3Adapter::window_TextChanged(const CEGUI::EventArgs& e)
{
	if (!mSelfUpdate) {
		EventValueChanged.emit();
	}
	return true;
}

}

}
