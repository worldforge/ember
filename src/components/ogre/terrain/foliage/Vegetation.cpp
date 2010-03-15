/*
 * Vegetation.cpp
 *
 *  Created on: 11 mar 2010
 *      Author: Erik Hjortsberg
 */

#include "Vegetation.h"
#include "ClusterPopulator.h"
#include "components/ogre/terrain/TerrainLayerDefinition.h"
#include <boost/shared_ptr.hpp>

namespace EmberOgre
{

namespace Terrain
{

namespace Foliage
{

Vegetation::Vegetation()
{
}

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
			scaler = new UniformScaler(atof(foliageDef.getParameter("minScale").c_str()), atof(foliageDef.getParameter("maxScale").c_str()));
		} else {
			scaler = new Scaler(atof(foliageDef.getParameter("minWidth").c_str()), atof(foliageDef.getParameter("maxWidth").c_str()), atof(foliageDef.getParameter("minHeight").c_str()), atof(foliageDef.getParameter("maxHeight").c_str()));
		}
		ClusterPopulator* populator = new ClusterPopulator(surfaceLayerIndex, scaler, mPopulators.size());
		populator->setClusterDistance(atof(foliageDef.getParameter("clusterDistance").c_str()));
		populator->setMinClusterRadius(atof(foliageDef.getParameter("minClusterRadius").c_str()));
		populator->setMaxClusterRadius(atof(foliageDef.getParameter("maxClusterRadius").c_str()));
		populator->setDensity(atof(foliageDef.getParameter("density").c_str()));
		populator->setFalloff(atof(foliageDef.getParameter("falloff").c_str()));
		unsigned char threshold(100);
		if (foliageDef.getParameter("threshold") != "") {
			threshold = static_cast<unsigned char> (atoi(foliageDef.getParameter("threshold").c_str()));
		}
		populator->setThreshold(threshold);

		mPopulators[foliageDef.getPlantType()] = populator;

	}
}

void Vegetation::populate(const std::string& plantType, PlantAreaQueryResult& result, SegmentRefPtr segmentRef, const WFMath::AxisBox<2>& area)
{
	PopulatorStore::const_iterator I = mPopulators.find(plantType);
	if (I != mPopulators.end()) {
		I->second->populate(result, segmentRef);
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
