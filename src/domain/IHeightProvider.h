/*
 Copyright (C) 2011 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef EMBER_DOMAIN_IHEIGHTPROVIDER_H_
#define EMBER_DOMAIN_IHEIGHTPROVIDER_H_

#include "Types.h"

#include <vector>

namespace Ember
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Provides height data for supplied positions in the world.
 *
 * This is used to better align the visual representation of entities with the world in general.
 * The typical example of an implementation of this is a class which allows entities placed on the ground to "snap" to the ground.
 * Other implementations might provide ways for entities to rest on tables or shelves.
 */
class IHeightProvider
{
public:
	virtual ~IHeightProvider()
	{
	}

	/**
	 * @brief Returns the height at the specified position in the world.
	 *
	 * This will be done using the underlying Mercator data, which depending on LOD techniques used can differ some from the actual graphical representation.
	 * @note The method used for lookup does interpolation, so it's a little bit more expensive than doing a instant data lookup. Calling this is therefore not recommended if you're building height data, but suitable if you're placing entities on the terrain and need a perfect height.
	 * @param atPosition The position, in world space, to get the height for.
	 * @param height The height, in world space, at the specified position.
	 * @returns True if there was a valid, populated segment at the position (and therefore also a valid height).
	 */
	virtual bool getHeight(const TerrainPosition& atPosition, float& height) const = 0;

    /**
     * @brief Performs a fast copy of the raw height data for the supplied area.
     * @param xMin Minimum x coord of the area.
     * @param xMax Maximum x coord of the area.
     * @param yMin Minimum y coord of the area.
     * @param yMax Maximum y coord of the area.
     * @param heights A vector into which heigh data will be placed. This should preferably already have a capacity reserved.
     */
	virtual void blitHeights(int xMin, int xMax, int yMin, int yMax, std::vector<float>& heights) const = 0;

};

}

#endif /* EMBER_DOMAIN_IHEIGHTPROVIDER_H_ */
