#include "Sun.h"
#include "CaelumSystem.h"

namespace caelum {

const Ogre::String Sun::SUN_MATERIAL_NAME = "CaelumSunMaterial";

Sun::Sun (Ogre::SceneManager *sceneMgr) {
	mInclination = Ogre::Degree (0);
	mSunColour = Ogre::ColourValue::White;

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
}

void Sun::preViewportUpdate (const Ogre::RenderTargetViewportEvent &e) {
	// TODO
	Ogre::Camera *cam = e.source->getCamera ();
	///make it so that the appears independent from the world
	mSunNode->setPosition (cam->getRealPosition () + (-mSunDirection * 50));

}

void Sun::update (const float time) {
	// Get the inclinated axis
	Ogre::Vector3 axis = Ogre::Vector3::UNIT_Z;
	axis = Ogre::Quaternion (mInclination, Ogre::Vector3::UNIT_X) * axis;

	// Get the inclinated light direction, according to the day time
	Ogre::Vector3 dir = Ogre::Vector3::UNIT_Y;
	dir = Ogre::Quaternion (Ogre::Radian (time * 2 * Ogre::Math::PI), axis) * dir;

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
	mSunDirection = dir.normalisedCopy ();
}

void Sun::setInclination (Ogre::Degree inc) {
	mInclination = inc;
}

Ogre::Degree Sun::getInclination () const {
	return mInclination;
}

Ogre::Vector3 Sun::getSunDirection () const {
	return mSunDirection;
}

void Sun::setSunColour (Ogre::ColourValue colour) {
	colour = colour * Ogre::ColourValue (1, 1, 0.9);
	colour = colour * 3;
	mMainLight->setDiffuseColour (colour);
	mMainLight->setSpecularColour (colour);

	mSunMaterial->setSelfIllumination (colour + Ogre::ColourValue (.5, .4, .2));
	
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
