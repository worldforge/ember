/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef EMBEROGRETERRAINHEIGHTMAP_H_
#define EMBEROGRETERRAINHEIGHTMAP_H_

#include "Types.h"
#include <boost/smart_ptr.hpp>
#include <map>


namespace EmberOgre
{

namespace Terrain
{

class IHeightMapSegment;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Keeps data about the height map of the terrain.
 * This class is safe for threading, in contrast to the Mercator::Terrain class which primarily provides height map features.
 * The whole reason for this class existing is basically Mercator not being thread safe. We want to be able to update the Mercator terrain in a background thread, but at the same time be able to provide real time height checking functionality for other subsystmes in Ember which are running in the main thread.
 */
class HeightMap
{
public:

    /**
     * @brief STL map to store sparse array of Segment pointers.
     */
    typedef std::map<int, boost::shared_ptr<IHeightMapSegment> > Segmentcolumn;

    /**
     * @brief STL map to store sparse array of Segment pointer columns.
     */
    typedef std::map<int, Segmentcolumn > Segmentstore;

    /**
     * @Ctor.
     * @param defaultLevel The default level of the terrain, if no valid segment can be found for a requested location.
     * @param segmentResolution The resolution of one segment, in world units.
     */
	HeightMap(float defaultLevel, unsigned int segmentResolution = 64);

	/**
	 * @brief Dtor.
	 */
	virtual ~HeightMap();

	/**
	 * @brief Inserts a new height map segment at the specified index.
	 * This transfers ownership of the segment to this instance.
	 * @param xIndex The x index.
	 * @param yIndex The y index.
	 * @param segment The segment to insert.
	 */
	void insert(int xIndex, int yIndex, IHeightMapSegment* segment);

	/**
	 * @brief Removes a segment at the specified index location.
	 * This will destroy the segment instance.
	 * @param xIndex The x index.
	 * @param yIndex The y index.
	 * @returns True if a segment was found at the specified location.
	 */
	bool remove(int xIndex, int yIndex);

	/**
	 * @brief Gets the height at the specified location.
	 * This is a crude and fast lookup method which won't take into account slopes.
	 * @see getHeightAndNormal()
	 * @param x The x location, in world units.
	 * @param y The y location, in world units.
	 * @returns The height at the location.
	 */
    float getHeight(float x, float y) const;

    /**
     * @brief Gets the height and normal at the location.
     * This calculates slopes and provides a precise height. It's therefore more time consuming than getHeight().
	 * @param x The x location, in world units.
	 * @param y The y location, in world units.
	 * @param height The height will be stored here.
	 * @param normal The normal will be stored here.
	 * @returns True if a segment was found.
     */
    bool getHeightAndNormal(float x, float y, float& height, WFMath::Vector<3>& normal) const;


private:

    /**
     * @brief A sparse map of height map segments.
     */
	Segmentstore mSegments;

	/**
	 * @brief The default height to report a height query is requested for a position for which there is no segment.
	 */
	float mDefaultLevel;

	/**
	 * @brief The size of one side of a segment.
	 */
	unsigned int mSegmentResolution;

	/**
	 * @brief Gets the segment at the specified index.
	 * @param xIndex The x index.
	 * @param yIndex The y index.
	 * @returns A shared pointer to a segment, or to null if no segment could be found.
	 */
	boost::shared_ptr<IHeightMapSegment> getSegment(int xIndex, int yIndex) const;
};

}

}

#endif /* EMBEROGRETERRAINHEIGHTMAP_H_ */
