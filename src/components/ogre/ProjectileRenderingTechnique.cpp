/*
 Copyright (C) 2019 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "ProjectileRenderingTechnique.h"


#include <OgreSceneManager.h>
#include <OgreRibbonTrail.h>
#include <OgreSceneNode.h>
#include <components/ogre/model/ModelRepresentation.h>
#include <components/ogre/model/Model.h>

namespace Ember {
namespace OgreView {

ProjectileRenderingTechnique::ProjectileRenderingTechnique(Ogre::SceneManager& sceneManager)
		: mSceneManager(sceneManager) {
	Ogre::NameValuePairList params;
	params["numberOfChains"] = "20";
	params["maxElements"] = "80";
	mTrail = dynamic_cast<Ogre::RibbonTrail*>( mSceneManager.createMovableObject("RibbonTrail", &params));

	if (mTrail) {
		mSceneManager.getRootSceneNode()->attachObject(mTrail);

		mTrail->setMaterialName("/common/base/ribbon");
		mTrail->setTrailLength(20);
		for (int i = 0; i < 20; ++i) {
			mTrail->setInitialColour(i, 1.0, 1.0, 1.0, 0.4 - ((i / 20.0) * 0.4));
			//mTrail->setColourChange(i, 0, 0, 0, 0.2);
			mTrail->setInitialWidth(i, 0.05 - ((i / 20.0) * 0.01));
			//mTrail->setWidthChange(i, 0.01);

		}
	}
}

ProjectileRenderingTechnique::~ProjectileRenderingTechnique() {
	mSceneManager.destroyMovableObject(mTrail);
}

void ProjectileRenderingTechnique::deregisterEntity(EmberEntity& entity) {
	if (mTrail) {
		auto model = Model::ModelRepresentation::getModelForEntity(entity);
		if (model && model->getNodeProvider()) {
			S_LOG_VERBOSE("Removing ribbon trail from " << entity.getId());
			mTrail->removeNode(model->getNodeProvider()->getNode());
		}
	}
}

void ProjectileRenderingTechnique::registerEntity(EmberEntity& entity) {
	if (mTrail) {
		auto model = Model::ModelRepresentation::getModelForEntity(entity);
		//Can't attach if there's already a Node listener.
		if (model && model->getNodeProvider() && !model->getNodeProvider()->getNode()->getListener()) {
			S_LOG_VERBOSE("Adding ribbon trail to " << entity.getId());
			try {
				mTrail->addNode(model->getNodeProvider()->getNode());
			} catch (const Ogre::Exception& ex) {
				S_LOG_WARNING("Error when trying to add ribbon trail." << ex);
			}
		}
	}
}
}
}