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

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

namespace Foliage
{


Vegetation::~Vegetation()
{
	for (PopulatorStore::const_iterator I = mPopulators.begin(); I != mPopulators.end(); ++I) {
		delete I->second;
	}

}

void Vegetation::createPopulator(const TerrainFoliageDefinition& foliageDef, unsigned int surfaceLayerIndex)
{
	if (foliageDef.getPopulationTechnique() == "cluster") {
		IScaler* scaler = 0;
		if (foliageDef.hasParameter("minScale")) {
			scaler = new UniformScaler(std::stof(foliageDef.getParameter("minScale")), std::stof(foliageDef.getParameter("maxScale")));
		} else {
			scaler = new Scaler(std::stof(foliageDef.getParameter("minWidth")),
								std::stof(foliageDef.getParameter("maxWidth")),
								std::stof(foliageDef.getParameter("minHeight")),
								std::stof(foliageDef.getParameter("maxHeight")));
		}
		ClusterPopulator* populator = new ClusterPopulator(surfaceLayerIndex, scaler, mPopulators.size());
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

		mPopulators[foliageDef.getPlantType()] = populator;

	}
}

void Vegetation::populate(const std::string& plantType, PlantAreaQueryResult& result, SegmentRefPtr segmentRef, const WFMath::AxisBox<2>& area)
{
	PopulatorStore::const_iterator I = mPopulators.find(plantType);
	if (I != mPopulators.end()) {
		I->second->populate(result, std::move(segmentRef));
	}
}

PlantPopulator* Vegetation::getPopulator(const std::string& plantType)
{
	PopulatorStore::const_iterator I = mPopulators.find(plantType);
	if (I != mPopulators.end()) {
		return I->second;
	}
	return 0;
}

}

}

}
}
