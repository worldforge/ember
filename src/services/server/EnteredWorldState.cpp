/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EnteredWorldState.h"
#include "ServerServiceSignals.h"

#include "services/config/ConfigService.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "framework/LoggingInstance.h"

#include <Atlas/Objects/Entity.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Message/Element.h>
#include <Atlas/Objects/Anonymous.h>

#include <Eris/Entity.h>
#include <Eris/Avatar.h>
#include <Eris/Account.h>
#include <Eris/Connection.h>
#include <Eris/View.h>
#include <Eris/TypeInfo.h>
#include <Eris/TypeService.h>

#include <wfmath/atlasconv.h>
#include <wfmath/quaternion.h>

#include <sstream>
#include <iostream>
#include <fstream>

namespace Ember
{

EnteredWorldState::EnteredWorldState(IState& parentState, Eris::Avatar& avatar, Eris::Account& account) :
	StateBase<void>::StateBase(parentState), Say("say", this, "Say something."), Emote("me", this, "Emotes something."), Delete("delete", this, "Deletes an entity."), AdminTell("admin_tell", this, "Uses admin mode to directly tell a NPC something. Usage: /admin_tell <entityid> <key> <value>"), mAvatar(avatar), mAccount(account), mAdapter(account, avatar)
{
	avatar.TransferRequested.connect(sigc::mem_fun(*this, &EnteredWorldState::avatar_transferRequest));
	getSignals().GotAvatar.emit(&mAvatar);
	getSignals().GotView.emit(&getView());

}

EnteredWorldState::~EnteredWorldState()
{
	getSignals().DestroyedView.emit();
	getSignals().DestroyedAvatar.emit();
}

bool EnteredWorldState::logout()
{
	mAvatar.deactivate();
	return true;
}

void EnteredWorldState::runCommand(const std::string &command, const std::string &args)
{
	if (Say == command) {
		mAdapter.say(args);
	} else if (Emote == command) {
		mAdapter.emote(args);
	} else if (Delete == command) {
		Tokeniser tokeniser = Tokeniser();
		tokeniser.initTokens(args);
		std::string entityId = tokeniser.nextToken();
		if (entityId != "") {
			Eris::Entity* entity = getView().getEntity(entityId);
			if (entity) {
				mAdapter.deleteEntity(entity);
			}
		}

		/*		// Touch Command
		 } else if (command==TOUCH) {
		 // TODO: make this switch call the touch method
		 // TODO: polish this rough check
		 S_LOG_VERBOSE("Touching");
		 if(!mAvatar) {
		 S_LOG_WARNING("No avatar.");
		 return;
		 }

		 Atlas::Objects::Operation::Touch touch;
		 Atlas::Message::MapType opargs;

		 opargs["id"] = args;
		 touch->setFrom(mAvatar->getId());
		 touch->setArgsAsList(Atlas::Message::ListType(1, opargs));

		 mConn->send(touch);*/
	} else if (AdminTell == command) {
		Tokeniser tokeniser = Tokeniser();
		tokeniser.initTokens(args);
		std::string entityId = tokeniser.nextToken();
		if (entityId != "") {
			std::string key = tokeniser.nextToken();
			if (key != "") {
				std::string value = tokeniser.nextToken();
				if (value != "") {
					mAdapter.adminTell(entityId, key, value);
				}
			}
		}
	}
}

void EnteredWorldState::avatar_transferRequest(const Eris::TransferInfo& transferInfo)
{
	TransferInfoStringSerializer serializer;
	std::string teleportFilePath(EmberServices::getSingleton().getConfigService()->getHomeDirectory() + "/teleports");
	std::ifstream teleportsFile(teleportFilePath.c_str());
	TransferInfoStringSerializer::TransferInfoStore transferObjects;
	if (teleportsFile.good()) {
		serializer.deserialize(transferObjects, teleportsFile);
	}
	teleportsFile.close();
	transferObjects.push_back(transferInfo);

	std::ofstream teleportsOutputFile(teleportFilePath.c_str());
	if (teleportsOutputFile.good()) {
		serializer.serialize(transferObjects, teleportsOutputFile);
	}
	teleportsOutputFile.close();

	transfer(transferInfo);
}

IServerAdapter& EnteredWorldState::getServerAdapter()
{
	return mAdapter;
}

Eris::Connection& EnteredWorldState::getConnection() const
{
	return *mAvatar.getConnection();
}

Eris::View& EnteredWorldState::getView() const
{
	return *mAvatar.getView();
}

}
