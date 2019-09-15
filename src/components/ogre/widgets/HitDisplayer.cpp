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

#include "IngameChatWidget.h"
#include "../Convert.h"
#include "../model/ModelRepresentation.h"
#include "../model/Model.h"

#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/Avatar.h>

#include <Atlas/Objects/RootOperation.h>
#include <wfmath/MersenneTwister.h>

#include <memory>

namespace Ember {
namespace OgreView {

namespace Gui {
HitDisplayer::HitDisplayer(Eris::View& view, Ogre::SceneManager& sceneManager)
		: mView(view),
		  mSceneManager(sceneManager) {

	Ogre::Root::getSingleton().addFrameListener(this);

	view.EntitySeen.connect([this](Eris::Entity* entity) {
		entity->Acted.connect([this, entity](const Atlas::Objects::Operation::RootOperation& arg) {
			entityActed(*entity, arg);
		});
	});
}

HitDisplayer::~HitDisplayer() {
	Ogre::Root::getSingleton().removeFrameListener(this);
}

void HitDisplayer::entityActed(Eris::Entity& entity, const Atlas::Objects::Operation::RootOperation& act) {
	if (act->getParent() == "hit" && !act->isDefaultTo()) {
		//Hits are special, since we need to check the "to" instead.
		auto hitEntity = mView.getEntity(act->getTo());
		if (hitEntity) {
			if (!act->getArgs().empty()) {
				auto& arg = act->getArgs().front();
				if (arg->hasAttr("damage")) {
					auto damageElem = arg->getAttr("damage");
					if (damageElem.isNum()) {
						std::stringstream ss;
						ss.precision(2);
						ss << damageElem.asNum();
						auto damageString = ss.str();
						auto pos = Convert::toOgre(hitEntity->getPredictedPos());
						auto model = Model::ModelRepresentation::getModelForEntity(*static_cast<EmberEntity*>(hitEntity));
						if (model && model->isLoaded()) {
							pos += model->getCombinedBoundingBox().getCenter();
						}
						createHit(pos, damageString);
					}
				}
			}
		}

	}
}

bool HitDisplayer::frameStarted(const Ogre::FrameEvent& evt) {

	for (auto I = mHits.begin(); I != mHits.end(); ++I) {
		auto hit = *I;
		hit->time += evt.timeSinceLastFrame;
		if (hit->time >= 2.0) {
			if (IngameChatWidget::sDetachTextNode) {
				IngameChatWidget::sDetachTextNode(hit->node);
			}
			I = mHits.erase(I);
		}
	}
	return true;
}

void HitDisplayer::createHit(const Ogre::Vector3& pos, std::string text) {
	if (IngameChatWidget::sAttachTextToNode) {
		auto sceneNode = mSceneManager.getRootSceneNode()->createChildSceneNode(pos);

		auto x = WFMath::MTRand::instance.rand() - 0.5f;

		Ogre::Vector2 direction(x * 0.01f, -0.1f);

		auto hit = std::make_shared<Hit>(Hit{0.f, sceneNode});
		IngameChatWidget::sAttachTextToNode(sceneNode, std::move(text), [hit, direction](Ogre::Vector2& screenPos) {
			screenPos += (direction * hit->time);
		});

		mHits.emplace_back(std::move(hit));
	}

}

}
}
}