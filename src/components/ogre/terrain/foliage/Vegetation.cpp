/*
 * Vegetation.cpp
 *
 *  Created on: 11 mar 2010
 *      Author: Erik Ogenvik
 */

#include "Vegetation.h"

#include <utility>
#include "ClusterPopulator.h"
#include "components/ogre/terrain/TerrainLayerDefinition.h"

namespace Ember {
namespace OgreView {

namespace Terrain {

namespace Foliage {


Vegetation::~Vegetation() = default;

void Vegetation::createPopulator(const TerrainFoliageDefinition& foliageDef, unsigned int surfaceLayerIndex) {
	if (foliageDef.mPopulationTechnique == "cluster") {
		std::unique_ptr<IScaler> scaler;
		if (foliageDef.hasParameter("minScale")) {
			scaler = std::make_unique<UniformScaler>(std::stof(foliageDef.getParameter("minScale")), std::stof(foliageDef.getParameter("maxScale")));
		} else {
			scaler = std::make_unique<Scaler>(std::stof(foliageDef.getParameter("minWidth")),
											  std::stof(foliageDef.getParameter("maxWidth")),
											  std::stof(foliageDef.getParameter("minHeight")),
											  std::stof(foliageDef.getParameter("maxHeight")));
		}
		auto populator = std::make_unique<ClusterPopulator>(surfaceLayerIndex, std::move(scaler), mPopulators.size());
		populator->setClusterDistance(std::stof(foliageDef.getParameter("clusterDistance")));
		populator->setMinClusterRadius(std::stof(foliageDef.getParameter("minClusterRadius")));
		populator->setMaxClusterRadius(std::stof(foliageDef.getParameter("maxClusterRadius")));
		populator->setDensity(std::stof(foliageDef.getParameter("density")));
		populator->setFalloff(std::stof(foliageDef.getParameter("falloff")));
		unsigned char threshold(100);
		if (!foliageDef.getParameter("threshold").empty()) {
			threshold = static_cast<unsigned char> (std::stoi(foliageDef.getParameter("threshold")));
		}
		populator->setThreshold(threshold);

		mPopulators[foliageDef.mPlantType] = std::move(populator);

	}
}

void Vegetation::populate(const std::string& plantType, PlantAreaQueryResult& result, SegmentRefPtr segmentRef, const WFMath::AxisBox<2>& area) {
	auto I = mPopulators.find(plantType);
	if (I != mPopulators.end()) {
		I->second->populate(result, std::move(segmentRef));
	}
}

PlantPopulator* Vegetation::getPopulator(const std::string& plantType) {
	auto I = mPopulators.find(plantType);
	if (I != mPopulators.end()) {
		return I->second.get();
	}
	return nullptr;
}

}

}

}
}
