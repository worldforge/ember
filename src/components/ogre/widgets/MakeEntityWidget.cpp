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
#include <Atlas/Objects/Operation.h>
#include <Atlas/Message/Element.h>
#include <wfmath/atlasconv.h>

#include "Widget.h"
#include "DimeOgre.h"
#include "services/DimeServices.h"
#include "services/server/ServerService.h"
#include "services/logging/LoggingService.h"
#include "Avatar.h"



#include "DimeEntity.h"
#include "DimePhysicalEntity.h"
#include "PersonDimeEntity.h"
#include "AvatarDimeEntity.h"

#include "MakeEntityWidget.h"

#include <CEGUIWindow.h>
#include <elements/CEGUIListbox.h> 
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIEditbox.h> 
#include <elements/CEGUIPushButton.h> 

namespace DimeOgre {

MakeEntityWidget::MakeEntityWidget(GUIManager* guiManager) :  Widget::Widget(guiManager)
{

}


MakeEntityWidget::~MakeEntityWidget()
{
}

void MakeEntityWidget::buildWidget()
{

	mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"widgets/MakeEntityWidget.xml", "MakeEntity/");
	mMainWindow->setVisible(false);
	
	mTypeList = static_cast<CEGUI::Listbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MakeEntity/TypeList"));
	mName = static_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MakeEntity/Name"));
	
	CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MakeEntity/CreateButton"));
	
	button->subscribeEvent(CEGUI::ButtonBase::EventMouseClick, 
		boost::bind(&MakeEntityWidget::createButton_Click, this, _1));

	
	

	dime::DimeServices::getInstance()->getServerService()->GotConnection.connect(SigC::slot(*this, &MakeEntityWidget::connectedToServer));
	dime::DimeServices::getInstance()->getServerService()->GotAvatar.connect(SigC::slot(*this, &MakeEntityWidget::gotAvatar));


	getMainSheet()->addChildWindow(mMainWindow); 


}

void MakeEntityWidget::gotAvatar(Eris::Avatar* avatar)
{
	mMainWindow->setVisible(true);
}

void MakeEntityWidget::connectedToServer(Eris::Connection* conn)
{
	mConn = conn;
	Eris::TypeService* typeservice = conn->getTypeService();
	typeservice->BoundType.connect(SigC::slot(*this, &MakeEntityWidget::boundAType));

}

void MakeEntityWidget::boundAType(Eris::TypeInfo* typeInfo)
{
	mTypes.insert(typeInfo);
	
	CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem(typeInfo->getName());
	item->setSelectionBrushImage((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MultiListSelectionBrush");
	item->setUserData(typeInfo);
	
	mTypeList->addItem(item);
}

bool MakeEntityWidget::createButton_Click(const CEGUI::EventArgs& args)
{
	//HACK: low level mucking, this should be possible through Eris
	CEGUI::ListboxItem* item = mTypeList->getFirstSelectedItem();
	Eris::TypeInfo* typeinfo = static_cast<Eris::TypeInfo*>(item->getUserData());
	
	Atlas::Objects::Operation::Create c;
	AvatarDimeEntity* avatar = DimeOgre::getSingleton().getAvatar()->getAvatarDimeEntity();
	c->setFrom(avatar->getId());
	
	Atlas::Message::MapType msg;
	msg["loc"] = avatar->getLocation()->getId();
	
	Ogre::Vector3 o_vector(2,0,0);
	Ogre::Vector3 o_pos = avatar->getSceneNode()->getPosition() + (avatar->getSceneNode()->getOrientation() * o_vector);
	
// 	WFMath::Vector<3> vector(0,2,0);
// 	WFMath::Point<3> pos = avatar->getPosition() + (avatar->getOrientation() * vector);
	WFMath::Point<3> pos = Ogre2Atlas(o_pos);
	
	msg["pos"] = pos.toAtlas();
	msg["name"] = mName->getText().c_str();
	msg["parents"] = Atlas::Message::ListType(1, typeinfo->getName());
	c->setArgsAsList(Atlas::Message::ListType(1, msg));
	mConn->send(c);
	std::cout << "Try to create entity of type " << typeinfo->getName() << " at position " << pos << std::endl;
	
//	dime::LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Try to create entity of type " << typeinfo->getName() << " at position " << pos << LoggingService::END_MESSAGE;
}



};
