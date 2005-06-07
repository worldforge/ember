//
// C++ Implementation: MakeEntityWidget
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
#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include <Atlas/Objects/Operation.h>
#include <Atlas/Message/Element.h>
#include <wfmath/atlasconv.h>

#include "Widget.h"
#include "../EmberOgre.h"
#include "services/logging/LoggingService.h"
#include "../Avatar.h"



#include "../EmberEntity.h"
#include "../EmberPhysicalEntity.h"
#include "../PersonEmberEntity.h"
#include "../AvatarEmberEntity.h"

#include "MakeEntityWidget.h"

#include <CEGUIWindow.h>
#include <elements/CEGUIListbox.h> 
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIEditbox.h> 
#include <elements/CEGUIPushButton.h> 
#include "framework/ConsoleBackend.h"

namespace EmberOgre {

/*template<> WidgetLoader WidgetLoaderHolder<MakeEntityWidget>::loader("MakeEntityWidget", &createWidgetInstance);*/
//WidgetLoader Widget::loader("MakeEntityWidget", &createWidgetInstance<MakeEntityWidget>);

const char * const MakeEntityWidget::CREATEENTITY= "createentity";

MakeEntityWidget::MakeEntityWidget()
 : mIsReady(false), Widget() 
{

	Ember::ConsoleBackend::getMainConsole()->registerCommand(CREATEENTITY,this);

}

	
MakeEntityWidget::~MakeEntityWidget()
{
}

void MakeEntityWidget::buildWidget()
{

	loadMainSheet("MakeEntityWidget.xml", "MakeEntity/");
	//mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"cegui/widgets/MakeEntityWidget.xml", "MakeEntity/");
	//mMainWindow->setVisible(false);
	
	mTypeList = static_cast<CEGUI::Listbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MakeEntity/TypeList"));
	mName = static_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MakeEntity/Name"));
	
	CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MakeEntity/CreateButton"));
	
	BIND_CEGUI_EVENT(button, CEGUI::ButtonBase::EventMouseClick,MakeEntityWidget::createButton_Click );

	
	

	Ember::EmberServices::getInstance()->getServerService()->GotConnection.connect(SigC::slot(*this, &MakeEntityWidget::connectedToServer));
	Ember::EmberServices::getInstance()->getServerService()->GotAvatar.connect(SigC::slot(*this, &MakeEntityWidget::gotAvatar));

	//getMainSheet()->addChildWindow(mMainWindow); 

	registerConsoleVisibilityToggleCommand("entitycreator");
	enableCloseButton();

}




void MakeEntityWidget::show()
{
	if (mIsReady)
	{
		if (mMainWindow) 
			mMainWindow->setVisible(true);
		S_LOG_INFO("Showing entity creator window.");
	} else {
		S_LOG_FAILURE("Can't show entity creator window before we're and have taken an avatar.");
	}
}



void MakeEntityWidget::gotAvatar(Eris::Avatar* avatar)
{
// 	loadAllTypes();
	mIsReady = true;
}





void MakeEntityWidget::loadAllTypes()
{
	Eris::TypeService* typeservice = mConn->getTypeService();
	Eris::TypeInfo* typeInfo = typeservice->getTypeByName("game_entity");
	addToList(typeInfo, 0);

}

void MakeEntityWidget::connectedToServer(Eris::Connection* conn)
{
	mConn = conn;
 	Eris::TypeService* typeservice = conn->getTypeService();
 	typeservice->BoundType.connect(SigC::slot(*this, &MakeEntityWidget::boundAType));

}

void MakeEntityWidget::addToList(Eris::TypeInfo* typeInfo, int level)
{
	std::stringstream levelindicator;
	for (int i = 0; i < level; ++i) {
		levelindicator << "-";
	}
	CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem(levelindicator.str() + typeInfo->getName());
	item->setSelectionBrushImage((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MultiListSelectionBrush");
	item->setUserData(typeInfo);
	mTypeList->addItem(item);
	
	if (typeInfo->hasUnresolvedChildren())
		typeInfo->resolveChildren();
	
	const Eris::TypeInfoSet children = typeInfo->getChildren();
	Eris::TypeInfoSet::const_iterator I = children.begin();
	Eris::TypeInfoSet::const_iterator I_end = children.end();
	
	for (;I != I_end; ++I)
	{
		addToList(*I, level+1);
	}
	
}

void MakeEntityWidget::boundAType(Eris::TypeInfo* typeInfo)
{
	std::stringstream levelindicator;
	Eris::TypeInfo* parentType;
	if (typeInfo->getParents().size()) {
		parentType = *typeInfo->getParents().begin();
		while (parentType) {
			levelindicator << "-";
			if (parentType->getParents().size()) {
				parentType = *parentType->getParents().begin();
			} else {
				parentType = 0;
			}
		}
	}
	
	CEGUI::ListboxTextItem* item = new ColoredListItem(levelindicator.str() + typeInfo->getName());
	item->setUserData(typeInfo);
	mTypes[typeInfo] = item;
	
	if (mTypes.size() == 0) {
		mTypeList->addItem(item);
	} else {
		CEGUI::ListboxItem* parentListItem = mTypes[*typeInfo->getParents().begin()];
		mTypeList->insertItem(item, parentListItem);
	}
	if (typeInfo->hasUnresolvedChildren())
		typeInfo->resolveChildren();
	
	
	//item->setSelectionBrushImage((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MultiListSelectionBrush");
	
}

void MakeEntityWidget::runCommand(const std::string &command, const std::string &args)
{
	if(command == CREATEENTITY)
	{
		Eris::TypeService* typeService = mConn->getTypeService();
		Eris::TypeInfo* typeinfo = typeService->getTypeByName(args);
		if (typeinfo) {
			createEntityOfType(typeinfo);
		}
	} else {
		Widget::runCommand(command, args);
	}

}


bool MakeEntityWidget::createButton_Click(const CEGUI::EventArgs& args)
{
	//HACK: low level mucking, this should be possible through Eris
	CEGUI::ListboxItem* item = mTypeList->getFirstSelectedItem();
	if (item) {
		Eris::TypeInfo* typeinfo = static_cast<Eris::TypeInfo*>(item->getUserData());
		createEntityOfType(typeinfo);
		
	}
	
//	Ember::LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Try to create entity of type " << typeinfo->getName() << " at position " << pos << LoggingService::END_MESSAGE;
}

void MakeEntityWidget::createEntityOfType(Eris::TypeInfo* typeinfo)
{
	Atlas::Objects::Operation::Create c;
	AvatarEmberEntity* avatar = EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity();
	c->setFrom(avatar->getId());
	
	Atlas::Message::MapType msg;
	msg["loc"] = avatar->getLocation()->getId();
	
	Ogre::Vector3 o_vector(2,0,0);
	Ogre::Vector3 o_pos = avatar->getSceneNode()->getPosition() + (avatar->getSceneNode()->getOrientation() * o_vector);
	
// 	WFMath::Vector<3> vector(0,2,0);
// 	WFMath::Point<3> pos = avatar->getPosition() + (avatar->getOrientation() * vector);
	WFMath::Point<3> pos = Ogre2Atlas(o_pos);
	WFMath::Quaternion orientation = avatar->getOrientation();
	
	msg["pos"] = pos.toAtlas();
	msg["name"] = mName->getText().c_str();
	msg["parents"] = Atlas::Message::ListType(1, typeinfo->getName());
	msg["orientation"] = orientation.toAtlas();
	
	c->setArgsAsList(Atlas::Message::ListType(1, msg));
	mConn->send(c);
	std::cout << "Try to create entity of type " << typeinfo->getName() << " at position " << pos << std::endl;

}


};
