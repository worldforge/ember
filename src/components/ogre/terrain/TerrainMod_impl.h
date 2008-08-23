//
// C++ Interface: TerrainMod_impl
//
// Description: 
//
//
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
#ifndef EMBEROGRE_TERRAINTERRAINMOD_IMPL_H
#define EMBEROGRE_TERRAINTERRAINMOD_IMPL_H

#include <Eris/Entity.h>
#include "../EmberEntity.h"
#include "TerrainGenerator.h"
#include "../EmberOgre.h"
#include <Mercator/TerrainMod.h>
#include <Mercator/TerrainMod_impl.h>
#include <wfmath/ball.h>


namespace EmberOgre {

namespace Terrain {

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class InnerTerrainMod_impl
{
public:
	InnerTerrainMod_impl() {}
	
	virtual ~InnerTerrainMod_impl() {}

	template <typename Shape>
	static bool parseShapeAtlasData(const Atlas::Message::MapType& shapeElement, WFMath::Point<3> pos, Shape** shape);
	
	virtual Mercator::TerrainMod* getModifier() = 0;
	
protected:

};


/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
template <typename Shape>
class InnerTerrainModSlope_impl : public InnerTerrainMod_impl
{
public:
	InnerTerrainModSlope_impl() {}
	
	virtual ~InnerTerrainModSlope_impl() {}
	
	bool createInstance(const Atlas::Message::MapType& shapeElement, WFMath::Point<3> pos, float level, float dx, float dy);
	
	inline virtual Mercator::TerrainMod* getModifier();

protected:
	Mercator::SlopeTerrainMod<Shape>* mTerrainMod;
	
// 	bool internalParseAtlasData(const Atlas::Message::MapType& shapeElement, WFMath::Point<3> pos);

};

template <typename Shape>
Mercator::TerrainMod* InnerTerrainModSlope_impl<Shape>::getModifier()
{
	return mTerrainMod;
}


template<>
bool InnerTerrainMod_impl::parseShapeAtlasData<WFMath::Ball<2> >(const Atlas::Message::MapType& shapeElement, WFMath::Point<3> pos, WFMath::Ball<2>** shape)
{
	// Get sphere's radius
	Atlas::Message::MapType::const_iterator shape_I = shapeElement.find("radius");
	if (shape_I != shapeElement.end()) {
		const Atlas::Message::Element& shapeRadiusElem(shape_I->second);
		if (shapeRadiusElem.isNum()) {
			float shapeRadius = shapeRadiusElem.asNum();
			// Make disc
			WFMath::Point<2> pos_2d(pos.x(), pos.y());
			*shape = new WFMath::Ball<2>(pos_2d, shapeRadius);
			return true;
		}
	}
	return false;
}

template<>
bool InnerTerrainMod_impl::parseShapeAtlasData<WFMath::Ball<3> >(const Atlas::Message::MapType& shapeElement, WFMath::Point<3> pos, WFMath::Ball<3>** shape)
{
	/// Get sphere's radius
	Atlas::Message::MapType::const_iterator shape_I = shapeElement.find("radius");
	if (shape_I != shapeElement.end()) {
		const Atlas::Message::Element& shapeRadiusElem(shape_I->second);
		if (shapeRadiusElem.isNum()) {
			float shapeRadius = shapeRadiusElem.asNum();
			/// Make ball
			*shape = new WFMath::Ball<3>(pos, shapeRadius);
			return true;
		}
	}
	return false;
}


template <typename Shape>
bool InnerTerrainModSlope_impl<Shape>::createInstance(const Atlas::Message::MapType& shapeElement, WFMath::Point<3> pos, float level, float dx, float dy)
{
	Shape* shape(0);
	if (parseShapeAtlasData<Shape>(shapeElement, pos, &shape)) {
		mTerrainMod = new Mercator::SlopeTerrainMod<Shape>(level, dx, dy, *shape);
		delete shape;
		return true;
	}
	delete shape;
	return false;
}

// template<>
// bool InnerTerrainModSlope_impl<WFMath::Ball<2> >::internalParseAtlasData(const Atlas::Message::MapType& shapeElement, WFMath::Point<3> pos)
// {
// 	WFMath::Ball<2>* ball(0);
// 	if (parseShapeAtlasData<WFMath::Ball<2> >(shapeElement, pos, &ball)) {
// 		mTerrainMod = new Mercator::SlopeTerrainMod<WFMath::Ball<2> >(0, 0, 0, *ball);
// 		return true;
// 	}
// 	return false;
// }


}

}

#endif
