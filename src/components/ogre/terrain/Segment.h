/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef EMBEROGRE_TERRAIN_SEGMENT_H_
#define EMBEROGRE_TERRAIN_SEGMENT_H_

#include <string>
#include <functional>

namespace Mercator
{
class Segment;
}

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Represents a Mercator::Segment instance.
 * An instance of this wraps around a Mercator::Segment instance. It's main purpose is to provide an abstraction over the underlying Mercator::Segment instance, so that it easier can be handled by the SegmentManager.
 */
class Segment
{
public:

	/**
	 * @brief Ctor.
	 * @param xIndex The x index of the segment.
	 * @param yIndex The y index of the segment.
	 * @param segmentProvider Function responsible for providing a segment.
	 * @param segmentInvalidator Function responsible for invalidating the segment.
	 */
	Segment(int xIndex, int yIndex, std::function<Mercator::Segment*()>& segmentProvider, std::function<void(Mercator::Segment*)>& segmentInvalidator);

	/**
	 * @brief Dtor.
	 */
	virtual ~Segment();

	/**
	 * @brief Gets the underlying Mercator segment.
	 *
	 * This method should only be called from the terrain handling thread; never from the main thread.
	 * @returns The underlying Mercator segment.
	 */
	Mercator::Segment& getMercatorSegment();

	/**
	 * @brief Gets the x index of the segment in the Mercator::Terrain.
	 * @returns The x index of the segment.
	 */
	int getXIndex() const;

	/**
	 * @brief Gets the y index of the segment in the Mercator::Terrain.
	 * @returns The y index of the segment.
	 */
	int getYIndex() const;

	/**
	 * @brief Gets a unique key for the segment, to be used for quick lookup.
	 * @returns A unique key for the segment.
	 */
	std::string getKey() const;

	/**
	 * @brief Invalidates this segment.
	 *
	 * This will free any memory held by the segment.
	 */
	virtual void invalidate();

protected:

	/**
	 * @brief The x index of the segment.
	 *
	 * Note that this isn't the "world position", but the "index".
	 */
	int mXIndex;

	/**
	 * @brief The y index of the segment.
	 *
	 * Note that this isn't the "world position", but the "index".
	 */
	int mYIndex;

	/**
	 * @brief The underlying Mercator segment which this class wraps.
	 *
	 * This is initially empty and later on set to the result of calling mSegmentCreator.
	 */
	Mercator::Segment* mSegment;

	/**
	 * @brief Returns a segment.
	 *
	 * This is meant to be called from the terrain handling thread.
	 */
	std::function<Mercator::Segment*()> mSegmentProvider;

	/**
	 * @brief Invalidates a segment.
	 */
	std::function<void(Mercator::Segment*)> mSegmentInvalidator;

};

}

}

}

#endif /* EMBEROGRE_TERRAIN_SEGMENT_H_ */
