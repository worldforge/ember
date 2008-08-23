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
#include "TerrainGenerator.h"
#include "../EmberOgre.h"
#include <Mercator/TerrainMod.h>

namespace EmberOgre {
namespace Terrain {

InnerTerrainMod::InnerTerrainMod(TerrainMod& terrainMod, const std::string& typemod)
: mTerrainMod(terrainMod)
{
}

InnerTerrainMod::~InnerTerrainMod()
{
}

const std::string& InnerTerrainMod::getTypename() const
{
	return mTypeName;
}

InnerTerrainModCrater::InnerTerrainModCrater(TerrainMod& terrainMod)
: InnerTerrainMod(terrainMod, "cratermod")
, mModifier(0)
{
}

InnerTerrainModCrater::~InnerTerrainModCrater()
{
	delete mModifier;
}

Mercator::TerrainMod* InnerTerrainModCrater::getModifier()
{
	return mModifier;
}


bool InnerTerrainModCrater::parseAtlasData(const Atlas::Message::MapType& modElement)
{
	Atlas::Message::MapType::const_iterator shape_I = modElement.find("shape");
	if (shape_I != modElement.end()) {
		const Atlas::Message::Element& shapeElement = shape_I->second;
		if (shapeElement.isMap()) {
			const Atlas::Message::MapType& shapeMap = shapeElement.asMap();
			std::string shapeType;
			
			// Get shape's type
			Atlas::Message::MapType::const_iterator type_I = shapeMap.find("type");
			if (type_I != shapeMap.end()) {
				const Atlas::Message::Element& shapeTypeElem(type_I->second);
				if (shapeTypeElem.isString()) {
					shapeType = shapeTypeElem.asString();
					///TODO: separate this into resuable code /ehj
					if (shapeType == "ball") {
						// Get sphere's radius
						Atlas::Message::MapType::const_iterator radius_I = shapeMap.find("radius");
						if (radius_I != shapeMap.end()) {
							const Atlas::Message::Element& shapeRadiusElem(radius_I->second);
							float shapeRadius = shapeRadiusElem.asNum();
							// Make sphere
							WFMath::Point<3> pos = mTerrainMod.getEntity()->getPosition();
							///HACK: This height adjustment shouldn't be necessary
							pos.z() = EmberOgre::getSingleton().getTerrainGenerator()->getHeight(TerrainPosition(pos.x(), pos.y()));
							
							WFMath::Ball<3> modShape(pos, shapeRadius); ///FIXME: assumes 3d ball...
						
							//TODO: make sure to delete mModifier and emit a signal for the TerrainGenerator to recieve and handle
							delete mModifier;
							// Make modifier
							mModifier = new Mercator::CraterTerrainMod(modShape);
							return true;
						}
					
					} else {
						//TODO: implement more shapes
						return false;
					}
				}
			}
		}
	}
	S_LOG_FAILURE("CraterTerrainMod defined with incorrect shape");
	return false;
}


InnerTerrainModSlope::InnerTerrainModSlope(TerrainMod& terrainMod)
: InnerTerrainMod(terrainMod, "slopemod")
, mModifier(0)
{
}

InnerTerrainModSlope::~InnerTerrainModSlope()
{
	delete mModifier;
}

Mercator::TerrainMod* InnerTerrainModSlope::getModifier()
{
	return mModifier;
}


bool InnerTerrainModSlope::parseAtlasData(const Atlas::Message::MapType& modElement)
{
	WFMath::Point<3> pos = mTerrainMod.getEntity()->getPosition();
	float dx, dy, level;
	// Get slopes
	mod_I = modElement.find("slopes");
	if (mod_I != modMap.end()) {
		const Atlas::Message::Element& modSlopeElem = mod_I->second;
		if (modSlopeElem.isList()) {
			const Atlas::Message::ListType & slopes = modSlopeElem.asList();
			dx = (int)slopes[0].asNum();
			dy = (int)slopes[1].asNum();
			// Get level
			mod_I = modMap.find("height");
			if (mod_I != modMap.end()) {
				///is this a required attribute or not? We'll treat it as an optional attribute for now. /ehj
				const Atlas::Message::Element& modHeightElem = mod_I->second;
				level = modHeightElem.asNum();
				pos.z() = level;        // Note that the height of the mod is in pos.z()
			}
			
			///TODO: add parsing of shapes here, but we need to provide better generalized functionality for parsing shapes that can be used by all subclasses.
			return true;
		
		
		}
	}
	S_LOG_FAILURE("SlopeTerrainMod defined with incorrect shape");
	return false;
}





TerrainMod::TerrainMod(EmberEntity* entity)
: mEntity(entity)
, mInnerMod(0)
{
}


TerrainMod::~TerrainMod()
{
}

bool TerrainMod::init()
{
	bool successfulParsing = parseMod();
	if (successfulParsing) {
		observeEntity();
	}
	return successfulParsing;
}

bool TerrainMod::parseMod()
{
	if (!mEntity->hasAttr("terrainmod")) {
		S_LOG_FAILURE("TerrainMod defined on entity with no terrainmod attribute");
		return false;
	}

	const Atlas::Message::Element& modifier(mEntity->valueOfAttr("terrainmod"));

	if (!modifier.isMap()) {
		S_LOG_FAILURE("Terrain modifier is not a map");
		return false;
	}
	const Atlas::Message::MapType & modMap = modifier.asMap();

	
	// Get modifier type
	Atlas::Message::MapType::const_iterator mod_I = modMap.find("type");
	if (mod_I != modMap.end()) {
		const Atlas::Message::Element& modTypeElem(mod_I->second);
		if (modTypeElem.isString()) {
			const std::string& modType = modTypeElem.asString();
			// Get modifier position
			
			// Build modifier from data
			if (modType == "slopemod") {
				mInnerMod = new InnerTerrainModSlope(*this);
			} else if (modType == "levelmod") {
/*				float level;
				// Get level
				mod_I = modMap.find("height");
				if (mod_I != modMap.end()) {
					const Atlas::Message::Element& modHeightElem = mod_I->second;
					level = modHeightElem.asNum();
				}
			
				pos.z() = level;        // Note that the level the terrain will be raised to is in pos.z()
				if ((mModifier = newLevelMod(shapeMap, pos)) != NULL) {
					return true;
				} else {
					return false;
				}*/
			
			} else if (modType == "adjustmod") {
/*				float level;
				// Get level
				mod_I = modMap.find("height");
				if (mod_I != modMap.end()) {
					const Atlas::Message::Element& modHeightElem = mod_I->second;
					level = modHeightElem.asNum();
				}
			
				pos.z() = level;        // Note that the level used in the adjustment is in pos.z()
				if ((mModifier = newAdjustMod(shapeMap, pos)) != NULL) {
					return true;
				} else {
					return false;
				}*/
			
			} else  if (modType == "cratermod") {
				mInnerMod = new InnerTerrainModCrater(*this);
			}
		}
	}
	if (mInnerMod) {
		if (mInnerMod->parseAtlasData(modMap)) {
			return true;
		} else {
			return false;
		}
	}


	return false;
}

void TerrainMod::attributeChanged(const Atlas::Message::Element& attributeValue)
{
	delete mInnerMod;
	mInnerMod = 0;
	if (parseMod()) {
		EventModChanged(this);
	}
}

void TerrainMod::entity_Moved()
{
	delete mInnerMod;
	mInnerMod = 0;
	if (parseMod()) {
		EventModChanged(this);
	}
}

void TerrainMod::entity_Deleted()
{
	EventModDeleted(this);
	delete mInnerMod;
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

Mercator::TerrainMod* TerrainMod::newCraterMod(const Atlas::Message::MapType shapeMap, WFMath::Point<3> pos)
{
	std::string shapeType;

	// Get modifier's shape
	Atlas::Message::MapType::const_iterator shape_I;
	// Get shape's type
	shape_I = shapeMap.find("type");
	if (shape_I != shapeMap.end()) {
		const Atlas::Message::Element& shapeTypeElem(shape_I->second);
		if (shapeTypeElem.isString()) {
			shapeType = shapeTypeElem.asString();
		}
	}
	// end shape data

	if (shapeType == "ball") {
		float shapeRadius;
		// Get sphere's radius
		Atlas::Message::MapType::const_iterator shape_I = shapeMap.find("radius");
		if (shape_I != shapeMap.end()) {
			const Atlas::Message::Element& shapeRadiusElem(shape_I->second);
			shapeRadius = shapeRadiusElem.asNum();
		}

		// Make sphere
        ///HACK: This height adjustment shouldn't be necessary
//         pos.z() = EmberOgre::getSingleton().getTerrainGenerator()->getTerrain().get(pos.x(), pos.y());
//         pos.z() += shapeRadius;

		WFMath::Ball<3> modShape = WFMath::Ball<3>(pos, shapeRadius); ///FIXME: assumes 3d ball...

		// Make modifier
		Mercator::CraterTerrainMod *NewMod;
		NewMod = new Mercator::CraterTerrainMod(modShape);

		return NewMod;
	}

	S_LOG_FAILURE("CraterTerrainMod defined with incorrect shape");
	return NULL;
}

Mercator::TerrainMod* TerrainMod::newLevelMod(const Atlas::Message::MapType shapeMap, WFMath::Point<3> pos)
{
	std::string shapeType;
	Atlas::Message::MapType::const_iterator shape_I;

	// Get shape's type
	shape_I = shapeMap.find("type");
	if (shape_I != shapeMap.end()) {
		const Atlas::Message::Element & shapeTypeElem(shape_I->second);
		if (shapeTypeElem.isString()) {
			shapeType = shapeTypeElem.asString();
		}
	}

	if (shapeType == "ball") {
		float shapeRadius;
		// Get sphere's radius
		shape_I = shapeMap.find("radius");
		if (shape_I != shapeMap.end()) {
			const Atlas::Message::Element& shapeRadiusElem(shape_I->second);
			shapeRadius = shapeRadiusElem.asNum();
		}

		// Make disc
		WFMath::Point<2> pos_2d(pos.x(), pos.y());
		WFMath::Ball<2> modShape = WFMath::Ball<2>(pos_2d, shapeRadius); ///FIXME: assumes 2d ball...

		// Make Modifier
		Mercator::LevelTerrainMod<WFMath::Ball<2> > *NewMod;
		NewMod = new Mercator::LevelTerrainMod<WFMath::Ball<2> >(pos.z(), modShape);

		return NewMod;
	}

	if (shapeType == "rotbox") {
		WFMath::Point<2> shapePoint;
		WFMath::Vector<2> shapeVector;
		// Get rotbox's position
		shape_I = shapeMap.find("point");
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
		///FIXME: needs to use shapeDim instead of 2
		WFMath::RotBox<2> modShape = WFMath::RotBox<2>(shapePoint, shapeVector, WFMath::RotMatrix<2>());

		// Make modifier
		Mercator::LevelTerrainMod<WFMath::RotBox<2> > *NewMod;
		NewMod = new Mercator::LevelTerrainMod<WFMath::RotBox<2> >(pos.z(), modShape);

		return NewMod;
	}

	S_LOG_FAILURE("LevelTerrainMod defined with incorrect shape data");
	return NULL;
}

Mercator::TerrainMod * TerrainMod::newSlopeMod(const Atlas::Message::MapType shapeMap,
		WFMath::Point<3> pos, float dx, float dy)
{
	std::string shapeType;

	// Get modifier's shape
	Atlas::Message::MapType::const_iterator shape_I;
	// Get shape's type
	shape_I = shapeMap.find("type");
	if (shape_I != shapeMap.end()) {
		const Atlas::Message::Element& shapeTypeElem(shape_I->second);
		if (shapeTypeElem.isString()) {
			shapeType = shapeTypeElem.asString();
		}
	}
	// end shape data

	if (shapeType == "ball") {
		float shapeRadius;
		// Get sphere's radius
		Atlas::Message::MapType::const_iterator shape_I = shapeMap.find("radius");
		if (shape_I != shapeMap.end()) {
			const Atlas::Message::Element& shapeRadiusElem(shape_I->second);
			shapeRadius = shapeRadiusElem.asNum();
		}

		// Make disc
		WFMath::Point<2> pos_2d(pos.x(), pos.y());
		WFMath::Ball<2> modShape = WFMath::Ball<2>(pos_2d, shapeRadius);

		// log(INFO, "Successfully parsed a slopemod");

		// Make modifier

	}

	S_LOG_FAILURE("SlopeTerrainMod defined with incorrect shape data");
	return NULL;
}

Mercator::TerrainMod * TerrainMod::newAdjustMod(const Atlas::Message::MapType shapeMap, WFMath::Point<3> pos)
{
	std::string shapeType;

	// Get modifier's shape
	Atlas::Message::MapType::const_iterator shape_I;
	// Get shape's type
	shape_I = shapeMap.find("type");
	if (shape_I != shapeMap.end()) {
		const Atlas::Message::Element& shapeTypeElem(shape_I->second);
		if (shapeTypeElem.isString()) {
			shapeType = shapeTypeElem.asString();
		}
	}
	// end shape data

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

		// Apply Modifier

	}

	S_LOG_FAILURE("AdjustTerrainMod defined with incorrect shape data");
	return NULL;
}

EmberEntity* TerrainMod::getEntity() const
{
	return mEntity;
}

} // close Namespace Terrain
} // close Namespace EmberOgre
