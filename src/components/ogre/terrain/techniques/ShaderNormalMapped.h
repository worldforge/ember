/*
 * ShaderNormalMapped.h
 *
 *  Created on: 26 nov 2009
 *      Author: erik
 */

#ifndef EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPED_H_
#define EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPED_H_

#include "Shader.h"

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

class ShaderNormalMapped : public Shader
{
public:
	/**
	 * @brief Ctor.
	 * @param includeShadows If true, shadows will be used.
	 */
	ShaderNormalMapped(bool includeShadows, const TerrainPageGeometry& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow, const TerrainPage& page);
protected:
	virtual ShaderPass* addPass(Ogre::Technique* technique);

};

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESSHADERNORMALMAPPED_H_ */
