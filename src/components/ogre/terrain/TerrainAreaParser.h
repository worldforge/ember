//
// C++ Interface: TerrainAreaParser
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
#ifndef EMBEROGRE_TERRAINTERRAINAREAPARSER_H
#define EMBEROGRE_TERRAINTERRAINAREAPARSER_H

#include <Atlas/Message/Element.h>
#include <wfmath/polygon.h>

namespace EmberOgre {

namespace Terrain {

class TerrainArea;


/**
@brief Parses area data from an Atlas element.
The element must be of map type and contain an int value named "layer", and a list of float tuples named "area".
@author Erik Hjortsberg
*/
class TerrainAreaParser
{
public:
/**
* @brief Parses the area data from the atlas element.
* @param areaData The root area element.
* @param poly The shape of the area will be put here.
* @param layer The layer of the area will be put here.
* @return True if the parsing was successful.
*/
bool parseArea(const Atlas::Message::MapType& areaData, WFMath::Polygon<2>& poly, int& layer);

const Atlas::Message::Element createElement(const WFMath::Polygon<2>& poly, int layer);

};

}

}

#endif
