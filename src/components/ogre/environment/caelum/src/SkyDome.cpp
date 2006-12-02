#include "SkyDome.h"
#include "CaelumSystem.h"
#include "GeometryFactory.h"

namespace caelum {

const Ogre::String SkyDome::mSphericDomeResourceName = "CaelumSphericDome";

SkyDome::SkyDome (Ogre::SceneManager *sceneMgr) {
	mAutoRadius = true;

	GeometryFactory::generateSphericDome (mSphericDomeResourceName, 32);
	Ogre::Entity *ent = sceneMgr->createEntity ("Dome", mSphericDomeResourceName);
	ent->setMaterialName (CaelumSystem::SKY_DOME_MATERIAL_NAME);
	ent->setRenderQueueGroup (Ogre::RENDER_QUEUE_SKIES_EARLY + 2);
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
}

void SkyDome::preViewportUpdate (const Ogre::RenderTargetViewportEvent &e) {
	Ogre::Camera *cam = e.source->getCamera ();
	mNode->setPosition (cam->getRealPosition ());
	if (mAutoRadius) {
		if (cam->getFarClipDistance () > 0)
			mNode->setScale (Ogre::Vector3::UNIT_SCALE * (cam->getFarClipDistance () - CAMERA_DISTANCE_MODIFIER));
		else
			mNode->setScale (Ogre::Vector3::UNIT_SCALE * (cam->getNearClipDistance () + CAMERA_DISTANCE_MODIFIER));
	}
}

void SkyDome::setSize (float radius) {
	if (radius > 0) {
		mNode->setScale (Ogre::Vector3::UNIT_SCALE * radius);
		mAutoRadius = false;
	}
	else {
		mAutoRadius = true;
	}
}

} // namespace caelum
