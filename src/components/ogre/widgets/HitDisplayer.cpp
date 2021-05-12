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

#include "HitDisplayer.h"

#include "../Convert.h"
#include "../model/ModelRepresentation.h"
#include "../model/Model.h"

#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/Avatar.h>

#include <Atlas/Objects/RootOperation.h>
#include <wfmath/MersenneTwister.h>

#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>

#include <memory>

namespace Ember {
namespace OgreView {

namespace Gui {
HitDisplayer::HitDisplayer(CEGUI::Window& mainSheet,
						   const UniqueWindowPtr<CEGUI::Window>& textTemplate,
						   Ogre::Camera& camera,
						   Eris::View& view,
						   Ogre::SceneManager& sceneManager)
		: mCamera(camera),
		  mView(view),
		  mSceneManager(sceneManager) {
    mBackgroundWindow.reset(CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "hit_displayer"));
    mainSheet.addChild(mBackgroundWindow.get());
    mBackgroundWindow->moveToBack();
    mBackgroundWindow->setMousePassThroughEnabled(true);
    mBackgroundWindow->setDisabled(true);
    mBackgroundWindow->setRiseOnClickEnabled(false);
    mTextNodeRenderer = std::make_unique<TextNodeRenderer>(*mBackgroundWindow, textTemplate);

	camera.addListener(mTextNodeRenderer.get());
	Ogre::Root::getSingleton().addFrameListener(this);


	view.EntitySeen.connect([this](Eris::Entity* entity) {
		entity->Hit.connect([this, entity](const Atlas::Objects::Operation::Hit& arg, const Eris::TypeInfo& typeInfo) {
			entityHit(*entity, arg);
		});
	});
}

HitDisplayer::~HitDisplayer() {
	mCamera.removeListener(mTextNodeRenderer.get());
	Ogre::Root::getSingleton().removeFrameListener(this);
}

void HitDisplayer::entityHit(Eris::Entity& entity, const Atlas::Objects::Operation::Hit& act) {
	if (!act->getArgs().empty()) {
		auto& arg = act->getArgs().front();
		if (arg->hasAttr("damage")) {
			auto damageElem = arg->getAttr("damage");
			if (damageElem.isNum()) {
				std::stringstream ss;
				ss.precision(2);
				ss << damageElem.asNum();
				auto damageString = ss.str();
				auto pos = Convert::toOgre(entity.getPredictedPos());
				auto model = Model::ModelRepresentation::getModelForEntity(static_cast<EmberEntity&>(entity));
				if (model && model->isLoaded()) {
					pos += model->getCombinedBoundingBox().getCenter();
				}
				createHit(pos, damageString);
			}
		}
	}
}

bool HitDisplayer::frameStarted(const Ogre::FrameEvent& evt) {

	for (auto I = mHits.begin(); I != mHits.end(); ++I) {
		auto hit = *I;
		hit->time += evt.timeSinceLastFrame;
		if (hit->time >= 2.0) {
			mTextNodeRenderer->detachTextNode(hit->node);
			I = mHits.erase(I);
		}
	}
	return true;
}

void HitDisplayer::createHit(const Ogre::Vector3& pos, const std::string& text) {
	auto sceneNode = mSceneManager.getRootSceneNode()->createChildSceneNode(pos);

	auto x = WFMath::MTRand::instance.rand<float>() - 0.5f;

	Ogre::Vector2 direction(x * 0.01f, -0.1f);

	auto hit = std::make_shared<Hit>(Hit{0.f, sceneNode});
	mTextNodeRenderer->attachTextToNode(sceneNode, text, [hit, direction](Ogre::Vector2& screenPos) {
		screenPos += (direction * hit->time);
	});

	mHits.emplace_back(std::move(hit));


}


}
}
}