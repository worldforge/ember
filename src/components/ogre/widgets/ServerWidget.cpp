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

#include "services/server/ServerService.h"
#include <Eris/Metaserver.h>
#include <Eris/ServerInfo.h>
#include <Eris/Connection.h>
#include "services/EmberServices.h"

#include "Widget.h"
#include "../GUIManager.h"

#include "ServerWidget.h"

#include <elements/CEGUIListbox.h> 
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIStaticText.h> 
#include <elements/CEGUIPushButton.h> 
#include <elements/CEGUIEditbox.h> 
#include <elements/CEGUIMultiLineEditbox.h>
#include <elements/CEGUIRadioButton.h>
#include <elements/CEGUIComboDropList.h> 
#include <elements/CEGUICombobox.h> 


namespace EmberOgre {



/*template<> WidgetLoader WidgetLoaderHolder<ServerWidget>::loader("ServerWidget", &createWidgetInstance);*/
//WidgetLoader Widget::loader("ServerWidget", &createWidgetInstance<ServerWidget>);



ServerWidget::~ServerWidget()
{
}

void ServerWidget::buildWidget()
{


	loadMainSheet("ServerWidget.xml", "Server/");
	mMainWindow->setVisible(false);

	CEGUI::PushButton* login = static_cast<CEGUI::PushButton*>(getWindow("LoginPanel/Login"));
	BIND_CEGUI_EVENT(login, CEGUI::ButtonBase::EventMouseClick, ServerWidget::Login_Click);
	CEGUI::PushButton* createAcc = static_cast<CEGUI::PushButton*>(getWindow("LoginPanel/CreateAcc"));
	BIND_CEGUI_EVENT(createAcc, CEGUI::ButtonBase::EventMouseClick, ServerWidget::CreateAcc_Click);

	mCharacterList = static_cast<CEGUI::Listbox*>(getWindow("ChooseCharacterPanel/CharacterList"));
	CEGUI::PushButton* chooseChar = static_cast<CEGUI::PushButton*>(getWindow("ChooseCharacterPanel/Choose"));
	mCreateChar = static_cast<CEGUI::PushButton*>(getWindow("CreateCharacterPanel/CreateButton"));
	
	BIND_CEGUI_EVENT(chooseChar, CEGUI::ButtonBase::EventMouseClick, ServerWidget::Choose_Click);
	BIND_CEGUI_EVENT(mCreateChar, CEGUI::ButtonBase::EventMouseClick, ServerWidget::CreateChar_Click);
	BIND_CEGUI_EVENT(mCharacterList, CEGUI::ButtonBase::EventMouseDoubleClick, ServerWidget::Choose_Click);
	
	
	mNewCharName = static_cast<CEGUI::Editbox*>(getWindow("CreateCharacterPanel/NameEdit"));
	mNewCharDescription = static_cast<CEGUI::MultiLineEditbox*>(getWindow("CreateCharacterPanel/Description"));
	mTypesList = static_cast<CEGUI::Combobox*>(getWindow("CreateCharacterPanel/Type"));
	
	
	//NOTE: hardcoded! we should get the values from the server somehow
	CEGUI::ListboxItem* item = new ColoredListItem("settler", 0, 0);
	mTypesList->addItem(item);

	
	mGenderRadioButton =  static_cast<CEGUI::RadioButton*>(getWindow("CreateCharacterPanel/Gender/Male"));
	CEGUI::RadioButton* femaleRadioButton =  static_cast<CEGUI::RadioButton*>(getWindow("CreateCharacterPanel/Gender/Female"));
	
	BIND_CEGUI_EVENT(mNewCharName, CEGUI::Editbox::EventTextChanged, ServerWidget::Name_TextChanged);
	BIND_CEGUI_EVENT(mNewCharDescription, CEGUI::Editbox::EventTextChanged, ServerWidget::Description_TextChanged);
	BIND_CEGUI_EVENT(mTypesList, CEGUI::Combobox::EventListSelectionChanged, ServerWidget::TypesList_SelectionChanged);
	BIND_CEGUI_EVENT(mGenderRadioButton, CEGUI::RadioButton::EventSelectStateChanged, ServerWidget::Gender_SelectionChanged);
	BIND_CEGUI_EVENT(femaleRadioButton, CEGUI::RadioButton::EventSelectStateChanged, ServerWidget::Gender_SelectionChanged);
	
	
	updateNewCharacter();
	
	
		
	Ember::EmberServices::getInstance()->getServerService()->GotAccount.connect(SigC::slot(*this, &ServerWidget::createdAccount));
	Ember::EmberServices::getInstance()->getServerService()->LoginSuccess.connect(SigC::slot(*this, &ServerWidget::loginSuccess));
	Ember::EmberServices::getInstance()->getServerService()->GotAvatar.connect(SigC::slot(*this, &ServerWidget::gotAvatar));
	Ember::EmberServices::getInstance()->getServerService()->GotAllCharacters.connect(SigC::slot(*this, &ServerWidget::gotAllCharacters));
	
	mMainWindow->setVisible(false);

	
//	getMainSheet()->addChildWindow(mMainWindow); 

}

void ServerWidget::createdAccount(Eris::Account* account) 
{
	mAccount = account;
	mMainWindow->setVisible(true);
	mMainWindow->moveToFront();
}

void ServerWidget::loginSuccess(Eris::Account* account) 
{
	getWindow("LoginPanel")->setVisible(false);
	getWindow("CharacterTabControl")->setVisible(true);
	account->refreshCharacterInfo();
	
}

void ServerWidget::gotAllCharacters(Eris::Account* account) 
{
	Eris::CharacterMap cm = account->getCharacters();
	Eris::CharacterMap::iterator I = cm.begin();
	Eris::CharacterMap::iterator I_end = cm.end();
	
	for(;I != I_end; ++I) {
		const Atlas::Objects::Entity::GameEntity entity = (*I).second;
		const Atlas::Message::Element nameElement = entity->getAttr("name");
		ColoredListItem* item = new ColoredListItem(nameElement.asString());
		std::string* id = new std::string(entity->getId());
		item->setUserData(id);
		mCharacterList->addItem(item);
	}
	
}

bool ServerWidget::Choose_Click(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mCharacterList->getFirstSelectedItem();
	if (item) {
/*	const Atlas::Objects::Entity::GameEntity & entity = static_cast<const Atlas::Objects::Entity::GameEntity &>(item->getUserData());*/
	
		std::string* id = static_cast<std::string*>(item->getUserData());
		
		Ember::EmberServices::getInstance()->getServerService()->takeCharacter(*id);
	}
	return true;
}

bool ServerWidget::CreateChar_Click(const CEGUI::EventArgs& args)
{
		
	Ember::EmberServices::getInstance()->getServerService()->createCharacter(mNewChar.name, mNewChar.gender, mNewChar.type, mNewChar.description);
	return true;
}

bool ServerWidget::TypesList_SelectionChanged(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mTypesList->getSelectedItem();
	if (item) {
	
		std::string type = item->getText().c_str();
		mNewChar.type = type;
	}
	updateNewCharacter();
	return true;
}
bool ServerWidget::Gender_SelectionChanged(const CEGUI::EventArgs& args)
{
	CEGUI::RadioButton* selected = mGenderRadioButton->getSelectedButtonInGroup();
	mNewChar.gender = selected->getText().c_str();
	
	updateNewCharacter();
	return true;
}
bool ServerWidget::Name_TextChanged(const CEGUI::EventArgs& args)
{
	std::string name = mNewCharName->getText().c_str();
	mNewChar.name = name;
	updateNewCharacter();

	return true;
}
bool ServerWidget::Description_TextChanged(const CEGUI::EventArgs& args)
{
	std::string description = mNewCharDescription->getText().c_str();
	mNewChar.description = description;
	updateNewCharacter();
	return true;
}

void ServerWidget::updateNewCharacter()
{
	mCreateChar->setEnabled(mNewChar.isValid());
}




bool ServerWidget::Login_Click(const CEGUI::EventArgs& args)
{
	CEGUI::Window* nameBox = getWindow("LoginPanel/NameEdit");
	CEGUI::Window* passwordBox = getWindow("LoginPanel/PasswordEdit");
	
	CEGUI::String name = nameBox->getText();
	CEGUI::String password = passwordBox->getText();
	
	mAccount->login(std::string(name.c_str()), std::string(password.c_str()));

	return true;
}

bool ServerWidget::CreateAcc_Click(const CEGUI::EventArgs& args)
{
	CEGUI::Window* nameBox = getWindow("LoginPanel/NameEdit");
	CEGUI::Window* passwordBox = getWindow("LoginPanel/PasswordEdit");
	
	CEGUI::String name = nameBox->getText();
	CEGUI::String password = passwordBox->getText();
	
	mAccount->createAccount(std::string(name.c_str()),std::string(name.c_str()),std::string(password.c_str()));
	return true;
}

void ServerWidget::gotAvatar(Eris::Avatar* avatar) 
{
	mMainWindow->setVisible(false);
/*	mGuiManager->removeWidget(this);
	delete this;*/
}



};

bool EmberOgre::NewCharacter::isValid( ) const
{
	return name != "" && gender != "" && type != "";
}
