/*
 Copyright (C) 2009  Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainParser.h"

#include "components/ogre/terrain/TerrainManager.h"
#include "components/ogre/terrain/TerrainDefPoint.h"
#include "components/ogre/Convert.h"
#include "framework/LoggingInstance.h"

#include <Mercator/BasePoint.h>

#include <Atlas/Message/Element.h>

namespace Ember {
namespace OgreView {

namespace Terrain {

TerrainParser::TerrainParser() = default;

TerrainDefPointStore TerrainParser::parseTerrain(const Atlas::Message::Element& terrain, const WFMath::Point<3>& offset) const {
	Terrain::TerrainDefPointStore pointStore;

	//_fpreset();
	if (!terrain.isMap()) {
		S_LOG_FAILURE("Terrain is not a map");
	} else {
		auto& tmap = terrain.Map();
		auto I = tmap.find("points");
		if (I == tmap.end()) {
			S_LOG_FAILURE("No terrain points");
		}

		auto parsePointFn = [&](const Atlas::Message::ListType& point) {
			if (point.size() < 3) {
				S_LOG_INFO("Point with less than 3 nums.");
				return;
			}

			Terrain::TerrainDefPoint defPoint;

			defPoint.position = WFMath::Point<2>(static_cast<int>(point[0].asNum() + offset.x()), static_cast<int>(point[1].asNum() + offset.z()));
			defPoint.height = static_cast<float>(point[2].asNum() + offset.y());
			if (point.size() > 3) {
				defPoint.roughness = point[3].asFloat();
			} else {
				defPoint.roughness = Mercator::BasePoint::ROUGHNESS;
			}
			if (point.size() > 4) {
				defPoint.falloff = point[4].asFloat();
			} else {
				defPoint.falloff = Mercator::BasePoint::FALLOFF;
			}
			pointStore.push_back(defPoint);

		};

		if (I->second.isMap()) {
			auto& plist = I->second.Map();
			auto J = plist.begin();
			for (; J != plist.end(); ++J) {
				if (!J->second.isList()) {
					S_LOG_INFO("Non list in points.");
					continue;
				}
				const Atlas::Message::ListType& point = J->second.List();
				parsePointFn(point);
			}
		} else {
			S_LOG_FAILURE("Terrain is the wrong type");
		}
	}
	return pointStore;

}

}

}
}
