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

#ifndef EMBEROGRE_TERRAIN_SEGMENTMANAGER_H_
#define EMBEROGRE_TERRAIN_SEGMENTMANAGER_H_

#include "Types.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <map>
#include <string>
#include <list>

namespace Mercator
{
class Segment;
class Terrain;
}

namespace EmberOgre
{

namespace Terrain
{
class SegmentReference;
class SegmentHolder;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Manages all Segment instances.
 *
 * The Mercator terrain defines the terrain as a collection of separate Segment instances. Each segment is a quadratic area of terrain, under normal circumstances 64*64 world units.
 * These segments hold their own height map data, as well as any surfaces (grass, sand etc.) they might contain. With a very large terrain, the memory used by these intances might grow very large, and it's therefore prudent to release data for segments which aren't currently used.
 * The main issue with doing this is however that Mercator is inherently non-thread safe. Deleting data in a segment without knowing what other parts of the system might access it is therefore forbidden.
 * As Ember uses separate threads for all terrain handling we need a mechanism for keeping track of all segments which are used in the system, no matter in what thread they might be. The main responsibility of this manager is therefore to provide this functionality.
 *
 * The base class in the system is the Segment class, which is a wrapper around a Mercator::Segment. A subcomponent in Ember is forbidden to directly access Mercator::Segment instances from the Mercator::Terrain instance. They MUST go through this manager.
 * Each Mercator::Segment is matched against a Segment instance. As long as the Mercator::Segment instance exists there should also be a corresponding Segment instance. As this is a very lightweight class there should be no memory issue holding many in the system.
 * The Segment instances are references from the manager through instances of SegmentHolder. This is a SegmentManager insternal class who's sole responsibility is to keep a count of how many references there are to the Segment instance. When there are no active references the Segment is eligible for data release.
 * Whenever an external subsystem needs to access a segment it will need to call the getSegmentReference() method to obtain a reference instance. As long as the reference instance is alive the Segment is considered in use and will not be "collected".
 *
 */
class SegmentManager
{
public:

	typedef std::map<int, std::pair<int, int> > IndexColumn;
	typedef std::map<int, IndexColumn> IndexMap;

	/**
	 * @brief Ctor.
	 * Note that no Segments will be created until syncWithTerrain() has been called.
	 * @param terrain The main Mercator terrain instance from which segments will be obtained.
	 * @param desiredSegmentBuffer The amount of segments we want to keep around, not collected. Often the segments closest to the avatar are most often used and updated, and it's a good idea to keep a number of these around without releasing their data.
	 */
	SegmentManager(Mercator::Terrain& terrain, int desiredSegmentBuffer);

	/**
	 * @brief Dtor.
	 * @note It's imperative that all segments are returned before the manager is destroyed.
	 */
	~SegmentManager();

	/**
	 * @brief Requests a segment reference.
	 * The reference returned will guarantee that the segment isn't released, as long as it's being held.
	 * @note Trying to get a reference for an invalid index position will not throw an error, but will return a null pointer.
	 * @param xIndex The x index.
	 * @param yIndex The y index.
	 * @returns A shared pointer to a SegmentReference instance, or null if no Segment could be found for the specified index position.
	 */
	SegmentRefPtr getSegmentReference(int xIndex, int yIndex);


	/**
	 * @brief Request a collection of segment references.
	 * This works like getSegmentReference() but allows you to more efficiently get multiple segment references at once.
	 * @param indices A collection of world segment indices to segments for obtain.
	 * @param segments A segment store which will be populated by SegmentReferences.
	 * @returns The number of segments placed into the segments store.
	 */
	size_t getSegmentReferences(const IndexMap& indices, SegmentRefStore& segments);


	/**
	 * @brief Syncs the internal store of Segment instances with the Mercator terrain.
	 * If there are segments in the Mercator terrain for which there are no corresponding Segment instances such instances will be created.
	 */
	void syncWithTerrain();

	/**
	 * @brief Releases memory of unused segments.
	 * A call to this is thread safe, but will be blocking for getSegmentReference.
	 */
	void pruneUnusedSegments();

	void markHolderAsDirtyAndUnused(SegmentHolder* holder);

	void unmarkHolder(SegmentHolder* holder);

protected:

	typedef std::map<std::string, SegmentHolder* > SegmentStore;
	typedef std::list<SegmentHolder*> SegmentList;

	/**
	 * @brief The main Mercator terrain instance.
	 */
	Mercator::Terrain& mTerrain;

	/**
	 * @brief The number of desired segments to keep around as a buffer.
	 */
	int mDesiredSegmentBuffer;

	/**
	 * @brief A store of Segment instances.
	 */
	SegmentStore mSegments;

	/**
	 * @brief A mutex for accessing mSegments.
	 */
	boost::shared_mutex mSegmentsMutex;

	SegmentList mUnusedAndDirtySegments;

	/**
	 * @brief A mutex for accessing mUnusedAndDirtySegments.
	 */
	boost::shared_mutex mUnusedAndDirtySegmentsMutex;

	/**
	 * @brief Adds a new Mercator segment and creates a corresponding Segment instance for it.
	 * @param segment The Mercator segment which we want to add to the manager.
	 */
	void addSegment(Mercator::Segment& segment);


};

}

}

#endif /* EMBEROGRE_TERRAIN_SEGMENTMANAGER_H_ */
