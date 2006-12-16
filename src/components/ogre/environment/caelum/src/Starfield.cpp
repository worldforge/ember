#include "Starfield.h"
#include "CaelumSystem.h"
#include "GeometryFactory.h"

namespace caelum {

const Ogre::String Starfield::mStarfieldDomeResourceName = "CaelumStarfieldDome";
const Ogre::String Starfield::STARFIELD_MATERIAL_NAME = "CaelumStarfieldMaterial";

Starfield::Starfield (Ogre::SceneManager *sceneMgr) {
	mAutoRadius = true;
	mInclination = Ogre::Degree (0);

	createStarfieldMaterial ();

	GeometryFactory::generateSphericDome (mStarfieldDomeResourceName, 32, GeometryFactory::DT_STARFIELD);
	Ogre::Entity *ent = sceneMgr->createEntity ("StarfieldDome", mStarfieldDomeResourceName);
	ent->setMaterialName (STARFIELD_MATERIAL_NAME);
	ent->setRenderQueueGroup (Ogre::RENDER_QUEUE_SKIES_EARLY + 1);
	ent->setCastShadows (false);

	mNode = sceneMgr->getRootSceneNode ()->createChildSceneNode ();
	mNode->attachObject (ent);
}

Starfield::~Starfield () {
	if (mNode) {
		// Detach and destroy attached entity.
		Ogre::Entity *ent = static_cast<Ogre::Entity *>(mNode->detachObject ("StarfieldDome"));
		ent->_getManager ()->destroyEntity (ent);

		// Destroy the node
		static_cast<Ogre::SceneNode *>(mNode->getParent ())->removeAndDestroyChild (mNode->getName ());
		mNode = 0;
	}

	destroyStarfieldMaterial ();
}

void Starfield::notifyCameraChanged (Ogre::Camera *cam) {
	mNode->setPosition (cam->getRealPosition ());
	if (mAutoRadius) {
		if (cam->getFarClipDistance () > 0)
			mNode->setScale (Ogre::Vector3::UNIT_SCALE * (cam->getFarClipDistance () - CAMERA_DISTANCE_MODIFIER));
		else
			mNode->setScale (Ogre::Vector3::UNIT_SCALE * (cam->getNearClipDistance () + CAMERA_DISTANCE_MODIFIER));
	}
}

void Starfield::setFarRadius (float radius) {
	if (radius > 0) {
		mNode->setScale (Ogre::Vector3::UNIT_SCALE * radius);
		mAutoRadius = false;
	}
	else {
		mAutoRadius = true;
	}
}

void Starfield::setInclination (Ogre::Degree inc) {
	mInclination = inc;
}

void Starfield::update (const float time) {
	Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
	orientation = orientation * Ogre::Quaternion (Ogre::Radian (mInclination + Ogre::Degree (90)), Ogre::Vector3::UNIT_X);
	orientation = orientation * Ogre::Quaternion (Ogre::Radian (time * 2 * Ogre::Math::PI), Ogre::Vector3::UNIT_Y);

	mNode->setOrientation (orientation);
}

void Starfield::updateMaterial (const Ogre::String &mapName) {
	// Update the starfield material
	mStarfieldMaterial->getBestTechnique ()->getPass (0)->getTextureUnitState (0)->setTextureName (mapName);
}

void Starfield::createStarfieldMaterial () {
	Ogre::MaterialPtr mat;

	LOG ("Generating starfield material...");
	if (!Ogre::MaterialManager::getSingleton ().resourceExists (STARFIELD_MATERIAL_NAME)) {
		LOG ("\tMaterial not found; creating...");
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().create (STARFIELD_MATERIAL_NAME, RESOURCE_GROUP_NAME));
		mat->setReceiveShadows (false);
		LOG ("\t\tMaterial [OK]");
		Ogre::Pass *pass = mat->getTechnique (0)->getPass (0);
		pass->setDepthCheckEnabled (false);
		pass->setDepthWriteEnabled (false);
		pass->setLightingEnabled (false);
		pass->setFog (true);
		LOG ("\t\tPass [OK]");
		Ogre::TextureUnitState *tus = pass->createTextureUnitState ();
		tus->setTextureAddressingMode (Ogre::TextureUnitState::TAM_WRAP, Ogre::TextureUnitState::TAM_WRAP, Ogre::TextureUnitState::TAM_WRAP);
		LOG ("\t\tTextureUnit [OK]");
		mat->load ();
		LOG ("\tDONE");
	}
	else {
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().getByName (STARFIELD_MATERIAL_NAME));
	}
	LOG ("DONE");

	mStarfieldMaterial = mat;
}

void Starfield::destroyStarfieldMaterial () {
	LOG ("Removing starfield material...");
	if (!Ogre::MaterialManager::getSingleton ().resourceExists (STARFIELD_MATERIAL_NAME)) {
		Ogre::MaterialManager::getSingleton ().remove (STARFIELD_MATERIAL_NAME);
	}
	mStarfieldMaterial.setNull ();
	LOG ("DONE");
}

} // namespace caelum
