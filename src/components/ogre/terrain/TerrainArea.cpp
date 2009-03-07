//
// C++ Implementation: TerrainArea
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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

#include "../EmberEntity.h"
#include <Mercator/Area.h>

namespace EmberOgre {
namespace Terrain {





TerrainArea::TerrainArea(EmberEntity* entity) : mArea(0), mOldArea(0), mEntity(entity)
{
}


TerrainArea::~TerrainArea()
{
	mAttrChangedSlot.disconnect();
	EventAreaRemoved.emit();
	delete mArea;
}

bool TerrainArea::init() {
	
 //   _fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR , _MCW_RC);
	bool successfulParsing = parseArea();
	if (successfulParsing) {
		observeEntity();
	}
	return successfulParsing;
	
}

bool TerrainArea::parseArea()
{
	if (!mEntity->hasAttr("area")) {
		S_LOG_FAILURE("TerrainArea created for entity with no area attribute");
		return false;
	}
	
	const Atlas::Message::Element areaElem(mEntity->valueOfAttr("area"));
	
	if (!areaElem.isMap()) {
		S_LOG_FAILURE("TerrainArea element ('area') must be of map type.");
		return false;
	}
	
	const Atlas::Message::MapType& areaData(areaElem.asMap());
	
	int layer = 0;
	WFMath::Polygon<2> poly;
	TerrainAreaParser parser;
	if (parser.parseArea(areaData, poly, layer)) {
		if (!mArea) {
			mArea = new Mercator::Area(layer, false);
		} else {
			///A bit of an ugly hack here since the Mercator system doesn't support changing the layer. We need to swap the old area for a new one if the layer has changed.
			if (mArea->getLayer() != layer) {
				mOldArea = mArea;
				mArea = new Mercator::Area(layer, false);
			}
		}
		/// transform polygon into terrain coords
		WFMath::Vector<3> xVec = WFMath::Vector<3>(1.0, 0.0, 0.0).rotate(mEntity->getOrientation());
		double theta = atan2(xVec.y(), xVec.x()); // rotation about Z
		
		WFMath::RotMatrix<2> rm;
		poly.rotatePoint(rm.rotation(theta), WFMath::Point<2>(0,0));
		poly.shift(WFMath::Vector<2>(mEntity->getPosition().x(), mEntity->getPosition().y()));

		mArea->setShape(poly);

		return true;	
	} else {
		return false;
	}
}

void TerrainArea::attributeChanged(const Atlas::Message::Element& attributeValue)
{
	if (parseArea()) {
		if (mOldArea) {
			EventAreaSwapped(*mOldArea);
			delete mOldArea;
			mOldArea = 0;
		} else {
			EventAreaChanged();
		}
	}
}

void TerrainArea::entity_Moved()
{
	if (parseArea()) {
		EventAreaChanged();
	}
}

void TerrainArea::observeEntity()
{
	mAttrChangedSlot.disconnect();
	if (mEntity) {
		mAttrChangedSlot = sigc::mem_fun(*this, &TerrainArea::attributeChanged);
		mEntity->observe("area", mAttrChangedSlot);
		mEntity->Moved.connect(sigc::mem_fun(*this, &TerrainArea::entity_Moved));
	}
}

}
}
