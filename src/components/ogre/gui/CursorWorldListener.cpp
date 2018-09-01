#include <memory>

/*
 Copyright (C) 2011 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CursorWorldListener.h"

#include "framework/LoggingInstance.h"
#include "framework/MainLoopController.h"
#include "framework/TimeFrame.h"
#include "components/ogre/camera/MainCamera.h"
#include "components/ogre/World.h"
#include "components/ogre/Avatar.h"
#include "components/ogre/EntityWorldPickListener.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/SubModel.h"
#include "domain/EmberEntity.h"

#include <OgreSceneNode.h>
#include <CEGUI/Window.h>
#include <CEGUI/WindowManager.h>
#include <components/ogre/OgreInfo.h>
#include <OgreRenderQueue.h>
#include <OgreSubEntity.h>
#include <OgreMovableObject.h>
#include <OgreInstancedEntity.h>


#include <OgreRenderQueueListener.h>
#include <Ogre.h>

// render queues
#define RENDER_QUEUE_OUTLINE_OBJECT		Ogre::RENDER_QUEUE_MAIN + 1
#define RENDER_QUEUE_OUTLINE_BORDER		Ogre::RENDER_QUEUE_9 + 2

// stencil values
#define STENCIL_VALUE_FOR_OUTLINE_GLOW 1
#define STENCIL_FULL_MASK 0xFFFFFFFF

namespace Ember
{
namespace OgreView
{
namespace Gui
{



class StencilOpQueueListener : public Ogre::RenderQueueListener {
public:
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


CursorWorldListener::CursorWorldListener(MainLoopController& mainLoopController, CEGUI::Window& mainWindow, World& world) :
		mMainWindow(mainWindow),
		mWorld(world),
		mHoverEventSent(false),
		mCursorLingerStart(0),
		mClickThresholdMilliseconds(200),
		mMousePressedTimeFrame(nullptr),
		mConfigListenerContainer(new ConfigListenerContainer())
{

	mainLoopController.EventAfterInputProcessing.connect(sigc::mem_fun(*this, &CursorWorldListener::afterEventProcessing));
	Ember::Input::getSingleton().EventMouseButtonReleased.connect(sigc::mem_fun(*this, &CursorWorldListener::input_MouseButtonReleased));

	mConnections.push_back(mMainWindow.subscribeEvent(CEGUI::Window::EventMouseEntersSurface, CEGUI::Event::Subscriber(&CursorWorldListener::windowMouseEnters, this)));
	mConnections.push_back(mMainWindow.subscribeEvent(CEGUI::Window::EventMouseLeavesSurface, CEGUI::Event::Subscriber(&CursorWorldListener::windowMouseLeaves, this)));

	mConnections.push_back(mMainWindow.subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&CursorWorldListener::windowMouseButtonDown, this)));
	mConnections.push_back(mMainWindow.subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&CursorWorldListener::windowMouseButtonUp, this)));
	mConnections.push_back(mMainWindow.subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::Event::Subscriber(&CursorWorldListener::windowMouseDoubleClick, this)));

	mConfigListenerContainer->registerConfigListenerWithDefaults("input", "clickthreshold", sigc::mem_fun(*this, &CursorWorldListener::Config_ClickThreshold), 200);

	world.getSceneManager().addRenderQueueListener(new StencilOpQueueListener());

}

CursorWorldListener::~CursorWorldListener()
{
	for (auto& entity: mOutline.generatedEntities) {
		if (entity->getParentSceneNode()) {
			entity->getParentSceneNode()->detachObject(entity);
		}
		mWorld.getSceneManager().destroyMovableObject(entity);
	}

	delete mConfigListenerContainer;
	for (auto& connection : mConnections) {
		connection->disconnect();
	}
	if (mMouseMovesConnection.isValid()) {
		mMouseMovesConnection->disconnect();
	}
}

void CursorWorldListener::afterEventProcessing(float timeslice)
{
	if (isInGUIMode()) {

		const auto& pixelPosition = mMainWindow.getGUIContext().getMouseCursor().getPosition();
		sendWorldClick(MPT_SELECT, pixelPosition, 50);

		if (!mHoverEventSent) {
			mCursorLingerStart += static_cast<long long>(timeslice * 1000);

			if (mCursorLingerStart > 500) {
				sendHoverEvent();
			}
		}

		highlightSelectedEntity();

		if (mMousePressedTimeFrame) {
			if (!mMousePressedTimeFrame->isTimeLeft()) {
				mMousePressedTimeFrame.reset();
				sendWorldClick(MPT_PRESSED, mMainWindow.getGUIContext().getMouseCursor().getPosition(), 50);
			}
		}
	} else {

		mWorld.getEntityPickListener().mFilter = [&](const EmberEntity& pickedEntity) {
			return &pickedEntity != &mWorld.getAvatar()->getEmberEntity();
		};

		MousePickerArgs pickerArgs{};
		pickerArgs.pickType = MousePickType::MPT_SELECT;
		pickerArgs.distance = 50;
		mWorld.getMainCamera().pickInWorld(0.5, 0.5, pickerArgs);
		highlightSelectedEntity();
		mWorld.getEntityPickListener().mFilter = nullptr;

	}
}

bool CursorWorldListener::windowMouseEnters(const CEGUI::EventArgs& args)
{
	mMouseMovesConnection = mMainWindow.subscribeEvent(CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber(&CursorWorldListener::windowMouseMoves, this));
	mHoverEventSent = false;
	return true;
}

bool CursorWorldListener::windowMouseLeaves(const CEGUI::EventArgs& args)
{
	if (mMouseMovesConnection.isValid()) {
		mMouseMovesConnection->disconnect();
		mMouseMovesConnection = CEGUI::Event::Connection();
	}
	mHoverEventSent = true;
	return true;
}

bool CursorWorldListener::windowMouseMoves(const CEGUI::EventArgs& args)
{
	mCursorLingerStart = 0;
	mHoverEventSent = false;
	return true;
}

void CursorWorldListener::sendHoverEvent()
{
	const auto& pixelPosition = mMainWindow.getGUIContext().getMouseCursor().getPosition();
	sendWorldClick(MPT_HOVER, pixelPosition, 30);
	mHoverEventSent = true;
}

void CursorWorldListener::input_MouseButtonReleased(Input::MouseButton button, Input::InputMode inputMode)
{
	mMousePressedTimeFrame.reset();
}

void CursorWorldListener::sendWorldClick(MousePickType pickType, const CEGUI::Vector2f& pixelPosition, float distance)
{

	const auto& position = mMainWindow.getGUIContext().getMouseCursor().getDisplayIndependantPosition();
	MousePickerArgs pickerArgs{};
	pickerArgs.windowX = pixelPosition.d_x;
	pickerArgs.windowY = pixelPosition.d_y;
	pickerArgs.pickType = pickType;
	pickerArgs.distance = distance;
	mWorld.getMainCamera().pickInWorld(position.d_x, position.d_y, pickerArgs);

}

bool CursorWorldListener::windowMouseButtonDown(const CEGUI::EventArgs& args)
{
	if (isInGUIMode()) {
		S_LOG_VERBOSE("Main sheet is capturing input");
		CEGUI::Window* aWindow = mMainWindow.getCaptureWindow();
		if (aWindow) {
			aWindow->releaseInput();
			aWindow->deactivate();
		}

		mMousePressedTimeFrame = std::make_unique<TimeFrame>(boost::posix_time::milliseconds(mClickThresholdMilliseconds));
		sendWorldClick(MPT_PRESS, mMainWindow.getGUIContext().getMouseCursor().getPosition(), 300);
	}

	return true;
}

bool CursorWorldListener::windowMouseButtonUp(const CEGUI::EventArgs& args)
{
	if (isInGUIMode()) {
		if (mMousePressedTimeFrame) {
			if (mMousePressedTimeFrame->isTimeLeft()) {
				mMousePressedTimeFrame.reset();
				sendWorldClick(MPT_CLICK, static_cast<const CEGUI::MouseEventArgs&>(args).position, 300);
			}
		}
	}
	return true;
}

bool CursorWorldListener::windowMouseDoubleClick(const CEGUI::EventArgs& args)
{
	auto& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>(args);
	sendWorldClick(MPT_DOUBLECLICK, mouseArgs.position, 300);

	return true;
}

const bool CursorWorldListener::isInGUIMode() const
{
	return Input::getSingleton().getInputMode() == Input::IM_GUI;
}

void CursorWorldListener::Config_ClickThreshold(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_int()) {
		mClickThresholdMilliseconds = static_cast<int>(variable);
	}
}

void CursorWorldListener::highlightSelectedEntity() {
	EmberEntity* selectedEntity = nullptr;

	auto& results = mWorld.getEntityPickListener().getResult();

	if (!results.empty()) {
		selectedEntity = results.front().entity;
	}



	if (mOutline.selectedEntity.get() != selectedEntity) {
		for (auto& entity: mOutline.generatedEntities) {
			if (entity->getParentSceneNode()) {
				entity->getParentSceneNode()->detachObject(entity);
			}
			mWorld.getSceneManager().destroyMovableObject(entity);
		}
		for (auto& material: mOutline.generatedMaterials) {
			material->getCreator()->remove(material);
		}

		if (mOutline.selectedEntity) {
			auto* oldEmberEntity = dynamic_cast<EmberEntity*>(mOutline.selectedEntity.get());
			auto* modelRep = dynamic_cast<Model::ModelRepresentation*>(oldEmberEntity->getGraphicalRepresentation());

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
					(*submodelI)->getEntity()->setRenderQueueGroup(mOutline.originalRenderQueueGroups[i]);

					//If instancing is used we've temporarily attached the Ogre::Entity to the nodes; need to detach it.
					if (model.useInstancing()) {
						model.getNodeProvider()->detachObject((*submodelI)->getEntity());
					}

					submodelI++;
				}

			}
		}
		mOutline = Glow{selectedEntity};

		if (selectedEntity) {
			auto* modelRep = dynamic_cast<Model::ModelRepresentation*>(selectedEntity->getGraphicalRepresentation());
			if (modelRep) {

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

							if (!ogreEntity->getParentSceneNode()) {
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
	}
}

}
}
}
