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
#include "Sun.h"

namespace caelum {

const Ogre::String Sun::SUN_MATERIAL_NAME = "CaelumSunMaterial";

Sun::Sun (Ogre::SceneManager *sceneMgr, Ogre::SceneNode *caelumRootNode, const Ogre::String &meshName): mScene(sceneMgr) {
	mSunSphereColour = Ogre::ColourValue::White;
	mSunLightColour = Ogre::ColourValue::White;

	mDiffuseMultiplier = Ogre::ColourValue(1, 1, 0.9);
	mSpecularMultiplier = Ogre::ColourValue(1, 1, 1);
	mAmbientMultiplier = Ogre::ColourValue(0.2, 0.2, 0.2);
	mManageAmbientLight = false;

	mMainLight = sceneMgr->createLight ("CaelumSun");
	mMainLight->setType (Ogre::Light::LT_DIRECTIONAL);
	// HDR power scale, REALLY bright:
	mMainLight->setPowerScale (10);

	createSunMaterial ();

	mSunEntity = sceneMgr->createEntity ("CaelumSun", meshName);
	mSunEntity->setMaterialName (SUN_MATERIAL_NAME);
	mSunEntity->setCastShadows (false);
	mSunEntity->setRenderQueueGroup (CAELUM_RENDER_QUEUE_SUN);
	mSunNode = caelumRootNode->createChildSceneNode ();
	mSunNode->attachObject (mSunEntity);
}

Sun::~Sun () {
	if (mSunNode) {
		mSunNode->detachObject (mSunEntity);
		static_cast<Ogre::SceneNode *>(mSunNode->getParent ())->removeAndDestroyChild (mSunNode->getName ());
		mSunNode = 0;
	}
	if (mSunEntity) {
		mSunEntity->_getManager ()->destroyEntity (mSunEntity);
		mSunEntity = 0;
	}

	destroySunMaterial ();

	if (mMainLight) {
		mMainLight->_getManager ()->destroyLight (mMainLight);
		mMainLight = 0;
	}
}

void Sun::notifyCameraChanged (Ogre::Camera *cam) {
    // This calls setFarRadius
    CameraBoundElement::notifyCameraChanged(cam);

    // Set sun position.
    Ogre::Real sunRadius = -mRadius * Ogre::Math::Tan (Ogre::Degree (0.01));
	mSunNode->setPosition(-mSunDirection * (mRadius + sunRadius));

    // Set sun scaling.
    float factor = 2 - mSunSphereColour.b / 3;
    float scale = factor * (mRadius + sunRadius) * Ogre::Math::Tan (Ogre::Degree (0.01));
    mSunNode->setScale (Ogre::Vector3::UNIT_SCALE * scale);
}

void Sun::setFarRadius (Ogre::Real radius) {
    CameraBoundElement::setFarRadius(radius);
	mRadius = radius;
}

void Sun::update (
        const Ogre::Vector3& sunDirection,
        const Ogre::ColourValue &sunLightColour,
        const Ogre::ColourValue &sunSphereColour)
{
    setSunDirection(sunDirection);
    setSunLightColour(sunLightColour);
    setSunSphereColour(sunSphereColour);
}

Ogre::Vector3 Sun::getSunDirection () const {
	return mSunDirection;
}

void Sun::setSunDirection (const Ogre::Vector3 &dir) {
	mSunDirection = dir;
	if (mMainLight != 0) {
		mMainLight->setDirection (mSunNode->getWorldOrientation() * dir);
	}
}

void Sun::setSunSphereColour (const Ogre::ColourValue &colour) {
	// Store this last colour
	mSunSphereColour = colour;

	// Set sun material colour.
	mSunMaterial->setSelfIllumination (colour);
}

Ogre::ColourValue Sun::getSunSphereColour () {
	return mSunSphereColour;
}

void Sun::setSunLightColour (const Ogre::ColourValue &colour) {
	// Store this last colour
	mSunLightColour = colour;

	// Set light colours.
	mMainLight->setDiffuseColour (colour * mDiffuseMultiplier);
	mMainLight->setSpecularColour (colour * mSpecularMultiplier);
	if (isManagingAmbientLight()) {
		mScene->setAmbientLight(colour * mAmbientMultiplier);
	}
}

Ogre::ColourValue Sun::getSunLightColour () {
	return mSunLightColour;
}

void Sun::setDiffuseMultiplier (const Ogre::ColourValue &diffuse) {
	mDiffuseMultiplier = diffuse;
}

Ogre::ColourValue Sun::getDiffuseMultiplier () {
	return mDiffuseMultiplier;
}

void Sun::setSpecularMultiplier (const Ogre::ColourValue &specular) {
	mSpecularMultiplier = specular;
}

Ogre::ColourValue Sun::getSpecularMultiplier () {
	return mSpecularMultiplier;
}

void Sun::setAmbientMultiplier (const Ogre::ColourValue &ambient) {
	mAmbientMultiplier = ambient;
}

Ogre::ColourValue Sun::getAmbientMultiplier () {
	return mAmbientMultiplier;
}

void Sun::setManageAmbientLight (bool manage) {
	mManageAmbientLight = manage;
}

bool Sun::isManagingAmbientLight () {
	return mManageAmbientLight;
}

void Sun::createSunMaterial () {
	Ogre::MaterialPtr mat;

	LOG ("Generating sun material...");
	if (!Ogre::MaterialManager::getSingleton ().resourceExists (SUN_MATERIAL_NAME)) {
		LOG ("\tMaterial not found; creating...");
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().create (SUN_MATERIAL_NAME, RESOURCE_GROUP_NAME));
		mat->setReceiveShadows (false);
		LOG ("\t\tMaterial [OK]");
		Ogre::Pass *pass = mat->getTechnique (0)->getPass (0);
        pass->setSceneBlending (Ogre::SBT_TRANSPARENT_COLOUR);
		pass->setDepthCheckEnabled (false);
		pass->setDepthWriteEnabled (false);
//		pass->setLightingEnabled (false);
		pass->setFog (true);
		pass->setAmbient (Ogre::ColourValue::Black);
		pass->setDiffuse (Ogre::ColourValue::Black);
		pass->setSpecular (Ogre::ColourValue::Black);
		LOG ("\t\tPass [OK]");
		mat->load ();
		LOG ("\tDONE");
	}
	else {
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().getByName (SUN_MATERIAL_NAME));
	}
	LOG ("DONE");

	mSunMaterial = mat;
}

void Sun::destroySunMaterial () {
	LOG ("Removing sun material...");
	if (Ogre::MaterialManager::getSingleton ().resourceExists (SUN_MATERIAL_NAME)) {
		Ogre::MaterialManager::getSingleton ().remove (SUN_MATERIAL_NAME);
	}
	mSunMaterial.setNull ();
	LOG ("DONE");
}

} // namespace caelum
