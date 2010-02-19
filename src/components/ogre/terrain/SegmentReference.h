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

#ifndef EMBEROGRE_TERRAIN_SEGMENTREFERENCE_H_
#define EMBEROGRE_TERRAIN_SEGMENTREFERENCE_H_

namespace EmberOgre
{

namespace Terrain
{

class SegmentHolder;
class Segment;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A reference to a Segment instance.
 * The sole purpose of this class is to provide reference counting for Segments, so that the SegmentManager can know which Segments are in use.
 */
class SegmentReference
{
friend class SegmentHolder;
public:

	/**
	 * @brief Dtor.
	 */
	~SegmentReference();

	/**
	 * @brief Gets the Segment instance that is being referred.
	 */
	Segment& getSegment();

protected:

	/**
	 * @brief Holds on to the owning holder so that it can be notified when an instance of this is destroyed.
	 */
	SegmentHolder& mSegmentHolder;

	/**
	 * @brief Ctor.
	 * This is private to prevent anything else that SegmentHolder from creating new instances.
	 */
	SegmentReference(SegmentHolder& segmentHolder);
};

}

}

#endif /* EMBEROGRE_TERRAIN_SEGMENTREFERENCE_H_ */
