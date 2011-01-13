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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityCreator.h"
#include "EntityCreatorCreationInstance.h"

#include "components/ogre/World.h"

//#include "../GUIManager.h"
//#include "components/ogre/widgets/adapters/atlas/AdapterFactory.h"

#include "QuickHelp.h"

#include "components/ogre/authoring/EntityRecipe.h"


#include <Atlas/Message/Element.h>
#include <Eris/TypeInfo.h>
#include <Eris/Connection.h>
#include <Eris/Avatar.h>
#include <Eris/View.h>

//#include <CEGUIWindow.h>
#include <wfmath/axisbox.h>

using namespace Ember;
namespace Ember
{
namespace OgreView
{

namespace Gui
{

EntityCreator::EntityCreator(World& world) :
	mWorld(world), mTypeService(*mWorld.getView().getAvatar()->getConnection()->getTypeService()), mRecipe(0), mCreationInstance(0), mAdapterValueChangedSlot(sigc::mem_fun(*this, &EntityCreator::adapterValueChanged))
{
	mTypeService.BoundType.connect(sigc::mem_fun(*this, &EntityCreator::typeService_BoundType));

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

	mWidget->getMainWindow()->setAlpha(0.6);

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
	delete mCreationInstance;
	mCreationInstance = 0;

	mWidget->getMainWindow()->setAlpha(1.0);
}

void EntityCreator::finalizeCreation()
{
	if (!mCreationInstance) {
		return;
	}
	mCreationInstance->finalizeCreation();

	createNewCreationInstance();

}

void EntityCreator::checkTypeInfoBound()
{
	if (mRecipe) {
		const std::string& typeName = mRecipe->getEntityType();
		///Calling getTypeByName will also send a request for type info to the server if no type info exists yet
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

void EntityCreator::creationInstance_AbortRequested()
{
	stopCreation();
}

void EntityCreator::creationInstance_FinalizeRequested()
{
	finalizeCreation();
}

void EntityCreator::adapterValueChanged()
{
	createNewCreationInstance();
}

void EntityCreator::createNewCreationInstance()
{
	delete mCreationInstance;
	mCreationInstance = 0;
	mCreationInstance = new EntityCreatorCreationInstance(mWorld, mTypeService, *mRecipe, mRandomizeOrientation, mAdapterValueChangedSlot);
	mCreationInstance->EventAbortRequested.connect(sigc::mem_fun(*this, &EntityCreator::creationInstance_AbortRequested));
	mCreationInstance->EventFinalizeRequested.connect(sigc::mem_fun(*this, &EntityCreator::creationInstance_FinalizeRequested));
	mCreationInstance->startCreation();
}


}
}
}
