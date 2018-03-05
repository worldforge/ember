//
// C++ Interface: TerrainArea
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRETERRAINAREA_H
#define EMBEROGRETERRAINAREA_H

#include <sigc++/signal.h>
#include <Eris/Entity.h>
#include <wfmath/polygon.h>

namespace Mercator
{
    class Area;
}

namespace Ember {
class EmberEntity;
namespace OgreView {
namespace Terrain {


/**
@brief Represents a terrain area, as defined by Mercator::Area.
A terrain area is a section of the terrain, as defined by a 2d polygon. It's connected to a specified layer in the terrain, and allows you to define stuff like roads, fields or any other feature you want added to the terrain.

The areas are always attached to an entity, and when the entity moves the area moves with it. The position of the area is defined in space coordinates relative to the entity's position.
This class acts like a wrapper and controller, binding an instance of EmberEntity and Mercator::Area together. The actual application of the area to the terrain is handled by TerrainHandler, but the logic for parsing the terrain area data from Atlas is contained in this class.
@author Erik Ogenvik
*/
class TerrainArea : public virtual sigc::trackable
{
public:

	/**
	* @brief Ctor.
	* @param entity The entity to which this area is connected. This cannot be null, since the definition of the area is kept within the Entity. When the entity moves, the EventEntityMoved signal will be emitted, requiring a container class to call updatePosition().
	*/
	explicit TerrainArea(EmberEntity& entity);

	/**
	* @brief Dtor, will delete the allocated Mercator::Area.
	*/
	virtual ~TerrainArea() = default;

	/**
	 * @brief Parses the data into an area, and tries to place it using the entity's position.
	 * @param area If the area and position are valid, this will point to a new Area instance. Ownership is transferred to the caller.
	 */
	void parse(const Atlas::Message::Element& value, Mercator::Area** area);

	/**
	 * @brief Updates the position of the area, using the position of the entity.
	 * @param area If the area and position are valid, this will point to a new Area instance. Ownership is transferred to the caller.
	 */
	void updatePosition(Mercator::Area** area);

protected:

	/**
	@brief The entity to which this area is connected to.
	*/
	EmberEntity& mEntity;

	/**
	 * @brief The last parsed polygon data.
	 */
	WFMath::Polygon<2> mParsedPoly;

	/**
	 * @brief The last parsed layer.
	 */
	int mParsedLayer;

	/**
	 * @brief Places the area.
	 * If no Mercator::Area instance has been created, one will be created here. If however there's already an existing area it will be updated.
	 * @return
	 */
	bool placeArea(WFMath::Polygon<2>& poly);

};


}
}
}

#endif
