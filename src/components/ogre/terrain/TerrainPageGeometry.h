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
#include <Mercator/Terrain.h>
#include <wfmath/point.h>
#include "../Types.h"
#include <vector>

namespace Mercator {
class Segment;
class Terrain;
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

/**
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
@brief Handles the geometry of a terrain page.

Each instance of TerrainPage holds a corresponding instance of this. The main purpose of this class is to provide a central place where all actions which alters and populates the Mercator::Segment instances which makes up a page occurs.
An instance of this is then used by TerrainPage and passed on to the other TerrainPage* classes which provide terrain page functions such as foliage, surfaces etc.

@note Be sure to call init() before an instance of this can be used.
*/
class TerrainPageGeometry
{
friend class TerrainPage;
public:
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
	 * @brief Gets the collection of valid segments which make up this geometry.
	 */
	const SegmentVector getValidSegments() const;

	/**
	 * @brief Gets the normal at the specified local position.
	 * @param localPosition The position, local to the page.
	 * @param normal The normal will be placed here.
	 * @return True if a valid normal could be found at the specified position.
	 */
	bool getNormal(const TerrainPosition& localPosition, WFMath::Vector<3>& normal) const;

private:

	/**
	 * @brief The page to which this geometry belongs.
	 */
	TerrainPage& mPage;

	const Mercator::Terrain& mTerrain;

	/**
	 * @brief A local copy of the segments for fast lookup. This will also include nonvalid segments.
	 * The keys will be the local indices.
	 */
	Mercator::Terrain::Segmentstore mLocalSegments;

	/**
	 * @brief Ctor.
	 * @note Be sure to call init() before this instance can be used.
	 * Private so that only TerrainPage can create an instance of this.
	 * @param page The TerrainPage instance to which this geometry belongs.
	 * @param defaultHeight The default height of any parts of the terrain where no segment has been initialized.
	 */
	TerrainPageGeometry(TerrainPage& page, const Mercator::Terrain& terrain, float defaultHeight);

	/**
	 * @brief Initializes the geometry.
	 * This must always be called before an instance of this can be used.
	 * During initialization, the segments which make up this geometry will be collected, and populated.
	 * Private so that only TerrainPage can initialize it.
	 */
	void init();

	/**
	 * @brief Repopulates the segments which make up the page.
	 * Private so that only TerrainPage can repopulate it.
	 */
	void repopulate();

	/**
	 * @brief Blits a Mercator::Segment heightmap to a larger ogre height map.
	 * @param ogreHeightData The Ogre height data. This is guaranteed to be <page size> * <page size>.
	 * @param segment The segment to blit.
	 * @param startX The starting x position in Ogre space.
	 * @param startY The starting y position in Ogre space.
	 */
	void blitSegmentToOgre(float* ogreHeightData, Mercator::Segment& segment, int startX, int startY);

	/**
	 * @brief Gets a segment for the x and y position in the page.
	 * @param x Local x index.
	 * @param y Local y index.
	 * @return A segment instance, or null if none could be found.
	 */
	Mercator::Segment* getSegmentAtLocalIndex(const Mercator::Terrain& terrain, int indexX, int indexY) const;

	/**
	 * @brief The default height of any parts of the terrain where no segment has been initialized.
	 */
	float mDefaultHeight;
};
}
}

#endif
