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


#include "MathConverter.h"
#include "OgreStringConverter.h"
#include "services/logging/LoggingService.h"
#include "DimeTerrainRenderable.h"

#include "TerrainGenerator.h"


using namespace Ogre;

TerrainGenerator* TerrainGenerator::_instance = 0;


TerrainGenerator & TerrainGenerator::getSingleton(void)
{
	//fprintf(stderr, "TRACE - ENTITY LISTENER - SINGLETON ENTERING\n");
	if(_instance == 0)
		_instance = new TerrainGenerator;
	return *_instance;
}


TerrainGenerator::TerrainGenerator() : mTerrain(Mercator::Terrain::SHADED)
{
    mTerrain.addShader(new Mercator::FillShader());
}

TerrainGenerator::~TerrainGenerator()
{}


void TerrainGenerator::prepareSegments(long segmentXStart, long segmentZStart, long numberOfSegments)
{
	int i,j;
	for (i = segmentXStart; i < segmentXStart + numberOfSegments; ++i) {
		for (j = segmentZStart; j < segmentZStart + numberOfSegments; ++j) {
			mTerrain.getSegment(i, j)->populate();
		}
	}
	
}


float TerrainGenerator::getHeight(long ogreX, long ogreZ)
{
	
	//convert our ogre coordinates to atlas
	long atlasX = ogreX;
	long atlasY = -ogreZ;
	
	float height = mTerrain.get(atlasX, atlasY);
	return height;
	
}

bool TerrainGenerator::initTerrain(Eris::Entity *we, Eris::World *world) 
{

   if (!we->hasProperty("terrain")) {
        std::cerr << "World entity has no terrain" << std::endl << std::flush;
        std::cerr << "World entity id " << we->getID() << std::endl
                  << std::flush;
        return false;
    }
    const Atlas::Message::Element &terrain = we->getProperty("terrain");
    if (!terrain.isMap()) {
        std::cerr << "Terrain is not a map" << std::endl << std::flush;
    }
    const Atlas::Message::Element::MapType & tmap = terrain.asMap();
    Atlas::Message::Element::MapType::const_iterator I = tmap.find("points");
    int xmin = 0, xmax = 0, ymin = 0, ymax = 0;
    if (I == tmap.end()) {
        std::cerr << "No terrain points" << std::endl << std::flush;
    }
	if (I->second.isList()) {
        // Legacy support for old list format.
        const Atlas::Message::Element::ListType & plist = I->second.asList();
        Atlas::Message::Element::ListType::const_iterator J = plist.begin();
        for(; J != plist.end(); ++J) {
            if (!J->isList()) {
                std::cout << "Non list in points" << std::endl << std::flush;
                continue;
            }
            const Atlas::Message::Element::ListType & point = J->asList();
            if (point.size() != 3) {
                std::cout << "point without 3 nums" << std::endl << std::flush;
                continue;
            }
            int x = (int)point[0].asNum();
            int y = (int)point[1].asNum();
            xmin = std::min(xmin, x);
            xmax = std::max(xmax, x);
            ymin = std::min(ymin, y);
            ymax = std::max(ymax, y);
    //        m_terrain.setBasePoint(x, y, point[2].asNum());
			mTerrain.setBasePoint(x,y,point[2].asNum());
        }
    } else if (I->second.isMap()) {

        const Atlas::Message::Element::MapType & plist = I->second.asMap();
        Atlas::Message::Element::MapType::const_iterator J = plist.begin();
        for(; J != plist.end(); ++J) {
            if (!J->second.isList()) {
                std::cout << "Non list in points" << std::endl << std::flush;
                continue;
            }
            const Atlas::Message::Element::ListType & point = J->second.asList();
            if (point.size() != 3) {
                std::cout << "point without 3 nums" << std::endl << std::flush;
                continue;
            }
            int x = (int)point[0].asNum();
            int y = (int)point[1].asNum();
            xmin = std::min(xmin, x);
            xmax = std::max(xmax, x);
            ymin = std::min(ymin, y);
            ymax = std::max(ymax, y);
          //  m_terrain.setBasePoint(x, y, point[2].asNum());
			mTerrain.setBasePoint(x,y,point[2].asNum());
//System::instance()->getGraphics()->getTerrainRenderer()->m_terrain.setBasePoint(x,y,point[2].asNum());
//System::Instance()->getGraphics(x,y,point[2].asNum());
        }

      

    } else {
        std::cerr << "Terrain is the wrong type" << std::endl << std::flush;
        return false;
    }
    mSegments = &mTerrain.getTerrain();
    return true;
}



