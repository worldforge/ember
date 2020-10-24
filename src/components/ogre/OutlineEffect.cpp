/*
 Copyright (C) 2020 Erik Ogenvik

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

#include "OutlineEffect.h"
#include "Scene.h"

#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/SubModel.h"
#include "components/ogre/OgreInfo.h"

#include <Ogre.h>

// render queues
#define RENDER_QUEUE_OUTLINE_OBJECT        Ogre::RENDER_QUEUE_MAIN + 1
#define RENDER_QUEUE_OUTLINE_BORDER        Ogre::RENDER_QUEUE_9 + 2

// stencil values
#define STENCIL_VALUE_FOR_OUTLINE_GLOW 1
#define STENCIL_FULL_MASK 0xFFFFFFFF


namespace Ember {
namespace OgreView {


struct StencilOpQueueListener : public Ogre::RenderQueueListener {
	void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation) override {
		if (queueGroupId == RENDER_QUEUE_OUTLINE_OBJECT) {
			Ogre::RenderSystem* renderSystem = Ogre::Root::getSingleton().getRenderSystem();

			renderSystem->clearFrameBuffer(Ogre::FBT_STENCIL);
			renderSystem->setStencilCheckEnabled(true);
			renderSystem->setStencilBufferParams(Ogre::CMPF_ALWAYS_PASS,
												 STENCIL_VALUE_FOR_OUTLINE_GLOW, STENCIL_FULL_MASK, STENCIL_FULL_MASK,
												 Ogre::SOP_KEEP, Ogre::SOP_KEEP, Ogre::SOP_REPLACE, false);
		}
		if (queueGroupId == RENDER_QUEUE_OUTLINE_BORDER) {
			Ogre::RenderSystem* renderSystem = Ogre::Root::getSingleton().getRenderSystem();
			renderSystem->setStencilCheckEnabled(true);
			renderSystem->setStencilBufferParams(Ogre::CMPF_NOT_EQUAL,
												 STENCIL_VALUE_FOR_OUTLINE_GLOW, STENCIL_FULL_MASK, STENCIL_FULL_MASK,
												 Ogre::SOP_KEEP, Ogre::SOP_KEEP, Ogre::SOP_REPLACE, false);
		}
	}

	void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation) override {
		if (queueGroupId == RENDER_QUEUE_OUTLINE_OBJECT || queueGroupId == RENDER_QUEUE_OUTLINE_BORDER) {
			Ogre::RenderSystem* rendersys = Ogre::Root::getSingleton().getRenderSystem();
			rendersys->setStencilCheckEnabled(false);
			rendersys->setStencilBufferParams();
		}
	}

};

OutlineEffect::OutlineEffect(Scene& scene, EmberEntity& entity)
		: mScene(scene),
		  mSelectedEntity(&entity),
		  mStencilOpQueueListener(std::make_unique<StencilOpQueueListener>()) {
	scene.getSceneManager().addRenderQueueListener(mStencilOpQueueListener.get());
	auto* modelRep = dynamic_cast<Model::ModelRepresentation*>(entity.getGraphicalRepresentation());
	if (modelRep && modelRep->getModel().getNodeProvider()) {
		mOutline.model = &modelRep->getModel();

		if (modelRep->getModel().useInstancing()) {
			modelRep->getModel().doWithMovables([](Ogre::MovableObject* movable, int index) {
				if (movable->getMovableType() == "InstancedEntity") {
					movable->setVisible(false);
				}
			});
		}

		auto& submodels = modelRep->getModel().getSubmodels();
		for (auto& submodel : submodels) {
			auto ogreEntity = submodel->getEntity();
			if (ogreEntity) {
				mOutline.originalRenderQueueGroups.push_back(ogreEntity->getRenderQueueGroup());

				if (ogreEntity->isVisible()) {

					ogreEntity->setRenderQueueGroup(RENDER_QUEUE_OUTLINE_OBJECT);

					if (!ogreEntity->getParentNode()) {
						modelRep->getModel().getNodeProvider()->attachObject(ogreEntity);
					}

					auto outlineEntity = ogreEntity->clone(OgreInfo::createUniqueResourceName("outline"));
					outlineEntity->setCastShadows(false);

					outlineEntity->setRenderQueueGroup(RENDER_QUEUE_OUTLINE_BORDER);
					if (outlineEntity->hasSkeleton()) {
						outlineEntity->shareSkeletonInstanceWith(ogreEntity);
					}
					for (size_t i = 0; i < ogreEntity->getNumSubEntities(); ++i) {
						auto outlineSubEntity = outlineEntity->getSubEntity(i);
						auto subEntity = ogreEntity->getSubEntity(i);

						outlineSubEntity->setVisible(subEntity->isVisible());
						Ogre::TexturePtr texture;
						if (subEntity->isVisible()) {
							auto& material = subEntity->getMaterial();
							auto tech = material->getBestTechnique();
							if (tech && tech->getNumPasses() > 0) {
								auto pass = tech->getPass(0);
								if (pass->getNumTextureUnitStates() > 0) {
									texture = pass->getTextureUnitState(0)->_getTexturePtr();
								}
							}
							if (!texture) {
								return;
							}

							auto outlineMaterial = Ogre::MaterialManager::getSingleton().getByName("/common/base/outline/nonculled")->clone(OgreInfo::createUniqueResourceName("outlineMaterial"));
							outlineMaterial->load();
							outlineMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTexture(texture);
							outlineSubEntity->setMaterial(outlineMaterial);
							mOutline.generatedMaterials.push_back(outlineMaterial);
						}
					}
					modelRep->getModel().getNodeProvider()->attachObject(outlineEntity);
					mOutline.generatedEntities.push_back(outlineEntity);

				}
			}
		}
	}
}

OutlineEffect::~OutlineEffect() {
	mScene.getSceneManager().removeRenderQueueListener(mStencilOpQueueListener.get());
	for (auto& entity: mOutline.generatedEntities) {
		if (mOutline.model && mOutline.model->getNodeProvider()) {
			mOutline.model->getNodeProvider()->detachObject(entity);
		}
		mScene.getSceneManager().destroyMovableObject(entity);
	}
	for (auto& material: mOutline.generatedMaterials) {
		material->getCreator()->remove(material);
	}

	if (mSelectedEntity) {
		auto& oldEmberEntity = *mSelectedEntity;
		auto* modelRep = dynamic_cast<Model::ModelRepresentation*>(oldEmberEntity.getGraphicalRepresentation());

		if (modelRep) {
			auto& model = modelRep->getModel();
			if (model.useInstancing()) {
				model.doWithMovables([](Ogre::MovableObject* movable, int index) {
					if (movable->getMovableType() == "InstancedEntity") {
						movable->setVisible(true);
					}
				});
			}

			auto submodelI = model.getSubmodels().begin();
			for (size_t i = 0; i < model.getSubmodels().size(); ++i) {
				//It could be that the entity has been reloaded in the interim, so we need to check that originalRenderQueueGroups size matches.
				if (i < mOutline.originalRenderQueueGroups.size()) {
					(*submodelI)->getEntity()->setRenderQueueGroup(mOutline.originalRenderQueueGroups[i]);

				}
				//If instancing is used we've temporarily attached the Ogre::Entity to the nodes; need to detach it.
				if (model.useInstancing() && model.getNodeProvider()) {
					model.getNodeProvider()->detachObject((*submodelI)->getEntity());
				}

				submodelI++;
			}
		}
	}
}
}
}
