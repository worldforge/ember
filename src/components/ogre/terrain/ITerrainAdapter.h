//
// C++ Interface: ITerrainAdapter
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//

#ifndef ITERRAINADAPTER_H
#define ITERRAINADAPTER_H

#include "domain/Types.h"
#include "../OgreIncludes.h"
#include <sigc++/slot.h>
#include <sigc++/connection.h>
#include <string>

namespace Ember {
class EmberEntity;
namespace OgreView {

struct IPageDataProvider;

namespace Terrain {

class TerrainPage;

class TerrainManager;

class TerrainPageGeometry;

struct ITerrainObserver;


/**
Acts as a bridge between the Mercator terrain system and the Ogre terrain rendering engine. We don't want to directly interface with the terrain rendering system since we want to have the ability to replace or alter that, thus we use an instance of this whenever we need to interact directly to it.

Thus, if you have a feature which requires some kind of interfacing with the terrain rendering system that's not currently available, this is where you put it (and then implement it in the concrete class which implements this interface).

@author Erik Ogenvik
*/
struct ITerrainAdapter {

	virtual ~ITerrainAdapter() = default;

	/**
	 * @brief Gets the size of the width of one page, in indices (so it's often a power of two + 1). This is the number of vertices along the edge of a page.
	 *
	 * @return The page size in indices.
	 */
	virtual int getPageSize() = 0;

	/**
	 * @brief Sets the size of the width of one page. This must be a power of two and at least 64. Depending on the implementation, this might require a full reload.
	 * @param size The number of vertices along one side of a page. Must be a power of two and at least 64.
	 */
	virtual void setPageSize(int size) = 0;

	/**
	 * @brief Sets the distance from the camera at which terrain pages are loaded.
	 * @param loadRadius The distance from the camera at which terrain pages are loaded.
	 */
	virtual void setLoadRadius(Ogre::Real loadRadius) = 0;

	/**
	 * @brief Gets the height.
	 * @param x The x position, in world coords.
	 * @param z The y position, in world coords.
	 * @param height Out parameter which will hold the height, if terrain exists.
	 * @return True if terrain exists at the position.
	 */
	virtual bool getHeightAt(Ogre::Real x, Ogre::Real z, float& height) = 0;

	/**
	 * @brief Sets the main camera used.
	 *
	 * The terrain system needs to know where the main camera is in order to properly calculate proper LOD levels and similiar.
	 * @param camera The main camera.
	 */
	virtual void setCamera(Ogre::Camera* camera) = 0;

	/**
	 * @brief Loads the whole scene.
	 *
	 * Call this when the terrain has been set up and you want to show it.
	 */
	virtual void loadScene() = 0;

	/**
	 * @brief Resets the whole terrain, removing any loaded pages.
	 */
	virtual void reset() = 0;

	/**
	 * @brief Reloads all pages.
	 *
	 * @note This can be very expensive since it will do a full reload of all the terrain. In most instances you don't want to do this and instead just want to reload a page.
	 * @see reloadPage
	 */
	virtual void reloadAllPages() = 0;

	/**
	 * @brief Reloads a single page.
	 *
	 * @param index The index of the page.
	 */
	virtual void reloadPage(const TerrainIndex& index) = 0;

	virtual void showPage(std::shared_ptr<TerrainPageGeometry> geometry) = 0;

	virtual bool isPageShown(const TerrainIndex& index) = 0;

	virtual void removePage(const TerrainIndex& index) = 0;
	/**
	 * @brief Reloads the material of a single page.
	 *
	 * @param index The index of the page.
	 */
	virtual void reloadPageMaterial(const TerrainIndex& index) = 0;

	/**
	 * @brief Provide a short debug information string to be shown in game, for the benefit of the developer.
	 *
	 * @return A short debug information string about the current situation of the scene manager.
	 */
	virtual std::string getDebugInfo() = 0;

	/**
	 * @brief Creates a new terrain observer.
	 * @return A new terrain observer.
	 */
	virtual std::unique_ptr<ITerrainObserver> createObserver() = 0;

	/**
	 * Checks if the given ray intersects the terrain. Returns whether there was an intersection,
	 * and if so, the position of the intersection.
	 * @param ray The Ogre ray to be tested for intersection with the terrain.
	 * @return A pair of an intersection bool and a intersection position.
	 */
	virtual std::pair<EmberEntity*, Ogre::Vector3> rayIntersects(const Ogre::Ray& ray) const = 0;

	/**
	 * Sets the provider used to retrieve terrain page data.
	 * @param pageDataProvider The page data provider
	 */
	virtual void setPageDataProvider(IPageDataProvider* pageDataProvider) = 0;

	/**
	 * @brief Binds a slot to be signalled when a new page is shown for the first time.
	 * The slot is called when the page has been fully loaded and is visible in the world.
	 * @param slot A slot.
	 */
	virtual sigc::connection bindTerrainShown(sigc::slot<void(const Ogre::TRect<Ogre::Real>)>& slot) = 0;

	virtual void setTerrainEntity(EmberEntity* entity) = 0;
};
}
}
}

#endif /* end of include guard: ITERRAINADAPTER_H */
