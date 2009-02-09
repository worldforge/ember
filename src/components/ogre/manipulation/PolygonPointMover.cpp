//
// C++ Implementation: PolygonPointMover
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

#include "PolygonPointMover.h"
#include "PolygonPoint.h"

#include "../MathConverter.h"

namespace EmberOgre {

namespace Manipulation {

PolygonPointMover::PolygonPointMover(PolygonPoint& point)
: mPoint(point)
{
}


PolygonPointMover::~PolygonPointMover()
{
}

const WFMath::Quaternion& PolygonPointMover::getOrientation() const
{
	mOrientation = Ogre2Atlas(mPoint.getNode()->_getDerivedOrientation());
	return mOrientation;
}

const WFMath::Point<3>& PolygonPointMover::getPosition() const
{
	mPosition = Ogre2Atlas(mPoint.getNode()->_getDerivedPosition());
	return mPosition;
}
void PolygonPointMover::setPosition(const WFMath::Point<3>& position)
{
	if (position.isValid()) {
		///We need to offset into local space.
		Ogre::Vector3 posOffset = Ogre::Vector3::ZERO;
		if (mPoint.getNode()->getParent()) {
			posOffset = mPoint.getNode()->getParent()->_getDerivedPosition();
		}
		Ogre::Vector3 newPos = Atlas2Ogre(position) - posOffset;
		WFMath::Vector<3> translation = Ogre2Atlas_Vector3(newPos - mPoint.getNode()->getPosition());
		///adjust it so that it moves according to the ground for example
		mPoint.translate(WFMath::Vector<2>(translation.x(), translation.y()));
	}
}
void PolygonPointMover::move(const WFMath::Vector<3>& directionVector)
{
	if (directionVector.isValid()) {
		mPoint.translate(WFMath::Vector<2>(directionVector.x(), directionVector.y()));
	}
}
void PolygonPointMover::setRotation (int axis, WFMath::CoordType angle)
{
	///not implemented yet
}

void PolygonPointMover::yaw(WFMath::CoordType angle)
{
}

void PolygonPointMover::setOrientation(const WFMath::Quaternion& rotation)
{
}

void PolygonPointMover::finalizeMovement()
{
}
void PolygonPointMover::cancelMovement()
{
}

}

}
