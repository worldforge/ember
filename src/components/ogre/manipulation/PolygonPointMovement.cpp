//
// C++ Implementation: PolygonPointMovement
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

#include "PolygonPointMovement.h"
#include "PolygonPoint.h"

namespace EmberOgre {

namespace Manipulation {

PolygonPointMovement::PolygonPointMovement(PolygonPoint& point, IMovementListener* movementListener)
: mPoint(point)
{
	// When the point is moved, an instance of this will be created and the movement handled by it.
	// Note that ownership will be transferred to the adapter, so we shouldn't keep a reference
	PolygonPointMover* bridge = new PolygonPointMover(point, movementListener);
	mMoveAdapter.attachToBridge(bridge);
}


PolygonPointMovement::~PolygonPointMovement()
{
}


}

}
