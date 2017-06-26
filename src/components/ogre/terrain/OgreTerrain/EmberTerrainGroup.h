/*
 Copyright (C) 2013 Erik Ogenvik

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

#ifndef EMBERTERRAINGROUP_H_
#define EMBERTERRAINGROUP_H_

#include <OgreTerrainGroup.h>
#include <sigc++/signal.h>

namespace Ember
{
namespace OgreView
{

class IPageDataProvider;
namespace Terrain
{

/**
 * @brief Creates EmberTerrain instances instead of Ogre::Terrain.
 *
 * Make sure you call setPageDataProvider().
 */
class EmberTerrainGroup: public Ogre::TerrainGroup
{
public:
	EmberTerrainGroup(Ogre::SceneManager* sm, Ogre::uint16 terrainSize, sigc::signal<void, const Ogre::TRect<Ogre::Real>>& terrainShownSignal, Ogre::TerrainMaterialGeneratorPtr materialGenerator);
	virtual ~EmberTerrainGroup();

	void loadAllTerrains(bool synchronous = false);

	void loadTerrain(long x, long y, bool synchronous = false);

	/**
	 * @brief Sets the page data provider.
	 *
	 * This needs to be called before any page can be created.
	 * @param pageDataProvider
	 */
	void setPageDataProvider(IPageDataProvider* pageDataProvider);

	void handleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ);

	/**
	 * @brief Signal emitted when an area of the terrain has been updated.
	 * The argument is the area (in world coordinates) that was updated.
	 */
	sigc::signal<void, Ogre::TRect<Ogre::Real>> EventTerrainAreaUpdated;


protected:

	/**
	 * @brief The page data provider is kept here since it needs to be told when a TerrainPage is destroyed.
	 */
	IPageDataProvider* mPageDataProvider;

	/**
	 * @brief Signal emitted when a page has been shown for the first time.
	 * The argument is the area (in world coordinates) that was shown.
	 */
	sigc::signal<void, const Ogre::TRect<Ogre::Real>>& mTerrainShownSignal;

	Ogre::TerrainMaterialGeneratorPtr mMaterialGenerator;

	/**
	 * @brief Keeps track of the number of currently loading tasks.
	 */
	static unsigned int sLoadingTaskNum;

	void loadEmberTerrainImpl(Ogre::TerrainGroup::TerrainSlot* slot, bool synchronous);

};

}
}
}
#endif /* EMBERTERRAINGROUP_H_ */
