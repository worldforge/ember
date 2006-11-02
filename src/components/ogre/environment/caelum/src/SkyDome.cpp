#include "SkyDome.h"
#include "CaelumSystem.h"
#include "GeometryFactory.h"

namespace caelum {

const Ogre::String SkyDome::mSphericDomeResourceName = "CaelumSphericDome";

SkyDome::SkyDome (Ogre::SceneManager *sceneMgr) {
	GeometryFactory::generateSphericDome (mSphericDomeResourceName, 32);
	mNode = sceneMgr->getRootSceneNode ()->createChildSceneNode ();
	Ogre::Entity *ent = sceneMgr->createEntity ("Dome", mSphericDomeResourceName);
	ent->setMaterialName (CaelumSystem::SKY_DOME_MATERIAL_NAME);
	ent->setRenderQueueGroup (Ogre::RENDER_QUEUE_SKIES_EARLY + 2);
	ent->setCastShadows (false);
	mNode->attachObject (ent);
}

SkyDome::~SkyDome () {
	// TODO: Detach and destroy all attached objects.
	static_cast<Ogre::SceneNode *>(mNode->getParent ())->removeAndDestroyChild (mNode->getName ());
}

void SkyDome::preViewportUpdate (const Ogre::RenderTargetViewportEvent &e) {
	Ogre::Camera *cam = e.source->getCamera ();
	mNode->setPosition (cam->getRealPosition ());
	if (cam->getFarClipDistance () > 0)
		mNode->setScale (Ogre::Vector3::UNIT_SCALE * (cam->getFarClipDistance () - CAMERA_DISTANCE_MODIFIER));
	else
		mNode->setScale (Ogre::Vector3::UNIT_SCALE * (cam->getNearClipDistance () + CAMERA_DISTANCE_MODIFIER));
}

} // namespace caelum
