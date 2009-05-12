//
// C++ Implementation: QuaternionAdapter
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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

#include "QuaternionAdapter.h"
#include "Widget.h"

#include <CEGUIWindow.h>
#include <OgreStringConverter.h>

namespace EmberOgre {

namespace Gui {

QuaternionAdapter::QuaternionAdapter(CEGUI::Window *degreeWindow, CEGUI::Window *xWindow, CEGUI::Window *yWindow, CEGUI::Window *zWindow, const Ogre::Quaternion& quaternion)
: mQuaternion(quaternion), mOriginalQuaternion(quaternion), mVectorAdapter(xWindow, yWindow, zWindow), mDegreeWindow(degreeWindow), mSelfUpdate(false)
{
	if (degreeWindow) {
		BIND_CEGUI_EVENT(degreeWindow, CEGUI::Window::EventTextChanged, QuaternionAdapter::window_TextChanged);
	}
	mVectorAdapter.EventValueChanged.connect(sigc::mem_fun(*this, &QuaternionAdapter::vectorAdapter_ValueChanged));

}

QuaternionAdapter::~QuaternionAdapter()
{
}

void QuaternionAdapter::setValue(const Ogre::Quaternion& quaternion)
{
	updateGui(quaternion);
	EventValueChanged.emit();
}


const Ogre::Quaternion& QuaternionAdapter::getValue() const
{
	const Ogre::Vector3& axis = mVectorAdapter.getValue();
	float degrees = 0;
	if (mDegreeWindow) {
		degrees = Ogre::StringConverter::parseReal( mDegreeWindow->getText().c_str());
	}

	mQuaternion.FromAngleAxis(Ogre::Degree(degrees), axis);
	return mQuaternion;
}

const Ogre::Quaternion& QuaternionAdapter::getOriginalValue() const
{
	return mOriginalQuaternion;
}

void QuaternionAdapter::updateGui(const Ogre::Quaternion& quaternion)
{
	mSelfUpdate = true;

	if (&quaternion) {
		Ogre::Vector3 axis;
		Ogre::Degree angle;
		quaternion.ToAngleAxis( angle, axis);
		mVectorAdapter.updateGui(axis);
		if (mDegreeWindow) {
			mDegreeWindow->setText(Ogre::StringConverter::toString(angle.valueDegrees()));
		}
	} else {
		mVectorAdapter.updateGui(Ogre::Vector3::ZERO);
		if (mDegreeWindow) {
			mDegreeWindow->setText("");
		}
	}
	mSelfUpdate = false;
}

bool QuaternionAdapter::window_TextChanged(const CEGUI::EventArgs& e)
{
	if (!mSelfUpdate) {
		EventValueChanged.emit();
	}
	return true;
}

void QuaternionAdapter::vectorAdapter_ValueChanged()
{
	if (!mSelfUpdate) {
		EventValueChanged.emit();
	}
}

}

}
