/*
 * Base.h
 *
 *  Created on: 26 nov 2009
 *      Author: erik
 */

#ifndef EMBEROGRETERRAINTECHNIQUESBASE_H_
#define EMBEROGRETERRAINTECHNIQUESBASE_H_

#include "components/ogre/terrain/TerrainPageSurfaceCompiler.h"

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

class Base : public TerrainPageSurfaceCompilerTechnique
{
public:
	Base(const TerrainPageGeometry& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow, const TerrainPage& page);
	virtual ~Base();
protected:
	const TerrainPageGeometry& mGeometry;
	const SurfaceLayerStore& mTerrainPageSurfaces;
	const TerrainPageShadow* mTerrainPageShadow;
	const TerrainPage& mPage;

};

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESBASE_H_ */
