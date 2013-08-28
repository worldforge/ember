/*
 Copyright (C) 2010 erik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CompilerTechniqueProvider.h"
#include "Shader.h"
#include "Simple.h"

#include "components/ogre/ShaderManager.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#include <OgreRoot.h>
#include <OgreGpuProgramManager.h>
#include <OgreRenderSystem.h>
#include <OgreRenderSystemCapabilities.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

namespace Techniques
{
CompilerTechniqueProvider::CompilerTechniqueProvider(ShaderManager& shaderManager, Ogre::SceneManager& sceneManager)
: mShaderManager(shaderManager), mSceneManager(sceneManager)
{

}

TerrainPageSurfaceCompilerTechnique* CompilerTechniqueProvider::createTechnique(const TerrainPageGeometryPtr& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow) const
{
	std::string preferredTech("");
	if (EmberServices::getSingleton().getConfigService().itemExists("terrain", "preferredtechnique")) {
		preferredTech = static_cast<std::string> (EmberServices::getSingleton().getConfigService().getValue("terrain", "preferredtechnique"));
	}

	bool shaderSupport = false;
	const Ogre::RenderSystemCapabilities* caps = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities();
	if (caps->hasCapability(Ogre::RSC_VERTEX_PROGRAM) && (caps->hasCapability(Ogre::RSC_FRAGMENT_PROGRAM))) {
		if ((Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_0") && Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("vs_2_0")) || (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("arbfp1") && Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("arbvp1"))) {
			shaderSupport = true;
		}
	}

	ShaderManager::GraphicsLevel graphicsLevel = mShaderManager.getGraphicsLevel();

	bool useNormalMapping = (preferredTech == "ShaderNormalMapped");
	if ((useNormalMapping || preferredTech == "Shader") && shaderSupport && graphicsLevel >= ShaderManager::LEVEL_HIGH) {
		//Use shader tech with shadows
		return new Techniques::Shader(true, geometry, terrainPageSurfaces, terrainPageShadow, mSceneManager, useNormalMapping);
	} else if (preferredTech == "Shader" && shaderSupport && graphicsLevel >= ShaderManager::LEVEL_MEDIUM) {
		//Use shader tech without shadows
		return new Techniques::Shader(false, geometry, terrainPageSurfaces, terrainPageShadow, mSceneManager, useNormalMapping);
	} else {
		return new Techniques::Simple(geometry, terrainPageSurfaces, terrainPageShadow);
	}
}

}
}
}
}
