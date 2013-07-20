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

#ifndef OGRETERRAINPAGEBRIDGE_H_
#define OGRETERRAINPAGEBRIDGE_H_

#include "../ITerrainPageBridge.h"

#include <OgrePrerequisites.h>
#include <condition_variable>
#include <thread>

namespace Ogre
{
class TerrainGroup;
}

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

/**
 * @brief Updates Ogre terrain rendering data based on Mercator-provided data.
 */
class OgreTerrainPageBridge : public Ember::OgreView::Terrain::ITerrainPageBridge
{
public:
	typedef std::pair<long, long> IndexType;

	OgreTerrainPageBridge(Ogre::TerrainGroup& terrainGroup, IndexType index);

	virtual ~OgreTerrainPageBridge();

	virtual void updateTerrain(TerrainPageGeometry& geometry);

	virtual void terrainPageReady();

	/** 
	 * The mutex used by OgreTerrainDefiner to wait for the loading of a terrain page to be finished.
	 */
	std::mutex mMutex;

	/** 
	 * The condition variable used by OgreTerrainDefiner to wait for the loading of a terrain page to be finished.
	 */
	std::condition_variable mConditionVariable;

private:
	Ogre::TerrainGroup& mTerrainGroup;

	IndexType mIndex;

	float* mHeightData;
};

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
#endif /* OGRETERRAINPAGEBRIDGE_H_ */
