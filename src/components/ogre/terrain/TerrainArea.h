//
// C++ Interface: TerrainArea
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
#ifndef EMBEROGRETERRAINAREA_H
#define EMBEROGRETERRAINAREA_H

#include <sigc++/signal.h>
#include <Eris/Entity.h>

namespace Mercator
{
    class Area;
}

namespace EmberOgre {

class EmberEntity;

namespace Terrain {


/**
@brief Represents a terrain area, as defined by Mercator::Area.
A terrain area is a section of the terrain, as defined by a 2d polygon. It's connected to a specified layer in the terrain, and allows you to define stuff like roads, fields or any other feature you want added to the terrain.

The areas are always attached to an entity, and when the entity moves the area moves with it. The position of the area is defined in space coordinates relative to the entity's position.
This class acts like a wrapper and controller, binding an instance of EmberEntity and Mercator::Area together. The actual application of the area to the terrain is handled by TerrainGenerator, but the logic for parsing the terrain area data from Atlas is contained in this class.
@author Erik Hjortsberg
*/
class TerrainArea : public virtual sigc::trackable
{
public:

	/**
		* @brief Ctor.
		* You must make sure to call init() after you've created your TerrainArea instance, to properly set it up.
		* @param entity The entity to wich this area is connected. This cannot be null, since the definition of the area is kept within the Entity. When the entity moves, the area will be updated.
		*/
	TerrainArea(EmberEntity& entity);

	/**
		* @brief Dtor, will delete the allocated Mercator::Area.
		*/
	~TerrainArea();

	/**
	 * @brief Sets up the observation of the entity, and parses the area info, creating the initial area instance.
	 * @return True if the atlas data was conformant and successfully parsed.
	 */
	bool init();

	/**
	 * @brief Accessor for the Mercator::Area, which is the Mercator representation of the area. We shouldn't do very much ourselves with this, instead passing it on to Mercator::Terrain in most cases.
	 * @return The Mercator::Area instance for this terrain area, or null if none could be created.
	 */
	Mercator::Area* getArea() const;

	/**
	 * @brief Setter for the Mercator::Area instance for this terrain area.
	 * Normally you would never call this, instead depending on the parsing of the terrain area from the atlas data, through parseArea().
	 * @param area The new Mercator::Area instance.
	 */
	void setArea(Mercator::Area* area);

	/**
	@brief Emitted when something about the area changes, and we need to tell the terrain to regenerate the visualization of it.
	*/
	sigc::signal<void> EventAreaChanged;

	/**
	@brief Emitted when the area is about to be removed, and we need to tell the terrain to regenerate the visualization of it.
	*/
	sigc::signal<void> EventAreaRemoved;


	/**
	@brief Emitted when the underlying terrain area was swapped since it had it's layer changed. Currently the system can't handle dynamic changing of the layer, so we need to create a new area and drop the old one when the layer is changed.
	*/
	sigc::signal<void, Mercator::Area&> EventAreaSwapped;


protected:

	/**
	@brief The internal Mercator::Area instance which this class wraps.
	*/
	Mercator::Area* mArea;

	/**
	@brief An internal Mercator::Area instance used to hold a temporary instance when the area needs to be swapped.
	*/
	Mercator::Area* mOldArea;

	/**
	@brief The entity to which this area is connected to.
	*/
	EmberEntity& mEntity;

	/**
	@brief The slot we use for listening to attribute changes in the entity. Mainly used to listen for changes to the "area" attribute, since that's the one which defined the area.
	*/
	Eris::Entity::AttrChangedSlot mAttrChangedSlot;

	/**
	 * @brief When the "area" attribute of the entity to which the terrain area belongs to changes we need to emit the EventAreaChanged signal. This method will take care of that.
	 * @param attributeValue The attribute which was changed.
	 */
	void attributeChanged(const Atlas::Message::Element& attributeValue);

	/**
	 * @brief Called when the entity moves, at which point we need to update the area.
	 */
	void entity_Moved();

	/**
	 * @brief Initializes the observation of the entity, setting up listeners.
	 */
	void observeEntity();

	/**
	 * @brief Parses the Atlas data looking for area definitions.
	 * If no Mercator::Area instance has been created, one will be created here. If however there's alreay an existing area it will be updated.
	 * @return
	 */
	bool parseArea();

};

inline Mercator::Area* TerrainArea::getArea() const
{
	return mArea;
}
inline void TerrainArea::setArea(Mercator::Area* area)
{
	mArea = area;
}

}
};

#endif
