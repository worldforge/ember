//
// C++ Implementation: PolygonPointUserObject
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

#include "PolygonPointUserObject.h"
#include "PolygonPoint.h"

#include <OgreEntity.h>
#include <OgreSceneNode.h>

namespace EmberOgre
{

namespace Authoring
{

const std::string PolygonPointUserObject::s_TypeName("PolygonPointMarker");

PolygonPointUserObject::PolygonPointUserObject(PolygonPoint& point) :
	mPoint(point)
{
}

PolygonPointUserObject::~PolygonPointUserObject()
{
}

PolygonPoint& PolygonPointUserObject::getPoint()
{
	return mPoint;
}

const Ogre::String & PolygonPointUserObject::getTypeName(void) const
{
	return s_TypeName;
}

void PolygonPointUserObject::markAsMoved()
{
	Ogre::Entity* entity = static_cast<Ogre::Entity*> (mPoint.getNode()->getAttachedObject(0));
	entity->setMaterialName("/global/authoring/polygon/point/moved");
}

void PolygonPointUserObject::resetMarking()
{
	Ogre::Entity* entity = static_cast<Ogre::Entity*> (mPoint.getNode()->getAttachedObject(0));
	entity->setMaterialName("/global/authoring/polygon/point");
}

}

}
