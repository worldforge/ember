//
// C++ Implementation: TerrainMod
//
// Description: 
//
//
// Author: Tamas Bates <rhymre@gmail.com>, (C) 2008
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

#include "TerrainMod.h"

#include "../EmberEntity.h"
#include <Mercator/TerrainMod.h>

namespace EmberOgre {
namespace Terrain {

TerrainMod::TerrainMod(EmberEntity* entity) : mModifier(0), mEntity(entity)
{
}


TerrainMod::~TerrainMod()
{
}

bool TerrainMod::init() {
    observeEntity();
    return parseMod();
}

bool TerrainMod::parseMod()
{
    if (!mEntity->hasAttr("terrainmod")) {
        S_LOG_FAILURE("TerrainMod defined on entity with no terrainmod attribute");
        return false;
    }

    const Atlas::Message::Element& modifier(mEntity->valueOfAttr("terrainmod"));

    if (!modifier.isMap()) {
        S_LOG_FAILURE( "Terrain modifier is not a map" );
         return NULL;
    }
    const Atlas::Message::MapType & modMap = modifier.asMap();

    std::string modType;
    std::string shapeType;
    int shapeDim;
    WFMath::Point<3> pos;
    Atlas::Message::MapType shapeMap;
    
    Atlas::Message::MapType::const_iterator mod_I;

    // Get modifier type
    mod_I = modMap.find("type");
    if (mod_I != modMap.end()) {
    const Atlas::Message::Element& modTypeElem(mod_I->second);
    if (modTypeElem.isString()) {
        modType = modTypeElem.asString();
        }
    }

    // Get modifier position
    pos = mEntity->getPosition();

    // Get modifier's shape
    mod_I = modMap.find("shape");
    if (mod_I != modMap.end()) {
    const Atlas::Message::Element& shapeElem(mod_I->second);
    if (shapeElem.isMap()) {
        shapeMap = shapeElem.asMap();
        Atlas::Message::MapType::const_iterator shape_I;
            // Get shape's type
        shape_I = shapeMap.find("type");
        if (shape_I != shapeMap.end()) {
            const Atlas::Message::Element& shapeTypeElem(shape_I->second);
            if (shapeTypeElem.isString()) {
                shapeType = shapeTypeElem.asString();
            }
        }
            // Get shape's dimension
        shape_I = shapeMap.find("dim");
        if (shape_I != shapeMap.end()) {
            const Atlas::Message::Element& shapeDimElem(shape_I->second);
            if (shapeDimElem.isInt()) {
                shapeDim = (int)shapeDimElem.asNum();
            }
        }
    } // end shape data

    // Check for additional modifier parameters
    if (modType == "slopemod") {
        float dx, dy, level;
        // Get slopes
        mod_I = modMap.find("slopes");
        if (mod_I != modMap.end()) {
                const Atlas::Message::Element& modSlopeElem = mod_I->second;
                if (modSlopeElem.isList()) {
                const Atlas::Message::ListType & slopes = modSlopeElem.asList();
                dx = (int)slopes[0].asNum();
                dy = (int)slopes[1].asNum();
                }
            }
        // Get level
        mod_I = modMap.find("height");
        if (mod_I != modMap.end()) {
            const Atlas::Message::Element& modHeightElem = mod_I->second;
            level = modHeightElem.asNum();
        }

        if (shapeType == "ball") {
            float shapeRadius;
            // Get sphere's radius
            Atlas::Message::MapType::const_iterator shape_I = shapeMap.find("radius");
            if (shape_I != shapeMap.end()) {
                const Atlas::Message::Element& shapeRadiusElem(shape_I->second);
                shapeRadius = shapeRadiusElem.asNum();
            }
            
            // Make disc
            WFMath::Point<2> pos_2d(pos.x(),pos.y());
            WFMath::Ball<2> modShape = WFMath::Ball<2>(pos_2d, shapeRadius);

            S_LOG_INFO("Successfully parsed a slopemod");

            // Make modifier

            return true;
        }
    
    } else if (modType == "levelmod") {
        float level;
        // Get level
        mod_I = modMap.find("height");
        if (mod_I != modMap.end()) {
            const Atlas::Message::Element& modHeightElem = mod_I->second;
            level = modHeightElem.asNum();
        }

        if (shapeType == "ball") {
            float shapeRadius;
            // Get sphere's radius
            Atlas::Message::MapType::const_iterator shape_I = shapeMap.find("radius");
            if (shape_I != shapeMap.end()) {
                const Atlas::Message::Element& shapeRadiusElem(shape_I->second);
                shapeRadius = shapeRadiusElem.asNum();
            }

            // Make disc
            int sx = pos.x() / 64;
            int sy = pos.y() / 64;
            if (pos.x() < 0) {
                sx -= 1;
            }
            if (pos.y() < 0) {
                sy -= 1;
            }
            int my = abs(pos.y() - (sy * 64));
            int mx = abs(pos.x() - (sx * 64));

            WFMath::Point<2> pos_2d(pos.x(),pos.y());
            WFMath::Ball<2> modShape = WFMath::Ball<2>(pos_2d, shapeRadius); ///FIXME: assumes 2d ball...
            
            S_LOG_INFO("Successfully parsed a levelmod");

            // Make Modifier
            Mercator::LevelTerrainMod<WFMath::Ball<2> > *NewMod;
            mModifier = new Mercator::LevelTerrainMod<WFMath::Ball<2> >(level, modShape);

            return true;
//             return mModifier;

        } else if (shapeType == "rotbox") {
            WFMath::Point<2> shapePoint;
            WFMath::Vector<2> shapeVector;
            // Get rotbox's position
            Atlas::Message::MapType::const_iterator shape_I = shapeMap.find("point");
            if (shape_I != shapeMap.end()) {
                const Atlas::Message::Element& shapePointElem(shape_I->second);
                if (shapePointElem.isList()) {
                    const Atlas::Message::ListType & pointList = shapePointElem.asList();
                    shapePoint = WFMath::Point<2>((int)pointList[0].asNum(), (int)pointList[1].asNum());
                }
            }
            // Get rotbox's vector
            shape_I = shapeMap.find("vector");
            if (shape_I != shapeMap.end()) {
                const Atlas::Message::Element& shapeVectorElem(shape_I->second);
                if (shapeVectorElem.isList()) {
                    const Atlas::Message::ListType & vectorList = shapeVectorElem.asList(); 
                    shapeVector = WFMath::Vector<2>((int)vectorList[0].asNum(), (int)vectorList[1].asNum());
                }
            }

            // Make rotbox
            int sx = pos.x() / 64;
            int sy = pos.y() / 64;
            int my = pos.y() - (sy * 64);
            int mx = pos.x() - (sx * 64);               // Was shapePoint
            WFMath::RotBox<2> modShape = WFMath::RotBox<2>(WFMath::Point<2>(mx,my), shapeVector, WFMath::RotMatrix<2>()); ///FIXME: needs to use shapeDim instead of 2

            // Make modifier
            Mercator::LevelTerrainMod<WFMath::RotBox<2> > *NewMod;
            mModifier = new Mercator::LevelTerrainMod<WFMath::RotBox<2> >(level, modShape);

            return true;
//             return mModifier;
        }       


    } else if (modType == "adjustmod") {
        float level;
        // Get level
        mod_I = modMap.find("height");
        if (mod_I != modMap.end()) {
            const Atlas::Message::Element& modHeightElem = mod_I->second;
            level = modHeightElem.asNum();
        }

        if (shapeType == "ball") {
            float shapeRadius;
            // Get sphere's radius
            Atlas::Message::MapType::const_iterator shape_I = shapeMap.find("radius");
            if (shape_I != shapeMap.end()) {
                const Atlas::Message::Element& shapeRadiusElem(shape_I->second);
                shapeRadius = shapeRadiusElem.asNum();
            }
        
            // Make sphere
            WFMath::Point<2> pos_2d(pos.x(), pos.y());
            WFMath::Ball<2> modShape = WFMath::Ball<2>(pos_2d, shapeRadius);
        
            // Make modifier

            return true;
        }

    } else if (modType == "cratermod") {
            
        // Get other shape parameters
        if (shapeType == "ball" ) {
            float shapeRadius;
            // Get sphere's radius
            Atlas::Message::MapType::const_iterator shape_I = shapeMap.find("radius");
            if (shape_I != shapeMap.end()) {
                const Atlas::Message::Element& shapeRadiusElem(shape_I->second);
                shapeRadius = shapeRadiusElem.asNum();
            }

            // Make sphere
            int sx = pos.x() / 64;
            int sy = pos.y() / 64;
            if (pos.x() < 0) {
                sx -= 1;
            }
            if (pos.y() < 0) {
                sy -= 1;
            }
            int my = abs(pos.y() - (sy * 64));
            int mx = abs(pos.x() - (sx * 64));
    
            WFMath::Ball<3> modShape = WFMath::Ball<3>(WFMath::Point<3>(pos.x(),pos.y(),pos.z()), shapeRadius); ///FIXME: assumes 3d ball...

            

            S_LOG_INFO("Successfully parsed a cratermod");
            // Make modifier
            Mercator::CraterTerrainMod *NewMod;
            mModifier = new Mercator::CraterTerrainMod(modShape);

             return true;
//             return mModifier;
        }
    }

    }
}

void TerrainMod::attributeChanged(const Atlas::Message::Element& attributeValue)
{
    delete mModifier;
    if (parseMod()) {
        EventModChanged(this);
    }
}

void TerrainMod::entity_Moved()
{
    delete mModifier;
    if (parseMod()) {
        EventModChanged(this);
    }
}

void TerrainMod::entity_Deleted()
{
    delete mModifier;
    EventModDeleted(this);
}

void TerrainMod::observeEntity()
{
    mAttrChangedSlot.disconnect();
    if (mEntity) {
        mAttrChangedSlot = sigc::mem_fun(*this, &TerrainMod::attributeChanged);
        mEntity->observe("terrainmod", mAttrChangedSlot);
        mEntity->Moved.connect(sigc::mem_fun(*this, &TerrainMod::entity_Moved));
        mEntity->BeingDeleted.connect(sigc::mem_fun(*this, &TerrainMod::entity_Deleted));
    }
}

}
}
