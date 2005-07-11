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

#include "EmberOgrePrerequisites.h"

#include <Mercator/Terrain.h>
#include <Mercator/Segment.h>
// #include <Mercator/FillShader.h>
// #include <Mercator/ThresholdShader.h>
// #include <Mercator/DepthShader.h>
// #include <Mercator/GrassShader.h>
#include <Mercator/Surface.h>
// #include <Mercator/Matrix.h>
namespace Ogre
{
	class TerrainOptions;
}

namespace EmberOgre {

class TerrainShader;
class TerrainGenerator;
class FoliageArea;

TYPEDEF_STL_VECTOR(Mercator::Segment*, SegmentVector);
TYPEDEF_STL_MAP(const Mercator::Shader*, TerrainShader*, ShaderMap);

struct PageSegment
{
	TerrainPosition pos;
	Mercator::Segment* segment;
};

/**

This is a bridge class between one Ogre::TerrainPage instance and one or many Mercator::Segment.


@author Erik Hjortsberg
*/
class TerrainPage{
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
	Ogre::MaterialPtr getMaterial() const;
	
	/**
	 *    creates the foliage for this page
	 */
	void createFoliage(TerrainShader* grassShader);
	
	/**
	 *    this adds a shader to the page, meaning that it will be used in rendering 
	 * @param shader 
	 */
	void addShader(TerrainShader* shader);
	
	void updateShaderTexture(TerrainShader* shader);
	
	void populateSurfaces();
	
	int getPageSize() const ;
	
	
private:


	void addShaderToSimpleTechnique(Ogre::Technique* technique, TerrainShader* shader);

	typedef std::map<TerrainShader*, Ogre::TexturePtr> ShaderTextureMap;
	ShaderTextureMap mShaderTextures;
	
	typedef std::vector<PageSegment> SegmentVector;
	SegmentVector mValidSegments;

	/**
	a list of the shaders to be used on the page
	*/
	std::list<TerrainShader*> mUsedShaders;
	
	FoliageArea* mFoliageArea;

	/**
	this holds a map of the area, to be used in a map widget etc.
	*/
	Ogre::TexturePtr mMap;


	TerrainGenerator* mGenerator;
	
	/**
	Internal position
	*/
	TerrainPosition mPosition;
	Ogre::MaterialPtr mMaterial;
	
	
	/**
	 *    gets a segment for the x and y position in the page
	 * @param x 
	 * @param y 
	 * @return 
	 */
	Mercator::Segment* getSegment(int x, int y) const;
	

	/**
	Not really used
	*/
	std::string mMaterialName;

	/**
	 * We can't use the alphamaps generated from WF. Thus we need to convert them first.
	 * Which is done by this method.
	 * 
	 * @param dataStart 
	 * @param factor 
	 * @return 
	 */
	Ogre::MemoryDataStreamPtr convertWFAlphaTerrainToOgreFormat(Ogre::uchar* dataStart, short factor);
	
	
	/**
	 * Method used when the mapping between mercator segments and ogre pages is 1:1
	 * @return 
	 */
	void generateTerrainTechniqueSimple(Ogre::Technique* technique);
	
	
	/**
	 *    generates the techniques for debug material
	 */
	void generateTerrainTechniqueDebug();
	/**
	 * Method used when the mapping between mercator segments and ogre pages isn't 1:1
	 * @return 
	 */
	void generateTerrainTechniqueComplex(Ogre::Technique* technique);
	
	void generateTerrainTechniqueComplexAtlas(Ogre::Technique* technique);
	
	/**
	EmberOgre::Shaders used for this page
	*/
	ShaderMap mShaderMap;
	
	/**
	 *    Creates an alpha texture for the supplied surface.
	 * @param name name of the new texture
	 * @param surface the surface used
	 * @return 
	 */
	Ogre::TexturePtr createAlphaTexture(Ogre::String name, Mercator::Surface* surface);
	
// 	inline const Ogre::TerrainOptions& getTerrainOptions() const;
	
	/**
	 * Prints the supplied image (as a dataChunk) to a image file.
	 */
	void printTextureToImage(Ogre::MemoryDataStreamPtr dataChunk, const Ogre::String name, Ogre::PixelFormat pixelFormat, int width, int height);

	/**
	 *    The size in pixels of one side of the AlphaTexture. This is in sizes of 64.
	 * @return 
	 */
	inline int getAlphaTextureSize() const;
	
	const unsigned int mBytesPerPixel;
	
	/**
	* How much to scale the alpha map. This is done to avoid pixelated terrain (a blur filter is applied)
	*/
	const unsigned int mAlphaMapScale;

	void fillAlphaLayer(unsigned char* imagePtr, unsigned char* wfImagePtr, unsigned int channel, int startX, int startY);

};



};

#endif
