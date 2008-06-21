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

//========================================================================================================================
// Base Sun
//========================================================================================================================

BaseSun::BaseSun (Ogre::SceneManager *sceneMgr, Ogre::SceneNode *caelumRootNode): mScene(sceneMgr) {
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

	sceneMgr->getRenderQueue()->getQueueGroup(CAELUM_RENDER_QUEUE_SUN)->setShadowsEnabled(false);

	mSunNode = caelumRootNode->createChildSceneNode ();
}

BaseSun::~BaseSun () {
	if (mSunNode) {
		static_cast<Ogre::SceneNode *>(mSunNode->getParent ())->removeAndDestroyChild (mSunNode->getName ());
		mSunNode = 0;
	}

	if (mMainLight) {
		mMainLight->_getManager ()->destroyLight (mMainLight);
		mMainLight = 0;
	}
}

void BaseSun::setFarRadius (Ogre::Real radius) {
    CameraBoundElement::setFarRadius(radius);
	mRadius = radius;
}

void BaseSun::update (
        const Ogre::Vector3& sunDirection,
        const Ogre::ColourValue &sunLightColour,
        const Ogre::ColourValue &sunSphereColour)
{
    setSunDirection(sunDirection);
    setSunLightColour(sunLightColour);
    setSunSphereColour(sunSphereColour);
}

const Ogre::Vector3& BaseSun::getSunDirection () const {
	return mSunDirection;
}

void BaseSun::setSunDirection (const Ogre::Vector3 &dir) {
	mSunDirection = dir;
	if (mMainLight != 0) {
		mMainLight->setDirection (mSunNode->_getDerivedOrientation() * dir);
	}
}

void BaseSun::setSunSphereColour (const Ogre::ColourValue &colour) {
	// Store this last colour
	mSunSphereColour = colour;
}

Ogre::ColourValue BaseSun::getSunSphereColour () const {
	return mSunSphereColour;
}

void BaseSun::setSunLightColour (const Ogre::ColourValue &colour) {
	// Store this last colour
	mSunLightColour = colour;
	// Apply change
	setMainLightColour(colour);
}

void BaseSun::setMainLightColour (const Ogre::ColourValue &colour) {
	// Set light colours.
	mMainLight->setDiffuseColour (colour * mDiffuseMultiplier);
	mMainLight->setSpecularColour (colour * mSpecularMultiplier);
	if (isManagingAmbientLight()) {
		mScene->setAmbientLight(colour * mAmbientMultiplier);
	}
}

Ogre::ColourValue BaseSun::getSunLightColour () const {
	return mSunLightColour;
}

void BaseSun::setDiffuseMultiplier (const Ogre::ColourValue &diffuse) {
	mDiffuseMultiplier = diffuse;
}

Ogre::ColourValue BaseSun::getDiffuseMultiplier () const {
	return mDiffuseMultiplier;
}

void BaseSun::setSpecularMultiplier (const Ogre::ColourValue &specular) {
	mSpecularMultiplier = specular;
}

Ogre::ColourValue BaseSun::getSpecularMultiplier () const {
	return mSpecularMultiplier;
}

void BaseSun::setAmbientMultiplier (const Ogre::ColourValue &ambient) {
	mAmbientMultiplier = ambient;
}

Ogre::ColourValue BaseSun::getAmbientMultiplier () const {
	return mAmbientMultiplier;
}

void BaseSun::setManageAmbientLight (bool manage) {
	mManageAmbientLight = manage;
}

bool BaseSun::isManagingAmbientLight () const {
	return mManageAmbientLight;
}

Ogre::Light* BaseSun::getMainLight() const {
	return mMainLight;
}

//========================================================================================================================
// Sphere Sun - deprecated, use Sprite Sun instead
//========================================================================================================================

const Ogre::String SphereSun::SUN_MATERIAL_NAME = "CaelumSphereSun";

SphereSun::SphereSun (Ogre::SceneManager *sceneMgr, Ogre::SceneNode *caelumRootNode, const Ogre::String &meshName) : BaseSun(sceneMgr, caelumRootNode) {
    mSunMaterial = Ogre::MaterialManager::getSingletonPtr()->getByName(SUN_MATERIAL_NAME);
	mSunMaterial = mSunMaterial->clone(SUN_MATERIAL_NAME + Ogre::StringConverter::toString((size_t)this));
    mSunMaterial->load();
	mSunEntity = sceneMgr->createEntity ("CaelumSun", meshName);
	mSunEntity->setMaterialName (mSunMaterial->getName());
	mSunEntity->setCastShadows (false);
	mSunEntity->setRenderQueueGroup (CAELUM_RENDER_QUEUE_SUN);
	mSunNode->attachObject (mSunEntity);
}

SphereSun::~SphereSun () {
	if (mSunNode) {
		mSunNode->detachObject (mSunEntity);
	}
	if (mSunEntity) {
		mSunEntity->_getManager ()->destroyEntity (mSunEntity);
		mSunEntity = 0;
	}
	Ogre::MaterialManager::getSingletonPtr()->remove(mSunMaterial->getHandle());
}

void SphereSun::setSunSphereColour (const Ogre::ColourValue &colour) {
	BaseSun::setSunSphereColour(colour);

	// Set sun material colour.
	mSunMaterial->setSelfIllumination (colour);
}

void SphereSun::notifyCameraChanged (Ogre::Camera *cam) {
    // This calls setFarRadius
    CameraBoundElement::notifyCameraChanged(cam);

    // Set sun position.
    Ogre::Real sunDistance = mRadius - mRadius * Ogre::Math::Tan (Ogre::Degree (0.01));
	mSunNode->setPosition(-mSunDirection * sunDistance);

    // Set sun scaling  in [1.6(6) ~ 2.0] range.
    float factor = 2 - mSunSphereColour.b / 3;
    float scale = factor * sunDistance * Ogre::Math::Tan (Ogre::Degree (0.01));
    mSunNode->setScale (Ogre::Vector3::UNIT_SCALE * scale);
}

//========================================================================================================================
// Sprite Sun
//========================================================================================================================

const Ogre::String SpriteSun::SUN_MATERIAL_NAME = "CaelumSpriteSun";

SpriteSun::SpriteSun (	Ogre::SceneManager *sceneMgr,
						Ogre::SceneNode *caelumRootNode, 
						const Ogre::String &sunTextureName,
						const Ogre::Degree& sunTextureAngularSize) 
	: BaseSun(sceneMgr, caelumRootNode)
	, mSunTextureAngularSize(sunTextureAngularSize)
{
    mSunMaterial = Ogre::MaterialManager::getSingletonPtr()->getByName(SUN_MATERIAL_NAME);
	mSunMaterial = mSunMaterial->clone(SUN_MATERIAL_NAME + Ogre::StringConverter::toString((size_t)this));
    mSunMaterial->load();
    assert(!mSunMaterial.isNull());
    setSunTexture(sunTextureName);

	mSunBillboardSet = sceneMgr->createBillboardSet("CaelumSpriteSun", 2);
	mSunBillboardSet->setMaterialName (mSunMaterial->getName());
	mSunBillboardSet->setCastShadows (false);
	mSunBillboardSet->setRenderQueueGroup (CAELUM_RENDER_QUEUE_SUN);
	mSunBillboardSet->setDefaultDimensions(1.0f, 1.0f);
	mSunBillboardSet->createBillboard(Ogre::Vector3::ZERO);

	mSunNode->attachObject (mSunBillboardSet);
}

SpriteSun::~SpriteSun () {
	if (mSunNode) {
		mSunNode->detachObject (mSunBillboardSet);
	}
	if (mSunBillboardSet) {
		mSunBillboardSet->_getManager ()->destroyBillboardSet (mSunBillboardSet);
		mSunBillboardSet = 0;
	}
	Ogre::MaterialManager::getSingletonPtr()->remove(mSunMaterial->getHandle());
}

void SpriteSun::setSunSphereColour (const Ogre::ColourValue &colour) {
	BaseSun::setSunSphereColour(colour);

	// Set sun material colour.
	mSunBillboardSet->getBillboard(0)->setColour(colour);
}

void SpriteSun::setSunTexture (const Ogre::String &textureName) {
	// Update the sun material
	assert(mSunMaterial->getBestTechnique ());
	assert(mSunMaterial->getBestTechnique ()->getPass (0));
	assert(mSunMaterial->getBestTechnique ()->getPass (0)->getTextureUnitState (0));
	mSunMaterial->getBestTechnique ()->getPass (0)->getTextureUnitState (0)->setTextureName (textureName);
}

void SpriteSun::setSunTextureAngularSize(const Ogre::Degree& sunTextureAngularSize){
	mSunTextureAngularSize = sunTextureAngularSize;
}

void SpriteSun::notifyCameraChanged (Ogre::Camera *cam) {
    // This calls setFarRadius
    BaseSun::notifyCameraChanged(cam);

    // Set sun position.
    Ogre::Real sunDistance = mRadius - mRadius * Ogre::Math::Tan(mSunTextureAngularSize);
	mSunNode->setPosition(-mSunDirection * sunDistance);

    // Set sun scaling in [1.0 ~ 1.2] range
    float factor = 1.2f - mSunSphereColour.b * 0.2f;
    float scale = factor * sunDistance * Ogre::Math::Tan(mSunTextureAngularSize);
    mSunNode->setScale (Ogre::Vector3::UNIT_SCALE * scale);
}

} // namespace caelum
