/*
 Copyright (C) 2013 Samuel Kogler

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "OgreTerrainObserver.h"
#include "components/ogre/Convert.h"

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

OgreTerrainObserver::OgreTerrainObserver() :
		mObservedArea(0, 0, 0, 0)
{
}

OgreTerrainObserver::~OgreTerrainObserver()
{
}

void OgreTerrainObserver::observeArea(const WFMath::AxisBox<2>& area)
{
	mObservedArea = Convert::toOgre(area);
}

void OgreTerrainObserver::observeArea(const Ogre::TRect<int>& area)
{
	mObservedArea = Ogre::TRect<Ogre::Real>(area.left, area.top, area.right, area.bottom);
}

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
