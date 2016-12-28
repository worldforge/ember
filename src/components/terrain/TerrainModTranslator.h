//
// C++ Interface: TerrainMod
//
// Description: The purpose of this class is to handle the bulk of the work
//		involved with using Mercator::TerrainMods. It handles parsing
//		the Atlas data and storing all the information needed by 
//		TerrainGenerator to add and remove them from the Terrain.
//
//		TerrainGenerator listens for changes in the modifier and
//		updates or removes the modifiers from the terrain as needed.
//
//
// Author: Tamas Bates <rhymer@gmail.com>, (C) 2008
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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
#ifndef TERRAINMODTRANSLATOR_H
#define TERRAINMODTRANSLATOR_H

#include <Atlas/Message/Element.h>

#include <wfmath/point.h>

#include <memory>

namespace Mercator
{
class TerrainMod;
}

namespace Ember
{
namespace Terrain
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Handles translation of terrain mod data into Mercator::TerrainMod instances.
 *
 */
class TerrainModTranslator
{
public:

	explicit TerrainModTranslator(const Atlas::Message::MapType& data);
	~TerrainModTranslator() = default;

	/**
	 * Creates a TerrainMod instance, if possible.
	 * @param pos
	 * @param orientation
	 * @return A terrain mod instance, or null if none could be created.
	 */
	Mercator::TerrainMod* parseData(const WFMath::Point<3> & pos, const WFMath::Quaternion & orientation);

	/**
	 * @brief True if there's a valid inner translator.
	 * @return
	 */
	bool isValid() const;

	/**
	 * Removes any internal translator.
	 */
	void reset();

	static float parsePosition(const WFMath::Point<3> & pos, const Atlas::Message::MapType& modElement);

	/**
	 * Interface for inner translator.
	 *
	 * Concrete templated subclasses are used for both shapes and mod types.
	 */
	class InnerTranslator
	{
	public:
		InnerTranslator(const Atlas::Message::MapType &);
		virtual Mercator::TerrainMod* createInstance(const WFMath::Point<3>& pos, const WFMath::Quaternion& orientation) = 0;
	protected:
		const Atlas::Message::MapType mData;
	};
    
protected:
	template<template<int> class Shape>
	InnerTranslator* buildTranslator(const Atlas::Message::MapType& modElement, const std::string & typeName, Shape<2> & shape, const Atlas::Message::Element & shapeElement);

	std::shared_ptr<TerrainModTranslator::InnerTranslator> mInnerTranslator;
};

}
}

#endif
