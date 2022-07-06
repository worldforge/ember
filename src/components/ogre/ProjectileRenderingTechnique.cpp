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
#include "Convert.h"

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
            mTrail->setInitialColour(i, 1.0f, 1.0f, 1.0f, 0.4f);
            mTrail->setColourChange(i, 0, 0, 0, 0.05f);
            mTrail->setInitialWidth(i, 0.05f);
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
        I->second.isActive = false;
    }
}

void ProjectileRenderingTechnique::registerEntity(EmberEntity& entity) {
    if (mTrail) {
        auto I = mActiveNodes.find(&entity);
        if (I != mActiveNodes.end()) {
            I->second.inactiveTime = 0.0f;

        } else {
            if (!mAvailableNodes.empty()) {
                try {
                    auto node = mAvailableNodes.back();
                    mAvailableNodes.pop_back();
                    if (entity.getPosition().isValid()) {
                        mActiveNodes.emplace(&entity, ActiveEntityEntry{node, 0.f, true, true});
                        //Set the initial position of the trail to where the projectile entity was reported to be when received from the server.
                        node->setPosition(Convert::toOgre(entity.getPosition()));
                        mTrail->addNode(node);

                        auto chainIndex = mTrail->getChainIndexForNode(node);
                        //Adapt the colour fading to the simulation speed.
                        mTrail->setColourChange(chainIndex, 0, 0, 0, 0.5f * (float) entity.getView()->getSimulationSpeed());
                    } else {
                        mActiveNodes.emplace(&entity, ActiveEntityEntry{node, 0.f, false, true});
                    }

                } catch (const Ogre::Exception& ex) {
                    S_LOG_WARNING("Error when trying to add ribbon trail." << ex);
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
        if (entry.second.isActive) {
            if (!entry.second.hasBeenAdded) {
                if (entry.first->getPosition().isValid()) {
                    auto* node = entry.second.particleNode;
                    //Add entry which previously didn't have a valid position
                    node->setPosition(Convert::toOgre(entry.first->getPosition()));
                    mTrail->addNode(node);

                    auto chainIndex = mTrail->getChainIndexForNode(node);
                    //Adapt the colour fading to the simulation speed.
                    mTrail->setColourChange(chainIndex, 0, 0, 0, 0.5f * (float) entry.first->getView()->getSimulationSpeed());
                    entry.second.hasBeenAdded = true;
                }
            }

            if (entry.second.hasBeenAdded && entry.first->getPosition().isValid()) {
                entry.second.particleNode->setPosition(Convert::toOgre(entry.first->getPredictedPos()));
            }
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