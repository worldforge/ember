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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CompilerTechniqueProvider.h"
#include "Shader.h"
#include "Simple.h"
#include "OnePixelMaterialGenerator.h"

#include "components/ogre/ShaderManager.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#include <OgreRoot.h>
#include <OgreGpuProgramManager.h>
#include <OgreTextureManager.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

namespace Techniques
{
CompilerTechniqueProvider::CompilerTechniqueProvider(ShaderManager& shaderManager, Ogre::SceneManager& sceneManager) :
		mShaderManager(shaderManager), mSceneManager(sceneManager), mOnePixelMaterialGenerator(new OnePixelMaterialGenerator())
{
	//Our shaders use the one pixel normal texture whenever there's no existing normal map, so we need to create it.
	const std::string onePixelMaterialName("dynamic/onepixel");
	if (Ogre::TextureManager::getSingleton().resourceExists(onePixelMaterialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)) {
		S_LOG_WARNING("Texture '" << onePixelMaterialName << "' already existed when CompilerTechniqueProvider was created; this should not be the case.");
	} else {
		Ogre::TextureManager::getSingleton().createManual(onePixelMaterialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 1, 1, 0, Ogre::PF_R8G8B8, Ogre::TU_DEFAULT, mOnePixelMaterialGenerator);
	}
}

CompilerTechniqueProvider::~CompilerTechniqueProvider()
{
	Ogre::TextureManager::getSingleton().remove("dynamic/onepixel", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	delete mOnePixelMaterialGenerator;
}

TerrainPageSurfaceCompilerTechnique* CompilerTechniqueProvider::createTechnique(const TerrainPageGeometryPtr& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow) const
{
	std::string preferredTech;
	if (EmberServices::getSingleton().getConfigService().itemExists("terrain", "preferredtechnique")) {
		preferredTech = static_cast<std::string>(EmberServices::getSingleton().getConfigService().getValue("terrain", "preferredtechnique"));
	}

	bool shaderSupport = true;
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
	}
//	if ((preferredTech == "Shader" || useNormalMapping) && shaderSupport && graphicsLevel >= ShaderManager::LEVEL_MEDIUM) {
		//Use shader tech without shadows
		return new Techniques::Shader(false, geometry, terrainPageSurfaces, terrainPageShadow, mSceneManager, false);
//	} else {
//		return new Techniques::Simple(geometry, terrainPageSurfaces, terrainPageShadow);
//	}
}

}
}
}
}
