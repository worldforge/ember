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
#include "TerrainPageSurfaceCompilerTechniqueShader.h"
#include "TerrainPageSurfaceCompilerTechniqueSimple.h"
#include "../ShaderManager.h"
#include "../EmberOgre.h"


#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include <Ogre.h>

namespace EmberOgre {
namespace Terrain {

TerrainPageSurfaceCompiler::TerrainPageSurfaceCompiler()
{
	selectTechnique();
}


TerrainPageSurfaceCompiler::~TerrainPageSurfaceCompiler()
{
}

void TerrainPageSurfaceCompiler::compileMaterial(Ogre::MaterialPtr material, std::map<int, TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow, TerrainPage& page)
{
	bool result = true;
	try {
		mTechnique->setPage(&page);
		result = mTechnique->compileMaterial(material, terrainPageSurfaces, terrainPageShadow);
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when creating terrain material, falling back to safe technique. Error message: " << ex.what());
		fallback(material, terrainPageSurfaces, terrainPageShadow, page);
	}
	if (!result) {
		S_LOG_FAILURE("Error when creating terrain material, falling back to safe technique.");
		fallback(material, terrainPageSurfaces, terrainPageShadow, page);
	}
}

void TerrainPageSurfaceCompiler::fallback(Ogre::MaterialPtr material, std::map<int, TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow, TerrainPage& page)
{
	mTechnique = std::auto_ptr<TerrainPageSurfaceCompilerTechnique>(new TerrainPageSurfaceCompilerTechniqueSimple());
	mTechnique->setPage(&page);
	mTechnique->compileMaterial(material, terrainPageSurfaces, terrainPageShadow);
}

void TerrainPageSurfaceCompiler::selectTechnique()
{
	std::string preferredTech("");
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("terrain", "preferredtechnique")) {
		preferredTech = static_cast<std::string>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("terrain", "preferredtechnique"));
	}
	
	bool shaderSupport = false;
	const Ogre::RenderSystemCapabilities* caps = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities();
	if (caps->hasCapability(Ogre::RSC_VERTEX_PROGRAM) && (caps->hasCapability(Ogre::RSC_FRAGMENT_PROGRAM)))
	{
		if ((Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_0") &&
			Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("vs_2_0")) ||
			(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("arbfp1") &&
			Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("arbvp1"))
			)
		{
			shaderSupport = true;
		}
	}
	
	ShaderManager::GraphicsLevel graphicsLevel = EmberOgre::getSingleton().getShaderManager()->getGraphicsLevel();
	
	if (preferredTech == "ShaderNormalMapped" && shaderSupport && graphicsLevel >= ShaderManager::LEVEL_HIGH) {
		mTechnique = std::auto_ptr<TerrainPageSurfaceCompilerTechnique>(new TerrainPageSurfaceCompilerTechniqueShaderNormalMapped());
	} else if (preferredTech == "Shader" && shaderSupport && graphicsLevel >= ShaderManager::LEVEL_MEDIUM) {
		mTechnique = std::auto_ptr<TerrainPageSurfaceCompilerTechnique>(new TerrainPageSurfaceCompilerTechniqueShader());
	} else {
		mTechnique = std::auto_ptr<TerrainPageSurfaceCompilerTechnique>(new TerrainPageSurfaceCompilerTechniqueSimple());
	}
// 	
	
//	mTechnique = std::auto_ptr<TerrainPageSurfaceCompilerTechnique>(new TerrainPageSurfaceCompilerTechniqueSimple());
}



}
}
