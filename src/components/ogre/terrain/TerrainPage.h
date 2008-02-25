//
// C++ Interface: TerrainPage
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
namespace Environment {
class FoliageArea;
}
namespace Terrain {
class TerrainShader;
class TerrainGenerator;
class TerrainPageSurface;
class TerrainPage;
class TerrainPageFoliage;
class TerrainPageSurfaceLayer;

// TYPEDEF_STL_VECTOR(Mercator::Segment*, SegmentVector);
TYPEDEF_STL_MAP(const Mercator::Shader*, TerrainShader*, ShaderMap);




/**

This is a bridge class between one Ogre::TerrainPage instance and one or many Mercator::Segment.


@author Erik Hjortsberg
*/
class TerrainPage{
friend class TerrainPageShadow;
friend class ITerrainPageShadowTechnique;
public:
	/**
	A store of plant positions. We keep this in ogre space for performance reasons.
	*/
// 	typedef std::vector<Ogre::Vector2> PlantsStore;

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
	 *    Fills the supplied data buffer with height data. Make sure that the supplied buffer can take getVerticeCount() number of elements.
	 * @param heightData 
	 */
	void createHeightData(Ogre::Real* heightData);

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
	TerrainPageSurfaceLayer* updateShaderTexture(TerrainShader* shader);
	
	/**
	 *    Updates all the shader textures of the page.
	 *    You should usually call this after you've made a change to the terrain and already have called populateSurfaces()
	 */
	void updateAllShaderTextures();

	void populateSurfaces();
	
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


private:

	SegmentVector mValidSegments;

	
	::EmberOgre::Environment::FoliageArea* mFoliageArea;
	

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
// 	void updateShadow();
// 	
// 	void createShadow();


};

inline int TerrainPage::getAlphaTextureSize( ) const
{
	return (getPageSize() - 1);

}
}
};

#endif
