//
// C++ Implementation: PolygonPointPickListener
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
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

#include "PolygonPointPickListener.h"
#include "PolygonPointUserObject.h"
#include "PolygonPoint.h"
#include "components/ogre/MousePicker.h"
#include <OgreMovableObject.h>
#include <OgreSceneQuery.h>

namespace Ember
{
namespace OgreView
{

namespace Authoring
{

PolygonPointPickListener::PolygonPointPickListener(Polygon& polygon) :
		mPolygon(polygon), mPickedUserObject(0)
{
}

PolygonPointPickListener::~PolygonPointPickListener()
{
}

void PolygonPointPickListener::processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& /*cameraRay */, const MousePickerArgs& mousePickerArgs)
{
	if (entry.movable) {
		Ogre::MovableObject* pickedMovable = entry.movable;
		if (pickedMovable->isVisible() && pickedMovable->getUserObjectBindings().getUserAny().getType() == typeid(PolygonPointUserObject*)) {
			continuePicking = false;
			if (mousePickerArgs.pickType == MPT_PRESS) {
				//TODO: make sure that it's a point which belongs to our polygon
				mPickedUserObject = Ogre::any_cast<PolygonPointUserObject*>(pickedMovable->getUserObjectBindings().getUserAny());
			}
		}
	}
}

void PolygonPointPickListener::processDelayedPick(const MousePickerArgs& /*mousePickerArgs*/) {
	//We don't do any delayed selection
}


void PolygonPointPickListener::initializePickingContext(bool& willParticipate, unsigned int& queryMask, const MousePickerArgs& pickArgs)
{
	//We will only react on press events, but we want do silence click and pressed events if they happen with our markers too.
	if (pickArgs.pickType == MPT_PRESS || pickArgs.pickType == MPT_CLICK || pickArgs.pickType == MPT_PRESSED) {
		willParticipate = true;
		queryMask = MousePicker::CM_UNDEFINED;
		mPickedUserObject = 0;
	}

}

void PolygonPointPickListener::endPickingContext(const MousePickerArgs& /*mousePickerArgs*/)
{
	if (mPickedUserObject) {
		EventPickedPoint.emit(mPickedUserObject->getPoint());
		mPickedUserObject = 0;
	}
}

}

}
}
