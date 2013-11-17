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
#include "EntityTextureManipulator.h"

#include "components/entitymapping/EntityMappingManager.h"
#include "components/entitymapping/EntityMapping.h"
#include "components/entitymapping/Definitions/EntityMappingDefinition.h"
#include "components/entitymapping/Definitions/MatchDefinition.h"
#include "components/entitymapping/Definitions/CaseDefinition.h"
#include "components/entitymapping/Definitions/ActionDefinition.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "components/ogre/GUIManager.h"

#include "services/server/ServerService.h"
#include "services/server/AvatarTransferInfo.h"
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
#include <Eris/View.h>
#include <varconf/varconf.h>
#include <fstream>

#include <CEGUI/widgets/Listbox.h>
#include <CEGUI/widgets/ListboxItem.h>
#include <CEGUI/widgets/ListboxTextItem.h>
#include <CEGUI/widgets/PushButton.h>
#include <CEGUI/widgets/Editbox.h>
#include <CEGUI/widgets/MultiLineEditbox.h>
#include <CEGUI/widgets/RadioButton.h>
#include <CEGUI/widgets/ToggleButton.h>
#include <CEGUI/widgets/ComboDropList.h>
#include <CEGUI/widgets/Combobox.h>
#include <CEGUI/widgets/TabControl.h>
#include <CEGUI/Window.h>
#include <CEGUI/Exceptions.h>

#include <sigc++/bind.h>

using namespace CEGUI;
namespace Ember
{
namespace OgreView
{
namespace Gui
{

ServerWidget::ServerWidget() :
	mAccount(0), mModelPreviewRenderer(0), mModelPreviewManipulator(0), mCharacterList(0), mCreateChar(0), mUseCreator(0), mNewCharName(0), mNewCharDescription(0), mTypesList(0), mGenderRadioButton(0), mAvatarTransferInfo(0)
{
}

ServerWidget::~ServerWidget()
{
	delete mModelPreviewManipulator;
	delete mModelPreviewRenderer;
	delete mAvatarTransferInfo;
}

void ServerWidget::buildWidget()
{

	if (loadMainSheet("ServerWidget.layout", "Server/")) {

		CEGUI::PushButton* okButton = static_cast<CEGUI::PushButton*> (getWindow("NoCharactersAlert/OkButton"));
		if (okButton) {
			BIND_CEGUI_EVENT(okButton, CEGUI::PushButton::EventClicked, ServerWidget::OkButton_Click);
		}

		CEGUI::PushButton* entityDestroyedOkButton = static_cast<CEGUI::PushButton*> (getWindow("EntityDestroyed/OkButton"));
		if (entityDestroyedOkButton) {
			BIND_CEGUI_EVENT(entityDestroyedOkButton, CEGUI::PushButton::EventClicked, ServerWidget::EntityDestroyedOkButton_Click);
		}

		CEGUI::PushButton* login = static_cast<CEGUI::PushButton*> (mMainWindow->getChild("LoginPanel/Login"));
		BIND_CEGUI_EVENT(login, CEGUI::PushButton::EventClicked, ServerWidget::Login_Click);
		CEGUI::PushButton* createAcc = static_cast<CEGUI::PushButton*> (mMainWindow->getChild("LoginPanel/CreateAcc"));
		BIND_CEGUI_EVENT(createAcc, CEGUI::PushButton::EventClicked, ServerWidget::CreateAcc_Click);

		mCharacterList = static_cast<CEGUI::Listbox*> (mMainWindow->getChild("LoggedInPanel/CharacterTabControl/ChooseCharacterPanel/CharacterList"));
		CEGUI::PushButton* chooseChar = static_cast<CEGUI::PushButton*> (mMainWindow->getChild("LoggedInPanel/CharacterTabControl/ChooseCharacterPanel/Choose"));
		mUseCreator = static_cast<CEGUI::PushButton*> (mMainWindow->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/UseCreator"));
		mCreateChar = static_cast<CEGUI::PushButton*> (mMainWindow->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/CreateButton"));

		BIND_CEGUI_EVENT(chooseChar, CEGUI::PushButton::EventClicked, ServerWidget::Choose_Click);
		BIND_CEGUI_EVENT(mUseCreator, CEGUI::PushButton::EventClicked, ServerWidget::UseCreator_Click);
		BIND_CEGUI_EVENT(mCreateChar, CEGUI::PushButton::EventClicked, ServerWidget::CreateChar_Click);
		BIND_CEGUI_EVENT(mCharacterList, CEGUI::ButtonBase::EventMouseDoubleClick, ServerWidget::Choose_Click);
		BIND_CEGUI_EVENT(static_cast<CEGUI::PushButton*> (mMainWindow->getChild("LoggedInPanel/LogoutButton")), CEGUI::PushButton::EventClicked, ServerWidget::LogoutButton_Click);

		mNewCharName = static_cast<CEGUI::Editbox*> (mMainWindow->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/NameEdit"));
		mNewCharDescription = static_cast<CEGUI::MultiLineEditbox*> (mMainWindow->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/Description"));
		mTypesList = static_cast<CEGUI::Combobox*> (mMainWindow->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/Type"));

		mGenderRadioButton = static_cast<CEGUI::RadioButton*> (mMainWindow->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/Male"));
		CEGUI::RadioButton* femaleRadioButton = static_cast<CEGUI::RadioButton*> (mMainWindow->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/Female"));

		BIND_CEGUI_EVENT(mNewCharName, CEGUI::Editbox::EventTextChanged, ServerWidget::Name_TextChanged);
		BIND_CEGUI_EVENT(mNewCharDescription, CEGUI::Editbox::EventTextChanged, ServerWidget::Description_TextChanged);
		BIND_CEGUI_EVENT(mTypesList, CEGUI::Combobox::EventListSelectionChanged, ServerWidget::TypesList_SelectionChanged);
		BIND_CEGUI_EVENT(mGenderRadioButton, CEGUI::RadioButton::EventSelectStateChanged, ServerWidget::Gender_SelectionChanged);
		BIND_CEGUI_EVENT(femaleRadioButton, CEGUI::RadioButton::EventSelectStateChanged, ServerWidget::Gender_SelectionChanged);

		BIND_CEGUI_EVENT(mMainWindow->getChild("LoggedInPanel/TeleportInfo/Yes"), CEGUI::PushButton::EventClicked, ServerWidget::TeleportYes_Click);
		BIND_CEGUI_EVENT(mMainWindow->getChild("LoggedInPanel/TeleportInfo/No"), CEGUI::PushButton::EventClicked, ServerWidget::TeleportNo_Click);

		updateNewCharacter();

		CEGUI::Window* nameBox = mMainWindow->getChild("LoginPanel/NameEdit");
		CEGUI::Window* passwordBox = mMainWindow->getChild("LoginPanel/PasswordEdit");

		BIND_CEGUI_EVENT(nameBox, CEGUI::Window::EventTextChanged, ServerWidget::nameBox_TextChanged);
		BIND_CEGUI_EVENT(passwordBox, CEGUI::Window::EventTextChanged, ServerWidget::passwordBox_TextChanged);
		BIND_CEGUI_EVENT(static_cast<CEGUI::PushButton*> (mMainWindow->getChild("LoginPanel/Disconnect")), CEGUI::PushButton::EventClicked, ServerWidget::Disconnect_Click);


		EmberServices::getSingleton().getServerService().GotAccount.connect(sigc::mem_fun(*this, &ServerWidget::createdAccount));
		EmberServices::getSingleton().getServerService().LoginSuccess.connect(sigc::mem_fun(*this, &ServerWidget::loginSuccess));
		EmberServices::getSingleton().getServerService().GotAvatar.connect(sigc::mem_fun(*this, &ServerWidget::gotAvatar));
		EmberServices::getSingleton().getServerService().GotAllCharacters.connect(sigc::mem_fun(*this, &ServerWidget::gotAllCharacters));
		EmberServices::getSingleton().getServerService().LoginFailure.connect(sigc::mem_fun(*this, &ServerWidget::showLoginFailure));
		EmberServices::getSingleton().getServerService().TransferInfoAvailable.connect(sigc::mem_fun(*this, &ServerWidget::server_TransferInfoAvailable));

		addTabbableWindow(mMainWindow->getChild("LoginPanel/NameEdit"));
		addTabbableWindow(mMainWindow->getChild("LoginPanel/PasswordEdit"));

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

		setConnection(EmberServices::getSingleton().getServerService().getConnection());
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

void ServerWidget::server_TransferInfoAvailable(const std::vector<AvatarTransferInfo>& transferInfos)
{
	if (transferInfos.size() > 0) {
		CEGUI::Window* teleportInfo = getWindow("TeleportInfo", true);
		teleportInfo->setVisible(true);
		mAvatarTransferInfo = new AvatarTransferInfo(transferInfos[0]);
	}
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
		CEGUI::Window* nameBox = mMainWindow->getChild("LoginPanel/NameEdit");
		CEGUI::Window* passwordBox = mMainWindow->getChild("LoginPanel/PasswordEdit");
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

	Services::ServerSettingsCredentials serverCredentials(sInfo);
	Services::ServerSettings& serverSettings = EmberServices::getSingleton().getServerSettingsService();
	if (serverSettings.findItem(serverCredentials, "username")) {
		user = static_cast<std::string>(serverSettings.getItem(serverCredentials, "username"));
	}
	if (serverSettings.findItem(serverCredentials, "password")) {
		pass = static_cast<std::string>(serverSettings.getItem(serverCredentials, "password"));
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
	CEGUI::ToggleButton* saveBox(0);
	try {
		nameBox = mMainWindow->getChild("LoginPanel/NameEdit");
		passwordBox = mMainWindow->getChild("LoginPanel/PasswordEdit");
		saveBox = static_cast<CEGUI::ToggleButton*> (mMainWindow->getChild("LoginPanel/SavePassCheck"));
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when getting windows from CEGUI." << ex);
		return false;
	}

	if (nameBox && passwordBox && saveBox) {

		// fetch info from widgets
		CEGUI::String name = nameBox->getText();
		CEGUI::String password = passwordBox->getText();
		Services::ServerSettingsCredentials serverCredentials(sInfo);
		Services::ServerSettings& serverSettings = EmberServices::getSingleton().getServerSettingsService();
		serverSettings.setItem(serverCredentials, "username", name.c_str());
		serverSettings.setItem(serverCredentials, "password", password.c_str());
		serverSettings.writeToDisk();
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

	CEGUI::ToggleButton* saveBox = static_cast<CEGUI::ToggleButton*> (mMainWindow->getChild("LoginPanel/SavePassCheck"));
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
	auto helpText = mMainWindow->getChild("LoginPanel/HelpText");
	helpText->setYPosition(UDim(0.6, 0));

	auto loginFailure = mMainWindow->getChild("LoginPanel/LoginFailure");
	loginFailure->setText(msg);
	loginFailure->setVisible(true);
}

bool ServerWidget::hideLoginFailure()
{
	auto helpText = mMainWindow->getChild("LoginPanel/HelpText");
	helpText->setYPosition(UDim(0.55, 0));

	auto loginFailure = mMainWindow->getChild("LoginPanel/LoginFailure");
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
	mTypesList->resetList();

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

	//If the account inherits from "admin" we're an admin and can create a creator entity. This also applies if we're a "sys" account.
	if (std::find(account->getParents().begin(), account->getParents().end(), "admin") != account->getParents().end() ||
			std::find(account->getParents().begin(), account->getParents().end(), "sys") != account->getParents().end()) {
		mUseCreator->setVisible(true);
		mUseCreator->setEnabled(true);
	} else {
		mUseCreator->setVisible(false);
		mUseCreator->setEnabled(false);
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
			unsigned int i = 0;
			for (std::vector<std::string>::const_iterator I = characters.begin(); I != characters.end(); ++I) {

				CEGUI::ListboxItem* item = new Gui::ColouredListItem(*I, i++, 0);
				mTypesList->addItem(item);
			}
		}
	}
}

void ServerWidget::gotAllCharacters(Eris::Account* account)
{
	mCharacterList->resetList();
	mCharacterModel.clear();
	Eris::CharacterMap cm = account->getCharacters();
	Eris::CharacterMap::iterator I = cm.begin();
	Eris::CharacterMap::iterator I_end = cm.end();

	if (I == I_end) {
		//if the user has no previous characters, show the create character tab

		CEGUI::TabControl* tabControl = static_cast<CEGUI::TabControl*> (getWindow("CharacterTabControl"));
		if (tabControl) {
			//try {
			tabControl->setSelectedTab("CreateCharacterPanel");
			//} catch (...) {};
		}
	} else {

		for (; I != I_end; ++I) {
			const Atlas::Objects::Entity::RootEntity& entity = (*I).second;

			std::string itemText;
			if (!entity->getName().empty()) {
				itemText = entity->getName();
			} else {
				//If there's no name try to print the type of entity instead.
				if (!entity->getParents().empty()) {
					itemText = entity->getParents().front();
				} else {
					itemText = "<unknown>";
				}
			}
			Gui::ColouredListItem* item = new Gui::ColouredListItem(itemText);

			mCharacterList->addItem(item);
			mCharacterModel.push_back(entity->getId());
		}
	}

}

bool ServerWidget::Choose_Click(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mCharacterList->getFirstSelectedItem();
	if (item) {

		std::string id = mCharacterModel[mCharacterList->getItemIndex(item)];

		EmberServices::getSingleton().getServerService().takeCharacter(id);
	}
	return true;
}

bool ServerWidget::UseCreator_Click(const CEGUI::EventArgs& args)
{
	//create a new admin character
	Atlas::Message::MapType extraProperties;
	//The admin character should always be transient.
	//NOTE: This is currently not handled in Cyphesis, as it allows no extra parameters. Nonetheless we'll keep it here, since it's a prudent thing to do.
	extraProperties.insert(std::make_pair("transient", -1.0f));
	EmberServices::getSingleton().getServerService().createCharacter("The Creator", "female", "creator", "Almighty", "", extraProperties);
	return true;
}

bool ServerWidget::CreateChar_Click(const CEGUI::EventArgs& args)
{
	EmberServices::getSingleton().getServerService().createCharacter(mNewChar.name, mNewChar.gender, mNewChar.type, mNewChar.description, mNewChar.spawnPoint);
	return true;
}

bool ServerWidget::LogoutButton_Click(const CEGUI::EventArgs& args)
{
	EmberServices::getSingleton().getServerService().logout();
	return true;
}

bool ServerWidget::Disconnect_Click(const CEGUI::EventArgs& args)
{
	EmberServices::getSingleton().getServerService().disconnect();
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

			//we need to get the model mapping definition for this type
			//once we have that, we will check for the first action of the first case of the first match (since that's guaranteed to be a show-model action
			Eris::TypeService* typeService = mAccount->getConnection()->getTypeService();
			Eris::TypeInfo* erisType = typeService->getTypeByName(mNewChar.type);
			if (erisType) {
				const EntityMapping::Definitions::EntityMappingDefinition* definition = Mapping::EmberEntityMappingManager::getSingleton().getManager().getDefinitionForType(erisType);
				if (definition) {
					EntityMapping::Definitions::MatchDefinition::CaseStore::const_iterator first = definition->getRoot().getCases().begin();
					if (first != definition->getRoot().getCases().end()) {
						const EntityMapping::Definitions::CaseDefinition& firstCase = *first;
						if (firstCase.getActions().begin() != firstCase.getActions().end()) {
							const EntityMapping::Definitions::ActionDefinition& firstAction = *firstCase.getActions().begin();
							if (firstAction.getType() == "display-model") {
								//update the model preview window
								mModelPreviewRenderer->showModel(firstAction.getValue());
								//mModelPreviewRenderer->showFull();
								//we want to zoom in a little
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
	CEGUI::Window* nameBox = mMainWindow->getChild("LoginPanel/NameEdit");
	CEGUI::Window* passwordBox = mMainWindow->getChild("LoginPanel/PasswordEdit");

	CEGUI::String name = nameBox->getText();
	CEGUI::String password = passwordBox->getText();

	mAccount->login(std::string(name.c_str()), std::string(password.c_str()));

	return true;
}

bool ServerWidget::CreateAcc_Click(const CEGUI::EventArgs& args)
{
	CEGUI::Window* nameBox = mMainWindow->getChild("LoginPanel/NameEdit");
	CEGUI::Window* passwordBox = mMainWindow->getChild("LoginPanel/PasswordEdit");

	CEGUI::String name = nameBox->getText();
	CEGUI::String password = passwordBox->getText();

	mAccount->createAccount(std::string(name.c_str()), std::string(name.c_str()), std::string(password.c_str()));
	return true;
}

bool ServerWidget::OkButton_Click(const CEGUI::EventArgs& args)
{
	CEGUI::Window* alert = getWindow("NoCharactersAlert");
	if (alert) {
		alert->hide();
	}
	return true;
}

bool ServerWidget::EntityDestroyedOkButton_Click(const CEGUI::EventArgs& args)
{
	CEGUI::Window* alert = getWindow("EntityDestroyed");
	if (alert) {
		alert->hide();
	}
	return true;
}


bool ServerWidget::TeleportYes_Click(const CEGUI::EventArgs& args)
{
	getWindow("TeleportInfo", true)->setVisible(false);
	if (mAvatarTransferInfo) {
		EmberServices::getSingleton().getServerService().takeTransferredCharacter(mAvatarTransferInfo->getTransferInfo());
	}
	return true;
}

bool ServerWidget::TeleportNo_Click(const CEGUI::EventArgs& args)
{
	getWindow("TeleportInfo", true)->setVisible(false);
	return true;
}

void ServerWidget::gotAvatar(Eris::Avatar* avatar)
{
	mAccount->AvatarDeactivated.connect(sigc::mem_fun(*this, &ServerWidget::avatar_Deactivated));
	avatar->getView()->AvatarEntityDeleted.connect(sigc::mem_fun(*this, &ServerWidget::avatar_EntityDeleted));
	hide();
}


void ServerWidget::avatar_EntityDeleted()
{
	CEGUI::Window* alert = getWindow("EntityDestroyed");
	if (alert) {
		alert->show();
	}
}


void ServerWidget::avatar_Deactivated(Eris::Avatar* avatar)
{
	mCharacterList->resetList();
	mCharacterModel.clear();
	mAccount->refreshCharacterInfo();
	show();
	mMainWindow->moveToFront();
	getWindow("LoginPanel")->setVisible(false);
	getWindow("LoggedInPanel")->setVisible(true);
	gotAllCharacters(mAccount);
}

void ServerWidget::createPreviewTexture()
{
	auto imageWidget = mMainWindow->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/Image");
	if (!imageWidget) {
		S_LOG_FAILURE("Could not find CreateCharacterPanel/Image, aborting creation of preview texture.");
	} else {
		mModelPreviewRenderer = new ModelRenderer(imageWidget, "newCharacterPreview");
		mModelPreviewManipulator = new CameraEntityTextureManipulator(*imageWidget, mModelPreviewRenderer->getEntityTexture());
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
}
}
