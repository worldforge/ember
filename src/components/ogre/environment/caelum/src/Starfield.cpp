#include "Starfield.h"
#include "CaelumSystem.h"
#include "GeometryFactory.h"

namespace caelum {

const Ogre::String Starfield::mStarfieldDomeResourceName = "CaelumStarfieldDome";

Starfield::Starfield (Ogre::SceneManager *sceneMgr) {
	mAutoRadius = true;
	mInclination = Ogre::Degree (0);

	GeometryFactory::generateSphericDome (mStarfieldDomeResourceName, 32, GeometryFactory::DT_STARFIELD);
	Ogre::Entity *ent = sceneMgr->createEntity ("StarfieldDome", mStarfieldDomeResourceName);
	ent->setMaterialName (CaelumSystem::STARFIELD_MATERIAL_NAME);
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
}

void Starfield::preViewportUpdate (const Ogre::RenderTargetViewportEvent &e) {
	Ogre::Camera *cam = e.source->getCamera ();
	mNode->setPosition (cam->getRealPosition ());
	if (mAutoRadius) {
		if (cam->getFarClipDistance () > 0)
			mNode->setScale (Ogre::Vector3::UNIT_SCALE * (cam->getFarClipDistance () - CAMERA_DISTANCE_MODIFIER));
		else
			mNode->setScale (Ogre::Vector3::UNIT_SCALE * (cam->getNearClipDistance () + CAMERA_DISTANCE_MODIFIER));
	}
}

void Starfield::setSize (float radius) {
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

} // namespace caelum
