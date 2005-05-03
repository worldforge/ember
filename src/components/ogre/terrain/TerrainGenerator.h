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


/**
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

	Ogre::SceneNode* generateTerrain();
    void setBasePoint(int x, int y, float z) {mTerrain->setBasePoint(x,y,z);}
	void prepareSegments(long segmentXStart, long segmentZStart, long numberOfSegments, bool alsoPushOntoTerrain);

	/**
	 * Prepares all segments aquired from Mercator. Note that this can be very,
	 * very expensive if there's a lot of terrain defined.
	 * If true is supplied the segments will also be pushed onto the terrain
	 */
	void TerrainGenerator::prepareAllSegments(bool alsoPushOntoTerrain);
	
	virtual float getHeight(const TerrainPosition& atPosition) const;
	virtual bool initTerrain(Eris::Entity *we, Eris::View *world);
	
	/**
	 * Return true if there is a valid piece of terrain at the supplied segment indices.
	 * By valid means a populated terrain-
	 */
	bool isValidTerrainAt(TerrainPosition& atPosition);
	
	const Ogre::TerrainOptions& getTerrainOptions() const;

	const Mercator::Terrain& getTerrain() const;
	
	const TerrainPosition getMax() const;
	const TerrainPosition getMin() const;
	
	/**
	 *    the size of one terrain segment
	 *	(note that this is different from Mercator segments, which always are of size 64)
	 * @return 
	 */
	int getSegmentSize() const;

// 	GroundCover* mGround;
// 	void generateUnderVegetation(long segmentXStart, long segmentZStart, long numberOfSegments);

protected:


	Mercator::Shader* mGrassShader;
	typedef std::map<std::string, Ogre::Material*> MaterialStore;
	MaterialStore materialStore;
	Mercator::Terrain* mTerrain;
	
	
	//static TerrainGenerator* _instance;

//	float* getDataFromMercator(Ogre::TerrainOptions * options);
	
	WFMath::Point<3> mCurrentPosition;

	const Mercator::Terrain::Segmentstore* mSegments;
	
	/**
	 * the min and max indices for segments
	 */
	int mXmin, mXmax, mYmin, mYmax;
	
	void loadTerrainOptions();
	
	Ogre::TerrainOptions mOptions;
	
	


	
	/**
	 * This holds a map of the TerrainShaders
	 */
	std::map<const Mercator::Shader*, TerrainShader*> mShaderMap;
	
	/**
	 * Adds a TerrainShader to the map of shaders.
	 * Note that this also registers the enclosed Mercator::Shader with the 
	 * Mercator::Terrain.
	 */
	void addShader(TerrainShader* shader);
	
	EmberTerrainPageSource* mTerrainPageSource;
};
}


#endif // TERRAINGENERATOR_H
