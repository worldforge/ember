/*
    Copyright (C) 2004  Erik Hjortsberg

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


#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <Ogre.h>
#include <OgreCodec.h>
#include <OgreImage.h>
#include <OgreImageCodec.h>
#include <OgreTextureManager.h>

#include <wfmath/point.h>

#include <Mercator/Terrain.h>
#include <Mercator/Segment.h>
#include <Mercator/FillShader.h>
#include <Mercator/ThresholdShader.h>
#include <Mercator/DepthShader.h>
#include <Mercator/GrassShader.h>
#include <Mercator/Surface.h>
#include <Mercator/Matrix.h>

#include <Eris/Entity.h>
#include <Eris/World.h>
/*
#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#include <Eris/TypeInfo.h>
#include <Eris/Factory.h>


#include <Eris/Connection.h>

#include <Atlas/Objects/Entity/GameEntity.h>
*/
namespace DimeOgre {
	
class TerrainShader;
class DimeTerrainPageSource;

class TerrainGenerator
{
public:

	TerrainGenerator();
	virtual ~TerrainGenerator();
	//static TerrainGenerator & getSingleton(void);
	
	static const int TerrainGenerator::segSize = 64;

	Ogre::SceneNode* generateTerrain();
    void setBasePoint(int x, int y, float z) {mTerrain.setBasePoint(x,y,z);}
	//void loadSegmentAt(WFMath::Point<3> aPoint);
	void prepareSegments(long segmentXStart, long segmentZStart, long numberOfSegments);
	
	virtual float getHeight(float x, float z) const;
	virtual bool initTerrain(Eris::Entity *we, Eris::World *world);
	Ogre::Material* getMaterialForSegment(long x, long y);


protected:

	typedef std::map<std::string, Ogre::Material*> MaterialStore;
	MaterialStore materialStore;
	Mercator::Terrain mTerrain;
	
	
	//static TerrainGenerator* _instance;

//	float* getDataFromMercator(Ogre::TerrainOptions * options);
	
	WFMath::Point<3> mCurrentPosition;

	const Mercator::Terrain::Segmentstore* mSegments;
	
	int mXmin, mXmax, mYmin, mYmax;
	void generateTerrainTexture(Mercator::Segment* segment, long segmentX, long segmentY);
	Ogre::DataChunk* convertWFAlphaTerrainToOgreFormat(Ogre::uchar* dataStart, short factor);
	Ogre::ushort mNumberOfTilesInATerrainPage;
	void createAlphaTexture(Ogre::String name, Mercator::Surface* surface);
	


	/*
	 * Prints the supplied image (as a dataChunk) to a image file.
	 */
	void printTextureToImage(Ogre::DataChunk* dataChunk, const Ogre::String name, Ogre::PixelFormat pixelFormat);
	
	/*
	 * This holds a map of the TerrainShaders
	 */
	std::map<const Mercator::Shader*, TerrainShader*> mShaderMap;
	
	/*
	 * Adds a TerrainShader to the map of shaders.
	 * Note that this also registers the enclosed Mercator::Shader with the 
	 * Mercator::Terrain.
	 */
	void addShader(TerrainShader* shader);
	
	DimeTerrainPageSource* mTerrainPageSource;
};
}


#endif // TERRAINGENERATOR_H
