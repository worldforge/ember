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

TYPEDEF_STL_VECTOR(Mercator::Segment*, SegmentVector);
TYPEDEF_STL_MAP(const Mercator::Shader*, TerrainShader*, ShaderMap);

/**

This is a bridge class between one Ogre::TerrainPage instance and one or many Mercator::Segment.


@author Erik Hjortsberg
*/
class TerrainPage{
public:
    TerrainPage(TerrainPosition& position, const std::map<const Mercator::Shader*, TerrainShader*> shaderMap, TerrainGenerator* generator);

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
	 *    The position of the page in ogre space
	 * @return 
	 */
	const Ogre::Vector3 getOgrePosition() const;
	
	/**
	 *    The material used for the page
	 * @return 
	 */
	Ogre::MaterialPtr getMaterial() const;
	
private:

	TerrainGenerator* mGenerator;
	
	/**
	Internal position
	*/
	TerrainPosition mPosition;
	Ogre::MaterialPtr mMaterial;
	
	/**
		A vector of all the Mercator segments needed for this page. If the ratio between Mercator segments and Ogre pages is 1:1
		this will only contain one element. If it's 2:1 it will contain 4, if 3:1 it will contain 9 and so on. (mercator_segments*mercator_segments)
		The segments are stored in a vector like this:
		|3|4|
		|---|
		|1|2|
		
		(where the horizontal axis is the x-axis and the vertical the y axis)
	*/
	SegmentVector mMercatorSegments;
// 	void createAlphaTexture(Ogre::String name, Mercator::Surface* surface);

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
	Ogre::MaterialPtr generateTerrainMaterialSimple();
	
	
	/**
	 * Method used when the mapping between mercator segments and ogre pages isn't 1:1
	 * @return 
	 */
	Ogre::MaterialPtr generateTerrainMaterialComplex();
	
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
	
	inline const Ogre::TerrainOptions& getTerrainOptions() const;
	
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

};



};

#endif
