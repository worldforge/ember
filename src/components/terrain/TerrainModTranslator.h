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
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
 @author Erik Hjortsberg <erik.hjortsberg@iteam.se>
 @brief Base class for all terrain mod specific classes.This is not meant to be used directly by anything else than the TerrainMod class.
 The TerrainMod class in itself doesn't hold the actual reference to the terrain mod, and doesn't handle the final parsing of Atlas data. This is instead handled by the different subclasses of this class. Since the different kinds of terrain mods found in Mercator behave differently depending on their type and the kind of shape used, we need to separate the code for handling them into different classes.
 */
class TerrainModTranslator
{
protected:

	class InnerTranslator
	{
	public:
		InnerTranslator(const Atlas::Message::MapType &);
		virtual Mercator::TerrainMod* createInstance(const WFMath::Point<3>& pos, const WFMath::Quaternion& orientation) = 0;
	protected:
		const Atlas::Message::MapType mData;
		static float parsePosition(const WFMath::Point<3> & pos, const Atlas::Message::MapType& modElement);
	};

	template<template<int> class Shape>
	Mercator::TerrainMod* parseStuff(const WFMath::Point<3> & pos, const WFMath::Quaternion & orientation, const Atlas::Message::MapType& modElement, const std::string & typeName, Shape<2> & shape, const Atlas::Message::Element & shapeElement);

	template<template<int> class Shape>
	InnerTranslator* buildTranslator(const Atlas::Message::MapType& modElement, const std::string & typeName, Shape<2> & shape, const Atlas::Message::Element & shapeElement);

public:

	explicit TerrainModTranslator(const Atlas::Message::MapType& data);

	Mercator::TerrainMod* parseData(const WFMath::Point<3> & pos, const WFMath::Quaternion &);

	bool isValid() const;

	void reset();

protected:

	std::shared_ptr<TerrainModTranslator::InnerTranslator> mInnerTranslator;
};

}
}

#endif
