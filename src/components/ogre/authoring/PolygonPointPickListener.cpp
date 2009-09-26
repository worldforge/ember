//
// C++ Implementation: PolygonPointPickListener
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
#include <OgreMovableObject.h>
#include <OgreSceneQuery.h>

namespace EmberOgre {

namespace Manipulation {

PolygonPointPickListener::PolygonPointPickListener(Polygon& polygon)
: mPolygon(polygon), mPickedUserObject(0)
{
}

PolygonPointPickListener::~PolygonPointPickListener()
{
}


void PolygonPointPickListener::processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs)
{
	if (entry.movable) {
		Ogre::MovableObject* pickedMovable = entry.movable;
		if (pickedMovable->isVisible() && pickedMovable->getUserObject() != 0 && pickedMovable->getUserObject()->getTypeName() == PolygonPointUserObject::s_TypeName) {
			///TODO: make sure that it's a point which belongs to our polygon
			mPickedUserObject = static_cast<PolygonPointUserObject*>(pickedMovable->getUserObject());
			continuePicking = false;
		}
	}
}

void PolygonPointPickListener::initializePickingContext()
{
	mPickedUserObject = 0;
}

void PolygonPointPickListener::endPickingContext(const MousePickerArgs& mousePickerArgs)
{
	if (mPickedUserObject) {
		EventPickedPoint.emit(mPickedUserObject->getPoint());
		mPickedUserObject = 0;
	}
}


}

}
