//
// Copyright (C) 2009 Alistair Riddoch
// Copyright (C) 2012 Erik Ogenvik
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#include "EntityExporter.h"

#include "LoggingInstance.h"
#include "Time.h"

#include <Atlas/MultiLineListFormatter.h>

#include <Eris/Account.h>
#include <Eris/Connection.h>
#include <Eris/Response.h>
#include <Eris/Avatar.h>

using Atlas::Objects::Root;
using Atlas::Objects::smart_dynamic_cast;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Entity::RootEntity;
using Atlas::Objects::Operation::Talk;
using Atlas::Objects::Operation::Get;
using Atlas::Message::Element;
using Atlas::Message::ListType;
using Atlas::Message::MapType;

namespace Ember
{

EntityExporter::EntityExporter(Eris::Account& account) :
		EntityExporterBase(account.getId(), account.getActiveCharacters().begin()->second->getId(), getCurrentTime()), mAccount(account)
{
}

EntityExporter::~EntityExporter()
{
}

long int EntityExporter::newSerialNumber()
{
	return Eris::getNewSerialno();
}

void EntityExporter::send(const Atlas::Objects::Operation::RootOperation& op)
{
	mAccount.getConnection()->send(op);
}

void EntityExporter::sendAndAwaitResponse(const Atlas::Objects::Operation::RootOperation& op, CallbackFunction& callback)
{
	mCallbacks.insert(std::make_pair(op->getSerialno(), callback));
	mAccount.getConnection()->getResponder()->await(op->getSerialno(), this, &EntityExporter::operationResult);
	mAccount.getConnection()->send(op);

}

Atlas::Formatter* EntityExporter::createMultiLineFormatter(std::iostream& s, Atlas::Bridge& b)
{
	return new Atlas::MultiLineListFormatter(s, b);
}

std::string EntityExporter::getCurrentTime()
{
	std::stringstream ss;
	ss << Time::currentTimeMillis();
	return ss.str();

}

void EntityExporter::fillWithServerData(Atlas::Message::MapType& serverMap)
{
	Eris::ServerInfo serverInfo;
	mAccount.getConnection()->getServerInfo(serverInfo);
	serverMap["host"] = serverInfo.getHostname();
	serverMap["name"] = serverInfo.getServername();
	serverMap["ruleset"] = serverInfo.getRuleset();
	serverMap["version"] = serverInfo.getVersion();
}

void EntityExporter::operationResult(const Atlas::Objects::Operation::RootOperation& op)
{
	auto I = mCallbacks.find(op->getRefno());
	if (I != mCallbacks.end()) {
		auto callback = I->second;
		mCallbacks.erase(I);
		callback(op);
	}
}

}
