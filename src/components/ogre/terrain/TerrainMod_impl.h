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
	@brief Base class for the inner terrain mod implemtations.
	We keep this separate from the TerrainMod classes because of the way these classes interacts with templated WFMath structures.
	The parseShapeAtlasData() method can also be used externally as it's a static public.
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class InnerTerrainMod_impl
{
public:

	/**
	 *    @brief Ctor.
	 */
	InnerTerrainMod_impl() {}
	
	/**
	 *    @brief Dtor.
	 */
	virtual ~InnerTerrainMod_impl() {}

	/**
	 * @brief Common method for parsing shape data from Atlas.
	 * Since each different shape expects different Atlas data this is a templated method with specialized implemtations for each available shape. If you call this and get error regarding missing implementations it probably means that there's no implementation for the type of shape you're calling it with.
	 * Note that a new shape instance will be put on the heap if the parsing is successful, and it's up to the calling code to properly delete it when done.
	 * @param shapeElement The atlas map element which contains the shape data. Often this is found with the key "shape" in the atlas data.
	 * @param pos The original position of the entity to which this shape will belong. The shape will be positioned according to this.
	 * @param shape The resulting shape is meant to be put here, if successfully created. That means that a new shape instance will be created, and it's then up to the calling method to properly delete it, to avoid memory leaks.
	 * @return True if the atlas data was successfully parsed and a shape was created.
	 */
	template <typename Shape>
	static bool parseShapeAtlasData(const Atlas::Message::MapType& shapeElement, WFMath::Point<3> pos, Shape** shape);
	
	/**
	 * @brief Gets the modifier which this instance represents.
	 * @return A pointer to a terrain modifier, or null if none could be created.
	 */
	virtual Mercator::TerrainMod* getModifier() = 0;
	
protected:

};

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

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
	@brief Handles instances of Mercator::SlopeTerrainMod with arbitrary shapes.
*/
template <typename Shape>
class InnerTerrainModSlope_impl : public InnerTerrainMod_impl
{
public:
	/**
	 * @brief Ctor.
	 */
	InnerTerrainModSlope_impl() {}
	
	/**
	 * @brief Dtor.
	 */
	virtual ~InnerTerrainModSlope_impl()
	{
		delete mTerrainMod;
	}
	
	/**
	 * @brief Tries to create a new instance from the passes in atlas data.
	 * @param shapeElement The atlas data containing shape information.
	 * @param pos The position where the mod should be created, in world space.
	 * @param level The level where the slope should be created.
	 * @param dx
	 * @param dy
	 * @return True if the atlas data could be successfully parsed an a mod created.
	 */
	bool createInstance(const Atlas::Message::MapType& shapeElement, WFMath::Point<3> pos, float level, float dx, float dy);
	
	/**
	 * @copydoc InnerTerrainMod_impl::getModifier()
	 */
	inline virtual Mercator::TerrainMod* getModifier();

protected:

	/**
	 * @brief The actual terrain mod instance, owned by this instance.
	 */
	Mercator::SlopeTerrainMod<Shape>* mTerrainMod;
};

template <typename Shape>
Mercator::TerrainMod* InnerTerrainModSlope_impl<Shape>::getModifier()
{
	return mTerrainMod;
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

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
	@brief Handles instances of Mercator::LevelTerrainMod with arbitrary shapes.
*/
template <typename Shape>
class InnerTerrainModLevel_impl : public InnerTerrainMod_impl
{
public:
	/**
	 * @brief Ctor.
	 */
	InnerTerrainModLevel_impl() {}
	
	/**
	 * @brief Dtor.
	 */
	virtual ~InnerTerrainModLevel_impl()
	{
		delete mTerrainMod;
	}
	
	/**
	 * @brief Tries to create a new instance from the passes in atlas data.
	 * @param shapeElement The atlas data containing shape information.
	 * @param pos The position where the mod should be created, in world space.
	 * @param height The height where the level should be created.
	 * @return True if the atlas data could be successfully parsed an a mod created.
	 */
	bool createInstance(const Atlas::Message::MapType& shapeElement, WFMath::Point<3> pos, float height);
	
	/**
	 * @copydoc InnerTerrainMod_impl::getModifier()
	 */
	inline virtual Mercator::TerrainMod* getModifier();

protected:

	/**
	 * @brief The actual terrain mod instance, owned by this instance.
	 */
	Mercator::LevelTerrainMod<Shape>* mTerrainMod;
};

template <typename Shape>
Mercator::TerrainMod* InnerTerrainModLevel_impl<Shape>::getModifier()
{
	return mTerrainMod;
}

template <typename Shape>
bool InnerTerrainModLevel_impl<Shape>::createInstance(const Atlas::Message::MapType& shapeElement, WFMath::Point<3> pos, float height)
{
	Shape* shape(0);
	if (parseShapeAtlasData<Shape>(shapeElement, pos, &shape)) {
		mTerrainMod = new Mercator::LevelTerrainMod<Shape>(height, *shape);
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
