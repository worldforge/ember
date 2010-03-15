/*
 * Vegetation.h
 *
 *  Created on: 11 mar 2010
 *      Author: Erik Hjortsberg
 */

#ifndef VEGETATION_H_
#define VEGETATION_H_

#include "components/ogre/terrain/Types.h"
#include <map>

namespace EmberOgre
{

namespace Terrain
{
class TerrainFoliageDefinition;
class PlantAreaQueryResult;


namespace Foliage
{

class PlantPopulator;

class Vegetation
{
public:
	Vegetation();
	virtual ~Vegetation();

	void createPopulator(const TerrainFoliageDefinition& foliageDef, unsigned int surfaceLayerIndex);

	void populate(const std::string& plantType, PlantAreaQueryResult& result, SegmentRefPtr segmentRef, const WFMath::AxisBox<2>& area);

	PlantPopulator* getPopulator(const std::string& plantType);

protected:
	typedef std::map<std::string, PlantPopulator*> PopulatorStore;

	PopulatorStore mPopulators;
};

}

}

}

#endif /* VEGETATION_H_ */
