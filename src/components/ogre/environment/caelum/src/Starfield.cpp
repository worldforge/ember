#include "Starfield.h"
#include "CaelumSystem.h"
#include "GeometryFactory.h"

namespace caelum {

const Ogre::String Starfield::mStarfieldDomeResourceName = "CaelumStarfieldDome";

Starfield::Starfield (Ogre::SceneManager *sceneMgr) {
	GeometryFactory::generateSphericDome (mStarfieldDomeResourceName, 32, GeometryFactory::DT_STARFIELD);
	mNode = sceneMgr->getRootSceneNode ()->createChildSceneNode ();
	Ogre::Entity *ent = sceneMgr->createEntity ("StarfieldDome", mStarfieldDomeResourceName);
	ent->setMaterialName (CaelumSystem::STARFIELD_MATERIAL_NAME);
	ent->setRenderQueueGroup (Ogre::RENDER_QUEUE_SKIES_EARLY + 1);
	ent->setCastShadows (false);
	mNode->attachObject (ent);

	mInclination = Ogre::Degree (0);
}

Starfield::~Starfield () {
	// TODO: Detach and destroy all attached objects.
	static_cast<Ogre::SceneNode *>(mNode->getParent ())->removeAndDestroyChild (mNode->getName ());
}

void Starfield::preViewportUpdate (const Ogre::RenderTargetViewportEvent &e) {
	Ogre::Camera *cam = e.source->getCamera ();
	mNode->setPosition (cam->getRealPosition ());
	if (cam->getFarClipDistance () > 0)
		mNode->setScale (Ogre::Vector3::UNIT_SCALE * (cam->getFarClipDistance () - CAMERA_DISTANCE_MODIFIER));
	else
		mNode->setScale (Ogre::Vector3::UNIT_SCALE * (cam->getNearClipDistance () + CAMERA_DISTANCE_MODIFIER));
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
