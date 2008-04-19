/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2007 Caelum team. See Contributors.txt for details.

Caelum is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Caelum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Caelum. If not, see <http://www.gnu.org/licenses/>.
*/

#include "CaelumPrecompiled.h"
#include "SkyDome.h"
#include "GeometryFactory.h"
#include "CaelumExceptions.h"

namespace caelum {

const Ogre::String SkyDome::SPHERIC_DOME_NAME = "CaelumSphericDome";
const Ogre::String SkyDome::SKY_DOME_MATERIAL_NAME = "CaelumSkyDomeMaterial";

SkyDome::SkyDome (Ogre::SceneManager *sceneMgr, Ogre::SceneNode *caelumRootNode) {
	mMaterial = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().getByName (SKY_DOME_MATERIAL_NAME));
	if (mMaterial.isNull()) {
		throw UnsupportedException (0, std::string("Could not find sky dome material (") + SKY_DOME_MATERIAL_NAME + ").",
				"SkyDome", "SkyDome.cpp", -1);
	}
	mMaterial->load ();
	mShadersEnabled = mMaterial->getBestTechnique()->getPass(0)->isProgrammable();

	sceneMgr->getRenderQueue()->getQueueGroup(CAELUM_RENDER_QUEUE_SKYDOME)->setShadowsEnabled(false);

	GeometryFactory::generateSphericDome (SPHERIC_DOME_NAME, 32);
	Ogre::Entity *ent = sceneMgr->createEntity ("Dome", SPHERIC_DOME_NAME);
	ent->setMaterialName (SKY_DOME_MATERIAL_NAME);
	ent->setRenderQueueGroup (CAELUM_RENDER_QUEUE_SKYDOME);
	ent->setCastShadows (false);

	mNode = caelumRootNode->createChildSceneNode ();
	mNode->attachObject (ent);
}

SkyDome::~SkyDome () {
	if (mNode) {
		// Detach and destroy attached entity.
		Ogre::Entity *ent = static_cast<Ogre::Entity *>(mNode->detachObject ("Dome"));
		ent->_getManager ()->destroyEntity (ent);

		// Destroy the node
		static_cast<Ogre::SceneNode *>(mNode->getParent ())->removeAndDestroyChild (mNode->getName ());
		mNode = 0;
	}
}

void SkyDome::notifyCameraChanged (Ogre::Camera *cam) {
    CameraBoundElement::notifyCameraChanged (cam);
}

void SkyDome::setFarRadius (Ogre::Real radius) {
    CameraBoundElement::setFarRadius(radius);
	mNode->setScale (Ogre::Vector3::UNIT_SCALE * radius);
}

void SkyDome::setSunDirection (Ogre::Vector3 sunDir) {
    float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y);
    elevation = elevation * 0.5 + 0.5;
	Ogre::Pass* pass = mMaterial->getBestTechnique()->getPass(0);
	if(mShadersEnabled)	{
		Ogre::GpuProgramParametersSharedPtr vpParams = pass->getVertexProgramParameters();
		Ogre::GpuProgramParametersSharedPtr fpParams = pass->getFragmentProgramParameters();
		vpParams->setNamedConstant ("sunDirection", sunDir);
		fpParams->setNamedConstant ("offset", elevation);
	} else {
		Ogre::TextureUnitState* gradientsTus = pass->getTextureUnitState(0);
		gradientsTus->setTextureUScroll (elevation);
	}
}

void SkyDome::setLightAbsorption (float absorption) const {
	if(!mShadersEnabled)
		return;

    if (absorption > 1) {
		absorption = 1;
    } else if (absorption < 0) {
		absorption = 0;
    }

	Ogre::GpuProgramParametersSharedPtr vpParams =
            mMaterial->getBestTechnique()->getPass(0)->getVertexProgramParameters();
	vpParams->setNamedConstant ("lightAbsorption", absorption);
}

void SkyDome::setLightScattering (float scattering) const {
	if(!mShadersEnabled)
		return;

    if (scattering <= 0) {
		scattering = 0.00001;
    }

	Ogre::GpuProgramParametersSharedPtr fpParams = 
			mMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
	fpParams->setNamedConstant ("lightInvScattering", 1.0f / scattering);
}

void SkyDome::setAtmosphereHeight (float height) const {
	if(!mShadersEnabled)
		return;

    if (height <= 0) {
		height = 0.00001;
    } else if (height > 1) {
		height = 1;
    }

	Ogre::GpuProgramParametersSharedPtr fpParams = 
			mMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
	fpParams->setNamedConstant ("atmosphereInvHeight", 1.0f / height);
}

void SkyDome::setSkyGradientsImage (const Ogre::String& gradients) {
    Ogre::TextureUnitState* gradientsTus =
            mMaterial->getTechnique (0)->getPass (0)->getTextureUnitState(0);

	gradientsTus->setTextureAddressingMode (Ogre::TextureUnitState::TAM_CLAMP);

    // Dagon and Eihort compatibility
    #if OGRE_VERSION < ((1 << 16) | (3 << 8))
        gradientsTus->setTextureName (gradients, Ogre::TEX_TYPE_2D, -1, true);
    #else
        gradientsTus->setTextureName (gradients, Ogre::TEX_TYPE_2D);
        gradientsTus->setIsAlpha (true);
    #endif
}

void SkyDome::setAtmosphereDepthImage (const Ogre::String& atmosphereDepth) {
	if(!mShadersEnabled)
		return;

    Ogre::TextureUnitState* atmosphereTus =
            mMaterial->getTechnique (0)->getPass (0)->getTextureUnitState(1);

	atmosphereTus->setTextureName (atmosphereDepth, Ogre::TEX_TYPE_1D);
	atmosphereTus->setTextureAddressingMode (Ogre::TextureUnitState::TAM_CLAMP, Ogre::TextureUnitState::TAM_WRAP, Ogre::TextureUnitState::TAM_WRAP);
}

bool SkyDome::getHazeEnabled () const {
    return mHazeEnabled;
}

void SkyDome::setHazeEnabled (bool value) {
    if (mHazeEnabled == value) {
        return;
    }
    mHazeEnabled = value;

	if(!mShadersEnabled)
		return;

	Ogre::Pass *pass = mMaterial->getTechnique (0)->getPass (0);
    if (value) {
        pass->setFragmentProgram("CaelumSkyDomeFP");
    } else {
        pass->setFragmentProgram("CaelumSkyDomeFP_NoHaze");
    }
    Ogre::GpuProgramParametersSharedPtr params = pass->getFragmentProgramParameters();
    params->setIgnoreMissingParams(true);
	params->setNamedConstant ("offset", 0.0f);
	params->setNamedAutoConstant ("hazeColour", Ogre::GpuProgramParameters::ACT_FOG_COLOUR);
}

} // namespace caelum
