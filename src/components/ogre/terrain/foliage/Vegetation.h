/*
 * Vegetation.h
 *
 *  Created on: 11 mar 2010
 *      Author: Erik Ogenvik
 */

#ifndef VEGETATION_H_
#define VEGETATION_H_

#include "components/ogre/terrain/Types.h"
#include "PlantPopulator.h"
#include <cstdlib>
#include <map>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{
struct TerrainFoliageDefinition;
class PlantAreaQueryResult;


namespace Foliage
{

class Vegetation
{
public:
	Vegetation() = default;
	virtual ~Vegetation();

	void createPopulator(const TerrainFoliageDefinition& foliageDef, unsigned int surfaceLayerIndex);

	void populate(const std::string& plantType, PlantAreaQueryResult& result, SegmentRefPtr segmentRef, const WFMath::AxisBox<2>& area);

	PlantPopulator* getPopulator(const std::string& plantType);

protected:

	std::map<std::string, std::unique_ptr<PlantPopulator>> mPopulators;
};

}

}

}

}

#endif /* VEGETATION_H_ */
