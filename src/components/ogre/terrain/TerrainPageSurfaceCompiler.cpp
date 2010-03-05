//
// C++ Implementation: TerrainPageSurfaceCompiler
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainPageSurfaceCompiler.h"
#include "TerrainPageShadow.h"
#include "TerrainPageSurfaceLayer.h"
#include "techniques/Shader.h"
#include "techniques/ShaderNormalMapped.h"
#include "techniques/Simple.h"
#include "TerrainPageGeometry.h"
#include "../ShaderManager.h"
#include "../EmberOgre.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include <Ogre.h>

namespace EmberOgre
{
namespace Terrain
{

TerrainPageSurfaceCompiler::TerrainPageSurfaceCompiler()
{
}

TerrainPageSurfaceCompiler::~TerrainPageSurfaceCompiler()
{
}

TerrainPageSurfaceCompilationInstance* TerrainPageSurfaceCompiler::createCompilationInstance(const TerrainPageGeometryPtr& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow) const
{
	return new TerrainPageSurfaceCompilationInstance(selectTechnique(geometry, terrainPageSurfaces, terrainPageShadow));

}

//void TerrainPageSurfaceCompiler::compileMaterial(const TerrainPageGeometry& geometry, Ogre::MaterialPtr material, const SurfaceLayerStore& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow, const TerrainPage& page)
//{
//	bool result = true;
//	try {
//		mTechnique->setPage(&page);
//		result = mTechnique->compileMaterial(geometry, material, terrainPageSurfaces, terrainPageShadow);
//	} catch (const std::exception& ex) {
//		S_LOG_FAILURE("Error when creating terrain material, falling back to safe technique." << ex);
//		fallback(geometry, material, terrainPageSurfaces, terrainPageShadow, page);
//	}
//	if (!result) {
//		S_LOG_FAILURE("Error when creating terrain material, falling back to safe technique.");
//		fallback(geometry, material, terrainPageSurfaces, terrainPageShadow, page);
//	}
//}

//void TerrainPageSurfaceCompiler::fallback(const TerrainPageGeometry& geometry, Ogre::MaterialPtr material, std::map<int, const TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow, const TerrainPage& page)
//{
//	mTechnique = std::auto_ptr<TerrainPageSurfaceCompilerTechnique>(new TerrainPageSurfaceCompilerTechniqueSimple());
//	mTechnique->setPage(&page);
//	mTechnique->compileMaterial(geometry, material, terrainPageSurfaces, terrainPageShadow);
//}

TerrainPageSurfaceCompilerTechnique* TerrainPageSurfaceCompiler::selectTechnique(const TerrainPageGeometryPtr& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow) const
{
	std::string preferredTech("");
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("terrain", "preferredtechnique")) {
		preferredTech = static_cast<std::string> (Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("terrain", "preferredtechnique"));
	}

	bool shaderSupport = false;
	const Ogre::RenderSystemCapabilities* caps = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities();
	if (caps->hasCapability(Ogre::RSC_VERTEX_PROGRAM) && (caps->hasCapability(Ogre::RSC_FRAGMENT_PROGRAM))) {
		if ((Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_0") && Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("vs_2_0")) || (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("arbfp1") && Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("arbvp1"))) {
			shaderSupport = true;
		}
	}

	ShaderManager::GraphicsLevel graphicsLevel = EmberOgre::getSingleton().getShaderManager()->getGraphicsLevel();

	if (preferredTech == "ShaderNormalMapped" && shaderSupport && graphicsLevel >= ShaderManager::LEVEL_HIGH) {
		///Use normal mapped shader tech with shadows
		return new Techniques::ShaderNormalMapped(true, geometry, terrainPageSurfaces, terrainPageShadow);
	} else if (preferredTech == "ShaderNormalMapped" && shaderSupport && graphicsLevel >= ShaderManager::LEVEL_MEDIUM) {
		///Use normal mapped shader tech without shadows
		return new Techniques::ShaderNormalMapped(false, geometry, terrainPageSurfaces, terrainPageShadow);
	} else if (preferredTech == "Shader" && shaderSupport && graphicsLevel >= ShaderManager::LEVEL_HIGH) {
		///Use shader tech with shadows
		return new Techniques::Shader(true, geometry, terrainPageSurfaces, terrainPageShadow);
	} else if (preferredTech == "Shader" && shaderSupport && graphicsLevel >= ShaderManager::LEVEL_MEDIUM) {
		///Use shader tech without shadows
		return new Techniques::Shader(false, geometry, terrainPageSurfaces, terrainPageShadow);
	} else {
		return new Techniques::Simple(geometry, terrainPageSurfaces, terrainPageShadow);
	}
	//

	//	mTechnique = std::auto_ptr<TerrainPageSurfaceCompilerTechnique>(new TerrainPageSurfaceCompilerTechniqueSimple());
}

TerrainPageSurfaceCompilationInstance::TerrainPageSurfaceCompilationInstance(TerrainPageSurfaceCompilerTechnique* technique)
: mTechnique(technique)
{

}

TerrainPageSurfaceCompilationInstance::~TerrainPageSurfaceCompilationInstance()
{
	delete mTechnique;
}

void TerrainPageSurfaceCompilationInstance::prepare()
{
	mTechnique->prepareMaterial();
}

void TerrainPageSurfaceCompilationInstance::compile(Ogre::MaterialPtr material)
{
	mTechnique->compileMaterial(material);
}

}
}
