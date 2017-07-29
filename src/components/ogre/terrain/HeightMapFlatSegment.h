/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

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

#ifndef EMBEROGRETERRAINHEIGHTMAPFLATSEGMENT_H_
#define EMBEROGRETERRAINHEIGHTMAPFLATSEGMENT_H_

#include "IHeightMapSegment.h"

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Represents one segment (mapped to a Mercator::Segment) in the height map which is completely flat.
 * To save space we only need one single height, and don't need to allocate memory for a height map buffer.
 */
class HeightMapFlatSegment : public IHeightMapSegment
{
public:
	HeightMapFlatSegment(float height);
	virtual ~HeightMapFlatSegment();

	/**
	 * @brief Gets the height at the specified location.
	 * This is a crude and fast lookup method which won't take into account slopes.
	 * @see getHeightAndNormal()
	 * @param x The x location, in world units.
	 * @param y The y location, in world units.
	 * @returns The height at the location.
	 */
	virtual float getHeight(int x, int y) const;

    /**
     * @brief Gets the height and normal at the location.
     * This calculates slopes and provides a precise height. It's therefore more time consuming than getHeight().
	 * @param x The x location, in world units.
	 * @param y The y location, in world units.
	 * @param height The height will be stored here.
	 * @param normal The normal will be stored here.
     */
	virtual void getHeightAndNormal(float x, float y, float& height, WFMath::Vector<3>& normal) const;


protected:
	float mHeight;
};

}

}

}

#endif /* EMBEROGRETERRAINHEIGHTMAPFLATSEGMENT_H_ */
