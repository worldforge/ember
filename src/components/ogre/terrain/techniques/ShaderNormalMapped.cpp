/*
 * ShaderNormalMapped.cpp
 *
 *  Created on: 26 nov 2009
 *      Author: erik
 */

#include "ShaderNormalMapped.h"
#include "ShaderNormalMappedPass.h"

#include <OgrePass.h>
#include <OgreTechnique.h>

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

ShaderNormalMapped::ShaderNormalMapped(bool includeShadows, const TerrainPageGeometry& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow, const TerrainPage& page) :
	Shader::Shader(includeShadows, geometry, terrainPageSurfaces, terrainPageShadow, page)
{
}

ShaderPass* ShaderNormalMapped::addPass(Ogre::Technique* technique)
{
	Ogre::Pass* pass = technique->createPass();
	ShaderPass* shaderPass = new ShaderNormalMappedPass(pass, mPage);
	return shaderPass;
}


}

}

}
