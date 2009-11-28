/*
 * Shader.cpp
 *
 *  Created on: 26 nov 2009
 *      Author: erik
 */

#include "Shader.h"
#include "ShaderPass.h"
#include "components/ogre/terrain/TerrainPageSurfaceLayer.h"
#include <OgrePass.h>
#include <OgreTechnique.h>

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

Shader::Shader(bool includeShadows, const TerrainPageGeometry& mGeometry, const SurfaceLayerStore& mTerrainPageSurfaces, const TerrainPageShadow* terrainPageShadow, const TerrainPage& page) :
	mIncludeShadows(includeShadows), Base::Base(mGeometry, mTerrainPageSurfaces, terrainPageShadow, page)
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

}

bool Shader::compileMaterial(Ogre::MaterialPtr material)
{
	reset();
	material->removeAllTechniques();
	Ogre::Technique* technique = material->createTechnique();

	ShaderPass* shaderPass = addPass(technique);
	if (shaderPass) {
		if (mIncludeShadows) {
			shaderPass->addShadowLayer(mTerrainPageShadow);
			shaderPass->addShadowLayer(mTerrainPageShadow);
			shaderPass->addShadowLayer(mTerrainPageShadow);
			shaderPass->addShadowLayer(mTerrainPageShadow);
			shaderPass->addShadowLayer(mTerrainPageShadow);
			for (SurfaceLayerStore::const_iterator I = mTerrainPageSurfaces.begin(); I != mTerrainPageSurfaces.end(); ++I) {
				const TerrainPageSurfaceLayer* surfaceLayer = I->second;
				if (shaderPass->hasRoomForLayer(surfaceLayer)) {
					if (I == mTerrainPageSurfaces.begin()) {
						shaderPass->setBaseLayer(surfaceLayer);
					} else {
						if (surfaceLayer->intersects(mGeometry)) {
							shaderPass->addLayer(mGeometry, surfaceLayer);
						}
					}
				} else {
					///TODO: handle new pass
				}
			}
			if (!shaderPass->finalize(true, "")) {
				return false;
			}
		} else {
			///We're not using shadows
			for (SurfaceLayerStore::const_iterator I = mTerrainPageSurfaces.begin(); I != mTerrainPageSurfaces.end(); ++I) {
				const TerrainPageSurfaceLayer* surfaceLayer = I->second;
				if (shaderPass->hasRoomForLayer(surfaceLayer)) {
					if (I == mTerrainPageSurfaces.begin()) {
						shaderPass->setBaseLayer(surfaceLayer);
					} else {
						if (surfaceLayer->getCoverageImage()) {
							if (surfaceLayer->intersects(mGeometry)) {
								shaderPass->addLayer(mGeometry, surfaceLayer);
							}
						}
					}
				} else {
					///TODO: handle new pass
				}
			}
			///The normal, shadowed, shaders have clones with the suffix "/NoShadows" which will skip the shadows.
			if (!shaderPass->finalize(false, "/NoShadows")) {
				return false;
			}
		}
	}

	///Now also add a "Low" technique, for use in the compass etc.
	technique = material->createTechnique();
	technique->setSchemeName("Low");

	shaderPass = addPass(technique);
	if (shaderPass) {
		for (SurfaceLayerStore::const_iterator I = mTerrainPageSurfaces.begin(); I != mTerrainPageSurfaces.end(); ++I) {
			const TerrainPageSurfaceLayer* surfaceLayer = I->second;
			if (shaderPass->hasRoomForLayer(surfaceLayer)) {
				if (I == mTerrainPageSurfaces.begin()) {
					shaderPass->setBaseLayer(surfaceLayer);
				} else {
					if (surfaceLayer->getCoverageImage()) {
						if (surfaceLayer->intersects(mGeometry)) {
							shaderPass->addLayer(mGeometry, surfaceLayer);
						}
					}
				}
			} else {
				///TODO: handle new pass
			}
		}
		///The normal, shadowed, shaders have clones with the suffix "/Simple" which will skip the shadows and the fog and restrict it to only one light source.
		if (!shaderPass->finalize(false, "/Simple")) {
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
	reset();
	if (material->getNumSupportedTechniques() == 0) {
		S_LOG_WARNING("The material '" << material->getName() << "' has no supported techniques. The reason for this is: \n" << material->getUnsupportedTechniquesExplanation());
		return false;
	}
	return true;
	// 	if (terrainPageShadow) {
	// 		addShadow(technique, terrainPageShadow);
	// 	}
}

ShaderPass* Shader::addPass(Ogre::Technique* technique)
{
	Ogre::Pass* pass = technique->createPass();
	ShaderPass* shaderPass(new ShaderPass(pass, mPage));
	mPasses.push_back(shaderPass);
	return shaderPass;
}

}

}

}
