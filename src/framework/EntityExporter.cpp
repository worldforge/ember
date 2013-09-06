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
#include "TinyXmlCodec.h"
#include "MultiLineListFormatter.h"

#include <Eris/Account.h>
#include <Eris/Connection.h>
#include <Eris/Response.h>
#include <Eris/ServerInfo.h>
#include <Eris/TypeService.h>
#include <Eris/TypeInfo.h>
#include <Eris/Avatar.h>

#include <Atlas/Codecs/XML.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Message/MEncoder.h>
#include <Atlas/Objects/Anonymous.h>
#include <Atlas/Objects/Encoder.h>
#include <Atlas/Objects/Operation.h>

using Atlas::Objects::Root;
using Atlas::Objects::smart_dynamic_cast;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Entity::RootEntity;
using Atlas::Objects::Operation::Talk;
using Atlas::Objects::Operation::Get;

long integerId(const std::string & id)
{
	long intId = strtol(id.c_str(), 0, 10);
	if (intId == 0 && id != "0") {
		intId = -1L;
	}

	return intId;
}
namespace Ember
{

bool idSorter(const std::string& lhs, const std::string& rhs)
{
	return integerId(lhs) < integerId(rhs);
}

EntityExporter::EntityExporter(Eris::Account& account) :
		mAccount(account), mCount(0), mComplete(false), mCancelled(false), mOutstandingGetRequestCounter(0), mExportTransient(false), mPreserveIds(false)
{
}

EntityExporter::~EntityExporter()
{
}

void EntityExporter::setDescription(const std::string& description)
{
	mDescription = description;
}

void EntityExporter::setName(const std::string& name)
{
	mName = name;
}

void EntityExporter::setExportTransient(bool exportTransient)
{
	mExportTransient = exportTransient;
}

void EntityExporter::setPreserveIds(bool preserveIds)
{
	mPreserveIds = preserveIds;
}

bool EntityExporter::getPreserveIds() const
{
	return mPreserveIds;
}

void EntityExporter::cancel()
{
	mCancelled = true;
}

void EntityExporter::dumpEntity(const RootEntity & ent)
{
	Atlas::Message::MapType entityMap;
	ent->addToMessage(entityMap);
	mEntities.push_back(entityMap);
}

void EntityExporter::dumpMind(const std::string& entityId, const Operation & op)
{
	auto thoughts = op->getArgsAsList();
	if (!thoughts.empty()) {
		Atlas::Message::MapType entityMap;
		entityMap["id"] = entityId;
		entityMap["thoughts"] = thoughts;
		mMinds.push_back(entityMap);
	}
}

void EntityExporter::thoughtOpArrived(const Operation & op)
{

	S_LOG_VERBOSE("Got commune result when dumping.");

	//What we receive here has been relayed from the mind of the entity. That means that this op
	//is potentially unsafe, as it could be of any type (Set, Logout etc.), all depending on what the
	//mind client decided to send (i.e. someone might want to try to hack). We should therefore treat it
	//very carefully.

	if (op->getClassNo() == Atlas::Objects::Operation::ROOT_OPERATION_NO) {
		//An empty root operation signals a timeout; we never got any answer from the entity.
		return;
	}

	//Since we'll just be iterating over the args we only need to do an extra check that what we got is a
	//"think" operation.
	if (op->getParents().empty()) {
		S_LOG_WARNING("Got think operation without any parent set.");
		return;
	}
	if (op->getParents().front() != "think") {
		S_LOG_WARNING("Got think operation with wrong type set.");
		return;
	}

	std::map<int, std::string>::const_iterator I = mThoughtsOutstanding.find(op->getRefno());
	if (I == mThoughtsOutstanding.end()) {
		S_LOG_WARNING("Got unrecognized thought info.");
		return;
	}
	const std::string entityId = I->second;

	dumpMind(entityId, op);

}

void EntityExporter::pollQueue()
{
	if (mQueue.empty() && mOutstandingGetRequestCounter == 0) {
		complete();
		return;
	}

	//Make sure that no more than 5 outstanding get requests are currently sent to the server.
	//The main reason for us not wanting more is that we then run the risk of overflowing the server connection (which will then be dropped).
	while (mOutstandingGetRequestCounter < 5 && !mQueue.empty()) {
		Get get;

		Anonymous get_arg;
		get_arg->setObjtype("obj");
		get_arg->setId(mQueue.front());

		get->setArgs1(get_arg);
		get->setFrom(mAccount.getId());
		get->setSerialno(Eris::getNewSerialno());

		mAccount.getConnection()->getResponder()->await(get->getSerialno(), this, &EntityExporter::operationGetResult);
		mAccount.getConnection()->send(get);
		S_LOG_VERBOSE("Requesting info about entity with id " << get_arg->getId());

		mOutstandingGetRequestCounter++;

		mQueue.pop_front();
	}
}

void EntityExporter::infoArrived(const Operation & op)
{
	if (op->isDefaultRefno()) {
		S_LOG_WARNING("Got op not belonging to us when dumping.");
		return;
	}
	const std::vector<Root> & args = op->getArgs();
	if (args.empty()) {
		return;
	}
	RootEntity ent = smart_dynamic_cast<RootEntity>(args.front());
	if (!ent.isValid()) {
		S_LOG_WARNING("Malformed OURS when dumping.");
		return;
	}
	S_LOG_VERBOSE("Got info when dumping about entity " << ent->getId() << ". Outstanding requests: " << mOutstandingGetRequestCounter);
	++mCount;
	EventProgress.emit(mCount);
	//If the entity is transient and we've been told not to export transient ones, we should skip this one (and all of its children).
	if (!ent->hasAttr("transient") || mExportTransient) {
		//Make a copy so that we can sort the contains list and update it in the
		//entity
		RootEntity entityCopy(ent->copy());
		std::list<std::string> contains = ent->getContains();
		//Sort the contains list so it's deterministic
		contains.sort(idSorter);
		entityCopy->setContains(contains);

		std::string persistedId = entityCopy->getId();

		if (!mPreserveIds && persistedId != "0") {
			std::stringstream ss;
			ss << mEntities.size();
			persistedId = ss.str();
			entityCopy->setId(persistedId);
		}
		mIdMapping.insert(std::make_pair(ent->getId(), persistedId));

		dumpEntity(entityCopy);
		std::list<std::string>::const_iterator I = contains.begin();
		std::list<std::string>::const_iterator Iend = contains.end();
		for (; I != Iend; ++I) {
			mQueue.push_back(*I);
		}

		Eris::TypeInfo* characterTypeInfo = mAccount.getConnection()->getTypeService()->getTypeByName("character");
		Eris::TypeInfo* entityType = mAccount.getConnection()->getTypeService()->getTypeByName(ent->getParentsAsList().begin()->asString());
		if (entityType->isA(characterTypeInfo)) {

			Atlas::Objects::Operation::Generic commune;
			std::list<std::string> parents;
			parents.emplace_back("commune");
			commune->setParents(parents);
			commune->setTo(ent->getId());

			//By setting it TO an entity and FROM our avatar we'll make the server deliver it as
			//if it came from the entity itself (the server rewrites the FROM to be of the entity).
			commune->setFrom(mAccount.getActiveCharacters().begin()->second->getId());
			//By setting a serial number we tell the server to "relay" the operation. This means that any
			//response operation from the target entity will be sent back to us.
			commune->setSerialno(Eris::getNewSerialno());

			mAccount.getConnection()->getResponder()->await(commune->getSerialno(), this, &EntityExporter::operationGetThoughtResult);
			mAccount.getConnection()->send(commune);
			mThoughtsOutstanding.insert(std::make_pair(commune->getSerialno(), persistedId));
			mOutstandingGetRequestCounter++;
			S_LOG_VERBOSE("Sending request for thoughts for entity with id " << ent->getId() << ".");

		}
	}
	pollQueue();
}

void EntityExporter::adjustReferencedEntities()
{
	S_LOG_VERBOSE("Adjusting referenced entity ids.");
	if (!mPreserveIds) {
		for (auto& mind : mMinds) {
			if (mind.isMap()) {
				if (mind.asMap().count("thoughts") > 0) {
					auto& thoughts = mind.asMap().find("thoughts")->second;
					if (thoughts.isList()) {
						for (auto& thought : thoughts.asList()) {
							//If the thought is a list of things the entity owns, we should adjust it with the new entity ids.
							if (thought.isMap()) {
								if (thought.asMap().count("things") > 0) {
									auto& thingsElement = thought.asMap().find("things")->second;
									if (thingsElement.isMap()) {
										for (auto& thingI : thingsElement.asMap()) {
											if (thingI.second.isList()) {
												Atlas::Message::ListType newList;
												for (auto& thingId : thingI.second.asList()) {
													if (thingId.isString()) {
														auto entityIdLookupI = mIdMapping.find(thingId.asString());
														//Check if the owned entity has been created with a new id. If so, replace the data.
														if (entityIdLookupI != mIdMapping.end()) {
															newList.push_back(entityIdLookupI->second);
														} else {
															newList.push_back(thingId);
														}
													} else {
														newList.push_back(thingId);
													}
												}
												thingI.second = newList;
											}
										}
									}
								} else if (thought.asMap().count("pending_things") > 0) {
									//things that the entity owns, but haven't yet discovered are expressed as a list of entity ids
									auto& pendingThingsElement = thought.asMap().find("pending_things")->second;
									if (pendingThingsElement.isList()) {
										Atlas::Message::ListType newList;
										for (auto& thingId : pendingThingsElement.asList()) {
											if (thingId.isString()) {
												auto entityIdLookupI = mIdMapping.find(thingId.asString());
												//Check if the owned entity has been created with a new id. If so, replace the data.
												if (entityIdLookupI != mIdMapping.end()) {
													newList.push_back(entityIdLookupI->second);
												} else {
													newList.push_back(thingId);
												}
											} else {
												newList.push_back(thingId);
											}
										}
										pendingThingsElement = newList;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	for (auto& entity : mEntities) {
		if (entity.isMap()) {
			if (entity.asMap().count("contains")) {
				auto& containsElem = entity.asMap().find("contains")->second;
				if (containsElem.isList()) {
					auto& contains = containsElem.asList();
					Atlas::Message::ListType newContains;
					newContains.reserve(contains.size());
					for (auto& entityElem : contains) {
						//we can assume that it's string
						auto I = mIdMapping.find(entityElem.asString());
						if (I != mIdMapping.end()) {
							newContains.push_back(I->second);
						}
					}
					contains = newContains;
				}
			}
		}
	}
}

void EntityExporter::complete()
{

	adjustReferencedEntities();

	Anonymous root;
	Atlas::Message::MapType meta;

	meta["name"] = mName;
	meta["description"] = mDescription;
	std::stringstream ss;
	ss << Time::currentTimeMillis();
	meta["timestamp"] = ss.str();
	meta["transients"] = mExportTransient;

	Atlas::Message::MapType server;
	Eris::ServerInfo serverInfo;
	mAccount.getConnection()->getServerInfo(serverInfo);
	server["host"] = serverInfo.getHostname();
	server["name"] = serverInfo.getServername();
	server["ruleset"] = serverInfo.getRuleset();
	meta["server"] = server;

	root->setAttr("meta", meta);

	root->setAttr("entities", mEntities);
	root->setAttr("minds", mMinds);

	std::fstream filestream(mFilename, std::ios::out);
	Atlas::Message::QueuedDecoder decoder;
	Atlas::Codecs::XML codec(filestream, decoder);
	MultiLineListFormatter formatter(filestream, codec);

	Atlas::Objects::ObjectsEncoder encoder(formatter);

	encoder.streamBegin();
	encoder.streamObjectsMessage(root);
	encoder.streamEnd();

	filestream.close();

	mEntities.clear();
	mMinds.clear();

	mComplete = true;
	EventCompleted.emit();
	S_LOG_INFO("Completed dumping " << mCount << " entities.");
}

void EntityExporter::start(const std::string& filename, const std::string& entityId)
{
	S_LOG_INFO("Starting world dump to file '" << filename << "'.");
	mFilename = filename;

	// Send a get for the requested object
	mOutstandingGetRequestCounter++;
	Get get;

	Anonymous get_arg;
	get_arg->setObjtype("obj");
	get_arg->setId(entityId);
	get->setArgs1(get_arg);

	get->setFrom(mAccount.getId());
	get->setSerialno(Eris::getNewSerialno());
	mAccount.getConnection()->getResponder()->await(get->getSerialno(), this, &EntityExporter::operationGetResult);
	mAccount.getConnection()->send(get);

}

void EntityExporter::operationGetResult(const Operation & op)
{
	mOutstandingGetRequestCounter--;
	if (!mCancelled) {
		if (op->getClassNo() == Atlas::Objects::Operation::INFO_NO) {
			infoArrived(op);
		} else {
			std::string errorMessage;
			if (op->getClassNo() == Atlas::Objects::Operation::ERROR_NO) {
				std::string errorMessage;
				if (!op->getArgs().empty()) {
					auto arg = op->getArgs().front();
					if (arg->hasAttr("message")) {
						const Atlas::Message::Element messageElem = arg->getAttr("message");
						if (messageElem.isString()) {
							errorMessage = messageElem.asString();
						}
					}
				}
			}
			S_LOG_WARNING("Got unexpected response on a GET request with operation of type " << op->getParents().front());
			S_LOG_WARNING("Error message: " << errorMessage);
		}
	}
}

void EntityExporter::operationGetThoughtResult(const Operation & op)
{
	mOutstandingGetRequestCounter--;
	if (!mCancelled) {
		thoughtOpArrived(op);
		pollQueue();
	}
}
}
