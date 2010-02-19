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

#ifndef EMBEROGRE_TERRAIN_SEGMENTHOLDER_H_
#define EMBEROGRE_TERRAIN_SEGMENTHOLDER_H_

#include <boost/thread/mutex.hpp>
#include <Mercator/Segment.h>



namespace EmberOgre
{

namespace Terrain
{

class Segment;
class SegmentManager;
class SegmentReference;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Holds a Segment instance.
 * Holds a Segment instance and manages the references to it. Whenever a component needs to access a Segment instance it needs to request a reference to it.
 * These references are counted, so that the holder instance knows how many currently are in use. This is of main use to allow for the SegmentManager to release data from Segments which aren't in use.
 * This class is internal to the SegmentManager system and shouldn't be used outside of it.
 */
class SegmentHolder
{
friend class SegmentReference;
public:

	/**
	 * @brief Ctor.
	 * @param segment The segment which this holder should refer to. Ownership is passed.
	 * @param segmentManager The segment manager to which the holder belongs to.
	 */
	SegmentHolder(Segment* segment, SegmentManager& segmentManager);

	/**
	 * @brief Dtor.
	 * The Segment instance held will be destroyed.
	 */
	~SegmentHolder();

	/**
	 * @brief Gets an instance of a reference to the Segment.
	 * This will increase the reference counter.
	 */
	boost::shared_ptr<SegmentReference> getReference();

	/**
	 * @brief Checks whether there are any references to the segment in use currently.
	 * @return True if there are references to the segment in use.
	 */
	bool isUnused();

	/**
	 * @brief Gets the segment held by this holder instance.
	 * @returns The segment held by this holder.
	 */
	Segment& getSegment();

protected:

	/**
	 * @brief The segment this holder manages.
	 */
	Segment* mSegment;

	/**
	 * @brief The segment manager to which this instance belongs.
	 */
	SegmentManager& mSegmentManager;

	/**
	 * @brief The number of currently active references to the Segment.
	 */
	unsigned int mRefCount;

	/**
	 * @brief Mutex used when accessing mRefCount.
	 */
	boost::mutex mRefCountMutex;

	/**
	 * @brief Called when a reference is destroyed. This will decrease the reference counter.
	 */
	void returnReference();

};

}

}

#endif /* EMBEROGRE_TERRAIN_SEGMENTHOLDER_H_ */
