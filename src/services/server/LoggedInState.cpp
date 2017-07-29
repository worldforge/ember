/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "LoggedInState.h"
#include "ServerServiceSignals.h"
#include "TransferInfoStringSerializer.h"
#include "AvatarTransferInfo.h"

#include "services/config/ConfigService.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "framework/LoggingInstance.h"

#include <Eris/Account.h>
#include <Eris/Avatar.h>
#include <Eris/Connection.h>
#include <Eris/Entity.h>
#include <Eris/EventService.h>

#include <fstream>

#include <sigc++/bind.h>

namespace Ember
{
LoggedInState::LoggedInState(IState& parentState, Eris::Account& account) :
		StateBase<EnteredWorldState>::StateBase(parentState), Logout("logout", this, "Logout from the connected server."), CreateChar("add", this, "Create a character on the server."), TakeChar("take", this, "Take control of one of your characters."), ListChars("list", this, "List you available characters on the server."), mAccount(account), mTransferEvent(0)
{
	mAccount.AvatarSuccess.connect(sigc::mem_fun(*this, &LoggedInState::gotAvatarSuccess));
	mAccount.GotCharacterInfo.connect(sigc::mem_fun(*this, &LoggedInState::gotCharacterInfo));
	mAccount.GotAllCharacters.connect(sigc::mem_fun(*this, &LoggedInState::gotAllCharacters));
	checkTransfer();

}

LoggedInState::~LoggedInState()
{
	delete mTransferEvent;
}

void LoggedInState::checkTransfer()
{
	TransferInfoStringSerializer serializer;
	std::string teleportFilePath(EmberServices::getSingleton().getConfigService().getHomeDirectory(BaseDirType_DATA) + "teleports");
	std::fstream teleportsFile(teleportFilePath.c_str(), std::ios_base::in);
	TransferInfoStringSerializer::TransferInfoStore transferObjects;
	if (teleportsFile.good()) {
		serializer.deserialize(transferObjects, teleportsFile);
	}
	teleportsFile.close();

	mTransferInfos.clear();

	for (TransferInfoStringSerializer::TransferInfoStore::const_iterator I = transferObjects.begin(); I != transferObjects.end(); ++I) {
		const AvatarTransferInfo& avatarTransferInfo(*I);
		const Eris::TransferInfo& transferInfo(avatarTransferInfo.getTransferInfo());
		if (transferInfo.getHost() == mAccount.getConnection()->getHost() && transferInfo.getPort() == mAccount.getConnection()->getPort()) {
			mTransferInfos.push_back(avatarTransferInfo);
			//			mAccount.takeTransferredCharacter(transferInfo.getPossessEntityId(), transferInfo.getPossessKey());
		}
	}

	if (mTransferInfos.size() > 0) {
		getSignals().TransferInfoAvailable(mTransferInfos);
	}

}

void LoggedInState::takeTransferredCharacter(const Eris::TransferInfo& transferInfo)
{
	S_LOG_INFO("Trying to take transferred character with id " << transferInfo.getPossessEntityId() << ".");
	mAccount.takeTransferredCharacter(transferInfo.getPossessEntityId(), transferInfo.getPossessKey());
}

void LoggedInState::takeCharacter(const std::string &id)
{
	mAccount.takeCharacter(id);
}

bool LoggedInState::createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName, const Atlas::Message::MapType& extraProperties)
{
	ConsoleBackend::getSingleton().pushMessage("Creating char...", "important");
	std::string msg;
	msg = "Creating character of type '" + type + "' with name '" + name + "' and sex '" + sex + "'.";
	ConsoleBackend::getSingleton().pushMessage(msg, "info");

	S_LOG_INFO("Creating character.");
	Atlas::Objects::Entity::RootEntity character;
	for (auto& property : extraProperties) {
		character->setAttr(property.first, property.second);
	}

	character->setParent(type);
	character->setName(name);
	character->setAttr("sex", sex);
	character->setAttr("description", description);
	if (spawnName != "") {
		character->setAttr("spawn_name", spawnName);
	}


	try {
		mAccount.createCharacter(character);
	} catch (const std::exception& except) {
		S_LOG_WARNING("Got Eris error on character creation." << except);
		return false;
	} catch (...) {
		S_LOG_WARNING("Got unknown error on character creation.");
		return false;
	}
	S_LOG_INFO("Done creating character.");

	return true;
}

bool LoggedInState::logout()
{
	Eris::Result result = mAccount.logout();
	if (result != Eris::NO_ERR) {
		//If something went wrong when logging out, just disconnect.
		disconnect();
	}

	return result == Eris::NO_ERR;
}

void LoggedInState::gotCharacterInfo(const Atlas::Objects::Entity::RootEntity & info)
{
	S_LOG_INFO("Got character info");
	getSignals().GotCharacterInfo.emit(info);
}

void LoggedInState::gotAllCharacters()
{
	S_LOG_INFO("Got all characters");
	getSignals().GotAllCharacters.emit(&mAccount);
}

void LoggedInState::gotAvatarSuccess(Eris::Avatar* avatar)
{
	//First check if there are any transfer infos for this server, and if this login operation means that one of them successfully has been used.
	//If so, it should be removed from the persistent storage.
	if (mTransferInfos.size() > 0) {
		for (AvatarTransferInfoStore::iterator I = mTransferInfos.begin(); I != mTransferInfos.end(); ++I) {
			AvatarTransferInfo& info = *I;
			if (info.getTransferInfo().getPossessEntityId() == avatar->getEntity()->getId()) {
				removeTransferInfo(info);

				break;
			}
		}
	}

	avatar->TransferRequested.connect(sigc::bind(sigc::mem_fun(*this, &LoggedInState::avatar_transferRequest), avatar));

	setChildState(new EnteredWorldState(*this, *avatar, mAccount));
	mAccount.AvatarDeactivated.connect(sigc::mem_fun(*this, &LoggedInState::gotAvatarDeactivated));
}

void LoggedInState::removeTransferInfo(const AvatarTransferInfo& transferInfo)
{
	TransferInfoStringSerializer serializer;
	const std::string teleportFilePath(EmberServices::getSingleton().getConfigService().getHomeDirectory(BaseDirType_DATA) + "teleports");
	std::fstream teleportsFile(teleportFilePath.c_str(), std::ios_base::in);
	TransferInfoStringSerializer::TransferInfoStore transferObjects;
	if (teleportsFile.good()) {
		serializer.deserialize(transferObjects, teleportsFile);
	} else {
		return;
	}
	teleportsFile.close();

	//Find the transfer info amongst the persisted ones and remove it.
	for (AvatarTransferInfoStore::iterator I = transferObjects.begin(); I != transferObjects.end(); ++I) {
		AvatarTransferInfo& info = *I;
		const Eris::TransferInfo& transferInfo = info.getTransferInfo();
		if (transferInfo.getHost() == mAccount.getConnection()->getHost() && transferInfo.getPort() == mAccount.getConnection()->getPort()) {
			transferObjects.erase(I);
		}
	}

	std::fstream teleportsOutputFile(teleportFilePath.c_str(), std::ios_base::out);
	if (teleportsOutputFile.good()) {
		serializer.serialize(transferObjects, teleportsOutputFile);
	} else {
		S_LOG_CRITICAL("Could not write teleports info to file. This means that the teleported character cannot be claimed.");
	}
	teleportsOutputFile.close();

}

void LoggedInState::avatar_transferRequest(const Eris::TransferInfo& transferInfo, const Eris::Avatar* avatar)
{
	TransferInfoStringSerializer serializer;
	std::string teleportFilePath(EmberServices::getSingleton().getConfigService().getHomeDirectory(BaseDirType_DATA) + "teleports");
	std::fstream teleportsFile(teleportFilePath.c_str(), std::ios_base::in);
	TransferInfoStringSerializer::TransferInfoStore transferObjects;
	if (teleportsFile.good()) {
		serializer.deserialize(transferObjects, teleportsFile);
	}
	teleportsFile.close();
	AvatarTransferInfo avatarTransferInfo(avatar->getEntity()->getName(), WFMath::TimeStamp::now(), transferInfo);
	transferObjects.push_back(avatarTransferInfo);

	std::fstream teleportsOutputFile(teleportFilePath.c_str(), std::ios_base::out);
	if (teleportsOutputFile.good()) {
		serializer.serialize(transferObjects, teleportsOutputFile);
	} else {
		S_LOG_CRITICAL("Could not write teleports info to file. This means that the teleported character cannot be claimed.");
	}
	teleportsOutputFile.close();

	mTransferEvent = new Eris::TimedEvent(mAccount.getConnection()->getEventService(), boost::posix_time::seconds(0), [=](){
		this->transfer(transferInfo);
	});
}

void LoggedInState::gotAvatarDeactivated(Eris::Avatar* avatar)
{
	destroyChildState();
}

void LoggedInState::runCommand(const std::string &command, const std::string &args)
{
	if (Logout == command) {
		ConsoleBackend::getSingleton().pushMessage("Logging out...", "important");
		mAccount.logout();

		// Create Character command
	} else if (CreateChar == command) {
		// Split string into name/type/sex/description
		Tokeniser tokeniser = Tokeniser();
		tokeniser.initTokens(args);
		std::string name = tokeniser.nextToken();
		std::string sex = tokeniser.nextToken();
		std::string type = tokeniser.nextToken();
		std::string spawnPoint = tokeniser.nextToken();
		std::string description = tokeniser.remainingTokens();

		createCharacter(name, sex, type, description, spawnPoint, Atlas::Message::MapType());

		// Take Character Command
	} else if (TakeChar == command) {

		takeCharacter(args);

		// List Characters Command
	} else if (ListChars == command) {

		mAccount.refreshCharacterInfo();

		// Say (In-Game chat) Command
	}
}

}
