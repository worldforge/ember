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

class TerrainGenerator
{
public:

	TerrainGenerator();
	virtual ~TerrainGenerator();
	static TerrainGenerator & getSingleton(void);
	
	static const int TerrainGenerator::segSize = 64;

	Ogre::SceneNode* generateTerrain();
    void setBasePoint(int x, int y, float z) {mTerrain.setBasePoint(x,y,z);}
	//void loadSegmentAt(WFMath::Point<3> aPoint);
	void prepareSegments(long segmentXStart, long segmentZStart, long numberOfSegments);
	
	virtual float getHeight(float x, float z);
	virtual bool initTerrain(Eris::Entity *we, Eris::World *world);


protected:
	Mercator::Terrain mTerrain;
	
	static TerrainGenerator* _instance;

//	float* getDataFromMercator(Ogre::TerrainOptions * options);
	
	WFMath::Point<3> mCurrentPosition;

	const Mercator::Terrain::Segmentstore* mSegments;

};

#endif // TERRAINGENERATOR_H
