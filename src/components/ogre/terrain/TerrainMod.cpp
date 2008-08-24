//
// C++ Implementation: TerrainMod
//
// Description:
//
//
// Author: Tamas Bates <rhymre@gmail.com>, (C) 2008
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
#include "TerrainMod_impl.h"

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

	const Atlas::Message::MapType* shapeMap(0);
	const std::string& shapeType = parseShape(modElement, &shapeMap);
	if (shapeMap) {
		if (shapeType == "ball") {
			WFMath::Point<3> pos = mTerrainMod.getEntity()->getPosition();
			///HACK: This height adjustment shouldn't be necessary
			pos.z() = EmberOgre::getSingleton().getTerrainGenerator()->getHeight(TerrainPosition(pos.x(), pos.y()));
			WFMath::Ball<3>* shape(0);
			if (InnerTerrainMod_impl::parseShapeAtlasData<WFMath::Ball<3> >(*shapeMap, pos, &shape)) {
				mModifier = new Mercator::CraterTerrainMod(*shape);
				delete shape;
				return true;
			}
			delete shape;
		}
	}
	S_LOG_FAILURE("Crater terrain mod defined with incorrect shape");
	return false;
}


InnerTerrainModSlope::InnerTerrainModSlope(TerrainMod& terrainMod)
: InnerTerrainMod(terrainMod, "slopemod")
, mModifier_impl(0)
{
}

InnerTerrainModSlope::~InnerTerrainModSlope()
{
	delete mModifier_impl;
}

Mercator::TerrainMod* InnerTerrainModSlope::getModifier()
{
	return mModifier_impl->getModifier();
}


bool InnerTerrainModSlope::parseAtlasData(const Atlas::Message::MapType& modElement)
{
	WFMath::Point<3> pos = mTerrainMod.getEntity()->getPosition();
	float dx, dy, level;
	// Get slopes
	Atlas::Message::MapType::const_iterator mod_I = modElement.find("slopes");
	if (mod_I != modElement.end()) {
		const Atlas::Message::Element& modSlopeElem = mod_I->second;
		if (modSlopeElem.isList()) {
			const Atlas::Message::ListType & slopes = modSlopeElem.asList();
			if (slopes.size() > 1) {
				if (slopes[0].isNum() && slopes[1].isNum()) {
					dx = slopes[0].asNum();
					dy = slopes[1].asNum();
					// Get level
					mod_I = modElement.find("height");
					if (mod_I != modElement.end()) {
						const Atlas::Message::Element& modHeightElem = mod_I->second;
						if (modHeightElem.isNum()) {
							level = modHeightElem.asNum();
							pos.z() = level;        // Note that the height of the mod is in pos.z()
							const Atlas::Message::MapType* shapeMap(0);
							const std::string& shapeType = parseShape(modElement, &shapeMap);
							if (shapeMap) {
								if (shapeType == "ball") {
									InnerTerrainModSlope_impl<WFMath::Ball<2> >* modifierImpl = new InnerTerrainModSlope_impl<WFMath::Ball<2> >();
									mModifier_impl = modifierImpl;
									return modifierImpl->createInstance(*shapeMap, pos, level, dx, dy);
								} else if (shapeType == "rotbox") {
									InnerTerrainModSlope_impl<WFMath::RotBox<2> >* modifierImpl = new InnerTerrainModSlope_impl<WFMath::RotBox<2> >();
									mModifier_impl = modifierImpl;
									return modifierImpl->createInstance(*shapeMap, pos, level, dx, dy);
								} else if (shapeType == "polygon") {
									InnerTerrainModSlope_impl<WFMath::Polygon<2> >* modifierImpl = new InnerTerrainModSlope_impl<WFMath::Polygon<2> >();
									mModifier_impl = modifierImpl;
									return modifierImpl->createInstance(*shapeMap, pos, level, dx, dy);
								}
							}
						}
					}
				}
			}
		}
	}
	S_LOG_FAILURE("SlopeTerrainMod defined with incorrect shape");
	return false;
}


InnerTerrainModLevel::InnerTerrainModLevel(TerrainMod& terrainMod)
: InnerTerrainMod(terrainMod, "levelmod")
, mModifier_impl(0)
{
}

InnerTerrainModLevel::~InnerTerrainModLevel()
{
	delete mModifier_impl;
}

Mercator::TerrainMod* InnerTerrainModLevel::getModifier()
{
	return mModifier_impl->getModifier();
}


bool InnerTerrainModLevel::parseAtlasData(const Atlas::Message::MapType& modElement)
{

	WFMath::Point<3> pos = mTerrainMod.getEntity()->getPosition();
	// Get level
	Atlas::Message::MapType::const_iterator mod_I = modElement.find("height");
	if (mod_I != modElement.end()) {
		const Atlas::Message::Element& modHeightElem = mod_I->second;
		if (modHeightElem.isNum()) {
			float height = modHeightElem.asNum();
			pos.z() = height;        // Note that the height of the mod is in pos.z()
			const Atlas::Message::MapType* shapeMap(0);
			const std::string& shapeType = parseShape(modElement, &shapeMap);
			if (shapeMap) {
				if (shapeType == "ball") {
					InnerTerrainModLevel_impl<WFMath::Ball<2> >* modifierImpl = new InnerTerrainModLevel_impl<WFMath::Ball<2> >();
					mModifier_impl = modifierImpl;
					return modifierImpl->createInstance(*shapeMap, pos, height);
				} else if (shapeType == "rotbox") {
					InnerTerrainModLevel_impl<WFMath::RotBox<2> >* modifierImpl = new InnerTerrainModLevel_impl<WFMath::RotBox<2> >();
					mModifier_impl = modifierImpl;
					return modifierImpl->createInstance(*shapeMap, pos, height);
				} else if (shapeType == "polygon") {
					InnerTerrainModLevel_impl<WFMath::Polygon<2> >* modifierImpl = new InnerTerrainModLevel_impl<WFMath::Polygon<2> >();
					mModifier_impl = modifierImpl;
					return modifierImpl->createInstance(*shapeMap, pos, height);
				}
			}
		}
	}
	S_LOG_FAILURE("Level terrain mod defined with incorrect shape");
	return false;
}

InnerTerrainModAdjust::InnerTerrainModAdjust(TerrainMod& terrainMod)
: InnerTerrainMod(terrainMod, "adjustmod")
, mModifier_impl(0)
{
}

InnerTerrainModAdjust::~InnerTerrainModAdjust()
{
	delete mModifier_impl;
}

Mercator::TerrainMod* InnerTerrainModAdjust::getModifier()
{
	return mModifier_impl->getModifier();
}


bool InnerTerrainModAdjust::parseAtlasData(const Atlas::Message::MapType& modElement)
{

	WFMath::Point<3> pos = mTerrainMod.getEntity()->getPosition();
	// Get level
	Atlas::Message::MapType::const_iterator mod_I = modElement.find("height");
	if (mod_I != modElement.end()) {
		const Atlas::Message::Element& modHeightElem = mod_I->second;
		if (modHeightElem.isNum()) {
			float height = modHeightElem.asNum();
			pos.z() = height;        // Note that the height of the mod is in pos.z()
			const Atlas::Message::MapType* shapeMap(0);
			const std::string& shapeType = parseShape(modElement, &shapeMap);
			if (shapeMap) {
				if (shapeType == "ball") {
					InnerTerrainModAdjust_impl<WFMath::Ball<2> >* modifierImpl = new InnerTerrainModAdjust_impl<WFMath::Ball<2> >();
					mModifier_impl = modifierImpl;
					return modifierImpl->createInstance(*shapeMap, pos, height);
				} else if (shapeType == "rotbox") {
					InnerTerrainModAdjust_impl<WFMath::RotBox<2> >* modifierImpl = new InnerTerrainModAdjust_impl<WFMath::RotBox<2> >();
					mModifier_impl = modifierImpl;
					return modifierImpl->createInstance(*shapeMap, pos, height);
				} else if (shapeType == "polygon") {
					InnerTerrainModAdjust_impl<WFMath::Polygon<2> >* modifierImpl = new InnerTerrainModAdjust_impl<WFMath::Polygon<2> >();
					mModifier_impl = modifierImpl;
					return modifierImpl->createInstance(*shapeMap, pos, height);
				}
			}
		}
	}
	S_LOG_FAILURE("Adjust terrain mod defined with incorrect shape");
	return false;
}


const std::string& InnerTerrainMod::parseShape(const Atlas::Message::MapType& modElement, const Atlas::Message::MapType** shapeMap)
{
	Atlas::Message::MapType::const_iterator shape_I = modElement.find("shape");
	if (shape_I != modElement.end()) {
		const Atlas::Message::Element& shapeElement = shape_I->second;
		if (shapeElement.isMap()) {
			const Atlas::Message::MapType& localShapeMap = shapeElement.asMap();
			*shapeMap = &localShapeMap;
			
			// Get shape's type
			Atlas::Message::MapType::const_iterator type_I = localShapeMap.find("type");
			if (type_I != localShapeMap.end()) {
				const Atlas::Message::Element& shapeTypeElem(type_I->second);
				if (shapeTypeElem.isString()) {
					const std::string& shapeType = shapeTypeElem.asString();
					return shapeType;
				}
			}
		}
	}
	static std::string empty("");
	return empty;
}

// template <typename InnerTerrainMod_implType>
// InnerTerrainMod_implType* InnerTerrainMod::createInnerTerrainMod_impInstance(const Atlas::Message::MapType& modElement)
// {
// 
// 	const Atlas::Message::MapType* shapeMap(0);
// 	const std::string& shapeType = parseShape(modElement, &shapeMap);
// 	if (shapeMap) {
// 		if (shapeType == "ball") {
// 			typename InnerTerrainMod_implType::template foo<WFMath::Ball<2> >();
//  			return new typename InnerTerrainMod_implType::template<WFMath::Ball<2> >();
// 		}
// 	}
// }
	


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

			if (modType == "slopemod") {
				mInnerMod = new InnerTerrainModSlope(*this);
			} else if (modType == "levelmod") {
				mInnerMod = new InnerTerrainModLevel(*this);
			} else if (modType == "adjustmod") {
				mInnerMod = new InnerTerrainModAdjust(*this);
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

EmberEntity* TerrainMod::getEntity() const
{
	return mEntity;
}

} // close Namespace Terrain
} // close Namespace EmberOgre
