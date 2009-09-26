//
// C++ Interface: IPolygonPositionProvider
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
#ifndef EMBEROGRE_MANIPULATIONIPOLYGONPOSITIONPROVIDER_H
#define EMBEROGRE_MANIPULATIONIPOLYGONPOSITIONPROVIDER_H

namespace EmberOgre
{

namespace Authoring
{

/**
 @brief Provides the ability to provide a height value to the otherwise 2d polygons.
 For example, if a polygon is placed on terrain we want the height to be adjusted so that it snaps to the ground.
 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class IPolygonPositionProvider
{
public:

	/**
	 * @brief Gets the height for the local position, within the polygon's space.
	 * @param localPosition The local position within the polygon's space.
	 * @return The height.
	 */
	virtual float getHeightForPosition(const WFMath::Point<2>& localPosition) = 0;

};

}

}

#endif
