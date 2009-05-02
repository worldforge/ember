//
// C++ Implementation: SnapToMovement
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

#include "SnapToMovement.h"
#include "components/ogre/EmberEntity.h"

#include <OgreSceneManager.h>

namespace EmberOgre {

namespace Manipulation {

SnapToMovement::SnapToMovement(EmberEntity& entity, float snapThreshold)
: mEntity(entity), mSnapThreshold(snapThreshold)
{
}


SnapToMovement::~SnapToMovement()
{
}

bool SnapToMovement::testSnapTo(const WFMath::Point<3>& position, WFMath::Point<3>& newPosition, EmberEntity* snappedToEntity)
{
	///First find all entities which are close enough
	///Then try to do a snap movement based on the points of the eris bounding boxes. I.e. we only provide support for snapping one corner of a bounding box to another corner (for now).
	Ogre::SceneManager* sceneMngr = mEntity.getSceneNode()->getCreator();
	if (sceneMngr) {
		Ogre::AxisAlignedBox bbox = mEntity.getSceneNode()->_getWorldAABB();
		bbox.
		Ogre::AxisAlignedBoxSceneQuery* query = sceneMngr->createAABBQuery();
	}
}



}

}
