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

#include "components/ogre/terrain/TerrainDefPoint.h"
#include "components/ogre/Convert.h"
#include "framework/LoggingInstance.h"

#include <Mercator/BasePoint.h>

#include <Atlas/Message/Element.h>

namespace Ember {
namespace OgreView {

namespace Terrain {

TerrainDefPointStore TerrainParser::parseTerrain(const Atlas::Message::MapType& points, const WFMath::Point<3>& offset) {
	Terrain::TerrainDefPointStore pointStore;


	auto parsePointFn = [&](const Atlas::Message::ListType& point) {
		if (point.size() < 3) {
			S_LOG_INFO("Point with less than 3 nums.");
			return;
		}

		Terrain::TerrainDefPoint defPoint;

		defPoint.position = WFMath::Point<2>(static_cast<int>(point[0].asNum() + offset.x()), static_cast<int>(point[1].asNum() + offset.z()));
		defPoint.height = static_cast<float>(point[2].asNum() + offset.y());
		if (point.size() > 3) {
			defPoint.roughness = (float)point[3].asNum();
		} else {
			defPoint.roughness = Mercator::BasePoint::ROUGHNESS;
		}
		if (point.size() > 4) {
			defPoint.falloff = (float)point[4].asNum();
		} else {
			defPoint.falloff = Mercator::BasePoint::FALLOFF;
		}
		pointStore.emplace_back(defPoint);

	};

	for (auto& entry: points) {
		if (!entry.second.isList()) {
			S_LOG_INFO("Non list in points.");
			continue;
		}
		try {
			parsePointFn(entry.second.List());
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when parsing terrain points data." << ex);
		}
	}

	return pointStore;
}

}

}
}
