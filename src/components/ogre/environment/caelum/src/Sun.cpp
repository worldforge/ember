#include "Sun.h"
#include "CaelumSystem.h"

namespace caelum {

const Ogre::String Sun::SUN_MATERIAL_NAME = "CaelumSunMaterial";

Sun::Sun (Ogre::SceneManager *sceneMgr) {
	mSunColour = Ogre::ColourValue::White;
	mAutoRadius = true;
	mSunPositionModel = 0;

	mMainLight = sceneMgr->createLight ("CaelumSun");
	mMainLight->setType (Ogre::Light::LT_DIRECTIONAL);
	mMainLight->setPowerScale (10);	// REALLY bright.

	createSunMaterial ();

	if (sceneMgr->hasSceneNode("SunNode")) {
		mSunNode = sceneMgr->getSceneNode( "SunNode");
	} else {
		mSunNode = sceneMgr->getRootSceneNode ()->createChildSceneNode("SunNode");
	}
	mSunEntity = sceneMgr->createEntity ("CaelumSun", "3d_objects/environment/sky/models/sphere/sphere.mesh");
	mSunEntity->setMaterialName (SUN_MATERIAL_NAME);
	mSunEntity->setCastShadows (false);
	mSunEntity->setRenderQueueGroup (Ogre::RENDER_QUEUE_SKIES_EARLY + 3);
	mSunNode->attachObject (mSunEntity);
	mSunNode->_update(true, false);
	if (mSunEntity->getBoundingRadius ()) {
		///make it one unit in size
		Ogre::Real scale = 1 / mSunEntity->getBoundingRadius ();
		mSunNode->setScale(scale, scale, scale);
	}
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

	if (mSunPositionModel) {
		delete mSunPositionModel;
	}
}

void Sun::notifyCameraChanged (Ogre::Camera *cam) {
	float sunRadius0;
	if (mAutoRadius) {
		if (cam->getFarClipDistance () > 0) {
			mRadius = (cam->getFarClipDistance () - CAMERA_DISTANCE_MODIFIER) * 0.5;
			sunRadius0 = -1;
		}
		else {
			mRadius = (cam->getNearClipDistance () + CAMERA_DISTANCE_MODIFIER) * 2;
			sunRadius0 = 1;
		}
	}
	sunRadius0 *= mRadius * Ogre::Math::Tan (Ogre::Degree (0.01));
	mSunNode->setPosition (cam->getRealPosition () - mSunDirection * (mRadius + sunRadius0));
	mSunNode->setScale (Ogre::Vector3::UNIT_SCALE * (mRadius + sunRadius0) * Ogre::Math::Tan (Ogre::Degree (0.01)));
}

void Sun::setFarRadius (float radius) {
	if (radius > 0) {
		mRadius = radius;
		mAutoRadius = false;
	}
	else {
		mAutoRadius = true;
	}
}

void Sun::update (const float time) {
	Ogre::Vector3 dir = Ogre::Vector3::NEGATIVE_UNIT_Y;

	if (mSunPositionModel) {
		dir = mSunPositionModel->update (time);
	}

	// Update the main light direction
	if (mMainLight != 0) {
		mMainLight->setDirection (dir);
		///hide the sun if it's below the horizon
		if (dir.y > 0) {
			mMainLight->setVisible(false);
		} else {
			mMainLight->setVisible(true);
		}
	}

	// Store the latest sun direction.
	mSunDirection = dir;
}

SunPositionModel *Sun::setSunPositionModel (SunPositionModel *model) {
	SunPositionModel *temp = mSunPositionModel;

	mSunPositionModel = model;

	return temp;
}

SunPositionModel *Sun::getSunPositionModel () const {
	return mSunPositionModel;
}

Ogre::Vector3 Sun::getSunDirection () const {
	return mSunDirection;
}

/**
"Normalizes" a colour value, i.e. makes sure that it stays within [0..1] range.
*/
Ogre::ColourValue& normalizeColour(Ogre::ColourValue& colour)
{
	Ogre::Real max = 0.0f;
	max = std::max<Ogre::Real>(colour[0], max);
	max = std::max<Ogre::Real>(colour[1], max);
	max = std::max<Ogre::Real>(colour[2], max);
	if (max > 1) {
		Ogre::Real adjust = 1.0f / max;
		colour[0] = adjust * colour[0];
		colour[1] = adjust * colour[1];
		colour[2] = adjust * colour[2];
	}
	return colour;
}

void Sun::setSunColour (Ogre::ColourValue colour) {
 	colour = Ogre::ColourValue (1, 1, 0.9);
/*	colour = colour * Ogre::ColourValue (1, 1, 0.9);
	colour = colour * 3;*/
	///we need to normalize it because some shaders might not be able to cope with colours that go beyond 1
//  	normalizeColour(colour);
	mMainLight->setDiffuseColour (colour);
	mMainLight->setSpecularColour (colour);

	colour += Ogre::ColourValue (.5, .4, .2);
// 	normalizeColour(colour);
	mSunMaterial->setSelfIllumination (colour);
	
	// Store this last colour
	mSunColour = colour;
}

Ogre::ColourValue Sun::getSunColour () {
	return mSunColour;
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
