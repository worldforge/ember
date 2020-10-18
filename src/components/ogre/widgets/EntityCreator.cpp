//
// C++ Implementation: EntityCreator
//
// Description:
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2008
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

#include "EntityCreator.h"
#include "EntityCreatorCreationInstance.h"
#include "EntityCreatorMovement.h"
#include "EntityCreatorMovementBridge.h"

#include "components/ogre/World.h"

#include "QuickHelp.h"

#include "components/ogre/authoring/EntityRecipe.h"

#include <Eris/TypeInfo.h>
#include <Eris/TypeService.h>
#include <Eris/View.h>

using namespace Ember;
namespace Ember {
namespace OgreView {

namespace Gui {

EntityCreator::EntityCreator(World& world) :
		mWorld(world),
		mTypeService(mWorld.getView().getTypeService()),
		mCreationInstance(nullptr),
		mRandomizeOrientation(false) {
	mLastOrientation.identity();
}

EntityCreator::~EntityCreator() {
	stopCreation();
}

void EntityCreator::setRandomizeOrientation(bool randomize) {
	mRandomizeOrientation = randomize;
}


void EntityCreator::startCreation(const std::vector<Atlas::Message::MapType>& entityMaps) {
	loadAllTypes();

	EventCreationStarted();

	createNewCreationInstance(entityMaps);

	Gui::HelpMessage message("EntityCreator", "Click the left mouse button to place the entity. Press Escape to exit from CREATE mode.", "entity creator placement", "entityCreatorMessage");
	Gui::QuickHelp::getSingleton().updateText(message);
}

void EntityCreator::loadAllTypes() {
	Eris::TypeInfo* typeInfo = mTypeService.getTypeByName("game_entity");
	if (typeInfo) {
		if (typeInfo->hasUnresolvedChildren()) {
			typeInfo->resolveChildren();
		}
	}
}

void EntityCreator::stopCreation() {
	if (mCreationInstance) {
		mLastOrientation = mCreationInstance->getOrientation();
		mCreationInstance.reset();
	}

	EventCreationEnded();
}

void EntityCreator::finalizeCreation() {
	if (!mCreationInstance) {
		return;
	}
	mLastOrientation = mCreationInstance->getOrientation();
	mCreationInstance->finalizeCreation();

	//Retain offset
	auto offset = mCreationInstance->getMovement()->getBridge()->getOffset();
	mCreationInstance.reset();
	//createNewCreationInstance(mCreationInstance->getAdapterValues());
	//mCreationInstance->getMovement()->getBridge()->setOffset(offset);
	EventCreationCompleted();
}

void EntityCreator::createNewCreationInstance(const std::vector<Atlas::Message::MapType>& entityMaps) {
	mCreationInstance.reset();

	mCreationInstance = std::make_unique<EntityCreatorCreationInstance>(mWorld, mTypeService, entityMaps, mRandomizeOrientation);
	mCreationInstance->EventMoved.connect([&](Eris::Entity* entity, const WFMath::Point<3>& pos) { EventMoved(entity, pos); });
	mCreationInstance->EventAbortRequested.connect([&]() { stopCreation(); });
	mCreationInstance->EventFinalizeRequested.connect([&]() { finalizeCreation(); });
	if (!mRandomizeOrientation) {
		mCreationInstance->setOrientation(mLastOrientation);
	}
	mCreationInstance->startCreation();
}

}
}
}
