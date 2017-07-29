/*
 Copyright (C) 2013 Erik Ogenvik

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

#include "LocalServerAdminCreator.h"

#include "services/server/ServerService.h"

#include <Eris/Account.h>
#include <Eris/Connection.h>
#include <Eris/Response.h>
#include <Eris/CustomEntities.h>
#include <Eris/SpawnPoint.h>

#include <Atlas/Objects/Operation.h>

#include <wfmath/MersenneTwister.h>

using Atlas::Message::Element;
using Atlas::Message::ListType;
using Atlas::Message::MapType;
using Atlas::Objects::Root;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Operation::Create;
using Atlas::Objects::Operation::Login;
using Atlas::Objects::Operation::RootOperation;

namespace Ember
{

LocalServerAdminCreator::LocalServerAdminCreator(ServerService& serverService) :
		mServerService(serverService)
{
	serverService.GotAccount.connect(sigc::mem_fun(*this, &LocalServerAdminCreator::server_GotAccount));
	serverService.LoginSuccess.connect(sigc::mem_fun(*this, &LocalServerAdminCreator::server_LoginSuccess));
}

LocalServerAdminCreator::~LocalServerAdminCreator()
{
}

void LocalServerAdminCreator::server_GotAccount(Eris::Account* account)
{
	//When connecting to a local socket we should use a "sys" account.
	//This works very much like an "admin" account, but is not persisted on the server.
	//Thus we will first create a new account on the server, with a random name and password.

	std::stringstream ss_password, ss_username;

	WFMath::MTRand rand;
	for (int i = 0; i < 20; ++i) {
		ss_password << rand.randInt(9);
	}

	ss_username << "admin_ember_";
	for (int i = 0; i < 5; ++i) {
		ss_username << rand.randInt(9);
	}

	Atlas::Objects::Entity::Sys sysAccountOp;

	sysAccountOp->setAttr("username", ss_username.str());
	sysAccountOp->setAttr("password", ss_password.str());
	account->createAccount(sysAccountOp);

}

void LocalServerAdminCreator::server_LoginSuccess(Eris::Account* account)
{
	//If there are any spawn points defined we'll use the first one.
	//It would be nice if we could present the user with an option to choose amongst many,
	//or just to spawn at origo. But for now this will do in its simplicity.
	const Eris::SpawnPointMap& spawnPoints = account->getSpawnPoints();
	std::string spawn;
	if (!spawnPoints.empty()) {
		spawn = spawnPoints.begin()->second.getName();
	}


	mServerService.createCharacter("admin", "female", "creator", "", spawn, Atlas::Message::MapType());
}
}
