//
// Copyright (C) 2009 Alistair Riddoch
// Copyright (C) 2013 Erik Ogenvik
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

#include "EntityImporter.h"

#include "AtlasObjectDecoder.h"
#include "LoggingInstance.h"
#include <Atlas/Codecs/XML.h>

#include <Eris/Account.h>
#include <Eris/Connection.h>
#include <Eris/Response.h>
#include <Eris/Avatar.h>

#include <fstream>
#include <boost/filesystem/operations.hpp>

using Atlas::Objects::Root;
using Atlas::Objects::smart_dynamic_cast;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Entity::RootEntity;
using Atlas::Objects::Operation::Create;
using Atlas::Objects::Operation::Get;
using Atlas::Objects::Operation::Look;
using Atlas::Objects::Operation::Set;
using Atlas::Message::Element;

namespace Ember {

EntityImporter::EntityImporter(Eris::Account& account) :
		EntityImporterBase(account.getId(), account.getActiveCharacters().begin()->second->getId()), mAccount(account) {
}

EntityImporter::~EntityImporter() = default;

long int EntityImporter::newSerialNumber() {
	return Eris::getNewSerialno();
}

void EntityImporter::send(const Atlas::Objects::Operation::RootOperation& op) {
	mAccount.getConnection().send(op);
}

void EntityImporter::sendAndAwaitResponse(const Atlas::Objects::Operation::RootOperation& op, CallbackFunction& callback) {
	mCallbacks.insert(std::make_pair(op->getSerialno(), callback));
	mAccount.getConnection().getResponder().await(op->getSerialno(), this, &EntityImporter::operationResult);
	mAccount.getConnection().send(op);

}

void EntityImporter::operationResult(const Atlas::Objects::Operation::RootOperation& op) {
	auto I = mCallbacks.find(op->getRefno());
	if (I != mCallbacks.end()) {
		auto callback = I->second;
		//Don't erase callbacks since there can be multiple responses to the same serial no.
		callback(op);
	}
}

Atlas::Objects::Root EntityImporter::loadFromFile(const std::string& filename) {
	std::fstream fileStream(filename, std::ios::in);
	Atlas::Objects::Factories factories;
	AtlasObjectDecoder atlasLoader(factories);

	Atlas::Codecs::XML codec(fileStream, fileStream, atlasLoader);
	codec.poll();

	return atlasLoader.getLastObject();
}

std::vector<EntityImporter::ShortInfo> EntityImporter::getInfoFromDirectory(const boost::filesystem::path& directoryPath) {
	std::vector<ShortInfo> infos;
	for (auto& dir_entry : boost::filesystem::directory_iterator(directoryPath)) {
		auto& file_path = dir_entry.path();
		try {
			ShortInfo info;

			std::fstream fileStream(file_path.string(), std::ios::in);
			Atlas::Objects::Factories factories;
			AtlasObjectDecoder atlasLoader(factories);

			Atlas::Codecs::XML codec(fileStream, fileStream, atlasLoader);
			codec.poll();

			auto rootObj = atlasLoader.getLastObject();

			if (!rootObj.isValid()) {
				continue;
			}
			Atlas::Message::Element metaElem;
			if (rootObj->copyAttr("meta", metaElem) == 0 && metaElem.isMap()) {
				auto meta = metaElem.asMap();
				info.filename = file_path.filename().string();
				if (meta["name"].isString() && meta["name"] != "") {
					info.name = meta["name"].asString();
				} else {
					info.name = file_path.filename().string();
				}
				if (meta["description"].isString()) {
					info.description = meta["description"].asString();
				}

				Atlas::Message::Element entitiesElem;
				if (rootObj->copyAttr("entities", entitiesElem) == 0 && entitiesElem.isList()) {
					info.entityCount = entitiesElem.asList().size();
				}
				if (rootObj->copyAttr("rules", entitiesElem) == 0 && entitiesElem.isList()) {
					info.rulesCount = entitiesElem.asList().size();
				}
				if (rootObj->copyAttr("minds", entitiesElem) == 0 && entitiesElem.isList()) {
					info.mindsCount = entitiesElem.asList().size();
				}
				infos.push_back(info);
			}

		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when trying to read import info from '" << file_path.string() << "'." << ex);
		}


	}

//Sort by name
	std::sort(infos.begin(), infos.end(), [](const ShortInfo& a, const ShortInfo& b) {
		return a.name < b.name;
	});
	return infos;
}
}
