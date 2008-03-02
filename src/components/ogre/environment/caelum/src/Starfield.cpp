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
#include "Starfield.h"
#include "GeometryFactory.h"

namespace caelum {

const Ogre::String Starfield::STARFIELD_DOME_NAME = "CaelumStarfieldDome";

const Ogre::String Starfield::STARFIELD_MATERIAL_NAME = "CaelumStarfieldMaterial";

Starfield::Starfield (Ogre::SceneManager *sceneMgr, const Ogre::String &textureName) {
	mInclination = Ogre::Degree (0);

	createStarfieldMaterial ();
    setTexture (textureName);

	GeometryFactory::generateSphericDome (STARFIELD_DOME_NAME, 32, GeometryFactory::DT_STARFIELD);
	Ogre::Entity *ent = sceneMgr->createEntity ("StarfieldDome", STARFIELD_DOME_NAME);
	ent->setMaterialName (STARFIELD_MATERIAL_NAME);
	ent->setRenderQueueGroup (CAELUM_RENDER_QUEUE_STARFIELD);
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
    CameraBoundElement::notifyCameraChanged (cam);
	mNode->setPosition (cam->getRealPosition ());
}

void Starfield::setFarRadius (Ogre::Real radius) {
    CameraBoundElement::setFarRadius(radius);
	mNode->setScale (Ogre::Vector3::UNIT_SCALE * radius);
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

void Starfield::setTexture (const Ogre::String &mapName) {
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
