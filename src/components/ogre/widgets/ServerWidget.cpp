//
// C++ Implementation: ServerWidget
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

#include <Eris/Metaserver.h>
#include <Eris/ServerInfo.h>
#include <Eris/Utils.h>
#include <Eris/Connection.h>
#include "services/DimeServices.h"
#include "services/server/ServerService.h"

#include "Widget.h"
#include "GUIManager.h"

#include "ServerWidget.h"

namespace DimeOgre {

class ServerWidgetListItem : public CEGUI::ListboxTextItem
{
public:
	ServerWidgetListItem(const CEGUI::String& text) : ListboxTextItem(text)
	{
		setSelectionBrushImage((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MultiListSelectionBrush");
	}
};


ServerWidget::ServerWidget(GUIManager* guiManager) :  Widget::Widget(guiManager)
{
}


ServerWidget::~ServerWidget()
{
}

void ServerWidget::buildWidget()
{


	mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"widgets/ServerWidget.xml");

	CEGUI::PushButton* login = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Server/LoginPanel/Login"));
	BIND_CEGUI_EVENT(login, CEGUI::ButtonBase::EventMouseClick, ServerWidget::Login_Click);

	mCharacterList = static_cast<CEGUI::Listbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Server/ChooseCharacterPanel/CharacterList"));
	CEGUI::PushButton* choose = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Server/ChooseCharacterPanel/Choose"));
	
	BIND_CEGUI_EVENT(choose, CEGUI::ButtonBase::EventMouseClick, ServerWidget::Choose_Click);
	
		
	dime::DimeServices::getInstance()->getServerService()->GotPlayer.connect(SigC::slot(*this, &ServerWidget::createdPlayer));
	dime::DimeServices::getInstance()->getServerService()->LoginSuccess.connect(SigC::slot(*this, &ServerWidget::loginSuccess));
	dime::DimeServices::getInstance()->getServerService()->GotAvatar.connect(SigC::slot(*this, &ServerWidget::gotAvatar));
	dime::DimeServices::getInstance()->getServerService()->GotAllCharacters.connect(SigC::slot(*this, &ServerWidget::gotAllCharacters));
	
	mMainWindow->setVisible(false);

	
	getMainSheet()->addChildWindow(mMainWindow); 

}

void ServerWidget::createdPlayer(Eris::Player* player) 
{
	mPlayer = player;
	mMainWindow->setVisible(true);
}

void ServerWidget::loginSuccess(Eris::Player* player) 
{
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Server/LoginPanel")->setVisible(false);
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Server/ChooseCharacterPanel")->setVisible(true);
	player->refreshCharacterInfo();
	
}

void ServerWidget::gotAllCharacters(Eris::Player* player) 
{
	Eris::CharacterMap cm = player->getCharacters();
	Eris::CharacterMap::iterator I = cm.begin();
	Eris::CharacterMap::iterator I_end = cm.end();
	
	for(;I != I_end; ++I) {
		const Atlas::Objects::Entity::GameEntity entity = (*I).second;
		const Atlas::Message::Element nameElement = entity.getAttr("name");
		ServerWidgetListItem* item = new ServerWidgetListItem(nameElement.asString());
		std::string id = std::string(entity.getId());
		item->setUserData(&(id));
		mCharacterList->addItem(item);
	}
	
}

bool ServerWidget::Choose_Click(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mCharacterList->getFirstSelectedItem();
/*	const Atlas::Objects::Entity::GameEntity & entity = static_cast<const Atlas::Objects::Entity::GameEntity &>(item->getUserData());*/
	
	std::string* id = static_cast<std::string*>(item->getUserData());
	
	dime::DimeServices::getInstance()->getServerService()->takeCharacter(*id);
}


bool ServerWidget::Login_Click(const CEGUI::EventArgs& args)
{
	CEGUI::Window* nameBox = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Server/LoginPanel/NameEdit");
	CEGUI::Window* passwordBox = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Server/LoginPanel/PasswordEdit");
	
	CEGUI::String name = nameBox->getText();
	CEGUI::String password = passwordBox->getText();
	
	mPlayer->login(std::string(name.c_str()), std::string(password.c_str()));

}

void ServerWidget::gotAvatar(Eris::Avatar* avatar) 
{
	mGuiManager->removeWidget(this);
	delete this;
}



};
