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

namespace Ember {

LocalServerAdminCreator::LocalServerAdminCreator(ServerService& serverService) {
	serverService.GotAccount.connect(sigc::mem_fun(*this, &LocalServerAdminCreator::server_GotAccount));
	serverService.LoginSuccess.connect(sigc::mem_fun(*this, &LocalServerAdminCreator::server_LoginSuccess));
}

void LocalServerAdminCreator::server_GotAccount(Eris::Account* account) {
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

void LocalServerAdminCreator::server_LoginSuccess(Eris::Account* account) {
	mAdminEntityCreator = std::make_unique<AdminEntityCreator>(*account);
}

AdminEntityCreator::AdminEntityCreator(Eris::Account& account)
		: mAccount(account) {
	//If there are any spawn points defined we'll use the first one.
	//It would be nice if we could present the user with an option to choose amongst many,
	//or just to spawn at origo. But for now this will do in its simplicity.
	auto& spawnPoints = account.getSpawnPoints();
	if (!spawnPoints.empty()) {
		//Get the first spawn
		auto& spawn = spawnPoints.front();

		Atlas::Objects::Operation::Get get;
		get->setSerialno(Eris::getNewSerialno());

		get->setTo(account.getId());
		get->setFrom(account.getId());
		Atlas::Objects::Entity::Anonymous ent;
		ent->setId(spawn.id);
		ent->setObjtype("object");
		get->setArgs1(std::move(ent));

		account.getConnection().getResponder().await(get->getSerialno(), this, &AdminEntityCreator::operationGetResult);
		account.getConnection().send(get);
	}
}

void AdminEntityCreator::operationGetResult(const Atlas::Objects::Operation::RootOperation& op) {
	if (op->getClassNo() == Atlas::Objects::Operation::INFO_NO) {
		if (!op->getArgs().empty()) {
			auto arg = Atlas::Objects::smart_dynamic_cast<Atlas::Objects::Entity::RootEntity>(op->getArgs().front());
			if (arg) {
				auto pos = arg->getPos();
				auto loc = arg->getLoc();

				Atlas::Objects::Entity::Anonymous ent;
				ent->setAttr("__account", mAccount.getUsername());
				ent->setName("admin");
				ent->setParent("creator");
				ent->setPos(pos);
				ent->setLoc(loc);
				Create c;
				c->setArgs1(ent);
				c->setFrom(mAccount.getId());
				c->setTo("0");
				mAccount.createCharacterThroughOperation(c);
			}
		}
	}

}

}
