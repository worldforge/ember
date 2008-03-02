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

SkyDome::SkyDome (Ogre::SceneManager *sceneMgr) {
	createSkyDomeMaterial ();

	GeometryFactory::generateSphericDome (SPHERIC_DOME_NAME, 32);
	Ogre::Entity *ent = sceneMgr->createEntity ("Dome", SPHERIC_DOME_NAME);
	ent->setMaterialName (SKY_DOME_MATERIAL_NAME);
	ent->setRenderQueueGroup (CAELUM_RENDER_QUEUE_SKYDOME);
	ent->setCastShadows (false);

	mNode = sceneMgr->getRootSceneNode ()->createChildSceneNode ();
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

	destroySkyDomeMaterial ();
}

void SkyDome::notifyCameraChanged (Ogre::Camera *cam) {
    CameraBoundElement::notifyCameraChanged (cam);
	mNode->setPosition (cam->getRealPosition ());
}

void SkyDome::setFarRadius (Ogre::Real radius) {
    CameraBoundElement::setFarRadius(radius);
	mNode->setScale (Ogre::Vector3::UNIT_SCALE * radius);
}

void SkyDome::setSunDirection (Ogre::Vector3 sunDir) {
    float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y);
    elevation = elevation * 0.5 + 0.5;

	Ogre::GpuProgramParametersSharedPtr vpParams = 
			mMaterial->getBestTechnique()->getPass(0)->getVertexProgramParameters();
	Ogre::GpuProgramParametersSharedPtr fpParams = 
			mMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
    Ogre::TextureUnitState* gradientsTus = 
			mMaterial->getBestTechnique()->getPass(0)->getTextureUnitState(0);

	vpParams->setNamedConstant ("sunDirection", sunDir);
    fpParams->setNamedConstant ("offset", elevation);
    gradientsTus->setTextureUScroll (elevation);
}

void SkyDome::setLightAbsorption (float absorption) const {
	if (absorption > 1)
		absorption = 1;
	else if (absorption < 0)
		absorption = 0;

	Ogre::GpuProgramParametersSharedPtr vpParams = 
			mMaterial->getBestTechnique()->getPass(0)->getVertexProgramParameters();
	vpParams->setNamedConstant ("lightAbsorption", absorption);
}

void SkyDome::setLightScattering (float scattering) const {
	if (scattering <= 0)
		scattering = 0.00001;

	Ogre::GpuProgramParametersSharedPtr fpParams = 
			mMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
	fpParams->setNamedConstant ("lightInvScattering", 1.0f / scattering);
}

void SkyDome::setAtmosphereHeight (float height) const {
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
    Ogre::TextureUnitState* atmosphereTus =
            mMaterial->getTechnique (0)->getPass (0)->getTextureUnitState(1);

	atmosphereTus->setTextureName (atmosphereDepth, Ogre::TEX_TYPE_1D);
	atmosphereTus->setTextureAddressingMode (Ogre::TextureUnitState::TAM_CLAMP, Ogre::TextureUnitState::TAM_WRAP, Ogre::TextureUnitState::TAM_WRAP);
}

void SkyDome::createSkyDomeMaterial () {
	// Check shader support.
    // It would be nice to check supported profiles in detail.
	if (!Ogre::Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_FRAGMENT_PROGRAM)) {
		throw new UnsupportedException (0, "The card doesn't support fragment programs for the sky dome material.", "SkyDome", "SkyDome.cpp", -1);
	}
	if (!Ogre::Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_VERTEX_PROGRAM)) {
		throw new UnsupportedException (0, "The card doesn't support vertex programs for the sky dome material.", "SkyDome", "SkyDome.cpp", -1);
	}

	LOG ("Generating sky dome material...");
	if (!Ogre::MaterialManager::getSingleton ().resourceExists (SKY_DOME_MATERIAL_NAME)) {
		LOG ("\tMaterial not found; creating...");
		mMaterial = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().create (SKY_DOME_MATERIAL_NAME, RESOURCE_GROUP_NAME));
		mMaterial->setReceiveShadows (false);
		LOG ("\t\tMaterial [OK]");

		Ogre::Pass *pass = mMaterial->getTechnique (0)->getPass (0);
		pass->setSceneBlending (Ogre::SBT_TRANSPARENT_ALPHA);
		pass->setDepthCheckEnabled (false);
		pass->setDepthWriteEnabled (false);
		pass->setLightingEnabled (false);
		pass->setFog (true);

        // Bind fragment program.
		if (Ogre::Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_FRAGMENT_PROGRAM)) {
			Ogre::HighLevelGpuProgramPtr fp = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram ("SkyDomeFP", RESOURCE_GROUP_NAME, "cg", Ogre::GPT_FRAGMENT_PROGRAM);
			fp->setSourceFile ("CaelumShaders.cg");
			fp->setParameter ("entry_point", "SkyDome_fp");
			fp->setParameter ("profiles", "ps_2_0 arbfp1");
			pass->setFragmentProgram ("SkyDomeFP");
			Ogre::GpuProgramParametersSharedPtr parameters = pass->getFragmentProgramParameters();
            parameters->setIgnoreMissingParams(true);
			parameters->setNamedConstant ("offset", 0.0f);
			parameters->setNamedAutoConstant ("hazeColour", Ogre::GpuProgramParameters::ACT_FOG_COLOUR);
		}

        // Bind vertex program.
		if (Ogre::Root::getSingleton ().getRenderSystem ()->getCapabilities ()->hasCapability (Ogre::RSC_VERTEX_PROGRAM)) {
			Ogre::HighLevelGpuProgramPtr vp = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram ("SkyDomeVP", RESOURCE_GROUP_NAME, "cg", Ogre::GPT_VERTEX_PROGRAM);
			vp->setSourceFile ("CaelumShaders.cg");
			vp->setParameter ("entry_point", "SkyDome_vp");
			vp->setParameter ("profiles", "vs_2_0 arbvp1");
			pass->setVertexProgram ("SkyDomeVP");
			Ogre::GpuProgramParametersSharedPtr parameters = pass->getVertexProgramParameters();
            parameters->setIgnoreMissingParams(true);
			parameters->setNamedAutoConstant ("worldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
			parameters->setNamedConstant ("sunDirection", Ogre::Vector3 (1, 0, 0));
		}
		LOG ("\t\tPass [OK]");

        // Create first texture unit; gradients image.
		pass->createTextureUnitState ();
        setSkyGradientsImage("EarthClearSky2.png");
		LOG ("\t\tTextureUnit - Sky gradient [OK]");

        // Create second texture unit; atmosphere depth image.
		pass->createTextureUnitState ();
        setAtmosphereDepthImage("AtmosphereDepth.png");
		LOG ("\t\tTextureUnit - Atmosphere depth [OK]");

		mMaterial->load ();
		LOG ("\tDONE");
	} else {
		mMaterial = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().getByName (SKY_DOME_MATERIAL_NAME));
	}
	LOG ("DONE");
}

void SkyDome::destroySkyDomeMaterial () {
	LOG ("Removing sky dome material...");
	if ((Ogre::HighLevelGpuProgramManager::getSingletonPtr()->resourceExists("SkyDomeFP"))) {
		LOG ("Removing sky dome SkyDomeFP...");
		Ogre::HighLevelGpuProgramManager::getSingletonPtr()->remove("SkyDomeFP");
	}

	if ((Ogre::HighLevelGpuProgramManager::getSingletonPtr()->resourceExists("SkyDomeVP"))) {
		LOG ("Removing sky dome SkyDomeVP...");
		Ogre::HighLevelGpuProgramManager::getSingletonPtr()->remove("SkyDomeVP");
	}

	if (!Ogre::MaterialManager::getSingleton ().resourceExists (SKY_DOME_MATERIAL_NAME)) {
		Ogre::MaterialManager::getSingleton ().remove (SKY_DOME_MATERIAL_NAME);
	}
	mMaterial.setNull ();
	LOG ("DONE");
}

} // namespace caelum
