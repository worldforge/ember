//
// C++ Interface: TerrainPage
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2005
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

#include <Mercator/Terrain.h>
#include <Mercator/Segment.h>
#include <Mercator/Surface.h>
#include <Mercator/TerrainMod.h>
#include "../MathConverter.h"

namespace EmberOgre {

struct PageSegment
{
	TerrainPosition pos;
	Mercator::Segment* segment;
};

typedef std::vector<PageSegment> SegmentVector;
}

#include "TerrainPageShadow.h"

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

TYPEDEF_STL_MAP(const Mercator::Shader*, TerrainShader*, ShaderMap);



class terrainModListEntry {
	public:
		terrainModListEntry();
		terrainModListEntry(int sx, int sy, int mx, int my, int mz, Mercator::TerrainMod *modifier);

		int X();
		int Y();
		int Z();
		TerrainPosition *Position();
		int SegX();
		int SegY();
		Mercator::TerrainMod *Modifier();
		
	private:
		 // Segment the modifier is applied to
		int	seg_x;
		int	seg_y;
		 // Position of modifier relative to segment
		int	mod_x;
		int	mod_y;
		int	mod_z;

		Mercator::TerrainMod *modifier;
};

/**

@brief Reprents one terrain page.

This is a bridge class between one Ogre terrain page instance and one or many Mercator::Segment. Since each Segment is 64x64 meters, but one Ogre page often is much larger, we need to combine many Segments for every single Ogre page.

Instances of this is created by TerrainGenerator.
One terrain page is composed of both height data, a material, textures for the material and plant positions for the foliage system. This class handles all of these, some of them directly and some by other classes.

@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class TerrainPage{
friend class TerrainPageShadow;
friend class ITerrainPageShadowTechnique;
public:

    TerrainPage(TerrainPosition position, const std::map<const Mercator::Shader*, TerrainShader*> shaderMap, TerrainGenerator* generator);

    ~TerrainPage();
	
	
	/**
	 *    The number of Mercator::Segments for each axis. I.e. the root of the total number of segments.
	 *    Mainly used to check if it's 1, in which case everything becomes much easier.
	 * @return 
	 */
	int getNumberOfSegmentsPerAxis() const;
	
	
	/**
	 *    The max height of this page
	 * @return 
	 */
	float getMaxHeight();
	
	/**
	 *    The minimum height of this page
	 * @return 
	 */
	float getMinHeight();
	
	
	/**
	 *    Generates the terrain materials needed.
	 * @return 
	 */
	Ogre::MaterialPtr generateTerrainMaterials();
	
	/**
	 *    Fills the bound height data with height data. If no buffer has been bound nothing will be done.
	 */
	void updateOgreHeightData(Ogre::Real* heightData);
	
	/**
	 *    The total number of vertices used for this page
	 * @return 
	 */
	long getVerticeCount() const;
	
	/**
	 *    The position of the page in Worldforge space
	 * @return 
	 */
	const TerrainPosition& getWFPosition() const;
	

	/**
	 *    The material used for the page
	 * @return 
	 */
	Ogre::MaterialPtr getMaterial();
	
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

	void populateSurfaces();
	
	void populateSurface(TerrainShader* shader);
	
	int getPageSize() const ;
	
	void update();
	
	void createShadow(const Ogre::Vector3& lightDirection);
	
	void updateShadow(const Ogre::Vector3& lightDirection);
	
		/**
	 *    The size in pixels of one side of the AlphaTexture. This is in sizes of 64.
	 * @return 
	 */
	inline int getAlphaTextureSize() const;
	
	SegmentVector& getValidSegments();
	
	/**
	 *    Gets the extent of this page in meters, in worldforge space.
	 * @return 
	 */
	const WFMath::AxisBox<2>& getExtent() const;
	
// 	const PlantsStore& getPlants() const;
	
	TerrainPageSurface* getSurface() const;
	
	TerrainPageFoliage* getPageFoliage() const;
	
	TerrainPageShadow& getPageShadow();
	
	/**
	 *    Gets the segment positioned at the supplied position in local space.
	 * @param pos A Wordforge position in local space, i.e. > 0 && < [width in meters of the page]
	 * @return A pointer to Mercator::Segment or null.
	 */
	Mercator::Segment* getSegmentAtLocalPosition(const TerrainPosition& pos) const;
	
	/**
	 *    Gets the segment positioned at the supplied position in local space and also translates the supplied position into a local position in the returned segment.
	 * @param pos A Wordforge position in local space, i.e. > 0 && < [width in meters of the page]
	 * @param localPositionInSegment The resulting position in the segment space.
	 * @return A pointer to Mercator::Segment or null.
	 */
	Mercator::Segment* getSegmentAtLocalPosition(const TerrainPosition& pos, TerrainPosition& localPositionInSegment) const;

	
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
	 *    @brief Adds the specified modifier to the segment at (x,y)
	 * This is currently called from TerrainGenerator -tb
	 */
	void addTerrainModifier(int sx, int sy, int mx, int my, int mz, Mercator::TerrainMod *modifier);

	/**
	 *    @brief Returns the (2d) position of an applied modifier
	 *
	 */
	TerrainPosition *getTerrainModifierPos();

	int getTerrainModifierZ();

	/**
	 *    @brief Returns the X index of the segment a modifier is applied to
	*/
	int	TerrainModifierSegX();

	/**
	 *    @brief Returns the Y index of the segment a modifier is applied to
	*/
	int	TerrainModifierSegY();

	/**
	 *    @brief Returns a modifier from the list.
	 *	Each call to this function will return a new modifier until the end of the list
	 */	
	terrainModListEntry NextModListEntry();

	/**
	 *    @brief Gives the size of mTModList
	*/
	int ModListSize();

private:

	SegmentVector mValidSegments;

	
	/**
	this holds a map of the area, to be used in a map widget etc.
	*/
	Ogre::TexturePtr mMap;


	TerrainGenerator* mGenerator;
	
	/**
	Internal position
	*/
	TerrainPosition mPosition;
	
	
	/**
	 *    gets a segment for the x and y position in the page
	 * @param x 
	 * @param y 
	 * @return 
	 */
	Mercator::Segment* getSegmentAtLocalIndex(int indexX, int indexY) const;
	


	
	/**
	* How much to scale the alpha map. This is done to avoid pixelated terrain (a blur filter is applied).
	This value is taken from the config file.
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
	A local copy of the segments for fast lookup. This will also include nonvalid segments.
	The keys will be the local indices.
	*/
	Mercator::Terrain::Segmentstore mLocalSegments;
	
	/**
	@brief Bridge to the ogre terrain engine.
	When the terrain data is changed we need to also update the actual ingame representation that the terrain engine provides. This instance will take care of that.
	*/
	ITerrainPageBridge* mBridge;


/*	typedef std::list<terrainModEntry *, std::allocator<terrainModEntry *> > terrainModList;

	terrainModList mModList;
*/



	std::list<terrainModListEntry> mTModList;
	std::list<terrainModListEntry>::iterator mNextMod;

};

inline int TerrainPage::getAlphaTextureSize( ) const
{
	return (getPageSize() - 1);

}
}
};

#endif
