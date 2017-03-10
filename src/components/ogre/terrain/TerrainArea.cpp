//
// C++ Implementation: TerrainArea
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainArea.h"
#include "TerrainAreaParser.h"

#include "domain/EmberEntity.h"

#include "framework/LoggingInstance.h"

#include <Mercator/Area.h>

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

TerrainArea::TerrainArea(EmberEntity& entity) :
	mEntity(entity), mParsedLayer(0)
{
	mEntity.Moved.connect([this](){EventEntityMoved.emit();});
	mEntity.BeingDeleted.connect([this](){EventEntityBeingDeleted.emit();});
}

TerrainArea::~TerrainArea()
{
}

void TerrainArea::parse(const Atlas::Message::Element& value, Mercator::Area** area)
{
	if (!value.isMap()) {
		S_LOG_FAILURE("TerrainArea element ('area') must be of map type.");
		return;
	}

	const Atlas::Message::MapType& areaData = value.Map();
	TerrainAreaParser parser;
	if (!parser.parseArea(areaData, mParsedPoly, mParsedLayer)) {
		return;
	} else {

		WFMath::Polygon<2> poly = mParsedPoly;
		if (!placeArea(poly)) {
			return;
		}
		//TODO: handle holes
		*area = new Mercator::Area(mParsedLayer, false);
		(*area)->setShape(poly);
	}
}

void TerrainArea::updatePosition(Mercator::Area** area)
{
	if (mParsedLayer == 0) {
		return;
	}

	if (!mParsedPoly.isValid() || mParsedPoly.numCorners() == 0) {
		return;
	}

	WFMath::Polygon<2> poly = mParsedPoly;
	if (!placeArea(poly)) {
		return;
	}
	//TODO: handle holes
	*area = new Mercator::Area(mParsedLayer, false);
	(*area)->setShape(poly);
}

bool TerrainArea::placeArea(WFMath::Polygon<2>& poly)
{
	//If the position if invalid we can't do anything with the area yet.
	if (!mEntity.getPosition().isValid()) {
		return false;
	}

	// transform polygon into terrain coords

	if (mEntity.getOrientation().isValid()) {
		WFMath::Vector<3> xVec = WFMath::Vector<3>(1.0, 0.0, 0.0).rotate(mEntity.getOrientation());
		double theta = atan2(xVec.y(), xVec.x()); // rotation about Z

		WFMath::RotMatrix<2> rm;
		poly.rotatePoint(rm.rotation(theta), WFMath::Point<2>(0, 0));
	}
	poly.shift(WFMath::Vector<2>(mEntity.getPosition().x(), mEntity.getPosition().y()));


	return true;
}

const std::string& TerrainArea::getEntityId() const
{
	return mEntity.getId();
}

}
}
}
