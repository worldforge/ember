//
// C++ Implementation: ServerWidget
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2004
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

#include "ServerWidget.h"

#include "ColouredListItem.h"
#include "ModelRenderer.h"

#include "components/entitymapping/EntityMappingManager.h"
#include "components/entitymapping/EntityMapping.h"
#include "components/entitymapping/Definitions/EntityMappingDefinition.h"
#include "components/entitymapping/Definitions/MatchDefinition.h"
#include "components/entitymapping/Definitions/CaseDefinition.h"
#include "components/entitymapping/Definitions/ActionDefinition.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "components/ogre/GUIManager.h"

#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/serversettings/ServerSettings.h"
#include "services/serversettings/ServerSettingsCredentials.h"
#include "services/EmberServices.h"

#include <Eris/ServerInfo.h>
#include <Eris/Connection.h>
#include <Eris/TypeInfo.h>
#include <Eris/TypeService.h>
#include <Eris/SpawnPoint.h>
#include <Eris/CharacterType.h>
#include <varconf/varconf.h>
#include <fstream>

#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxItem.h>
#include <elements/CEGUIListboxTextItem.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIEditbox.h>
#include <elements/CEGUIMultiLineEditbox.h>
#include <elements/CEGUIRadioButton.h>
#include <elements/CEGUICheckbox.h>
#include <elements/CEGUIComboDropList.h>
#include <elements/CEGUICombobox.h>
#include <elements/CEGUITabControl.h>
#include <elements/CEGUIGUISheet.h>
#include <CEGUIExceptions.h>

#include <sigc++/bind.h>



using namespace CEGUI;
namespace EmberOgre
{
namespace Gui
{

/*template<> WidgetLoader WidgetLoaderHolder<ServerWidget>::loader("ServerWidget", &createWidgetInstance);*/
//WidgetLoader Widget::loader("ServerWidget", &createWidgetInstance<ServerWidget>);

ServerWidget::ServerWidget() :
	mAccount(0), mModelPreviewRenderer(0), mCharacterList(0), mCreateChar(0), mUseCreator(0), mNewCharName(0), mNewCharDescription(0), mTypesList(0), mGenderRadioButton(0)
{
}

ServerWidget::~ServerWidget()
{
	if (mCharacterList) {
		///we'll store the id of the characters as string pointers in the ListBox, so we need to delete them ourselves when we cleanup
		for (unsigned int i = 0; i < mCharacterList->getItemCount(); ++i) {
			delete static_cast<std::string*> (mCharacterList->getListboxItemFromIndex(i)->getUserData());
		}
	}
	delete mModelPreviewRenderer;
}

void ServerWidget::buildWidget()
{

	if (loadMainSheet("ServerWidget.layout", "Server/")) {

		CEGUI::PushButton* okButton = static_cast<CEGUI::PushButton*> (getWindow("NoCharactersAlert/OkButton"));
		if (okButton) {
			BIND_CEGUI_EVENT(okButton, CEGUI::PushButton::EventClicked, ServerWidget::OkButton_Click);
		}

		CEGUI::PushButton* login = static_cast<CEGUI::PushButton*> (getWindow("LoginPanel/Login"));
		BIND_CEGUI_EVENT(login, CEGUI::PushButton::EventClicked, ServerWidget::Login_Click);
		CEGUI::PushButton* createAcc = static_cast<CEGUI::PushButton*> (getWindow("LoginPanel/CreateAcc"));
		BIND_CEGUI_EVENT(createAcc, CEGUI::PushButton::EventClicked, ServerWidget::CreateAcc_Click);

		mCharacterList = static_cast<CEGUI::Listbox*> (getWindow("ChooseCharacterPanel/CharacterList"));
		CEGUI::PushButton* chooseChar = static_cast<CEGUI::PushButton*> (getWindow("ChooseCharacterPanel/Choose"));
		mUseCreator = static_cast<CEGUI::PushButton*> (getWindow("UseCreator"));
		mCreateChar = static_cast<CEGUI::PushButton*> (getWindow("CreateCharacterPanel/CreateButton"));

		BIND_CEGUI_EVENT(chooseChar, CEGUI::PushButton::EventClicked, ServerWidget::Choose_Click);
		BIND_CEGUI_EVENT(mUseCreator, CEGUI::PushButton::EventClicked, ServerWidget::UseCreator_Click);
		BIND_CEGUI_EVENT(mCreateChar, CEGUI::PushButton::EventClicked, ServerWidget::CreateChar_Click);
		BIND_CEGUI_EVENT(mCharacterList, CEGUI::ButtonBase::EventMouseDoubleClick, ServerWidget::Choose_Click);
		BIND_CEGUI_EVENT(static_cast<CEGUI::PushButton*> (getWindow("LogoutButton")), CEGUI::PushButton::EventClicked, ServerWidget::LogoutButton_Click);

		mNewCharName = static_cast<CEGUI::Editbox*> (getWindow("CreateCharacterPanel/NameEdit"));
		mNewCharDescription = static_cast<CEGUI::MultiLineEditbox*> (getWindow("CreateCharacterPanel/Description"));
		mTypesList = static_cast<CEGUI::Combobox*> (getWindow("CreateCharacterPanel/Type"));

		mGenderRadioButton = static_cast<CEGUI::RadioButton*> (getWindow("CreateCharacterPanel/Gender/Male"));
		CEGUI::RadioButton* femaleRadioButton = static_cast<CEGUI::RadioButton*> (getWindow("CreateCharacterPanel/Gender/Female"));

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
		BIND_CEGUI_EVENT(static_cast<CEGUI::PushButton*> (getWindow("LoginPanel/Disconnect")), CEGUI::PushButton::EventClicked, ServerWidget::Disconnect_Click);


		Ember::EmberServices::getSingletonPtr()->getServerService()->GotAccount.connect(sigc::mem_fun(*this, &ServerWidget::createdAccount));
		Ember::EmberServices::getSingletonPtr()->getServerService()->LoginSuccess.connect(sigc::mem_fun(*this, &ServerWidget::loginSuccess));
		Ember::EmberServices::getSingletonPtr()->getServerService()->GotAvatar.connect(sigc::mem_fun(*this, &ServerWidget::gotAvatar));
		Ember::EmberServices::getSingletonPtr()->getServerService()->GotAllCharacters.connect(sigc::mem_fun(*this, &ServerWidget::gotAllCharacters));
		Ember::EmberServices::getSingletonPtr()->getServerService()->LoginFailure.connect(sigc::mem_fun(*this, &ServerWidget::showLoginFailure));

		addTabbableWindow(getWindow("LoginPanel/NameEdit"));
		addTabbableWindow(getWindow("LoginPanel/PasswordEdit"));

		addEnterButton(login);
		/*	addTabbableWindow(login);
		 addTabbableWindow(createAcc);*/
		closeTabGroup();

		addTabbableWindow(mNewCharName);
		// 	addTabbableWindow(mTypesList);
		/*	addTabbableWindow(mGenderRadioButton);
		 addTabbableWindow(femaleRadioButton);*/
		addTabbableWindow(mNewCharDescription);
		addEnterButton(mCreateChar);
		closeTabGroup();

		createPreviewTexture();

		setConnection(Ember::EmberServices::getSingleton().getServerService()->getConnection());
	}

}

void ServerWidget::connection_GotServerInfo(Eris::Connection* connection)
{
	showServerInfo(connection);
}

void ServerWidget::setConnection(Eris::Connection* connection)
{
	connection->GotServerInfo.connect(sigc::bind(sigc::mem_fun(*this, &ServerWidget::connection_GotServerInfo), connection));
	connection->refreshServerInfo();
	connection->Disconnected.connect(sigc::mem_fun(*this, &ServerWidget::connection_Disconnected));
}

void ServerWidget::connection_Disconnected()
{
	mGuiManager->destroyWidget(this);
}

void ServerWidget::createdAccount(Eris::Account* account)
{
	mAccount = account;
	show();
	mMainWindow->moveToFront();
}

void ServerWidget::showServerInfo(Eris::Connection* connection)
{
	try {
		CEGUI::Window* info = getWindow("Info");
		Eris::ServerInfo sInfo;
		connection->getServerInfo(sInfo);
		std::stringstream ss;
		ss << "Server name: " << sInfo.getServername() << "\n";
		ss << "Ruleset: " << sInfo.getRuleset() << "\n";
		ss << "Server type: " << sInfo.getServer() << " (v. " << sInfo.getVersion() << ")\n";
		ss << "Ping: " << sInfo.getPing() << "\n";
		ss << "Uptime: " << static_cast<int> (sInfo.getUptime() / (60 * 60 * 24)) << " days\n";
		ss << "Number of clients: " << sInfo.getNumClients() << "\n";
		info->setText(ss.str());

		/*
		 * Since we're using the server getHostname as a section name
		 * we must wait until there is a connection before we can fetch
		 * the credentials
		 */
		CEGUI::Window* nameBox = getWindow("LoginPanel/NameEdit");
		CEGUI::Window* passwordBox = getWindow("LoginPanel/PasswordEdit");
		std::string savedUser = "";
		std::string savedPass = "";
		if (fetchCredentials(connection, savedUser, savedPass)) {
			nameBox->setText(savedUser);
			passwordBox->setText(savedPass);
		}

	} catch (...) {
		S_LOG_WARNING("Error when getting the server info window.");
		return;
	}
}

bool ServerWidget::fetchCredentials(Eris::Connection* connection, std::string& user, std::string& pass)
{
	S_LOG_VERBOSE("Fetching saved credentials.");

	Eris::ServerInfo sInfo;
	connection->getServerInfo(sInfo);
	std::string sname = sInfo.getHostname();

	Ember::Services::ServerSettingsCredentials serverCredentials(sname);
	Ember::Services::ServerSettings* serverSettings = Ember::EmberServices::getSingleton().getServerSettingsService();
	if (serverSettings->findItem(serverCredentials, "username")) {
		user = static_cast<std::string>(serverSettings->getItem(serverCredentials, "username"));
	}
	if (serverSettings->findItem(serverCredentials, "password")) {
		pass = static_cast<std::string>(serverSettings->getItem(serverCredentials, "password"));
	}
	return pass != "" && user != "";
}

bool ServerWidget::saveCredentials()
{
	S_LOG_VERBOSE("Saving credentials.");

	// check the main account is good, and fetch server info
	assert(mAccount);
	Eris::ServerInfo sInfo;
	mAccount->getConnection()->getServerInfo(sInfo);

	// pull widget references
	CEGUI::Window* nameBox(0);
	CEGUI::Window* passwordBox(0);
	CEGUI::Checkbox* saveBox(0);
	try {
		nameBox = getWindow("LoginPanel/NameEdit");
		passwordBox = getWindow("LoginPanel/PasswordEdit");
		saveBox = static_cast<CEGUI::Checkbox*> (getWindow("LoginPanel/SavePassCheck"));
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when getting windows from CEGUI: " << ex.getMessage().c_str());
		return false;
	}

	if (nameBox && passwordBox && saveBox) {

		// fetch info from widgets
		CEGUI::String name = nameBox->getText();
		CEGUI::String password = passwordBox->getText();
		std::string sname = sInfo.getHostname();
		Ember::Services::ServerSettingsCredentials serverCredentials(sname);
		Ember::Services::ServerSettings* serverSettings = Ember::EmberServices::getSingleton().getServerSettingsService();
		serverSettings->setItem(serverCredentials, "username", name.c_str());
		serverSettings->setItem(serverCredentials, "password", password.c_str());
		serverSettings->writeToDisk();
		return true;
	}
	return false;
}

void ServerWidget::logoutComplete(bool clean)
{
	getWindow("LoginPanel")->setVisible(true);
	getWindow("LoggedInPanel")->setVisible(false);
}

void ServerWidget::loginSuccess(Eris::Account* account)
{
	account->LogoutComplete.connect(sigc::mem_fun(*this, &ServerWidget::logoutComplete));
	getWindow("LoginPanel")->setVisible(false);
	getWindow("LoggedInPanel")->setVisible(true);
	account->refreshCharacterInfo();
	fillAllowedCharacterTypes(account);

	CEGUI::Checkbox* saveBox = static_cast<CEGUI::Checkbox*> (getWindow("LoginPanel/SavePassCheck"));
	if (saveBox->isSelected()) {
		try {
			saveCredentials();
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when saving password."<< ex);
		} catch (...) {
			S_LOG_FAILURE("Unspecified error when saving password.");
		}
	}

}

void ServerWidget::showLoginFailure(Eris::Account* account, std::string msg)
{
	CEGUI::GUISheet* helpText = static_cast<CEGUI::GUISheet*> (getWindow("LoginPanel/HelpText"));
	helpText->setYPosition(UDim(0.6, 0));

	CEGUI::GUISheet* loginFailure = static_cast<CEGUI::GUISheet*> (getWindow("LoginPanel/LoginFailure"));
	loginFailure->setText(msg);
	loginFailure->setVisible(true);
}

bool ServerWidget::hideLoginFailure()
{
	CEGUI::GUISheet* helpText = static_cast<CEGUI::GUISheet*> (getWindow("LoginPanel/HelpText"));
	helpText->setYPosition(UDim(0.55, 0));

	CEGUI::GUISheet* loginFailure = static_cast<CEGUI::GUISheet*> (getWindow("LoginPanel/LoginFailure"));
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
	/**
	 * The preferred way is to use the spawn point system. However, older servers don't support that, and instead present a single list of characters.
	 * For spawn points, we still want to show a single list of available characters. In those cases where a character is present in multiple spawn points we'll print out the spawn point name in parenthesis.
	 */
	const Eris::SpawnPointMap& spawnPoints = account->getSpawnPoints();

	for (Eris::SpawnPointMap::const_iterator I = spawnPoints.begin(); I != spawnPoints.end(); ++I) {
		const Eris::SpawnPoint& spawnPoint = I->second;
		for (Eris::CharacterTypeStore::const_iterator J = spawnPoint.getAvailableCharacterTypes().begin(); J != spawnPoint.getAvailableCharacterTypes().end(); ++J) {
			mCharacterAndSpawns.insert(CharacterAndSpawnsStore::value_type(J->getName(), spawnPoint.getName()));
		}
	}

	const std::vector<std::string>& characters = account->getCharacterTypes();

	//If the account inherits from "admin" we're an admin and can create a creator entity.
	if (std::find(account->getParents().begin(), account->getParents().end(), "admin") != account->getParents().end()) {
		mUseCreator->setVisible(true);
		mUseCreator->setEnabled(true);

	}

	if (mCharacterAndSpawns.size() == 0 && characters.size() == 0) {
		showNoCharactersAlert();
	} else {
		//The preferred way is by using spawn points. Using the character type list is kept for backwards compatibility.
		if (mCharacterAndSpawns.size() > 0) {
			unsigned int i = 0;
			for (CharacterAndSpawnsStore::const_iterator I = mCharacterAndSpawns.begin(); I != mCharacterAndSpawns.end(); ++I) {
				std::string name = I->first;
				if (mCharacterAndSpawns.count(I->first) > 1) {
					name += " (" + I->second + ")";
				}
				mTypesList->addItem(new Gui::ColouredListItem(name, i++, 0));
			}

		} else {
			for (std::vector<std::string>::const_iterator I = characters.begin(); I != characters.end(); ++I) {

				CEGUI::ListboxItem* item = new Gui::ColouredListItem(*I, 0, 0);
				mTypesList->addItem(item);
			}
		}
	}
}

void ServerWidget::gotAllCharacters(Eris::Account* account)
{
	Eris::CharacterMap cm = account->getCharacters();
	Eris::CharacterMap::iterator I = cm.begin();
	Eris::CharacterMap::iterator I_end = cm.end();

	if (I == I_end) {
		//if the user has no previous characters, show the create character tab

		CEGUI::TabControl* tabControl = static_cast<CEGUI::TabControl*> (getWindow("CharacterTabControl"));
		if (tabControl) {
			//try {
			tabControl->setSelectedTab(getPrefix() + "CreateCharacterPanel");
			//} catch (...) {};
		}
	} else {

		for (; I != I_end; ++I) {
			const Atlas::Objects::Entity::RootEntity& entity = (*I).second;

			std::string itemText("");
			if (entity->hasAttr("name")) {
				const Atlas::Message::Element& nameElement = entity->getAttr("name");
				if (nameElement.isString()) {
					itemText += nameElement.asString();
				}
			}
			//TODO: prevent this from leaking memory. Granted, it isn't much, but it would be better if we didn't leak
			Gui::ColouredListItem* item = new Gui::ColouredListItem(itemText);
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

		std::string* id = static_cast<std::string*> (item->getUserData());

		Ember::EmberServices::getSingletonPtr()->getServerService()->takeCharacter(*id);
	}
	return true;
}

bool ServerWidget::UseCreator_Click(const CEGUI::EventArgs& args)
{
	///create a new admin character
	Ember::EmberServices::getSingletonPtr()->getServerService()->createCharacter("The Creator", "female", "creator", "Almighty", "");
	return true;
}

bool ServerWidget::CreateChar_Click(const CEGUI::EventArgs& args)
{
	Ember::EmberServices::getSingletonPtr()->getServerService()->createCharacter(mNewChar.name, mNewChar.gender, mNewChar.type, mNewChar.description, mNewChar.spawnPoint);
	return true;
}

bool ServerWidget::LogoutButton_Click(const CEGUI::EventArgs& args)
{
	Ember::EmberServices::getSingletonPtr()->getServerService()->logout();
	return true;
}

bool ServerWidget::Disconnect_Click(const CEGUI::EventArgs& args)
{
	Ember::EmberServices::getSingleton().getServerService()->disconnect();
	return true;
}

bool ServerWidget::TypesList_SelectionChanged(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mTypesList->getSelectedItem();
	if (item) {

		unsigned int itemIndex = item->getID();
		//Check if we have a list of spawn points, or if not the older single character type list.
		if (mCharacterAndSpawns.size() > 0) {
			CharacterAndSpawnsStore::const_iterator I = mCharacterAndSpawns.begin();
			for (unsigned int i = 0; i < itemIndex; ++i) {
				I++;
			}
			mNewChar.type = I->first;
			mNewChar.spawnPoint = I->second;
		} else {
			mNewChar.type = mAccount->getCharacterTypes()[itemIndex];
		}

		if (mModelPreviewRenderer) {

			///we need to get the model mapping definition for this type
			///once we have that, we will check for the first action of the first case of the first match (since that's guaranteed to be a show-model action
			Eris::TypeService* typeService = mAccount->getConnection()->getTypeService();
			Eris::TypeInfo* erisType = typeService->getTypeByName(mNewChar.type);
			if (erisType) {
				const Ember::EntityMapping::Definitions::EntityMappingDefinition* definition = Mapping::EmberEntityMappingManager::getSingleton().getManager().getDefinitionForType(erisType);
				if (definition) {
					Ember::EntityMapping::Definitions::MatchDefinition::CaseStore::const_iterator first = definition->getRoot().getCases().begin();
					if (first != definition->getRoot().getCases().end()) {
						const Ember::EntityMapping::Definitions::CaseDefinition& firstCase = *first;
						if (firstCase.getActions().begin() != firstCase.getActions().end()) {
							const Ember::EntityMapping::Definitions::ActionDefinition& firstAction = *firstCase.getActions().begin();
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

	mAccount->createAccount(std::string(name.c_str()), std::string(name.c_str()), std::string(password.c_str()));
	return true;
}

bool ServerWidget::OkButton_Click(const CEGUI::EventArgs& args)
{
	CEGUI::Window* alert = getWindow("NoCharactersAlert");
	if (alert) {
		alert->setVisible(false);
	}
	return true;
}

void ServerWidget::gotAvatar(Eris::Avatar* avatar)
{
	mAccount->AvatarDeactivated.connect(sigc::mem_fun(*this, &ServerWidget::avatar_Deactivated));
	hide();
}

void ServerWidget::avatar_Deactivated(bool clean)
{
	show();
	mMainWindow->moveToFront();
	getWindow("LoginPanel")->setVisible(false);
	getWindow("LoggedInPanel")->setVisible(true);
}

void ServerWidget::createPreviewTexture()
{
	CEGUI::GUISheet* imageWidget = static_cast<CEGUI::GUISheet*> (getWindow("CreateCharacterPanel/Image"));
	if (!imageWidget) {
		S_LOG_FAILURE("Could not find CreateCharacterPanel/Image, aborting creation of preview texture.");
	} else {
		mModelPreviewRenderer = new ModelRenderer(imageWidget);
	}

}

void ServerWidget::showNoCharactersAlert()
{
	CEGUI::Window* alert = getWindow("NoCharactersAlert");
	alert->setVisible(true);
	alert->moveToFront();
}

bool NewCharacter::isValid() const
{
	return name != "" && gender != "" && type != "";
}

}
;
}
;

