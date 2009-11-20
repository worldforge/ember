/*
 Copyright (C) 2009 Erik Hjortsberg

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

#include "RawTypeInfoRepository.h"
#include "services/server/ServerService.h"
#include "framework/LoggingInstance.h"
#include <Atlas/Objects/Operation.h>
#include <Atlas/Objects/RootOperation.h>

namespace EmberOgre
{
namespace Authoring
{

RawTypeInfoRepository::RawTypeInfoRepository(Ember::ServerService& serverService)
{
	setupConnections(serverService);
}

RawTypeInfoRepository::~RawTypeInfoRepository()
{
}

Atlas::Objects::Root RawTypeInfoRepository::getRawTypeData(const std::string& typeName) const
{
	RawTypeDataStore::const_iterator I = mRawTypeData.find(typeName);
	if (I != mRawTypeData.end()) {
		return I->second;
	}
	return Atlas::Objects::Root();
}

void RawTypeInfoRepository::setupConnections(Ember::ServerService& serverService)
{
	serverService.EventReceivedObject.connect(sigc::mem_fun(*this, &RawTypeInfoRepository::server_ReceivedObject));
}

void RawTypeInfoRepository::server_ReceivedObject(const Atlas::Objects::Root& op)
{
	if (op->instanceOf(Atlas::Objects::Operation::INFO_NO)) {
		const Atlas::Objects::Operation::RootOperation& rootOp = Atlas::Objects::smart_static_cast<Atlas::Objects::Operation::RootOperation>(op);
		const std::vector<Atlas::Objects::Root>& args(rootOp->getArgs());
		std::string objType = args.front()->getObjtype();

		if ((objType == "meta") || (objType == "class") || (objType == "op_definition")) {
			parseAndSaveTypeData(args.front());
		}
	}
}

void RawTypeInfoRepository::parseAndSaveTypeData(const Atlas::Objects::Root& rawType)
{
	mRawTypeData[rawType->getId()] = rawType;
}

}
}
