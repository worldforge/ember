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

#include "OgreTerrainDefiner.h"

#include "components/ogre/TerrainPageDataProvider.h"
#include "OgreTerrainPageBridge.h"

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

OgreTerrainDefiner::OgreTerrainDefiner(IPageDataProvider* provider)
:
		mProvider(provider)
{
}

OgreTerrainDefiner::~OgreTerrainDefiner()
{
}

void OgreTerrainDefiner::define(Ogre::TerrainGroup* terrainGroup, long x, long y)
{
	OgreTerrainPageBridge* bridge = new OgreTerrainPageBridge(*terrainGroup, OgreTerrainPageBridge::IndexType(x, y));
	//TODO SK: fix ogre index types to be uniform
	mProvider->setUpTerrainPageAtIndex(IPageDataProvider::OgreIndex(x, y), bridge);
}

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
