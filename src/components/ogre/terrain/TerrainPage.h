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

#include "TerrainPageShadow.h"

#include <Mercator/Terrain.h>

#include <wfmath/point.h> // <wfmath/axisbox.h> doesn't include it at this point

#include <vector>


namespace WFMath {
	template<int> class AxisBox;
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
class TerrainGenerator;
class TerrainPageSurface;
class TerrainPage;
class TerrainPageFoliage;
class TerrainPageSurfaceLayer;
class ITerrainPageBridge;
//class terrainModListEntry;
class TerrainPageGeometry;
class PlantAreaQuery;


/**

@brief Reprents one terrain page.

This is a bridge class between one Ogre terrain page instance and one or many Mercator::Segment. Since each Segment is 64x64 meters, but one Ogre page often is much larger, we need to combine many Segments for every single Ogre page.

Instances of this is created by TerrainGenerator.
One terrain page is composed of both height data, a material, textures for the material and plant positions for the foliage system. This class handles all of these, some of them directly and some by other classes.

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class TerrainPage{
friend class TerrainPageShadow;
friend class ITerrainPageShadowTechnique;
public:

	TerrainPage(TerrainPosition position, TerrainGenerator& generator);

	~TerrainPage();


	/**
	 *    The number of Mercator::Segments for each axis. I.e. the root of the total number of segments.
	 *    Mainly used to check if it's 1, in which case everything becomes much easier.
	 * @return
	 */
	int getNumberOfSegmentsPerAxis() const;


	/**
	 * @brief Generates the terrain materials needed.
	 * @param reselectTechnique If true, we'll also see if we need to reselect the technique to use.
	 * @return The new material created.
	 */
	Ogre::MaterialPtr generateTerrainMaterials(bool reselectTechnique);

	/**
	 *    Fills the bound height data with height data. If no buffer has been bound nothing will be done.
	 */
	void updateOgreHeightData(Ogre::Real* heightData);

	/**
	 *    The total number of vertices used for this page
	 * @return Number of vertices
	 */
	int getVerticeCount() const;

	/**
	 *    The position of the page in Worldforge space
	 * @return
	 */
	const TerrainPosition& getWFPosition() const;


	/**
	 *    The material used for the page
	 * @return
	 */
	const Ogre::MaterialPtr getMaterial() const;

	/**
	 *    creates a new foliage for this page, but does not show it yet
	 */
	void prepareFoliage();

	/**
	 *    shows the foliage for this page, if no foliage exists it's created
	 */
	void showFoliage();


	/**
	 *    hides the foliage for this page, but does not destroy it
	 */
	void hideFoliage();


	/**
	 *    destroys the foliage for this page
	 */
	void destroyFoliage();

	/**
	 *    this adds a shader to the page, meaning that it will be used in rendering
	 * @param shader
	 */
	TerrainPageSurfaceLayer* addShader(TerrainShader* shader);

	/**
	 *    Updates the shader texture for the specific shader
	 * @param shader
	 */
	TerrainPageSurfaceLayer* updateShaderTexture(TerrainShader* shader, bool repopulate = true);

	/**
	 *    Updates all the shader textures of the page.
	 *    You should usually call this after you've made a change to the terrain and already have called populateSurfaces()
	 */
	void updateAllShaderTextures(bool repopulate = true);

//	void populateSurfaces();

	int getPageSize() const;

	void update();

	void createShadow(const Ogre::Vector3& lightDirection);

	void updateShadow(const Ogre::Vector3& lightDirection);

		/**
	 *    The size in pixels of one side of the AlphaTexture. This is in sizes of 64.
	 * @return
	 */
	int getAlphaTextureSize() const;

	/**
	 *    The max height of this page
	 * @return Max height
	 */
	float getMaxHeight() const;

	/**
	 *    Gets the extent of this page in meters, in worldforge space.
	 * @return
	 */
	const WFMath::AxisBox<2>& getExtent() const;

// 	const PlantsStore& getPlants() const;

	const TerrainPageSurface* getSurface() const;

	/**
	 *    Place the plants for the supplied area in the supplied store.
	 * @param layer The layer which we should use as base for determining what plants to get.
	 * @param plantType The plant type.
	 * @param area The enclosing area.
	 * @param store The store in which to place the plants.
	 */
	void getPlantsForArea(PlantAreaQuery& query) const;

	//	const TerrainPageFoliage* getPageFoliage() const;

	const TerrainPageShadow& getPageShadow() const;



	/**
	 *    @brief Binds a bridge instance to this page.
	 * The bridge will be responsible for updating the terrain engine after the Mercator terrain has changed.
	 * This class won't take ownership of the bridge, so it's up to the calling class to make sure that it's properly destroyed, and when so also calling @see unregisterBridge()
	 * @param bridge A vlid bridge instance.
	 */
	void registerBridge(ITerrainPageBridge* bridge);

	/**
	 *    @brief Unregisters the current terrain bridge.
	 * Make sure to call this when the bridge is destroyed, so as not to leave any dangling pointers. This won't however delete the bridge.
	 */
	void unregisterBridge();


	/**
	 * @brief Gets the normal at the specified local position.
	 * @param localPosition The position, local to the page.
	 * @param normal The normal will be placed here.
	 * @return True if a valid normal could be found at the specified position.
	 */
	bool getNormal(const TerrainPosition& localPosition, WFMath::Vector<3>& normal) const;


private:


	/**
	 * @brief The main terrain generator, which acts as a hub for all terrain functionality.
	 */
	TerrainGenerator& mGenerator;

	/**
	Internal position
	*/
	TerrainPosition mPosition;

	std::auto_ptr<TerrainPageGeometry> mGeometry;



	/**
	* @brief How much to scale the alpha map. This is done to avoid pixelated terrain (a blur filter is applied).
	* This value is taken from the config file.
	*/
	unsigned int getAlphaMapScale() const;

	std::auto_ptr<TerrainPageSurface> mTerrainSurface;
	TerrainPageShadow mShadow;
	ITerrainPageShadowTechnique* mShadowTechnique;
	void setupShadowTechnique();

	/**
	The extent of this page in meters, in WF space.
	*/
	const WFMath::AxisBox<2> mExtent;
	std::auto_ptr<TerrainPageFoliage> mPageFoliage;



	/**
	@brief Bridge to the ogre terrain engine.
	When the terrain data is changed we need to also update the actual ingame representation that the terrain engine provides. This instance will take care of that.
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
