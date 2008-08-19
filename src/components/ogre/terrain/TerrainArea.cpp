//
// C++ Implementation: TerrainArea
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainArea.h"

#include "../EmberEntity.h"
#include <Mercator/Area.h>

namespace EmberOgre {
namespace Terrain {

TerrainArea::TerrainArea(EmberEntity* entity) : mArea(0), mEntity(entity)
{
}


TerrainArea::~TerrainArea()
{
	mAttrChangedSlot.disconnect();
	EventAreaRemoved.emit(this);
	delete mArea;
}

bool TerrainArea::init() {
	
 //   _fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR , _MCW_RC);
	bool successfulParsing = parseArea();
	if (successfulParsing) {
		observeEntity();
	}
	return successfulParsing;
	
}

bool TerrainArea::parseArea()
{
	if (!mEntity->hasAttr("area")) {
        S_LOG_FAILURE("TerrainArea created for entity with no area attribute");
        return false;
    }
    
    const Atlas::Message::MapType& areaData(mEntity->valueOfAttr("area").asMap());
    Atlas::Message::MapType::const_iterator it = areaData.find("points");
    if ((it == areaData.end()) || !it->second.isList()) {
        S_LOG_FAILURE("malformed area attribute on entity, no points data");
        return false;
    }
    
    const Atlas::Message::ListType& pointsData(it->second.asList());
    it = areaData.find("layer");
    if ((it == areaData.end()) || !it->second.isInt()) {
        S_LOG_FAILURE("malformed area attribute on entity, no layer data");
        return false;
    }

    int layer = it->second.asInt();
    if (!mArea) {
    	mArea = new Mercator::Area(layer, false);
    }
       
    WFMath::Polygon<2> poly;
    for (size_t p=0; p<pointsData.size(); ++p) {
        if (!pointsData[p].isList()) {
            S_LOG_FAILURE("skipped malformed point in area");
            continue;
        }
        
        const Atlas::Message::ListType& point(pointsData[p].asList());
        if ((point.size() < 2) || !point[0].isNum() || !point[1].isNum()) {
           S_LOG_FAILURE("skipped malformed point in area");
            continue;
        }
        
        WFMath::Point<2> wpt(point[0].asNum(), point[1].asNum());
        poly.addCorner(poly.numCorners(), wpt);
    }
//	if (poly.numCorners() < 3) { TODO: check whether we really can define an area with only one point
	if (poly.numCorners() == 0) {
		S_LOG_FAILURE("Could not find enough points to define the area. Found " << poly.numCorners() << " points.");
		return false;
	}
 // transform polygon into terrain coords
    WFMath::Vector<3> xVec = WFMath::Vector<3>(1.0, 0.0, 0.0).rotate(mEntity->getOrientation());
    double theta = atan2(xVec.y(), xVec.x()); // rotation about Z
    
    WFMath::RotMatrix<2> rm;
    poly.rotatePoint(rm.rotation(theta), WFMath::Point<2>(0,0));
    poly.shift(WFMath::Vector<2>(mEntity->getPosition().x(), mEntity->getPosition().y()));
    
	//poly.shift(WFMath::Vector<2>(-10,-10));
	
/*	for (int i = 0; i < poly.numCorners();++i) {
		WFMath::Point<2> wpt = poly.getCorner(i);
		int j = 0;
	}*/
   
// 	if (poly.numCorners()) {
    	mArea->setShape(poly);
// 	}
	
	return true;
}

void TerrainArea::attributeChanged(const Atlas::Message::Element& attributeValue)
{
	if (parseArea()) {
		EventAreaChanged(this);
	}
}

void TerrainArea::entity_Moved()
{
	if (parseArea()) {
		EventAreaChanged(this);
	}
}

void TerrainArea::observeEntity()
{
	mAttrChangedSlot.disconnect();
	if (mEntity) {
		mAttrChangedSlot = sigc::mem_fun(*this, &TerrainArea::attributeChanged);
		mEntity->observe("area", mAttrChangedSlot);
		mEntity->Moved.connect(sigc::mem_fun(*this, &TerrainArea::entity_Moved));
	}
}

}
}
