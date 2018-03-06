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
#include <Eris/View.h>

using namespace Ember;
namespace Ember
{
namespace OgreView
{

namespace Gui
{

EntityCreator::EntityCreator(World& world) :
		mWorld(world),
		mTypeService(mWorld.getView().getTypeService()),
		mRecipe(nullptr),
		mCreationInstance(nullptr),
		mRandomizeOrientation(false),
		mAdapterValueChangedSlot([&](){createNewCreationInstance();})
{
	mTypeService.BoundType.connect(sigc::mem_fun(*this, &EntityCreator::typeService_BoundType));
	mLastOrientation.identity();
}

EntityCreator::~EntityCreator()
{
	stopCreation();
}

void EntityCreator::setRecipe(Authoring::EntityRecipe& recipe)
{
	mRecipe = &recipe;
	checkTypeInfoBound();
}

void EntityCreator::toggleCreateMode()
{
	if (!mCreationInstance) {
		startCreation();
	} else {
		stopCreation();
	}
}

void EntityCreator::setRandomizeOrientation(bool randomize)
{
	mRandomizeOrientation = randomize;
}

void EntityCreator::startCreation()
{
	loadAllTypes();
	// No recipe selected, nothing to do
	if (!mRecipe) {
		return;
	}

	EventCreationStarted();

	createNewCreationInstance();

	Gui::HelpMessage message("EntityCreator", "Click the left mouse button to place the entity. Press Escape to exit from CREATE mode.", "entity creator placement", "entityCreatorMessage");
	Gui::QuickHelp::getSingleton().updateText(message);
}

void EntityCreator::loadAllTypes()
{
	Eris::TypeInfo* typeInfo = mTypeService.getTypeByName("game_entity");
	if (typeInfo) {
		if (typeInfo->hasUnresolvedChildren()) {
			typeInfo->resolveChildren();
		}
	}
}

void EntityCreator::stopCreation()
{
	if (mCreationInstance) {
		mLastOrientation = mCreationInstance->getOrientation();
		delete mCreationInstance;
		mCreationInstance = nullptr;
	}

	EventCreationEnded();
}

void EntityCreator::finalizeCreation()
{
	if (!mCreationInstance) {
		return;
	}
	mLastOrientation = mCreationInstance->getOrientation();
	mCreationInstance->finalizeCreation();

	//Retain offset
	auto offset = mCreationInstance->getMovement()->getBridge()->getOffset();
	createNewCreationInstance();
	mCreationInstance->getMovement()->getBridge()->setOffset(offset);

}

void EntityCreator::checkTypeInfoBound()
{
	if (mRecipe) {
		const std::string& typeName = mRecipe->getEntityType();
		//Calling getTypeByName will also send a request for type info to the server if no type info exists yet
		Eris::TypeInfo* typeInfo = mTypeService.getTypeByName(typeName);
		if (typeInfo) {
			if (typeInfo->isBound()) {
				EventTypeInfoLoaded.emit();
			}
		}
	}
}

void EntityCreator::typeService_BoundType(Eris::TypeInfo* typeInfo)
{
	if (mRecipe) {
		if (typeInfo->getName() == mRecipe->getEntityType()) {
			EventTypeInfoLoaded.emit();
		}
	}
}

void EntityCreator::createNewCreationInstance()
{
	delete mCreationInstance;
	mCreationInstance = nullptr;
	mCreationInstance = new EntityCreatorCreationInstance(mWorld, mTypeService, *mRecipe, mRandomizeOrientation, mAdapterValueChangedSlot);
	mCreationInstance->EventAbortRequested.connect([&](){stopCreation();});
	mCreationInstance->EventFinalizeRequested.connect([&](){finalizeCreation();});
	if (!mRandomizeOrientation) {
		mCreationInstance->setOrientation(mLastOrientation);
	}
	mCreationInstance->startCreation();
}

}
}
}
