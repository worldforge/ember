//
// C++ Implementation: InspectWidget
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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
#include "Widget.h"
#include "../GUIManager.h"
#include "../EmberEntity.h"


#include "InspectWidget.h"
#include "../EmberOgre.h"

#include <CEGUIWindowManager.h>
#include <elements/CEGUIListbox.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIStaticText.h> 

#include <Eris/TypeInfo.h>
#include <elements/CEGUIPushButton.h>
#include <Atlas/Codecs/Bach.h>
#include <Atlas/Message/DecoderBase.h>
#include <Atlas/Objects/Encoder.h>
#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"

namespace EmberOgre {


const std::string InspectWidget::INSPECT("inspect");

class Decoder : public Atlas::Message::DecoderBase {
  private:
    virtual void messageArrived(const Atlas::Message::MapType& obj) {
        m_check = true;
        m_obj = obj;
    }
    bool m_check;
    Atlas::Message::MapType m_obj;
  public:
    Decoder() : m_check (false) { }
    bool check() const { return m_check; }
    const Atlas::Message::MapType & get() {
        m_check = false; return m_obj;
    }
};




/*template<> WidgetLoader WidgetLoaderHolder<InspectWidget>::loader("InspectWidget", &createWidgetInstance);*/
//WidgetLoader Widget::loader("InspectWidget", &createWidgetInstance<InspectWidget>);


InspectWidget::InspectWidget() : mCurrentEntity(0)
{

	Ember::ConsoleBackend::getMainConsole()->registerCommand(INSPECT,this);
	Ember::EmberServices::getSingletonPtr()->getServerService()->GotView.connect(sigc::mem_fun(*this, &InspectWidget::Server_GotView));
}

void InspectWidget::Server_GotView(Eris::View* view)
{
	view->EntityDeleted.connect(sigc::mem_fun(*this, &InspectWidget::View_EntityDeleted));
}

void InspectWidget::View_EntityDeleted(Eris::Entity* entity)
{
	if (entity == mCurrentEntity)
	{
		mCurrentEntity = 0;
	}
}

void InspectWidget::buildWidget()
{
	

	loadMainSheet("InspectWidget.xml", "InspectWidget/");
	mMainWindow->setVisible(false);
//	mMainWindow->setAlwaysOnTop(true);
	
	mChildList = static_cast<CEGUI::Listbox*>(getWindow("ChildList"));
	mInfo = static_cast<CEGUI::StaticText*>(getWindow("EntityInfo"));
	
	
	mGuiManager->EventEntityAction.connect(sigc::mem_fun(*this, &InspectWidget::handleAction));
	enableCloseButton();

	CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(getWindow("ShowOgreBoundingBox"));
	BIND_CEGUI_EVENT(button, CEGUI::ButtonBase::EventMouseClick, InspectWidget::ShowOgreBoundingBox_Click)
	
	button = static_cast<CEGUI::PushButton*>(getWindow("ShowErisBoundingBox"));
	BIND_CEGUI_EVENT(button, CEGUI::ButtonBase::EventMouseClick, InspectWidget::ShowErisBoundingBox_Click);
	
		
/*	EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(SigC::slot(*this, &InventoryWidget::createdAvatarEmberEntity));*/
}

void InspectWidget::runCommand(const std::string &command, const std::string &args)
{
	if(command == INSPECT)
	{
		//the first argument must be a valid entity id
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string entityId = tokeniser.nextToken();
		if (entityId != "") {
			EmberEntity* entity = EmberOgre::getSingleton().getEntity(entityId);
			if (entity != 0) {
				startInspecting(entity);
			}
		} else {
			Ember::ConsoleBackend::getMainConsole()->pushMessage("You must specifify a valid entity id to inspect.");
		}
		
	} else {
		Widget::runCommand(command, args);
	}

}


void InspectWidget::handleAction(const std::string& action, EmberEntity* entity) {

	if (action == "inspect") {
		startInspecting(entity);
	}
}

void InspectWidget::startInspecting(EmberEntity* entity)
{
	mMainWindow->setVisible(true);
	mCurrentEntity = entity;
	showEntityInfo(entity);
}

void InspectWidget::frameStarted(const Ogre::FrameEvent & evt)
{
	if (mMainWindow->isVisible() && mCurrentEntity) {
		showEntityInfo(mCurrentEntity);
	}
}

void InspectWidget::showEntityInfo(EmberEntity* entity)
{
	
	//TODO: restructure this so it won't be done all over each frame, instead cache the values and only update upon changes to the entity
	Eris::Entity* parent = entity->getLocation();
	std::stringstream ss;
	
	ss << "Name: " << entity->getName() << "\n";
	ss << "Id: " << entity->getId() << "\n";
	ss << "Parent: ";
	if (parent) {
		ss << parent->getName() << " (Id: " << parent->getId() << ")";
	} else {
		ss << "none";
	}
	ss << "\n";
	
	ss << "PredPosition: " << entity->getPredictedPos() << "\n";
	ss << "Position: " << entity->getPosition() << "\n";
	WFMath::Vector<3> velocity = entity->getPredictedVelocity();
	if (velocity.isValid()) {
		ss << "Velocity: " << velocity << ": " << sqrt(velocity.sqrMag()) << "\n";
	} else {
		ss << "Velocity: 0\n";
	}
	ss << "Orientation: " << entity->getOrientation() << "\n";
	ss << "Boundingbox: " << entity->getBBox() << "\n";
	
/*	std::set<std::string> parents = entity->getInherits();
	
	ss << "Inherits:\n";
	std::set<std::string>::iterator I = parents.begin();
	std::set<std::string>::iterator I_end = parents.end();
	
	for (; I != I_end; ++I) {
		ss << "    " << *I;
	}*/
	
	ss << "Type: " << entity->getType()->getName() << "\n";
	
	ss << "Attributes:\n";
	
 	const Eris::Entity::AttrMap attributes = entity->getAttributes();
 	for(Eris::Entity::AttrMap::const_iterator I = attributes.begin(); I != attributes.end(); ++I) {
		if (I->second.isString()) {
			ss  << I->first << ": " << I->second.asString() << "\n";
		}
 	}


// 	Decoder bridge;
// 	Atlas::Codecs::Bach codec(ss, bridge);
// 	Atlas::Objects::ObjectsEncoder enc(codec);
// 	
// 	const Eris::Entity::AttrMap attributes = entity->getAttributes();
// 	for(Eris::Entity::AttrMap::const_iterator I = attributes.begin(); I != attributes.end(); ++I) {
// 		codec.streamBegin();
// 
// 		I->second.sendContents(bridge);
// 		//enc.streamObjectsMessage(I->second);
// 		codec.streamEnd();
// 	}



	mInfo->setText(ss.str());
	
	
	unsigned int numberOfChildren = entity->numContained();
	mChildList->resetList();
	
	for (unsigned int i = 0; i < numberOfChildren;  ++i) {
		Eris::Entity* child = entity->getContained(i);
		CEGUI::String name(child->getType()->getName() + " ("+ child->getId() +" : "+child->getName()+")");
		mChildList->addItem(ColoredListItem::createColoredListItem(name));
	}
	
	
	
	
}

bool InspectWidget::ShowOgreBoundingBox_Click(const CEGUI::EventArgs& args)
{
	if (mCurrentEntity) {
		mCurrentEntity->showOgreBoundingBox(!mCurrentEntity->getShowOgreBoundingBox());
	}
	return true;
}

bool InspectWidget::ShowErisBoundingBox_Click(const CEGUI::EventArgs& args)
{
	if (mCurrentEntity) {
		mCurrentEntity->showErisBoundingBox(!mCurrentEntity->getShowErisBoundingBox());
	}
	return true;
}




};
