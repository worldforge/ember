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



namespace Ogre
{
	class TerrainOptions;
}

namespace EmberOgre {
	
class TerrainShader;

class TerrainPage;


/**
 * This class takes care of generating terrain for Ogre's scenemanager.
 * This involves getting terraing from Mercator, converting this to ogre
 * format and creating materials to make it look good.
 * 
 * It works closely with EmberTerrainPageSource.
 * 
 */
class TerrainGenerator :  public Ogre::FrameListener
{
public:

	/**
	 * STL map to store sparse array of TerrainPage pointers.
	 * 
	 */ 
	typedef std::map<int, TerrainPage *> TerrainPagecolumn;
     
			   
	
	/**
	* STL map to store sparse array of TerrainPage pointer columns.
	*/
	typedef std::map<int, TerrainPagecolumn > TerrainPagestore;
	
	
	TerrainGenerator();
	virtual ~TerrainGenerator();

	
	virtual bool frameStarted(const Ogre::FrameEvent & evt);
	
	Ogre::SceneNode* generateTerrain();
    void setBasePoint(int x, int y, float z) {mTerrain->setBasePoint(x,y,z);}
	void prepareSegments(long segmentXStart, long segmentZStart, long numberOfSegments, bool alsoPushOntoTerrain);

	/**
	 * Prepares all segments aquired from Mercator. Note that this can be very,
	 * very expensive if there's a lot of terrain defined.
	 */
	void TerrainGenerator::prepareAllSegments();
	
	virtual float getHeight(const TerrainPosition& atPosition) const;
	virtual bool initTerrain(Eris::Entity *we, Eris::View *world);
	
	/**
	 * Return true if there is a valid piece of terrain at the supplied segment indices.
	 * By valid means a populated terrain-
	 */
	bool isValidTerrainAt(const TerrainPosition& pos);
	
	//const Ogre::TerrainOptions& getTerrainOptions() const;

	const Mercator::Terrain& getTerrain() const;
	
	const TerrainPosition getMax() const;
	const TerrainPosition getMin() const;
	
	/**
	 *    the size of one terrain segment
	 *	(note that this is different from Mercator segments, which always are of size 64)
	 * @return 
	 */
	int getSegmentSize() const;
	
	
	/**
	 *    Adds a new Mercator::Area to the terrain.
	 * @param area 
	 */
	void addArea(Mercator::Area* area);
	
//	TerrainPage* getTerrainPage(uint x, uint z);
	
	/**
	 *    Returns a TerrainPage. If there is no yet existing, a new one is created.
	 * @param ogrePosition 
	 * @return 
	 */
	TerrainPage* getTerrainPage(const Ogre::Vector2& ogrePosition);
	

// 	GroundCover* mGround;
// 	void generateUnderVegetation(long segmentXStart, long segmentZStart, long numberOfSegments);
	
	int getPageSize() const;


protected:
	typedef std::map<int,TerrainShader*> AreaShaderstore;
  	AreaShaderstore mAreaShaders;

	void markShaderForUpdate(TerrainShader* shader);
	
	typedef std::set<TerrainShader*> ShaderSet;
	ShaderSet mShadersToUpdate;
	
	typedef std::map<std::string, TerrainPage*> PageStore;
	PageStore mPages;
	
	TerrainPagestore mTerrainPages;
	
	TerrainShader* mGrassShader;
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
	
	//Ogre::TerrainOptions mOptions;
	
	

	/**
	 *   Creates a new TerrainPage and puts it in mTerrainPages
	 * @param pos 
	 */
	TerrainPage* createPage(const TerrainPosition& pos);

	
	/**
	 * This holds a map of the TerrainShaders
	 */
	std::map<const Mercator::Shader*, TerrainShader*> mShaderMap;
	
	/**
	a list of the shaders, which will all be used on all Pages
	*/
	std::list<TerrainShader*> mBaseShaders;
	 
	/**
	 * Create and registers a new texture shader.
	 * @param textureName 
	 * @param mercatorShader 
	 * @return 
	 */
	TerrainShader* createShader(const std::string& textureName, Mercator::Shader* mercatorShader);
	
	TerrainShader* createShader(Ogre::MaterialPtr material, Mercator::Shader* mercatorShader);
	
	//EmberTerrainPageSource* mTerrainPageSource;
};
}


#endif // TERRAINGENERATOR_H
