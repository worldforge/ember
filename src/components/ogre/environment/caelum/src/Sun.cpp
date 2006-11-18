#include "Sun.h"

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
	mSunEntity = sceneMgr->createEntity ("CaelumSun", "sphere.mesh");
	mSunEntity->setMaterialName (SUN_MATERIAL_NAME);
	mSunEntity->setCastShadows (false);
	mSunEntity->setRenderQueueGroup (Ogre::RENDER_QUEUE_SKIES_EARLY + 3);
	mSunNode->attachObject (mSunEntity);
}

Sun::~Sun () {
	// TODO: Destroy the sun node and entity;

	mMainLight->_getManager ()->destroyLight (mMainLight);
	mMainLight = 0;
}

void Sun::preViewportUpdate (const Ogre::RenderTargetViewportEvent &e) {
	// TODO
	Ogre::Camera *cam = e.source->getCamera ();
	mSunNode->setPosition (cam->getRealPosition () + mSunDirection * -10000);
//mNode->setScale(Ogre::Vector3::UNIT_SCALE * ((cam->getFarClipDistance () == 0 ) ? 100000 : (cam->getFarClipDistance ()-0.2)));
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
	mSunNode->setPosition (mSunDirection);
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
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().create (SUN_MATERIAL_NAME, "Caelum"));
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

} // namespace caelum
