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

#include "EmberOgrePrerequisites.h"
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
#include <Eris/View.h>

#include "EmberSceneManager/include/OgreTerrainRenderable.h"

/*
#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#include <Eris/TypeInfo.h>
#include <Eris/Factory.h>


#include <Eris/Connection.h>

#include <Atlas/Objects/Entity/GameEntity.h>
*/
// class GroundCover;
namespace Ogre
{
	class TerrainOptions;
}

namespace EmberOgre {
	
class TerrainShader;
class EmberTerrainPageSource;


/*
 * This class takes care of generating terrain for Ogre's scenemanager.
 * This involves getting terraing from Mercator, converting this to ogre
 * format and creating materials to make it look good.
 * 
 * It works closely with EmberTerrainPageSource.
 * 
 */
class TerrainGenerator
{
public:

	TerrainGenerator();
	virtual ~TerrainGenerator();
	//static TerrainGenerator & getSingleton(void);
	
	//static const int TerrainGenerator::segSize = 64;

	Ogre::SceneNode* generateTerrain();
    void setBasePoint(int x, int y, float z) {mTerrain->setBasePoint(x,y,z);}
	//void loadSegmentAt(WFMath::Point<3> aPoint);
	void prepareSegments(long segmentXStart, long segmentZStart, long numberOfSegments, bool alsoPushOntoTerrain);

	/*
	 * Prepares all segments aquired from Mercator. Note that this can be very,
	 * very expensive if there's a lot of terrain defined.
	 * If true is supplied the segments will also be pushed onto the terrain
	 */
	void TerrainGenerator::prepareAllSegments(bool alsoPushOntoTerrain);
	
	virtual float getHeight(float x, float z) const;
	virtual bool initTerrain(Eris::Entity *we, Eris::View *world);
	Ogre::Material* getMaterialForSegment(long x, long y);

	float TerrainGenerator::getMaxHeightForSegment(int ogreX, int ogreZ) const;
	float TerrainGenerator::getMinHeightForSegment(int ogreX, int ogreZ) const;
	
	/*
	 * Return true if there is a valid piece of terrain at the supplied segment indices.
	 * By valid means a populated terrain with a corresponding material-
	 */
	bool isValidTerrainAt(int x, int y);
	
	const Ogre::TerrainOptions& getTerrainOptions() const;

	const Mercator::Terrain& getTerrain() const;

// 	GroundCover* mGround;
// 	void generateUnderVegetation(long segmentXStart, long segmentZStart, long numberOfSegments);

protected:

	typedef std::map<std::string, Ogre::Material*> MaterialStore;
	MaterialStore materialStore;
	Mercator::Terrain* mTerrain;
	
	
	//static TerrainGenerator* _instance;

//	float* getDataFromMercator(Ogre::TerrainOptions * options);
	
	WFMath::Point<3> mCurrentPosition;

	const Mercator::Terrain::Segmentstore* mSegments;
	
	/*
	 * the min and max indices for segments
	 */
	int mXmin, mXmax, mYmin, mYmax;
	
	/*
	 * Creates a material for the supplied segment. This is done through the 
	 * registered TerrainShaders.
	 * The created material is then put into mShaderMap.
	 * Use getMaterialForSegment(...) to access the material.
	 */
	void generateTerrainMaterials(Mercator::Segment* segment, long segmentX, long segmentY);

	void loadTerrainOptions();
	
	Ogre::TerrainOptions mOptions;
	
	
	/*
	 * We can't use the alphamaps generated from WF. Thus we need to convert them first.
	 * Which is done by this method.
	 */
	Ogre::DataChunk* convertWFAlphaTerrainToOgreFormat(Ogre::uchar* dataStart, short factor);
	//Ogre::ushort mNumberOfTilesInATerrainPage;

	/*
	 * Creates an alpha texture for the supplied surface.
	 */
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
	
	EmberTerrainPageSource* mTerrainPageSource;
};
}


#endif // TERRAINGENERATOR_H
