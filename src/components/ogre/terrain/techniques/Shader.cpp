/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "Shader.h"
#include "ShaderPass.h"
#include "components/ogre/terrain/TerrainPageSurfaceLayer.h"
#include <OgrePass.h>
#include <OgreTechnique.h>

#include <boost/shared_ptr.hpp>

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

Shader::Shader(bool includeShadows, const TerrainPageGeometryPtr& mGeometry, const SurfaceLayerStore& mTerrainPageSurfaces, const TerrainPageShadow* terrainPageShadow) :
	mIncludeShadows(includeShadows), Base::Base(mGeometry, mTerrainPageSurfaces, terrainPageShadow)
{
}

Shader::~Shader()
{
	for (PassStore::iterator I = mPasses.begin(); I != mPasses.end(); ++I) {
		delete *I;
	}
}

void Shader::reset()
{
	for (PassStore::iterator I = mPasses.begin(); I != mPasses.end(); ++I) {
		delete *I;
	}
	mPasses.clear();
}

bool Shader::prepareMaterial()
{
	reset();
	ShaderPass* shaderPass = addPass();
	if (shaderPass) {
		if (mIncludeShadows) {
			shaderPass->addShadowLayer(mTerrainPageShadow);
			shaderPass->addShadowLayer(mTerrainPageShadow);
			shaderPass->addShadowLayer(mTerrainPageShadow);
			shaderPass->addShadowLayer(mTerrainPageShadow);
			shaderPass->addShadowLayer(mTerrainPageShadow);
		}
		for (SurfaceLayerStore::const_iterator I = mTerrainPageSurfaces.begin(); I != mTerrainPageSurfaces.end(); ++I) {
			const TerrainPageSurfaceLayer* surfaceLayer = I->second;
			if (shaderPass->hasRoomForLayer(surfaceLayer)) {
				if (I == mTerrainPageSurfaces.begin()) {
					shaderPass->setBaseLayer(surfaceLayer);
				} else {
					if (surfaceLayer->intersects(*mGeometry)) {
						shaderPass->addLayer(*mGeometry, surfaceLayer);
					}
				}
			} else {
				///TODO: handle new pass
			}
		}
	}
	//We don't need the geometry any more, so we'll release it as soon as we can.
	mGeometry.reset();
}

bool Shader::compileMaterial(Ogre::MaterialPtr material)
{
	material->removeAllTechniques();
	Ogre::Technique* technique = material->createTechnique();

	///The normal, shadowed, shaders have clones with the suffix "/NoShadows" which will skip the shadows.
	std::string materialSuffix = "";
	if (!mIncludeShadows) {
		materialSuffix = "/NoShadows";
	}
	for (PassStore::iterator I = mPasses.begin(); I != mPasses.end(); ++I) {
		ShaderPass* shaderPass(*I);
		Ogre::Pass* pass = technique->createPass();
		if (!shaderPass->finalize(*pass, mIncludeShadows, materialSuffix)) {
			return false;
		}
	}

	///Now also add a "Low" technique, for use in the compass etc.
	technique = material->createTechnique();
	technique->setSchemeName("Low");

	for (PassStore::iterator I = mPasses.begin(); I != mPasses.end(); ++I) {
		ShaderPass* shaderPass(*I);
		Ogre::Pass* pass = technique->createPass();
		if (!shaderPass->finalize(*pass, false, "/Simple")) {
			return false;
		}
	}

	// add lighting pass
	/*
	 Ogre::Pass* pass = technique->createPass();
	 pass->setSceneBlending(Ogre::SBT_MODULATE);
	 pass->setVertexProgram("splat_light_vp");
	 pass->setFragmentProgram("splat_light_fp");
	 Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
	 textureUnitState->setContentType(Ogre::TextureUnitState::CONTENT_SHADOW);
	 */

	///we need to load it before we can see how many techniques are supported
	material->load();
	if (material->getNumSupportedTechniques() == 0) {
		S_LOG_WARNING("The material '" << material->getName() << "' has no supported techniques. The reason for this is: \n" << material->getUnsupportedTechniquesExplanation());
		return false;
	}
	return true;
	// 	if (terrainPageShadow) {
	// 		addShadow(technique, terrainPageShadow);
	// 	}
}

ShaderPass* Shader::addPass()
{
	ShaderPass* shaderPass(new ShaderPass(mPage));
	mPasses.push_back(shaderPass);
	return shaderPass;
}

}

}

}
