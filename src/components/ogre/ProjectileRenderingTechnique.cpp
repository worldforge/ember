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

#include <Eris/View.h>
#include <OgreSceneManager.h>
#include <OgreRibbonTrail.h>
#include <OgreSceneNode.h>
#include <OgreRoot.h>
#include <components/ogre/model/ModelRepresentation.h>
#include <components/ogre/model/Model.h>

namespace Ember {
namespace OgreView {

ProjectileRenderingTechnique::ProjectileRenderingTechnique(Ogre::SceneManager& sceneManager)
		: mSceneManager(sceneManager) {

	Ogre::Root::getSingleton().addFrameListener(this);
	Ogre::NameValuePairList params;
	params["numberOfChains"] = "20";
	params["maxElements"] = "80";
	mTrail = dynamic_cast<Ogre::RibbonTrail*>(mSceneManager.createMovableObject("RibbonTrail", &params));

	if (mTrail) {
		mSceneManager.getRootSceneNode()->attachObject(mTrail);

		mTrail->setMaterialName("/common/base/ribbon");
		mTrail->setTrailLength(20);
		mAvailableNodes.reserve(20);
		for (int i = 0; i < 20; ++i) {
			mTrail->setInitialColour(i, 1.0, 1.0, 1.0, 0.4);
			mTrail->setColourChange(i, 0, 0, 0, 0.05);
			mTrail->setInitialWidth(i, 0.05);
			//mTrail->setWidthChange(i, 0.01);
			mAvailableNodes.emplace_back(mSceneManager.getRootSceneNode()->createChildSceneNode());
		}

	}
}

ProjectileRenderingTechnique::~ProjectileRenderingTechnique() {
	for (const auto& entity: mActiveNodes) {
		mSceneManager.destroySceneNode(entity.second.particleNode);
	}
	mActiveNodes.clear();
	for (const auto& entity: mAvailableNodes) {
		mSceneManager.destroySceneNode(entity);
	}
	mAvailableNodes.clear();


	mSceneManager.destroyMovableObject(mTrail);
	Ogre::Root::getSingleton().removeFrameListener(this);
}

void ProjectileRenderingTechnique::deregisterEntity(EmberEntity& entity) {
	auto I = mActiveNodes.find(&entity);
	if (I != mActiveNodes.end()) {
		I->second.entityNode = nullptr;
	}
}

void ProjectileRenderingTechnique::registerEntity(EmberEntity& entity) {
	if (mTrail) {
		auto I = mActiveNodes.find(&entity);
		if (I != mActiveNodes.end()) {
			auto model = Model::ModelRepresentation::getModelForEntity(entity);
			if (model && model->getNodeProvider()) {
				I->second.entityNode = model->getNodeProvider()->getNode();
				I->second.inactiveTime = 0.0f;
			}
		} else {
			if (!mAvailableNodes.empty()) {
				auto model = Model::ModelRepresentation::getModelForEntity(entity);
				if (model && model->getNodeProvider()) {
					try {
						auto node = mAvailableNodes.back();
						mAvailableNodes.pop_back();
						auto entityNode = model->getNodeProvider()->getNode();
						mActiveNodes.emplace(&entity, ActiveEntityEntry{entityNode, node, 0.f});
						node->setPosition(entityNode->_getDerivedPosition());
						mTrail->addNode(node);

						auto chainIndex = mTrail->getChainIndexForNode(node);
						//Adapt the colour fading to the simulation speed.
						mTrail->setColourChange(chainIndex, 0, 0, 0, 0.5f * entity.getView()->getSimulationSpeed());

					} catch (const Ogre::Exception& ex) {
						S_LOG_WARNING("Error when trying to add ribbon trail." << ex);
					}
				}
			}
		}
	}
}

bool ProjectileRenderingTechnique::frameStarted(const Ogre::FrameEvent& evt) {

	//Make sure that the particle nodes are synced with the position of the entity nodes,
	//unless the entity has been deregistered, at which we'll instead retain it for some seconds
	//before removing it and returning the SceneNode to the list of available nodes.
	std::vector<EmberEntity*> nodesToRemove;
	for (auto& entry : mActiveNodes) {
		if (entry.second.entityNode) {
			entry.second.particleNode->setPosition(entry.second.entityNode->getPosition());
		} else {
			entry.second.inactiveTime += evt.timeSinceLastFrame;
			if (entry.second.inactiveTime > 3.0f) {
				nodesToRemove.emplace_back(entry.first);
			}
		}
	}

	for (auto entity: nodesToRemove) {
		auto I = mActiveNodes.find(entity);
		mAvailableNodes.emplace_back(I->second.particleNode);
		mTrail->removeNode(I->second.particleNode);
		mActiveNodes.erase(I);
	}
	return true;
}

}
}