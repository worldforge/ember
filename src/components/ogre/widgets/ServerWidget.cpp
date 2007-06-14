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
#include <Eris/TypeInfo.h>
#include <Eris/TypeService.h>
#include "services/EmberServices.h"

#include "Widget.h"
#include "ColouredListItem.h"
#include "../GUIManager.h"

#include "ServerWidget.h"

#include <elements/CEGUIListbox.h> 
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIPushButton.h> 
#include <elements/CEGUIEditbox.h> 
#include <elements/CEGUIMultiLineEditbox.h>
#include <elements/CEGUIRadioButton.h>
#include <elements/CEGUIComboDropList.h> 
#include <elements/CEGUICombobox.h> 
#include <elements/CEGUITabControl.h> 
#include <elements/CEGUIGUISheet.h> 

#include "ModelRenderer.h"

#include "components/ogre/model/mapping/ModelMappingManager.h"
#include "components/ogre/model/mapping/ModelMapping.h"
#include "components/ogre/model/mapping/Definitions/ModelMappingDefinition.h"
#include "components/ogre/model/mapping/Definitions/MatchDefinition.h"
#include "components/ogre/model/mapping/Definitions/CaseDefinition.h"
#include "components/ogre/model/mapping/Definitions/ActionDefinition.h"
#include "components/ogre/model/mapping/EmberModelMappingManager.h"


using namespace CEGUI;
namespace EmberOgre {



/*template<> WidgetLoader WidgetLoaderHolder<ServerWidget>::loader("ServerWidget", &createWidgetInstance);*/
//WidgetLoader Widget::loader("ServerWidget", &createWidgetInstance<ServerWidget>);

ServerWidget::ServerWidget() : mModelPreviewRenderer(0)
{
}


ServerWidget::~ServerWidget()
{
	delete mModelPreviewRenderer;
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
	mUseCreator =  static_cast<CEGUI::PushButton*>(getWindow("UseCreator"));
	mCreateChar = static_cast<CEGUI::PushButton*>(getWindow("CreateCharacterPanel/CreateButton"));
	
	BIND_CEGUI_EVENT(chooseChar, CEGUI::ButtonBase::EventMouseClick, ServerWidget::Choose_Click);
	BIND_CEGUI_EVENT(mUseCreator, CEGUI::ButtonBase::EventMouseClick, ServerWidget::UseCreator_Click);
	BIND_CEGUI_EVENT(mCreateChar, CEGUI::ButtonBase::EventMouseClick, ServerWidget::CreateChar_Click);
	BIND_CEGUI_EVENT(mCharacterList, CEGUI::ButtonBase::EventMouseDoubleClick, ServerWidget::Choose_Click);
	
	mNewCharName = static_cast<CEGUI::Editbox*>(getWindow("CreateCharacterPanel/NameEdit"));
	mNewCharDescription = static_cast<CEGUI::MultiLineEditbox*>(getWindow("CreateCharacterPanel/Description"));
	mTypesList = static_cast<CEGUI::Combobox*>(getWindow("CreateCharacterPanel/Type"));
    
	mGenderRadioButton =  static_cast<CEGUI::RadioButton*>(getWindow("CreateCharacterPanel/Gender/Male"));
	CEGUI::RadioButton* femaleRadioButton =  static_cast<CEGUI::RadioButton*>(getWindow("CreateCharacterPanel/Gender/Female"));
	
	BIND_CEGUI_EVENT(mNewCharName, CEGUI::Editbox::EventTextChanged, ServerWidget::Name_TextChanged);
	BIND_CEGUI_EVENT(mNewCharDescription, CEGUI::Editbox::EventTextChanged, ServerWidget::Description_TextChanged);
	BIND_CEGUI_EVENT(mTypesList, CEGUI::Combobox::EventListSelectionChanged, ServerWidget::TypesList_SelectionChanged);
	BIND_CEGUI_EVENT(mGenderRadioButton, CEGUI::RadioButton::EventSelectStateChanged, ServerWidget::Gender_SelectionChanged);
	BIND_CEGUI_EVENT(femaleRadioButton, CEGUI::RadioButton::EventSelectStateChanged, ServerWidget::Gender_SelectionChanged);
	
	
	updateNewCharacter();

	CEGUI::Window* nameBox = getWindow("LoginPanel/NameEdit");
	CEGUI::Window* passwordBox = getWindow("LoginPanel/PasswordEdit");
	BIND_CEGUI_EVENT(nameBox, CEGUI::Window::EventTextChanged, ServerWidget::nameBox_TextChanged);
	BIND_CEGUI_EVENT(passwordBox, CEGUI::Window::EventTextChanged, ServerWidget::passwordBox_TextChanged);
	
		
	Ember::EmberServices::getSingletonPtr()->getServerService()->GotConnection.connect(sigc::mem_fun(*this, &ServerWidget::connection_GotConnection));
	Ember::EmberServices::getSingletonPtr()->getServerService()->GotAccount.connect(sigc::mem_fun(*this, &ServerWidget::createdAccount));
	Ember::EmberServices::getSingletonPtr()->getServerService()->LoginSuccess.connect(sigc::mem_fun(*this, &ServerWidget::loginSuccess));
	Ember::EmberServices::getSingletonPtr()->getServerService()->GotAvatar.connect(sigc::mem_fun(*this, &ServerWidget::gotAvatar));
	Ember::EmberServices::getSingletonPtr()->getServerService()->GotAllCharacters.connect(sigc::mem_fun(*this, &ServerWidget::gotAllCharacters));
    Ember::EmberServices::getSingletonPtr()->getServerService()->LoginFailure.connect(sigc::mem_fun(*this, &ServerWidget::showLoginFailure));
	
	addTabbableWindow(getWindow("LoginPanel/NameEdit"));
	addTabbableWindow(getWindow("LoginPanel/PasswordEdit"));
/*	addTabbableWindow(login);
	addTabbableWindow(createAcc);*/
	closeTabGroup();
	
	addTabbableWindow(mNewCharName);
// 	addTabbableWindow(mTypesList);
/*	addTabbableWindow(mGenderRadioButton);
	addTabbableWindow(femaleRadioButton);*/
	addTabbableWindow(mNewCharDescription);
	closeTabGroup();
	
	hide();

	createPreviewTexture();
//	getMainSheet()->addChildWindow(mMainWindow); 

}

void ServerWidget::connection_GotServerInfo()
{
	showServerInfo();
}

void ServerWidget::connection_GotConnection(Eris::Connection* connection)
{
	connection->GotServerInfo.connect(sigc::mem_fun(*this, &ServerWidget::connection_GotServerInfo));
	connection->refreshServerInfo();
}


void ServerWidget::createdAccount(Eris::Account* account) 
{

	mAccount = account;
	show();
	mMainWindow->moveToFront();
}

void ServerWidget::showServerInfo()
{
	try {
		CEGUI::Window* info = getWindow("Info");
		assert(mAccount);
		Eris::ServerInfo sInfo;
		mAccount->getConnection()->getServerInfo(sInfo);
		std::stringstream ss;
		ss << "Server name: " << sInfo.getServername() << "\n";
		ss << "Ruleset: " << sInfo.getRuleset() << "\n";
		ss << "Server type: " << sInfo.getServer() << " (v. "<< sInfo.getVersion() << ")\n";
		ss << "Ping: " << sInfo.getPing() << "\n";
		ss << "Uptime: " << static_cast<int>(sInfo.getUptime() / (60*60*24)) << " days\n";
		ss << "Number of clients: " << sInfo.getNumClients() << "\n";
		info->setText(ss.str());
	} catch (...) {
		S_LOG_WARNING("Error when getting the server info window.");
		return;
	}
}

void ServerWidget::loginSuccess(Eris::Account* account) 
{
	getWindow("LoginPanel")->setVisible(false);
	getWindow("CharacterTabControl")->setVisible(true);
	account->refreshCharacterInfo();
	fillAllowedCharacterTypes(account);
	
}

void ServerWidget::showLoginFailure(Eris::Account* account, std::string msg)
{
	CEGUI::GUISheet* helpText = static_cast<CEGUI::GUISheet*>(getWindow("LoginPanel/HelpText"));
	helpText->setYPosition(UDim(0.6, 0));

	CEGUI::GUISheet* loginFailure = static_cast<CEGUI::GUISheet*>(getWindow("LoginPanel/LoginFailure"));
	loginFailure->setText(msg);
	loginFailure->setVisible(true);
}

bool ServerWidget::hideLoginFailure()
{
	CEGUI::GUISheet* helpText = static_cast<CEGUI::GUISheet*>(getWindow("LoginPanel/HelpText"));
	helpText->setYPosition(UDim(0.55, 0));

	CEGUI::GUISheet* loginFailure = static_cast<CEGUI::GUISheet*>(getWindow("LoginPanel/LoginFailure"));
	loginFailure->setVisible(false);

	return true;
}

bool ServerWidget::passwordBox_TextChanged(const CEGUI::EventArgs& args)
{
	hideLoginFailure();

	return true;
}

bool ServerWidget::nameBox_TextChanged(const CEGUI::EventArgs& args)
{
	hideLoginFailure();

	return true;
}

void ServerWidget::fillAllowedCharacterTypes(Eris::Account* account)
{
	const std::vector< std::string >& characters = account->getCharacterTypes();
	
	for(std::vector< std::string >::const_iterator I = characters.begin(); I != characters.end(); ++I) {
		
		///if the user has access to the "creator" character, he/she can log in as this to get admin privileges
		///thus we active our special "admin button"
		if (*I == "creator") {
			mUseCreator->setVisible(true);
			mUseCreator->setEnabled(true);
		}
		
		CEGUI::ListboxItem* item = new Gui::ColouredListItem(*I, 0, 0);
		mTypesList->addItem(item);
	}
}

void ServerWidget::gotAllCharacters(Eris::Account* account) 
{
	Eris::CharacterMap cm = account->getCharacters();
	Eris::CharacterMap::iterator I = cm.begin();
	Eris::CharacterMap::iterator I_end = cm.end();
	
	if (I == I_end) {
	//if the user has no previous characters, show the create character tab
	
		CEGUI::TabControl* tabControl =  static_cast<CEGUI::TabControl*>(getWindow("CharacterTabControl"));
		if (tabControl) {
			//try {
				tabControl->setSelectedTab(getPrefix() + "CreateCharacterPanel");
			//} catch (...) {};
		}
	} else {
	
		for(;I != I_end; ++I) {
			const Atlas::Objects::Entity::RootEntity& entity = (*I).second;
			const Atlas::Message::Element& nameElement = entity->getAttr("name");
			Gui::ColouredListItem* item = new Gui::ColouredListItem(nameElement.asString());
			std::string* id = new std::string(entity->getId());
			item->setUserData(id);
			mCharacterList->addItem(item);
		}
	}
	
}

bool ServerWidget::Choose_Click(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mCharacterList->getFirstSelectedItem();
	if (item) {
/*	const Atlas::Objects::Entity::GameEntity & entity = static_cast<const Atlas::Objects::Entity::GameEntity &>(item->getUserData());*/
	
		std::string* id = static_cast<std::string*>(item->getUserData());
		
		Ember::EmberServices::getSingletonPtr()->getServerService()->takeCharacter(*id);
	}
	return true;
}

bool ServerWidget::UseCreator_Click(const CEGUI::EventArgs& args)
{
	///create a new admin character
	Ember::EmberServices::getSingletonPtr()->getServerService()->createCharacter("Lordi", "female", "creator", "Almighty");
	return true;
}


bool ServerWidget::CreateChar_Click(const CEGUI::EventArgs& args)
{
		
	Ember::EmberServices::getSingletonPtr()->getServerService()->createCharacter(mNewChar.name, mNewChar.gender, mNewChar.type, mNewChar.description);
	return true;
}

bool ServerWidget::TypesList_SelectionChanged(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mTypesList->getSelectedItem();
	if (item) {
	
		std::string type = item->getText().c_str();
		mNewChar.type = type;
		
		if (mModelPreviewRenderer) {
		
			///we need to get the model mapping definition for this type
			///once we have that, we will check for the first action of the first case of the first match (since that's guaranteed to be a show-model action
			Eris::TypeService* typeService = Ember::EmberServices::getSingletonPtr()->getServerService()->getConnection()->getTypeService();
 			Eris::TypeInfo* erisType = typeService->getTypeByName(type);
 			if (erisType) {
				const Model::Mapping::Definitions::ModelMappingDefinition* definition = Model::Mapping::EmberModelMappingManager::getSingleton().getManager().getDefinitionForType(erisType);
				if (definition) {
					Model::Mapping::Definitions::MatchDefinition::CaseStore::const_iterator first = definition->getRoot().getCases().begin();
					if (first != definition->getRoot().getCases().end()) {
						const Model::Mapping::Definitions::CaseDefinition& firstCase = *first;
						if (firstCase.getActions().begin() != firstCase.getActions().end()) {
							const Model::Mapping::Definitions::ActionDefinition& firstAction = *firstCase.getActions().begin();
							if (firstAction.getType() == "display-model") {
								///update the model preview window
								mModelPreviewRenderer->showModel(firstAction.getValue());
								//mModelPreviewRenderer->showFull();
								///we want to zoom in a little
								mModelPreviewRenderer->setCameraDistance(0.7);
							}
						}
					}
				}
			}
		}
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
	hide();
/*	mGuiManager->removeWidget(this);
	delete this;*/
}

void ServerWidget::createPreviewTexture()
{
	CEGUI::GUISheet* imageWidget = static_cast<CEGUI::GUISheet*>(getWindow("CreateCharacterPanel/Image"));
	if (!imageWidget) {
		S_LOG_FAILURE("Could not find CreateCharacterPanel/Image, aborting creation of preview texture.");
	} else {
		mModelPreviewRenderer = new ModelRenderer(imageWidget);
	}

}


};

bool EmberOgre::NewCharacter::isValid( ) const
{
	return name != "" && gender != "" && type != "";
}

