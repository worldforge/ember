//
// C++ Implementation: InspectWidget
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2004
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

#include "InspectWidget.h"

#include "ColouredListItem.h"
#include "../GUIManager.h"
#include "domain/EmberEntity.h"
#include "../World.h"
#include "components/ogre/authoring/AuthoringManager.h"


#include "../EmberOgre.h"
#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"

#include <CEGUI/Window.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/widgets/Listbox.h>
#include <CEGUI/widgets/PushButton.h>

#include <Eris/TypeInfo.h>

#include <Atlas/PresentationBridge.h>


namespace Ember {
namespace OgreView {
namespace Gui {


InspectWidget::InspectWidget() :
Inspect("inspect", this, "Inspect an entity."),
mChildList(0), mInfo(0), mCurrentEntity(0), mChangedThisFrame(false)
{
}
InspectWidget::~InspectWidget()
{
}


void InspectWidget::entity_BeingDeleted()
{
	disconnectFromEntity();
	mCurrentEntity = 0;
}

void InspectWidget::buildWidget()
{


	loadMainSheet("InspectWidget.layout", "InspectWidget/");
	mMainWindow->setVisible(false);
//	mMainWindow->setAlwaysOnTop(true);

	mChildList = static_cast<CEGUI::Listbox*>(getWindow("ChildList"));
	BIND_CEGUI_EVENT(mChildList, CEGUI::Listbox::EventMouseDoubleClick, InspectWidget::ChildList_MouseDoubleClick);

	mInfo = getWindow("EntityInfo");


	mGuiManager->EventEntityAction.connect(sigc::mem_fun(*this, &InspectWidget::handleAction));
	enableCloseButton();

	if (CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(getWindow("ShowOgreBoundingBox"))) {
		BIND_CEGUI_EVENT(button, CEGUI::PushButton::EventClicked, InspectWidget::ShowOgreBoundingBox_Click);
	}

	if (CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(getWindow("ShowErisBoundingBox"))) {
		BIND_CEGUI_EVENT(button, CEGUI::PushButton::EventClicked, InspectWidget::ShowErisBoundingBox_Click);
	}

	if (CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(getWindow("ShowCollision"))) {
		BIND_CEGUI_EVENT(button, CEGUI::PushButton::EventClicked, InspectWidget::ShowCollision_Click);
	}

}

void InspectWidget::updateAttributeString()
{
	std::stringstream ss;

	Atlas::PresentationBridge bridge(ss);
	bridge.setMaxItemsPerLevel(15);
	Atlas::Message::Encoder encoder(bridge);
	encoder.streamMessageElement(mCurrentEntity->getAttributes());

	mAttributesString = ss.str();
}

void InspectWidget::runCommand(const std::string &command, const std::string &args)
{
	if(Inspect == command && EmberOgre::getSingleton().getWorld())
	{
		//the first argument must be a valid entity id
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string entityId = tokeniser.nextToken();
		if (entityId != "") {
			EmberEntity* entity = EmberOgre::getSingleton().getWorld()->getEmberEntity(entityId);
			if (entity != 0) {
				startInspecting(entity);
			}
		} else {
			ConsoleBackend::getSingletonPtr()->pushMessage("You must specifify a valid entity id to inspect.", "error");
		}

	} else {
		Widget::runCommand(command, args);
	}

}

void InspectWidget::disconnectFromEntity()
{
	mChangedConnection.disconnect();
	mChildAddedConnection.disconnect();
	mChildRemovedConnection.disconnect();
}

void InspectWidget::handleAction(const std::string& action, EmberEntity* entity) {

	if (action == "inspect") {
		startInspecting(entity);
	}
}

void InspectWidget::startInspecting(EmberEntity* entity)
{
	disconnectFromEntity();

	mChangedThisFrame = true;

	show();

	mCurrentEntity = entity;
	updateAttributeString();
	showEntityInfo(entity);

	mChangedConnection = entity->Changed.connect(sigc::mem_fun(*this, &InspectWidget::entity_Changed));
	mChildAddedConnection = entity->ChildAdded.connect(sigc::mem_fun(*this, &InspectWidget::entity_ChildAdded));
	mChildRemovedConnection = entity->ChildRemoved.connect(sigc::mem_fun(*this, &InspectWidget::entity_ChildRemoved));
	mBeingDeletedConnection = entity->BeingDeleted.connect(sigc::mem_fun(*this, &InspectWidget::entity_BeingDeleted));


	fillChildrenList();

}

void InspectWidget::frameStarted(const Ogre::FrameEvent & evt)
{
	if (mMainWindow->isVisible() && mCurrentEntity && mChangedThisFrame) {
		showEntityInfo(mCurrentEntity);
	}
}

void InspectWidget::showEntityInfo(EmberEntity* entity)
{
	Eris::Entity* parent = entity->getLocation();
	std::stringstream ss;
	ss.precision(4);

	ss << "Name: " << entity->getName() << "\n";
	ss << "Id: " << entity->getId() << "\n";
	ss << "Parent: ";
	if (parent) {
		ss << parent->getName() << " (Id: " << parent->getId() << ")";
	} else {
		ss << "none";
	}
	ss << "\n";

	if (entity->getPredictedPos().isValid()) {
		ss << "PredPosition: " << entity->getPredictedPos() << "\n";
	}
	if (entity->getPosition().isValid()) {
		ss << "Position: " << entity->getPosition() << "\n";
	}
	WFMath::Vector<3> velocity = entity->getPredictedVelocity();
	if (velocity.isValid()) {
		ss << "Velocity: " << velocity << ": " << sqrt(velocity.sqrMag()) << "\n";
	}

	if (entity->getOrientation().isValid()) {
		ss << "Orientation: " << entity->getOrientation() << "\n";
	}
	if (entity->getBBox().isValid()) {
		ss << "Boundingbox: " << entity->getBBox() << "\n";
	}

	ss << "Type: " << entity->getType()->getName() << "\n";

	ss << "Attributes:\n";

	ss << mAttributesString;

	mInfo->setText(ss.str());
	mChangedThisFrame = false;

}

void InspectWidget::fillChildrenList()
{
	unsigned int numberOfChildren = mCurrentEntity->numContained();
	mChildList->resetList();

	for (unsigned int i = 0; i < numberOfChildren;  ++i) {
		Eris::Entity* child = mCurrentEntity->getContained(i);
		addChildToList(child);
	}

}

void InspectWidget::addChildToList(Eris::Entity* child)
{
	CEGUI::String name(child->getType()->getName());
	if (child->getName() == "") {
		name += " ("+ child->getId() + ")";
	} else {
		name += " ("+ child->getId() +" : "+child->getName()+")";
	}
	if (!child->isVisible()) {
		name += " (not visible)";
	}
	CEGUI::ListboxItem* item = Gui::ColouredListItem::createColouredListItem(name);
	item->setUserData(child);
	mChildList->addItem(item);
}

void InspectWidget::entity_ChildAdded(Eris::Entity* entity)
{
	addChildToList(entity);
}

void InspectWidget::entity_ChildRemoved(Eris::Entity* entity)
{
	for (unsigned int i = 0; i < mChildList->getItemCount(); ++i) {
		CEGUI::ListboxItem* item = mChildList->getListboxItemFromIndex(i);
		if (item->getUserData() == entity) {
			mChildList->removeItem(item);
			break;
		}
	}
}



void InspectWidget::entity_Changed(const Eris::StringSet& attributes)
{
	updateAttributeString();
	mChangedThisFrame = true;
}


bool InspectWidget::ShowCollision_Click(const CEGUI::EventArgs& args)
{
	if (mCurrentEntity) {
		mCurrentEntity->setVisualize("CollisionObject", !mCurrentEntity->getVisualize("CollisionObject"));
	}
	return true;
}


bool InspectWidget::ShowOgreBoundingBox_Click(const CEGUI::EventArgs& args)
{
	if (mCurrentEntity) {
		mCurrentEntity->setVisualize("OgreBBox", !mCurrentEntity->getVisualize("OgreBBox"));
	}
	return true;
}

bool InspectWidget::ShowErisBoundingBox_Click(const CEGUI::EventArgs& args)
{
	if (mCurrentEntity && EmberOgre::getSingleton().getWorld()) {
		if (EmberOgre::getSingleton().getWorld()->getAuthoringManager().hasSimpleEntityVisualization(*mCurrentEntity)) {
			EmberOgre::getSingleton().getWorld()->getAuthoringManager().hideSimpleEntityVisualization(*mCurrentEntity);
		} else {
			EmberOgre::getSingleton().getWorld()->getAuthoringManager().displaySimpleEntityVisualization(*mCurrentEntity);
		}
	}
	return true;
}


bool InspectWidget::ChildList_MouseDoubleClick(const CEGUI::EventArgs& args)
{
	//Inspect the child entity
	CEGUI::ListboxItem* item = mChildList->getFirstSelectedItem();
	if (item) {
		startInspecting(static_cast<EmberEntity*>(item->getUserData()));
	}

	return true;
}




}
}
}
