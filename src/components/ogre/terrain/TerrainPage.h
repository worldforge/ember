//
// C++ Interface: TerrainPage
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
#ifndef EMBEROGRETERRAINPAGE_H
#define EMBEROGRETERRAINPAGE_H

#include "../EmberOgrePrerequisites.h"
#include "../Types.h"

#include <wfmath/point.h>
#include <wfmath/axisbox.h>

#include <vector>


namespace WFMath {
	template<int> class Vector;
}

namespace Mercator {
	class Segment;
}


namespace Ogre
{
	class TerrainOptions;
}

namespace EmberOgre {
namespace Terrain {
class TerrainShader;
class TerrainManager;
class TerrainPageSurface;
class TerrainPage;
class TerrainPageFoliage;
class TerrainPageSurfaceLayer;
class ITerrainPageBridge;
class PlantAreaQueryResult;
class TerrainPageGeometry;

/**

@brief Represents one terrain page.

This is a bridge class between one Ogre terrain page instance and one or many Mercator::Segment. Since each Segment is 64x64 meters, but one Ogre page often is much larger, we need to combine many Segments for every single Ogre page.
An instance of this is very light weight as it doesn't keep any geometry data around. Each time an action involving page geometry is to occur an instance of TerrainPageGeometry must therefore be created.

Instances of this is created by TerrainManager.
One terrain page is composed of both height data, a material, textures for the material and plant positions for the foliage system. This class handles all of these, some of them directly and some by other classes.

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class TerrainPage {
friend class TerrainPageShadow;
friend class ITerrainPageShadowTechnique;
public:

	/**
	 * @brief Ctor.
	 * @param position The page index in WF space.
	 * @param manager The terrain manager.
	 */
	TerrainPage(const TerrainPosition& position, TerrainManager& manager);

	/**
	 * @brief Dtor.
	 */
	~TerrainPage();

	/**
	 * @brief The number of Mercator::Segments for each axis. I.e. the root of the total number of segments.
	 * Mainly used to check if it's 1, in which case everything becomes much easier.
	 * @return
	 */
	int getNumberOfSegmentsPerAxis() const;

	/**
	 * @brief The total number of vertices used for this page.
	 * @return Number of vertices.
	 */
	int getVerticeCount() const;

	/**
	 * @brief The position of the page in Worldforge space.
	 * @return
	 */
	const TerrainPosition& getWFPosition() const;

	/**
	 * @brief The material used for the page.
	 * @return
	 */
	const Ogre::MaterialPtr getMaterial() const;

	/**
	 * @brief Creates a new foliage for this page, but does not show it yet.
	 */
	void prepareFoliage();

	/**
	 * @brief Shows the foliage for this page, if no foliage exists it's created.
	 */
	void showFoliage();

	/**
	 * @brief Hides the foliage for this page, but does not destroy it.
	 */
	void hideFoliage();


	/**
	 * @brief Destroys the foliage for this page.
	 */
	void destroyFoliage();

	/**
	 * @brief Adds a shader to the page, meaning that it will be used in rendering.
	 * @param shader The new shader to add.
	 */
	TerrainPageSurfaceLayer* addShader(const TerrainShader* shader);

	/**
	 * @brief Updates the shader texture for the specific shader.
	 * @param shader The shader to update.
	 */
	TerrainPageSurfaceLayer* updateShaderTexture(const TerrainShader* shader, TerrainPageGeometry& geometry, bool repopulate = true);

	/**
	 * @brief Updates all the shader textures of the page.
	 * You should usually call this after you've made a change to the terrain and already have called populateSurfaces().
	 */
	void updateAllShaderTextures(TerrainPageGeometry& geometry, bool repopulate = true);

	void regenerateCoverageMap();

	int getPageSize() const;

	void signalGeometryChanged();

	/**
	 * @brief The size in pixels of one side of the AlphaTexture. This is in sizes of 64.
	 * @return
	 */
	int getAlphaTextureSize() const;


	/**
	 * @brief Gets the extent of this page in meters, in worldforge space.
	 * @return
	 */
	const WFMath::AxisBox<2>& getExtent() const;

	/**
	 * @brief Accessor for the page surface belonging to this page.
	 * @returns The page surface instance belonging to this page.
	 */
	const TerrainPageSurface* getSurface() const;

	/**
	 * @brief Place the plants for the supplied area in the supplied store.
	 * @param query A query result instance which will be populated by the method.
	 * @param geometry A geometry instance for this page.
	 */
	void getPlantsForArea(PlantAreaQueryResult& queryResult, TerrainPageGeometry& geometry) const;

	/**
	 * @brief Binds a bridge instance to this page.
	 * The bridge will be responsible for updating the terrain engine after the Mercator terrain has changed.
	 * This class won't take ownership of the bridge, so it's up to the calling class to make sure that it's properly destroyed, and when so also calling @see unregisterBridge()
	 * @param bridge A valid bridge instance.
	 */
	void registerBridge(ITerrainPageBridge* bridge);

	/**
	 * @brief Unregisters the current terrain bridge.
	 * Make sure to call this when the bridge is destroyed, so as not to leave any dangling pointers. This won't however delete the bridge.
	 */
	void unregisterBridge();

	/**
	 * @brief Notifies Ogre throught the bridge that page is ready.
	 */
	void notifyBridgePageReady();

	/**
	 * @brief Accessor for a bridge registered to this page.
	 * @returns A bridge instance, which acts as a connection between the page and any rendering system. Null if no bridge has been registered with the page.
	 */
	ITerrainPageBridge* getBridge() const;

	/**
	 * @brief Gets the normal at the specified local position.
	 * @param localPosition The position, local to the page.
	 * @param normal The normal will be placed here.
	 * @return True if a valid normal could be found at the specified position.
	 */
	bool getNormal(const TerrainPosition& localPosition, WFMath::Vector<3>& normal) const;

private:

	/**
	 * @brief The main terrain manager, which acts as a hub for all terrain functionality.
	 */
	TerrainManager& mManager;

	/**
	 * @brief Internal position
	 */
	TerrainPosition mPosition;

	/**
	* @brief How much to scale the alpha map. This is done to avoid pixelated terrain (a blur filter is applied).
	* This value is taken from the config file.
	*/
	unsigned int getAlphaMapScale() const;

	std::auto_ptr<TerrainPageSurface> mTerrainSurface;

	/**
	 * @brief The extent of this page in meters, in WF space.
	 */
	const WFMath::AxisBox<2> mExtent;
	std::auto_ptr<TerrainPageFoliage> mPageFoliage;



	/**
	 *@brief Bridge to the ogre terrain engine.
	 * When the terrain data is changed we need to also update the actual ingame representation that the terrain engine provides. This instance will take care of that.
	 */
	ITerrainPageBridge* mBridge;


};

inline int TerrainPage::getAlphaTextureSize() const
{
	return (getPageSize() - 1);

}
}
}

#endif
