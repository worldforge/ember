//
// C++ Implementation: ServerWidget
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ServerWidget.h"

#include "ColouredListItem.h"
#include "ModelRenderer.h"
#include "EntityTextureManipulator.h"

#include "components/entitymapping/EntityMappingManager.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "components/ogre/GUIManager.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/mapping/ModelActionCreator.h"

#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/serversettings/ServerSettings.h"
#include "services/serversettings/ServerSettingsCredentials.h"

#include <Eris/SpawnPoint.h>
#include <Eris/CharacterType.h>
#include <Eris/TypeService.h>

#include <CEGUI/widgets/Listbox.h>
#include <CEGUI/widgets/PushButton.h>
#include <CEGUI/widgets/Editbox.h>
#include <CEGUI/widgets/MultiLineEditbox.h>
#include <CEGUI/widgets/RadioButton.h>
#include <CEGUI/widgets/ComboDropList.h>
#include <CEGUI/widgets/Combobox.h>
#include <CEGUI/widgets/TabControl.h>

#include <sigc++/bind.h>
#include <boost/algorithm/string.hpp>


WidgetPluginCallback registerWidget(Ember::OgreView::GUIManager& guiManager) {
	auto connectFn = [&guiManager](Eris::Connection* connection) mutable {
		auto widget = std::make_shared<Ember::OgreView::Gui::ServerWidget>(guiManager, *connection);

		connection->Disconnecting.connect([widget]() mutable {
			widget.reset();
			return true;
		});

	};
	auto con = Ember::EmberServices::getSingleton().getServerService().GotConnection.connect(connectFn);

	return [=]() {
		con->disconnect();
	};



//
//The following would allow for live reload, but there are other issues with connections not being fully cleaned up in ServerWidget to allow for it.
//For now we'll be happy with requiring that the user disconnects and connects again.
//struct ConnectionHolder {
//		sigc::connection conn;
//	};
//	auto connectionInner = std::make_shared<ConnectionHolder>();
//
//	auto connectFn = [connectionInner, &guiManager](Eris::Connection* connection) mutable {
//		auto widget = std::make_shared<Ember::OgreView::Gui::ServerWidget>(guiManager, *connection);
//
//		auto sigc_connection = connection->Disconnecting.connect([widget]() mutable {
//			widget.reset();
//			return true;
//		});
//		connectionInner->conn = sigc_connection;
//
//	};
//	auto con = Ember::EmberServices::getSingleton().getServerService().GotConnection.connect(connectFn);
//
//	if (Ember::EmberServices::getSingleton().getServerService().getConnection()) {
//		connectFn(Ember::EmberServices::getSingleton().getServerService().getConnection());
//	}
//
//	return [=]() {
//		if (connectionInner->conn) {
//			connectionInner->conn.disconnect();
//		}
//
//		con->disconnect();
//	};
//
}


using namespace CEGUI;
namespace Ember {
namespace OgreView {
namespace Gui {

ServerWidget::ServerWidget(GUIManager& guiManager, Eris::Connection& connection) :
		mWidget(guiManager.createWidget()),
		mConnection(connection),
		mAccount(nullptr),
		mCharacterList(nullptr),
		mCreateChar(nullptr),
		mUseCreator(nullptr),
		mTypesList(nullptr) {

	connection.GotServerInfo.connect([&]() { showServerInfo(connection); });
	connection.refreshServerInfo();

	buildWidget();
}

ServerWidget::~ServerWidget() {
	mWidget->getGUIManager().removeWidget(mWidget);
}

void ServerWidget::buildWidget() {

	if (mWidget->loadMainSheet("ServerWidget.layout", "Server/")) {

		CEGUI::PushButton* okButton = dynamic_cast<CEGUI::PushButton*> (mWidget->getMainWindow()->getChild("NoCharactersAlert/OkButton"));
		if (okButton) {
			BIND_CEGUI_EVENT(okButton, CEGUI::PushButton::EventClicked, ServerWidget::OkButton_Click)
		}

		CEGUI::PushButton* entityDestroyedOkButton = dynamic_cast<CEGUI::PushButton*> (mWidget->getMainWindow()->getChild("EntityDestroyed/OkButton"));
		if (entityDestroyedOkButton) {
			BIND_CEGUI_EVENT(entityDestroyedOkButton, CEGUI::PushButton::EventClicked, ServerWidget::EntityDestroyedOkButton_Click)
		}

		CEGUI::PushButton* login = dynamic_cast<CEGUI::PushButton*> (mWidget->getMainWindow()->getChild("LoginPanel/Login"));
		BIND_CEGUI_EVENT(login, CEGUI::PushButton::EventClicked, ServerWidget::Login_Click)
		CEGUI::PushButton* createAcc = dynamic_cast<CEGUI::PushButton*> (mWidget->getMainWindow()->getChild("LoginPanel/CreateAcc"));
		BIND_CEGUI_EVENT(createAcc, CEGUI::PushButton::EventClicked, ServerWidget::CreateAcc_Click)

		mCharacterList = dynamic_cast<CEGUI::Listbox*> (mWidget->getMainWindow()->getChild("LoggedInPanel/CharacterTabControl/ChooseCharacterPanel/CharacterList"));
		CEGUI::PushButton* chooseChar = dynamic_cast<CEGUI::PushButton*> (mWidget->getMainWindow()->getChild("LoggedInPanel/CharacterTabControl/ChooseCharacterPanel/Choose"));
		mUseCreator = dynamic_cast<CEGUI::PushButton*> (mWidget->getMainWindow()->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/UseCreator"));
		mCreateChar = dynamic_cast<CEGUI::PushButton*> (mWidget->getMainWindow()->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/CreateButton"));

		auto chooseFn = [=]() {
			CEGUI::ListboxItem* item = mCharacterList->getFirstSelectedItem();
			if (item) {

				std::string id = mCharacterModel[mCharacterList->getItemIndex(item)];

				EmberServices::getSingleton().getServerService().takeCharacter(id);
			}
			return true;
		};

		chooseChar->subscribeEvent(CEGUI::PushButton::EventClicked, chooseFn);
		mCharacterList->subscribeEvent(CEGUI::PushButton::EventMouseDoubleClick, chooseFn);
		BIND_CEGUI_EVENT(mUseCreator, CEGUI::PushButton::EventClicked, ServerWidget::UseCreator_Click)
		mCreateChar->subscribeEvent(CEGUI::PushButton::EventClicked, [=]() {
			EmberServices::getSingleton().getServerService().createCharacter(mNewChar.name, mNewChar.sex, mNewChar.type, mNewChar.description, mNewChar.spawnPoint);
			return true;
		});


		mWidget->getMainWindow()->getChild("LoggedInPanel/LogoutButton")->subscribeEvent(CEGUI::PushButton::EventClicked, [=]() {
			EmberServices::getSingleton().getServerService().logout();
			return true;
		});

		auto newCharName = dynamic_cast<CEGUI::Editbox*> (mWidget->getMainWindow()->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/NameEdit"));
		auto newCharDescription = dynamic_cast<CEGUI::MultiLineEditbox*> (mWidget->getMainWindow()->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/Description"));
		mTypesList = dynamic_cast<CEGUI::Combobox*> (mWidget->getMainWindow()->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/Type"));

		auto maleRadioButton = dynamic_cast<CEGUI::RadioButton*> (mWidget->getMainWindow()->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/Male"));
		auto femaleRadioButton = dynamic_cast<CEGUI::RadioButton*> (mWidget->getMainWindow()->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/Female"));

		newCharName->subscribeEvent(CEGUI::Editbox::EventTextChanged, [=]() {
			std::string name = newCharName->getText().c_str();
			mNewChar.name = std::move(name);
			updateNewCharacter();
			return true;
		});

		newCharDescription->subscribeEvent(CEGUI::Editbox::EventTextChanged, [=]() {
			std::string description = newCharDescription->getText().c_str();
			mNewChar.description = std::move(description);
			updateNewCharacter();
			return true;
		});

		BIND_CEGUI_EVENT(mTypesList, CEGUI::Combobox::EventListSelectionChanged, ServerWidget::TypesList_SelectionChanged)

		auto sexSelectionChangedFn = [=]() {
			CEGUI::RadioButton* selected = maleRadioButton->getSelectedButtonInGroup();
			if (selected) {
				mNewChar.sex = boost::algorithm::to_lower_copy(std::string(selected->getText().c_str()));
				updateNewCharacter();
			}
			return true;
		};

		maleRadioButton->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, sexSelectionChangedFn);
		femaleRadioButton->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, sexSelectionChangedFn);

		mWidget->getMainWindow()->getChild("LoggedInPanel/TeleportInfo/Yes")->subscribeEvent(CEGUI::PushButton::EventClicked, [=]() {
			mWidget->getWindow("TeleportInfo", true)->setVisible(false);
			if (mAvatarTransferInfo) {
				EmberServices::getSingleton().getServerService().takeTransferredCharacter(mAvatarTransferInfo->getTransferInfo());
			}
			return true;
		});

		mWidget->getMainWindow()->getChild("LoggedInPanel/TeleportInfo/No")->subscribeEvent(CEGUI::PushButton::EventClicked, [=]() {
			mWidget->getWindow("TeleportInfo", true)->setVisible(false);
			return true;
		});

		updateNewCharacter();

		CEGUI::Window* nameBox = mWidget->getMainWindow()->getChild("LoginPanel/NameEdit");
		CEGUI::Window* passwordBox = mWidget->getMainWindow()->getChild("LoginPanel/PasswordEdit");

		nameBox->subscribeEvent(CEGUI::Window::EventTextChanged, [=]() {
			hideLoginFailure();
			return true;
		});
		passwordBox->subscribeEvent(CEGUI::Window::EventTextChanged, [=]() {
			hideLoginFailure();
			return true;
		});

		mWidget->getMainWindow()->getChild("LoginPanel/Disconnect")->subscribeEvent(CEGUI::PushButton::EventClicked, [=]() {
			EmberServices::getSingleton().getServerService().disconnect();
			return true;
		});


		EmberServices::getSingleton().getServerService().GotAccount.connect(sigc::mem_fun(*this, &ServerWidget::createdAccount));
		EmberServices::getSingleton().getServerService().LoginSuccess.connect(sigc::mem_fun(*this, &ServerWidget::loginSuccess));
		EmberServices::getSingleton().getServerService().GotAvatar.connect(sigc::mem_fun(*this, &ServerWidget::gotAvatar));
		EmberServices::getSingleton().getServerService().GotAllCharacters.connect(sigc::mem_fun(*this, &ServerWidget::gotAllCharacters));
		EmberServices::getSingleton().getServerService().LoginFailure.connect(sigc::mem_fun(*this, &ServerWidget::showLoginFailure));
		EmberServices::getSingleton().getServerService().TransferInfoAvailable.connect(sigc::mem_fun(*this, &ServerWidget::server_TransferInfoAvailable));

		mWidget->addTabbableWindow(mWidget->getMainWindow()->getChild("LoginPanel/NameEdit"));
		mWidget->addTabbableWindow(mWidget->getMainWindow()->getChild("LoginPanel/PasswordEdit"));

		mWidget->addEnterButton(login);
		/*	addTabbableWindow(login);
		 addTabbableWindow(createAcc);*/
		mWidget->closeTabGroup();

		mWidget->addTabbableWindow(newCharName);
		// 	addTabbableWindow(mTypesList);
		/*	addTabbableWindow(mSexRadioButton);
		 addTabbableWindow(femaleRadioButton);*/
		mWidget->addTabbableWindow(newCharDescription);
		mWidget->addEnterButton(mCreateChar);
		mWidget->closeTabGroup();

		createPreviewTexture();
	}

}


void ServerWidget::server_TransferInfoAvailable(const std::vector<AvatarTransferInfo>& transferInfos) {
	if (!transferInfos.empty()) {
		CEGUI::Window* teleportInfo = mWidget->getWindow("TeleportInfo", true);
		teleportInfo->setVisible(true);
		mAvatarTransferInfo = transferInfos[0];
	}
}

void ServerWidget::createdAccount(Eris::Account* account) {
	mAccount = account;
	mWidget->show();
	mWidget->getMainWindow()->moveToFront();
}

void ServerWidget::showServerInfo(Eris::Connection& connection) {
	try {
		CEGUI::Window* info = mWidget->getWindow("Info");
		Eris::ServerInfo sInfo;
		connection.getServerInfo(sInfo);
		std::stringstream ss;
		ss << "Server name: " << sInfo.name << "\n";
		ss << "Ruleset: " << sInfo.ruleset << "\n";
		ss << "Server type: " << sInfo.server << " (v. " << sInfo.version << ")\n";
		ss << "Ping: " << sInfo.ping << "\n";
		ss << "Uptime: " << static_cast<int> (sInfo.uptime / (60 * 60 * 24)) << " days\n";
		ss << "Number of clients: " << sInfo.clients << "\n";
		info->setText(ss.str());

		/*
		 * Since we're using the server getHostname as a section name
		 * we must wait until there is a connection before we can fetch
		 * the credentials
		 */
		CEGUI::Window* nameBox = mWidget->getMainWindow()->getChild("LoginPanel/NameEdit");
		CEGUI::Window* passwordBox = mWidget->getMainWindow()->getChild("LoginPanel/PasswordEdit");
		std::string savedUser;
		std::string savedPass;
		if (fetchCredentials(connection, savedUser, savedPass)) {
			nameBox->setText(savedUser);
			passwordBox->setText(savedPass);
		}

	} catch (...) {
		S_LOG_WARNING("Error when getting the server info window.");
		return;
	}
}

bool ServerWidget::fetchCredentials(Eris::Connection& connection, std::string& user, std::string& pass) {
	S_LOG_VERBOSE("Fetching saved credentials.");

	Eris::ServerInfo sInfo;
	connection.getServerInfo(sInfo);

	Services::ServerSettingsCredentials serverCredentials(sInfo);
	Services::ServerSettings& serverSettings = EmberServices::getSingleton().getServerSettingsService();
	if (serverSettings.findItem(serverCredentials, "username")) {
		user = static_cast<std::string>(serverSettings.getItem(serverCredentials, "username"));
	}
	if (serverSettings.findItem(serverCredentials, "password")) {
		pass = static_cast<std::string>(serverSettings.getItem(serverCredentials, "password"));
	}
	return !pass.empty() && !user.empty();
}

bool ServerWidget::saveCredentials() {
	S_LOG_VERBOSE("Saving credentials.");

	// check the main account is good, and fetch server info
	assert(mAccount);
	Eris::ServerInfo sInfo;
	mAccount->getConnection().getServerInfo(sInfo);

	// pull widget references

	try {
		auto nameBox = mWidget->getMainWindow()->getChild("LoginPanel/NameEdit");
		auto passwordBox = mWidget->getMainWindow()->getChild("LoginPanel/PasswordEdit");
		auto saveBox = dynamic_cast<CEGUI::ToggleButton*> (mWidget->getMainWindow()->getChild("LoginPanel/SavePassCheck"));
		if (nameBox && passwordBox && saveBox) {

			// fetch info from widgets
			const CEGUI::String& name = nameBox->getText();
			const CEGUI::String& password = passwordBox->getText();
			Services::ServerSettingsCredentials serverCredentials(sInfo);
			Services::ServerSettings& serverSettings = EmberServices::getSingleton().getServerSettingsService();
			serverSettings.setItem(serverCredentials, "username", name.c_str());
			serverSettings.setItem(serverCredentials, "password", password.c_str());
			serverSettings.writeToDisk();
			return true;
		}
		return false;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when getting windows from CEGUI." << ex);
		return false;
	}
}

void ServerWidget::logoutComplete(bool clean) {
	mWidget->getWindow("LoginPanel")->setVisible(true);
	mWidget->getWindow("LoggedInPanel")->setVisible(false);
	mTypeServiceConnection.disconnect();
}

void ServerWidget::loginSuccess(Eris::Account* account) {
	account->LogoutComplete.connect(sigc::mem_fun(*this, &ServerWidget::logoutComplete));
	mWidget->getWindow("LoginPanel")->setVisible(false);
	mWidget->getWindow("LoggedInPanel")->setVisible(true);
	account->refreshCharacterInfo();
	fillAllowedCharacterTypes(account);

	CEGUI::ToggleButton* saveBox = dynamic_cast<CEGUI::ToggleButton*> (mWidget->getMainWindow()->getChild("LoginPanel/SavePassCheck"));
	if (saveBox->isSelected()) {
		try {
			saveCredentials();
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when saving password." << ex);
		} catch (...) {
			S_LOG_FAILURE("Unspecified error when saving password.");
		}
	}

	mTypeServiceConnection = account->getConnection().getTypeService().BoundType.connect(sigc::mem_fun(*this, &ServerWidget::typeService_TypeBound));

}

void ServerWidget::showLoginFailure(Eris::Account* account, std::string msg) {
	auto helpText = mWidget->getMainWindow()->getChild("LoginPanel/HelpText");
	helpText->setYPosition(UDim(0.6, 0));

	auto loginFailure = mWidget->getMainWindow()->getChild("LoginPanel/LoginFailure");
	loginFailure->setText(msg);
	loginFailure->setVisible(true);
}

bool ServerWidget::hideLoginFailure() {
	auto helpText = mWidget->getMainWindow()->getChild("LoginPanel/HelpText");
	helpText->setYPosition(UDim(0.55, 0));

	auto loginFailure = mWidget->getMainWindow()->getChild("LoginPanel/LoginFailure");
	loginFailure->setVisible(false);

	return true;
}


void ServerWidget::fillAllowedCharacterTypes(Eris::Account* account) {
	mTypesList->resetList();

	/**
	 * The preferred way is to use the spawn point system. However, older servers don't support that, and instead present a single list of characters.
	 * For spawn points, we still want to show a single list of available characters. In those cases where a character is present in multiple spawn points we'll print out the spawn point name in parenthesis.
	 */
	const Eris::SpawnPointMap& spawnPoints = account->getSpawnPoints();

	for (const auto& entry : spawnPoints) {
		const Eris::SpawnPoint& spawnPoint = entry.second;
		for (const auto& characterMapEntry : spawnPoint.getAvailableCharacterTypes()) {
			mCharacterAndSpawns.insert(CharacterAndSpawnsStore::value_type(characterMapEntry.getName(), spawnPoint.getName()));
		}
	}

	const std::vector<std::string>& characters = account->getCharacterTypes();

	//If the account inherits from "admin" we're an admin and can create a creator entity. This also applies if we're a "sys" account.
	if (account->getParent() == "admin" || account->getParent() == "sys") {
		mUseCreator->setVisible(true);
		mUseCreator->setEnabled(true);
	} else {
		mUseCreator->setVisible(false);
		mUseCreator->setEnabled(false);
	}

	if (mCharacterAndSpawns.empty() && characters.empty()) {
		showNoCharactersAlert();
	} else {
		//The preferred way is by using spawn points. Using the character type list is kept for backwards compatibility.
		if (!mCharacterAndSpawns.empty()) {
			unsigned int i = 0;
			for (CharacterAndSpawnsStore::const_iterator I = mCharacterAndSpawns.begin(); I != mCharacterAndSpawns.end(); ++I) {
				std::string name = I->first;
				if (mCharacterAndSpawns.count(I->first) > 1) {
					name += " (" + I->second + ")";
				}
				mTypesList->addItem(new Gui::ColouredListItem(name, i++, nullptr));
			}

		} else {
			unsigned int i = 0;
			for (const auto& character : characters) {

				CEGUI::ListboxItem* item = new Gui::ColouredListItem(character, i++, nullptr);
				mTypesList->addItem(item);
			}
		}
	}
	//If only one selection, select the first one.
	if (mTypesList->getItemCount() == 1) {
		mTypesList->setItemSelectState(mTypesList->getListboxItemFromIndex(0), true);
	}
}

void ServerWidget::gotAllCharacters(Eris::Account* account) {
	mCharacterList->resetList();
	mCharacterModel.clear();
	Eris::CharacterMap cm = account->getCharacters();
	auto I = cm.begin();
	auto I_end = cm.end();

	if (I == I_end) {
		//if the user has no previous characters, show the create character tab

		CEGUI::TabControl* tabControl = dynamic_cast<CEGUI::TabControl*> (mWidget->getWindow("CharacterTabControl"));
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
				if (!entity->getParent().empty()) {
					itemText = entity->getParent();
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

bool ServerWidget::UseCreator_Click(const CEGUI::EventArgs& args) {
	//create a new admin character
	Atlas::Message::MapType extraProperties;
	//The admin character should always be transient.
	//NOTE: This is currently not handled in Cyphesis, as it allows no extra parameters. Nonetheless we'll keep it here, since it's a prudent thing to do.
	extraProperties.insert(std::make_pair("transient", -1.0f));

	//If there are any spawn points defined we'll use the first one.
	//It would be nice if we could present the user with an option to choose amongst many,
	//or just to spawn at origo. But for now this will do in its simplicity.
	const Eris::SpawnPointMap& spawnPoints = mAccount->getSpawnPoints();
	std::string spawn;
	if (!spawnPoints.empty()) {
		spawn = spawnPoints.begin()->second.getName();
	}
	EmberServices::getSingleton().getServerService().createCharacter("The Creator", "female", "creator", "Almighty", spawn, extraProperties);
	return true;
}


bool ServerWidget::TypesList_SelectionChanged(const CEGUI::EventArgs& args) {
	CEGUI::ListboxItem* item = mTypesList->getSelectedItem();
	if (item) {

		unsigned int itemIndex = item->getID();
		//Check if we have a list of spawn points, or if not the older single character type list.
		if (!mCharacterAndSpawns.empty()) {
			CharacterAndSpawnsStore::const_iterator I = mCharacterAndSpawns.begin();
			for (unsigned int i = 0; i < itemIndex; ++i) {
				++I;
			}
			mNewChar.type = I->first;
			mNewChar.spawnPoint = I->second;
		} else {
			mNewChar.type = mAccount->getCharacterTypes()[itemIndex];
		}

		if (mModelPreviewRenderer) {

			mPreviewTypeName = mNewChar.type;
			preparePreviewForTypeOrArchetype(mPreviewTypeName);

		}
	}
	updateNewCharacter();
	return true;
}


void ServerWidget::preparePreviewForTypeOrArchetype(std::string typeOrArchetype) {
	auto& typeService = mAccount->getConnection().getTypeService();
	Eris::TypeInfo* erisType = typeService.getTypeByName(typeOrArchetype);
	//If the type is an archetype, we need to instead check what kind of entity will be created and show a preview for that.
	if (erisType && erisType->isBound()) {
		if (erisType->getObjType() == "archetype") {
			if (!erisType->getEntities().empty()) {
				//Get the first entity and use that
				const Atlas::Message::Element& firstEntityElement = erisType->getEntities().front();
				//TODO: Make is possible to create a DetachedEntity from archetypes, and use that
				if (firstEntityElement.isMap()) {
					auto parentElementI = firstEntityElement.Map().find("parent");
					if (parentElementI != firstEntityElement.Map().end() && parentElementI->second.isString()) {
						mPreviewTypeName = parentElementI->second.String();
						preparePreviewForTypeOrArchetype(mPreviewTypeName);
					}
				}
			}
		} else {
			Authoring::DetachedEntity entity("0", erisType, typeService);
			Atlas::Message::MapType message = {{"sex", mNewChar.sex}};
			entity.setFromMessage(message);
			showPreview(entity);
		}
	}
}

void ServerWidget::showPreview(Ember::OgreView::Authoring::DetachedEntity& entity) {
	Mapping::ModelActionCreator actionCreator(entity, [&](const std::string& model) {
		//update the model preview window
		mModelPreviewRenderer->showModel(model);
		//mModelPreviewRenderer->showFull();
		//we want to zoom in a little
		mModelPreviewRenderer->setCameraDistance(0.7);
	}, [&](const std::string& part) {
		if (mModelPreviewRenderer->getModel()) {
			mModelPreviewRenderer->getModel()->showPart(part);
		}
	});

	auto mapping = Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(entity, actionCreator, entity.getType()->getTypeService(), nullptr);
	if (mapping) {
		mapping->initialize();
	}
}

void ServerWidget::typeService_TypeBound(Eris::TypeInfo* type) {
	if (type->getName() == mPreviewTypeName) {
		preparePreviewForTypeOrArchetype(type->getName());
	}
}

void ServerWidget::updateNewCharacter() {
	mCreateChar->setEnabled(mNewChar.isValid());
	if (!mPreviewTypeName.empty()) {
		preparePreviewForTypeOrArchetype(mPreviewTypeName);
	}
}

bool ServerWidget::Login_Click(const CEGUI::EventArgs& args) {
	CEGUI::Window* nameBox = mWidget->getMainWindow()->getChild("LoginPanel/NameEdit");
	CEGUI::Window* passwordBox = mWidget->getMainWindow()->getChild("LoginPanel/PasswordEdit");

	const CEGUI::String& name = nameBox->getText();
	const CEGUI::String& password = passwordBox->getText();

	mAccount->login(std::string(name.c_str()), std::string(password.c_str()));

	return true;
}

bool ServerWidget::CreateAcc_Click(const CEGUI::EventArgs& args) {
	CEGUI::Window* nameBox = mWidget->getMainWindow()->getChild("LoginPanel/NameEdit");
	CEGUI::Window* passwordBox = mWidget->getMainWindow()->getChild("LoginPanel/PasswordEdit");

	const CEGUI::String& name = nameBox->getText();
	const CEGUI::String& password = passwordBox->getText();

	mAccount->createAccount(name.c_str(), name.c_str(), password.c_str());
	return true;
}

bool ServerWidget::OkButton_Click(const CEGUI::EventArgs& args) {
	CEGUI::Window* alert = mWidget->getWindow("NoCharactersAlert");
	if (alert) {
		alert->hide();
	}
	return true;
}

bool ServerWidget::EntityDestroyedOkButton_Click(const CEGUI::EventArgs& args) {
	CEGUI::Window* alert = mWidget->getWindow("EntityDestroyed");
	if (alert) {
		alert->hide();
	}
	return true;
}

void ServerWidget::gotAvatar(Eris::Avatar* avatar) {
	mTypeServiceConnection.disconnect();

	mAccount->AvatarDeactivated.connect(sigc::mem_fun(*this, &ServerWidget::avatar_Deactivated));
	avatar->CharacterEntityDeleted.connect(sigc::mem_fun(*this, &ServerWidget::avatar_EntityDeleted));
	mWidget->hide();
}


void ServerWidget::avatar_EntityDeleted() {
	CEGUI::Window* alert = mWidget->getWindow("EntityDestroyed");
	if (alert) {
		alert->show();
	}
}


void ServerWidget::avatar_Deactivated(const std::string& avatarId) {
	mCharacterList->resetList();
	mCharacterModel.clear();
	mAccount->refreshCharacterInfo();
	mWidget->show();
	mWidget->getMainWindow()->moveToFront();
	mWidget->getWindow("LoginPanel")->setVisible(false);
	mWidget->getWindow("LoggedInPanel")->setVisible(true);
	gotAllCharacters(mAccount);
}

void ServerWidget::createPreviewTexture() {
	auto imageWidget = mWidget->getMainWindow()->getChild("LoggedInPanel/CharacterTabControl/CreateCharacterPanel/Image");
	if (!imageWidget) {
		S_LOG_FAILURE("Could not find CreateCharacterPanel/Image, aborting creation of preview texture.");
	} else {
		mModelPreviewRenderer = std::make_unique<ModelRenderer>(imageWidget, "newCharacterPreview");
		mModelPreviewManipulator = std::make_unique<CameraEntityTextureManipulator>(*imageWidget, mModelPreviewRenderer->getEntityTexture());
	}

}

void ServerWidget::showNoCharactersAlert() {
	CEGUI::Window* alert = mWidget->getWindow("NoCharactersAlert");
	alert->setVisible(true);
	alert->moveToFront();
}


bool NewCharacter::isValid() const {
	return !name.empty() && !sex.empty() && !type.empty();
}

}
}
}
