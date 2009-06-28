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
*/
class TerrainPageGeometry{
public:
	TerrainPageGeometry(TerrainPage& page);

	~TerrainPageGeometry();

	void init(const Mercator::Terrain& terrain);

	/**
	 *    The max height of this page
	 * @return Max height
	 */
	float getMaxHeight() const;


	/**
	 *    Fills the bound height data with height data. If no buffer has been bound nothing will be done.
	 */
	void updateOgreHeightData(float* heightData);

	/**
	 *    Gets the segment positioned at the supplied position in local space.
	 * @param pos A Wordforge position in local space, i.e. > 0 && < [width in meters of the page]
	 * @return A pointer to Mercator::Segment or null.
	 */
	const Mercator::Segment* getSegmentAtLocalPosition(const TerrainPosition& pos) const;

	/**
	 *    Gets the segment positioned at the supplied position in local space and also translates the supplied position into a local position in the returned segment.
	 * @param pos A Wordforge position in local space, i.e. > 0 && < [width in meters of the page]
	 * @param localPositionInSegment The resulting position in the segment space.
	 * @return A pointer to Mercator::Segment or null.
	 */
	const Mercator::Segment* getSegmentAtLocalPosition(const TerrainPosition& pos, TerrainPosition& localPositionInSegment) const;

	const SegmentVector& getValidSegments() const;

private:

	/**
	 * @brief A store of the valid mercator segments which make up this page.
	 */
	SegmentVector mValidSegments;


	TerrainPage& mPage;

	/**
	A local copy of the segments for fast lookup. This will also include nonvalid segments.
	The keys will be the local indices.
	*/
	Mercator::Terrain::Segmentstore mLocalSegments;

	void blitSegmentToOgre(float* ogreHeightData, Mercator::Segment& segment, int startX, int startY);

	/**
	 * @brief Gets a segment for the x and y position in the page.
	 * @param x
	 * @param y
	 * @return
	 */
	Mercator::Segment* getSegmentAtLocalIndex(const Mercator::Terrain& terrain, int indexX, int indexY) const;

};
}
}

#endif
