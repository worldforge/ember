#include <memory>

//
// C++ Implementation: EntityWorldPickListener
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityWorldPickListener.h"
#include "EmberEntityUserObject.h"
#include "Scene.h"

#include "domain/EmberEntity.h"

#include "MousePicker.h"
#include "Convert.h"
#include "EntityCollisionInfo.h"

#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/SubModel.h"
#include "OutlineEffect.h"

#include <OgreSceneQuery.h>
#include <OgreSceneManager.h>

#include <Eris/View.h>
#include <Eris/Avatar.h>
#include <Ogre.h>

namespace Ember {
namespace OgreView {

EntityWorldPickListenerVisualizer::EntityWorldPickListenerVisualizer(EntityWorldPickListener& pickListener, Ogre::SceneManager& sceneManager) :
		mEntity(nullptr),
		mDebugNode(nullptr) {
	mDebugNode = sceneManager.getRootSceneNode()->createChildSceneNode();
	try {
		mEntity = sceneManager.createEntity("pickerDebugObject", "common/primitives/model/sphere.mesh");
		//start out with a normal material
		mEntity->setMaterialName("BasePointMarkerMaterial");
		mEntity->setRenderingDistance(300);
		mEntity->setQueryFlags(MousePicker::CM_NONPICKABLE);
		mDebugNode->attachObject(mEntity);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when creating picking visualizer." << ex);
	}

	pickListener.EventPickedEntity.connect(sigc::mem_fun(*this, &EntityWorldPickListenerVisualizer::picker_EventPickedEntity));
}

EntityWorldPickListenerVisualizer::~EntityWorldPickListenerVisualizer() {
	if (mEntity) {
		mEntity->_getManager()->destroyEntity(mEntity);
	}
	if (mDebugNode) {
		mDebugNode->getCreator()->destroySceneNode(mDebugNode);
	}
}

void EntityWorldPickListenerVisualizer::picker_EventPickedEntity(const std::vector<EntityPickResult>& result, const MousePickerArgs& mouseArgs) {
	mDebugNode->setPosition(result.begin()->position);
}

EntityWorldPickListener::EntityWorldPickListener(Eris::View& view, Scene& scene) :
		VisualizePicking("visualize_picking", this, "Visualize mouse pickings."),
		mClosestPickingDistance(0),
		mFurthestPickingDistance(0),
		mContinuePickingThisContext(true),
		mVisualizer(nullptr),
		mView(view),
		mScene(scene) {
}

EntityWorldPickListener::~EntityWorldPickListener() = default;

void EntityWorldPickListener::initializePickingContext(bool& willParticipate, const MousePickerArgs& pickArgs) {
	mResult.clear();
	mPersistedResult.clear();
	if (pickArgs.pickType == MPT_PRESS || pickArgs.pickType == MPT_HOVER || pickArgs.pickType == MPT_SELECT) {
		willParticipate = true;

		mClosestPickingDistance = 0;
		mFurthestPickingDistance = 0;
		mContinuePickingThisContext = true;
	}
}

void EntityWorldPickListener::endPickingContext(const MousePickerArgs& mousePickerArgs, const std::vector<PickResult>& results) {
	if (mousePickerArgs.pickType == MPT_SELECT) {
		highlightSelectedEntity();
	}
	if (!mResult.empty()) {
		EventPickedEntity(mResult, mousePickerArgs);

		if (mousePickerArgs.pickType != MPT_HOVER) {
			mPersistedResult.reserve(mResult.size());
			for (auto& resultEntry : mResult) {
				PersistentEntityPickResult persistedEntry;
				persistedEntry.entityRef = EmberEntityRef(resultEntry.entity);
				persistedEntry.distance = resultEntry.distance;
				persistedEntry.isTransparent = resultEntry.isTransparent;
				persistedEntry.position = resultEntry.position;
				mPersistedResult.push_back(persistedEntry);
			}
		}
	}
}

void EntityWorldPickListener::processPickResult(bool& continuePicking, PickResult& result, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs) {
	if (!mContinuePickingThisContext) {
		return;
	}

	if (result.collisionInfo.type() == typeid(EntityCollisionInfo)) {
		auto& entityCollisionInfo = boost::any_cast<EntityCollisionInfo&>(result.collisionInfo);

		if (mFilter) {
			if (!mFilter(*entityCollisionInfo.entity)) {
				return;
			}
		}

		//handle composed entities

		std::list<EmberEntity*> entities;
		entities.push_front(entityCollisionInfo.entity);
		EmberEntity* entity = entityCollisionInfo.entity->getEmberLocation();
		while (entity) {
			if (entity->getCompositionMode() == EmberEntity::CM_COMPOSITION) {
				entities.push_front(entity);
			} else if (entity->getCompositionMode() == EmberEntity::CM_COMPOSITION_EXCLUSIVE) {
				entities.clear();
				entities.push_front(entity);
			}
			entity = entity->getEmberLocation();
		}

		for (auto& pickedEntity: entities) {
			mResult.emplace_back(EntityPickResult{pickedEntity, Convert::toOgre(result.point), result.distance, entityCollisionInfo.isTransparent});
			if (!entityCollisionInfo.isTransparent) {
				mContinuePickingThisContext = false;
			}
		}
	}
}

void EntityWorldPickListener::processDelayedPick(const MousePickerArgs& mousePickerArgs) {
	if (!mPersistedResult.empty()) {
		std::vector<EntityPickResult> resolvedResult;

		for (auto& persistedEntry : mPersistedResult) {
			if (persistedEntry.entityRef) {
				EntityPickResult entry;
				entry.entity = persistedEntry.entityRef.get();
				entry.distance = persistedEntry.distance;
				entry.isTransparent = persistedEntry.isTransparent;
				entry.position = persistedEntry.position;
				resolvedResult.push_back(entry);
			}
		}

		if (!resolvedResult.empty()) {
			EventPickedEntity(resolvedResult, mousePickerArgs);
		}

	}
}

void EntityWorldPickListener::runCommand(const std::string& command, const std::string& args) {
	if (VisualizePicking == command) {
		if (mVisualizer) {
			mVisualizer.reset();
		} else {
			mVisualizer = std::make_unique<EntityWorldPickListenerVisualizer>(*this, mScene.getSceneManager());
		}
	}
}

const std::vector<PersistentEntityPickResult>& EntityWorldPickListener::getPersistentResult() const {
	return mPersistedResult;
}

const std::vector<EntityPickResult>& EntityWorldPickListener::getResult() const {
	return mResult;
}

void EntityWorldPickListener::highlightSelectedEntity() {

	if (!mResult.empty()) {
		auto selectedEntity = mResult.front().entity;
		if (!mOutlineEffect || !mOutlineEffect->getEntity() || mOutlineEffect->getEntity() != selectedEntity) {
			mOutlineEffect = std::make_unique<OutlineEffect>(mScene, *selectedEntity);
		}
	} else {
		mOutlineEffect.reset();
	}
//
//
//	if (mOutline.selectedEntity.get() != selectedEntity) {
//		for (auto& entity: mOutline.generatedEntities) {
//			if (mOutline.model && mOutline.model->getNodeProvider()) {
//				mOutline.model->getNodeProvider()->detachObject(entity);
//			}
//			mScene.getSceneManager().destroyMovableObject(entity);
//		}
//		for (auto& material: mOutline.generatedMaterials) {
//			material->getCreator()->remove(material);
//		}
//
//		if (mOutline.selectedEntity) {
//			auto* oldEmberEntity = mOutline.selectedEntity.get();
//			auto* modelRep = dynamic_cast<Model::ModelRepresentation*>(oldEmberEntity->getGraphicalRepresentation());
//
//			if (modelRep) {
//				auto& model = modelRep->getModel();
//				if (model.useInstancing()) {
//					model.doWithMovables([](Ogre::MovableObject* movable, int index) {
//						if (movable->getMovableType() == "InstancedEntity") {
//							movable->setVisible(true);
//						}
//					});
//				}
//
//				auto submodelI = model.getSubmodels().begin();
//				for (size_t i = 0; i < model.getSubmodels().size(); ++i) {
//					//It could be that the entity has been reloaded in the interim, so we need to check that originalRenderQueueGroups size matches.
//					if (i < mOutline.originalRenderQueueGroups.size()) {
//						(*submodelI)->getEntity()->setRenderQueueGroup(mOutline.originalRenderQueueGroups[i]);
//
//					}
//					//If instancing is used we've temporarily attached the Ogre::Entity to the nodes; need to detach it.
//					if (model.useInstancing() && model.getNodeProvider()) {
//						model.getNodeProvider()->detachObject((*submodelI)->getEntity());
//					}
//
//					submodelI++;
//				}
//			}
//		}
//		mOutline = Outline{selectedEntity};
//
//		if (selectedEntity) {
//			auto* modelRep = dynamic_cast<Model::ModelRepresentation*>(selectedEntity->getGraphicalRepresentation());
//			if (modelRep && modelRep->getModel().getNodeProvider()) {
//				mOutline.model = &modelRep->getModel();
//
//				if (modelRep->getModel().useInstancing()) {
//					modelRep->getModel().doWithMovables([](Ogre::MovableObject* movable, int index) {
//						if (movable->getMovableType() == "InstancedEntity") {
//							movable->setVisible(false);
//						}
//					});
//				}
//
//				auto& submodels = modelRep->getModel().getSubmodels();
//				for (auto& submodel : submodels) {
//					auto ogreEntity = submodel->getEntity();
//					if (ogreEntity) {
//						mOutline.originalRenderQueueGroups.push_back(ogreEntity->getRenderQueueGroup());
//
//						if (ogreEntity->isVisible()) {
//
//							ogreEntity->setRenderQueueGroup(RENDER_QUEUE_OUTLINE_OBJECT);
//
//							if (!ogreEntity->getParentNode()) {
//								modelRep->getModel().getNodeProvider()->attachObject(ogreEntity);
//							}
//
//							auto outlineEntity = ogreEntity->clone(OgreInfo::createUniqueResourceName("outline"));
//							outlineEntity->setCastShadows(false);
//
//							outlineEntity->setRenderQueueGroup(RENDER_QUEUE_OUTLINE_BORDER);
//							if (outlineEntity->hasSkeleton()) {
//								outlineEntity->shareSkeletonInstanceWith(ogreEntity);
//							}
//							for (size_t i = 0; i < ogreEntity->getNumSubEntities(); ++i) {
//								auto outlineSubEntity = outlineEntity->getSubEntity(i);
//								auto subEntity = ogreEntity->getSubEntity(i);
//
//								outlineSubEntity->setVisible(subEntity->isVisible());
//								Ogre::TexturePtr texture;
//								if (subEntity->isVisible()) {
//									auto& material = subEntity->getMaterial();
//									auto tech = material->getBestTechnique();
//									if (tech && tech->getNumPasses() > 0) {
//										auto pass = tech->getPass(0);
//										if (pass->getNumTextureUnitStates() > 0) {
//											texture = pass->getTextureUnitState(0)->_getTexturePtr();
//										}
//									}
//									if (!texture) {
//										return;
//									}
//
//									auto outlineMaterial = Ogre::MaterialManager::getSingleton().getByName("/common/base/outline/nonculled")->clone(OgreInfo::createUniqueResourceName("outlineMaterial"));
//									outlineMaterial->load();
//									outlineMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTexture(texture);
//									outlineSubEntity->setMaterial(outlineMaterial);
//									mOutline.generatedMaterials.push_back(outlineMaterial);
//								}
//							}
//							modelRep->getModel().getNodeProvider()->attachObject(outlineEntity);
//							mOutline.generatedEntities.push_back(outlineEntity);
//
//						}
//					}
//				}
//			}
//		}
//	}
}


}
}
