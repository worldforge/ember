//
// C++ Interface: TerrainPageGeometry
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRETERRAINPAGEGEOMETRY_H
#define EMBEROGRETERRAINPAGEGEOMETRY_H
#include <wfmath/point.h>
#include "Types.h"
#include <vector>

namespace Mercator
{
class Segment;
}

namespace EmberOgre {
namespace Terrain {

/**
 * @brief Holds a Mercator::Segment instance as well as a index of this segment in local page coords.
 */
struct PageSegment
{
	/**
	 * @brief The index of this segment within the page.
	 */
	TerrainPosition index;

	/**
	 * @brief A mercator segment instance.
	 */
	Mercator::Segment* segment;
};

typedef std::vector<PageSegment> SegmentVector;


class TerrainPage;
class SegmentManager;

/**
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
@brief Handles the geometry of a terrain page.

Each instance of TerrainPage is light weight, and doesn't hold any terrain data. This data is also volatile, as we don't want to keep it in memory all the time. The main reason for this is that with a large terrain the amount of needed memory just for the terrain would grow very large.
Instead we use an instance of this each time we want to access terrain data. Each instance of this class is meant to be in use a very short time. As soon as it's destroyed the Segments used by it will be returned to the SegmentManager, and optionally have had their memory freed.

*/
class TerrainPageGeometry
{
friend class TerrainPage;
public:

	/**
	 * @brief Ctor.
	 * @param page The TerrainPage instance to which this geometry belongs.
	 * @param segmentManager The segment manager from which we'll obtain SegmentReferences.
	 * @param defaultHeight The default height of any parts of the terrain where no segment has been initialized.
	 */
	TerrainPageGeometry(TerrainPage& page, SegmentManager& segmentManager, float defaultHeight);

	/**
	 * @brief Dtor.
	 */
	~TerrainPageGeometry();

	/**
	 * @brief The max height of this page
	 * @return Max height
	 */
	float getMaxHeight() const;


	/**
	 * @brief Fills the bound height data with height data. If no buffer has been bound nothing will be done.
	 */
	void updateOgreHeightData(float* heightData);

	/**
	 * @brief Gets the segment positioned at the supplied position in local space.
	 * @param pos A Wordforge position in local space, i.e. > 0 && < [width in meters of the page]
	 * @return A pointer to Mercator::Segment or null.
	 */
	const Mercator::Segment* getSegmentAtLocalPosition(const TerrainPosition& pos) const;

	/**
	 * @brief Gets the segment positioned at the supplied position in local space and also translates the supplied position into a local position in the returned segment.
	 * @param pos A Wordforge position in local space, i.e. > 0 && < [width in meters of the page]
	 * @param localPositionInSegment The resulting position in the segment space.
	 * @return A pointer to Mercator::Segment or null.
	 */
	const Mercator::Segment* getSegmentAtLocalPosition(const TerrainPosition& pos, TerrainPosition& localPositionInSegment) const;

	/**
	 * @brief Gets the collection of segments which make up this geometry.
	 */
	const SegmentVector getValidSegments() const;

	/**
	 * @brief Gets the normal at the specified local position.
	 * @param localPosition The position, local to the page.
	 * @param normal The normal will be placed here.
	 * @return True if a valid normal could be found at the specified position.
	 */
	bool getNormal(const TerrainPosition& localPosition, WFMath::Vector<3>& normal) const;

	/**
	 * @brief Repopulates the segments which make up the page.
	 */
	void repopulate();


	/**
	 * @brief Gets the page to which this geometry belongs.
	 * @returns The page to which the geometry belongs.
	 */
	TerrainPage& getPage();

private:

	/**
	 * @brief The page to which this geometry belongs.
	 */
	TerrainPage& mPage;

	/**
	 * @brief A store of all the SegmentReferences which make up this geometry. These are indexed using local coords.
	 */
	SegmentRefStore mLocalSegments;

	/**
	 * @brief The default height of any parts of the terrain where no segment has been initialized.
	 */
	float mDefaultHeight;


	/**
	 * @brief Blits a Mercator::Segment heightmap to a larger ogre height map.
	 * @param ogreHeightData The Ogre height data. This is guaranteed to be <page size> * <page size>.
	 * @param segment The segment to blit.
	 * @param startX The starting x position in Ogre space.
	 * @param startY The starting y position in Ogre space.
	 */
	void blitSegmentToOgre(float* ogreHeightData, Mercator::Segment& segment, int startX, int startY);

};
}
}

#endif
