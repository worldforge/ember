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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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

	~OgreTerrainPageBridge() override;

	void updateTerrain(TerrainPageGeometry& geometry) override;

	void terrainPageReady() override;

	bool isPageShown() const override;

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

	/**
	 * @brief The height data, in Ogre format.
	 *
	 * This is updated in a background thread through a call to updateTerrain(...), and then transfered to
	 * the terrain component in terrainPageReady().
	 * We're using a shared_ptr to make sure that the data isn't deleted if different threads are accessing
	 * updateTerrain() and terrainPageReady() at the same time (without having to use locks).
	 */
	std::shared_ptr<float> mHeightData;

};

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
#endif /* OGRETERRAINPAGEBRIDGE_H_ */
