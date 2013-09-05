//
// Copyright (C) 2009 Alistair Riddoch
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

#include "MultiLineListFormatter.h"
#include "AtlasObjectDecoder.h"
#include "LoggingInstance.h"
#include "osdir.h"

#include <Eris/Account.h>
#include <Eris/Connection.h>
#include <Eris/Response.h>
#include <Eris/Avatar.h>

#include <Atlas/Objects/Anonymous.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Codecs/XML.h>

#include <fstream>

using Atlas::Objects::Root;
using Atlas::Objects::smart_dynamic_cast;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Entity::RootEntity;
using Atlas::Objects::Operation::Create;
using Atlas::Objects::Operation::Get;
using Atlas::Objects::Operation::Look;
using Atlas::Objects::Operation::Set;

namespace Ember
{
StackEntry::StackEntry(const Atlas::Objects::Entity::RootEntity & o, const std::list<std::string>::const_iterator & c) :
		obj(o), currentChildIterator(c)
{
}

StackEntry::StackEntry(const Atlas::Objects::Entity::RootEntity & o) :
		obj(o)
{
	currentChildIterator = obj->getContains().end();
}

bool EntityImporter::getEntity(const std::string & id, OpVector & res)
{
	std::map<std::string, Root>::const_iterator I = mPersistedEntities.find(id);
	if (I == mPersistedEntities.end()) {
		S_LOG_VERBOSE("Could not find entity with id " << id << "; this one was probably transient.");
		//This will often happen if the child entity was transient, and therefore wasn't exported (but is still references from the parent entity).
		return false;
	}
	const RootEntity& obj = smart_dynamic_cast<RootEntity>(I->second);
	if (!obj.isValid()) {
		S_LOG_FAILURE("Corrupt dump - non entity found " << id << ".");
		return false;
	}

	m_state = WALKING;
	m_treeStack.emplace_back(obj);

	Anonymous get_arg;
	get_arg->setId(id);
	get_arg->setObjtype("obj");

	Get get;
	get->setArgs1(get_arg);
	get->setFrom(mAccount.getId());
	get->setSerialno(Eris::getNewSerialno());
	res.push_back(get);
	S_LOG_VERBOSE("EntityImporter: Getting entity with id " << id);
	return true;
}

void EntityImporter::walk(OpVector & res)
{
	if (m_treeStack.empty()) {
		sendMinds();
	} else {
		StackEntry & current = m_treeStack.back();
		//Check if there are any children. If not, we should pop the stack and
		if (current.obj->getContains().empty()) {
			// Pop: Go back to WALKING parent
			assert(!m_treeStack.empty());
			m_treeStack.pop_back();
			while (!m_treeStack.empty()) {
				StackEntry & se = m_treeStack.back();
				//Try to get the next child entity (unless we've reached the end of the list of children).
				//Since some entities are references but not persisted we need to loop until we find one that we know of.
				for (; ++se.currentChildIterator, se.currentChildIterator != se.obj->getContains().end();) {
					if (getEntity(*se.currentChildIterator, res)) {
						//We've sent a request for an entity; we should break out and await a response from the server.
						return;
					}
				}

				//If we've reached the end of the contained entities we should pop the current stack entry.
				m_treeStack.pop_back();
			}
		} else {
			//Iterate until we find an entity that's persisted.
			//Note that there might be a reference to an entity in CONTAINS which can't be found
			//in the list of entities; this happens with transient entities.
			for (auto I = current.obj->getContains().begin(); I != current.obj->getContains().end(); ++I) {
				current.currentChildIterator = I;
				bool foundEntity = getEntity(*current.currentChildIterator, res);
				if (foundEntity) {
					return;
				}
			}
		}
		if (m_treeStack.empty()) {
			sendMinds();
		}
	}
}

void EntityImporter::sendMinds()
{
	if (!mResolvedMindMapping.empty()) {
		S_LOG_INFO("Sending minds.");
		for (auto mind : mResolvedMindMapping) {
			Atlas::Message::MapType message;
			mind.second->addToMessage(message);

			Atlas::Message::Element thoughtsElem = message["thoughts"];
			Atlas::Message::ListType thoughtArgs;

			if (thoughtsElem.isList()) {
				Atlas::Message::ListType thoughtList = thoughtsElem.asList();

				for (auto& thought : thoughtList) {
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
												const auto& entityIdLookupI = m_entityIdMap.find(thingId.asString());
												//Check if the owned entity has been created with a new id. If so, replace the data.
												if (entityIdLookupI != m_entityIdMap.end()) {
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
										const auto& entityIdLookupI = m_entityIdMap.find(thingId.asString());
										//Check if the owned entity has been created with a new id. If so, replace the data.
										if (entityIdLookupI != m_entityIdMap.end()) {
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
					thoughtArgs.push_back(thought);
				}
			}

			Atlas::Objects::Operation::RootOperation thinkOp;
			thinkOp->setArgsAsList(thoughtArgs);
			std::list<std::string> parents;
			parents.emplace_back("think");
			thinkOp->setParents(parents);
			thinkOp->setTo(mind.first);
			//By setting it TO an entity and FROM our avatar we'll make the server deliver it as
			//if it came from the entity itself (the server rewrites the FROM to be of the entity).
			thinkOp->setFrom(mAccount.getActiveCharacters().begin()->first);
			mStats.mindsProcessedCount++;
			S_LOG_INFO("Restoring mind of " << mind.first);
			mAccount.getConnection()->send(thinkOp);
			EventProgress.emit();
		}
		mResolvedMindMapping.clear();
	}
	S_LOG_INFO("Restore done.");
	S_LOG_INFO("Restored " << mStats.entitiesProcessedCount<< ", created: " << mStats.entitiesCreateCount << ", updated: " << mStats.entitiesUpdateCount << ", create errors: " << mStats.entitiesCreateErrorCount << " .");
	EventCompleted.emit();
}

void EntityImporter::create(const RootEntity & obj, OpVector & res)
{
	++mStats.entitiesProcessedCount;
	++mStats.entitiesCreateCount;
	EventProgress.emit();

	m_state = CREATING;

	assert(m_treeStack.size() > 1);
	std::deque<StackEntry>::reverse_iterator I = m_treeStack.rbegin();
	++I;
	assert(I != m_treeStack.rend());
	const std::string & loc = I->restored_id;

	RootEntity create_arg = obj.copy();

	create_arg->removeAttrFlag(Atlas::Objects::Entity::CONTAINS_FLAG);
	create_arg->removeAttrFlag(Atlas::Objects::Entity::VELOCITY_FLAG);
	create_arg->removeAttrFlag(Atlas::Objects::ID_FLAG);
	create_arg->setLoc(loc);

	Create create;
	create->setArgs1(create_arg);
	create->setFrom(mAccount.getActiveCharacters().begin()->second->getId());
	create->setSerialno(Eris::getNewSerialno());

	mCreateEntityMapping.insert(std::make_pair(create->getSerialno(), obj->getId()));

	res.push_back(create);
}

void EntityImporter::errorArrived(const Operation & op, OpVector & res)
{
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

	switch (m_state) {
	case WALKING:
	{
		//An error here just means that the entity we asked for didn't exist on the server, and we need
		//to create it. This is an expected result.
		assert(!m_treeStack.empty());
		StackEntry & current = m_treeStack.back();
		const RootEntity& obj = current.obj;

		assert(obj.isValid());

		create(obj, res);
	}
		break;
	case CREATING:
	{
		//An error here means that something went wrong when trying to create an entity. This is wrong.
		//It probably means that there's something wrong with the data we're sending. Either the
		//persisted data is corrupt, or there have been changes on the server (for example entity types
		//renamed or removed).
		std::string entityType = "unknown";

		auto I = mCreateEntityMapping.find(op->getRefno());
		if (I != mCreateEntityMapping.end()) {
			auto J = mPersistedEntities.find(I->second);
			if (J != mPersistedEntities.end()) {
				auto& entity = J->second;
				if (!entity->getParents().empty()) {
					entityType = entity->getParents().front();
				}
			}
		}
		S_LOG_FAILURE("Could not create entity of type '" << entityType << "', continuing with next. Server message: " << errorMessage);
		mStats.entitiesCreateErrorCount++;
		walk(res);
	}
		break;
	default:
		S_LOG_FAILURE("Unexpected state in state machine. Server message: " << errorMessage);
		break;
	};
}

void EntityImporter::infoArrived(const Operation & op, OpVector & res)
{
	if (op->isDefaultRefno()) {
		return;
	}
	if (op->isDefaultArgs() || op->getArgs().empty()) {
		S_LOG_FAILURE("Info with no arg.");
		return;
	}

	if (m_state == CREATING) {
		if (!op.isValid()) {
			return;
		}
		const Root& created = op->getArgs().front();
		m_newIds.insert(created->getId());
		StackEntry & current = m_treeStack.back();
		current.restored_id = created->getId();
		S_LOG_VERBOSE("Created: " << created->getParents().front() << "(" << created->getId() << ")");

		auto I = mCreateEntityMapping.find(op->getRefno());
		if (I != mCreateEntityMapping.end()) {
			//Check if there's a mind that we should send further on
			auto mindI = mPersistedMinds.find(I->second);
			if (mindI != mPersistedMinds.end()) {
				mResolvedMindMapping.emplace_back(created->getId(), mindI->second);
			}
			m_entityIdMap.insert(std::make_pair(I->second, created->getId()));
			mCreateEntityMapping.erase(op->getRefno());
		} else {
			S_LOG_WARNING("Got info about create for an entity which we didn't seem to have sent.");
		}

		walk(res);
	} else if (m_state == WALKING) {
		const Root & arg = op->getArgs().front();
		const RootEntity& ent = smart_dynamic_cast<RootEntity>(arg);
		if (!ent.isValid()) {
			S_LOG_FAILURE("Info response is not entity.");
			return;
		}
		if (arg->isDefaultId()) {
			S_LOG_FAILURE("Corrupted info response: no id.");
		}
		const std::string & id = arg->getId();

		StackEntry & current = m_treeStack.back();
		const RootEntity& obj = current.obj;

		assert(id == obj->getId());

		if (m_newIds.find(id) != m_newIds.end() || (m_treeStack.size() != 1 && ent->isDefaultLoc()) || ent->getParents().front() != obj->getParents().front()) {
			create(obj, res);
		} else {

			Root update = obj.copy();

			current.restored_id = id;

			S_LOG_VERBOSE("Updating: " << obj->getId() << " ," << obj->getParents().front());

			update->removeAttrFlag(Atlas::Objects::Entity::CONTAINS_FLAG);
			update->removeAttrFlag(Atlas::Objects::STAMP_FLAG);

			Set set;
			set->setArgs1(update);
			set->setFrom(mAccount.getActiveCharacters().begin()->second->getId());
			set->setTo(id);
			set->setSerialno(Eris::getNewSerialno());

			res.push_back(set);

			//Check if there's a mind, and if so put it in our map of resolved entity-to-mind mappings (since we know the entity id)
			auto mindI = mPersistedMinds.find(obj->getId());
			if (mindI != mPersistedMinds.end()) {
				mResolvedMindMapping.emplace_back(obj->getId(), mindI->second);
			}

			++mStats.entitiesProcessedCount;
			++mStats.entitiesUpdateCount;
			EventProgress.emit();

			m_state = UPDATING;
		}
	}
}

void EntityImporter::sightArrived(const Operation & op, OpVector & res)
{
	if (op->isDefaultArgs() || op->getArgs().empty()) {
		S_LOG_FAILURE("No arg");
		return;
	}
	const Root & arg = op->getArgs().front();
	switch (m_state) {
	case INIT:
		if (op->isDefaultRefno()) {
			break;
		}
		if (arg->isDefaultId()) {
			S_LOG_WARNING("Corrupted top level entity: no id");
		}

		getEntity(arg->getId(), res);

		// Expecting sight of world root
		break;
	case UPDATING:
	{
		const Operation& sub_op = smart_dynamic_cast<Operation>(arg);
		if (!sub_op.isValid()) {
			break;
		}
		if (sub_op->getClassNo() != Atlas::Objects::Operation::SET_NO || sub_op->getArgs().empty() || sub_op->isDefaultSerialno()) {
			S_LOG_FAILURE("This is not our entity update response.");
			break;
		}
		walk(res);
	}
		break;
	default:
		S_LOG_WARNING("Unexpected state in state machine.");
		break;
	};
}

EntityImporter::EntityImporter(Eris::Account& account) :
		mAccount(account), mStats( { }), m_state(INIT)
{
}

EntityImporter::~EntityImporter()
{
}

void EntityImporter::start(const std::string& filename)
{

	std::fstream fileStream(filename, std::ios::in);
	AtlasObjectDecoder atlasLoader;

	Atlas::Codecs::XML codec(fileStream, atlasLoader);
	codec.poll(true);

	auto rootObj = atlasLoader.getLastObject();

	if (!rootObj.isValid()) {
		EventCompleted.emit();
		return;
	}
	Atlas::Message::Element metaElem;
	Atlas::Message::Element entitiesElem;
	Atlas::Message::Element mindsElem;
	rootObj->copyAttr("meta", metaElem);
	rootObj->copyAttr("entities", entitiesElem);
	rootObj->copyAttr("minds", mindsElem);

	if (!entitiesElem.isList()) {
		S_LOG_WARNING("Entities element is not list.");
		EventCompleted.emit();
		return;
	}
	if (!mindsElem.isList()) {
		S_LOG_WARNING("Minds element is not list.");
		EventCompleted.emit();
		return;
	}
	auto factories = Atlas::Objects::Factories::instance();
	for (auto& entityMessage : entitiesElem.asList()) {
		if (entityMessage.isMap()) {
			auto object = factories->createObject(entityMessage.asMap());
			if (object.isValid()) {
				if (!object->isDefaultId()) {
					mPersistedEntities.insert(std::make_pair(object->getId(), object));
				}
			}
		}
	}
	for (auto& mindMessage : mindsElem.asList()) {
		if (mindMessage.isMap()) {
			auto object = factories->createObject(mindMessage.asMap());
			if (object.isValid()) {
				if (!object->isDefaultId()) {
					mPersistedMinds.insert(std::make_pair(object->getId(), object));
				}
			}
		}
	}

	S_LOG_INFO("Starting loading of world. Number of entities: " << mPersistedEntities.size() << " Number of minds: " << mPersistedMinds.size());
	mStats.entitiesCount = static_cast<unsigned int>(mPersistedEntities.size());
	mStats.mindsCount = static_cast<unsigned int>(mPersistedMinds.size());

	EventProgress.emit();

	Look l;

	l->setFrom(mAccount.getActiveCharacters().begin()->second->getId());
	l->setSerialno(Eris::getNewSerialno());

	sendOperation(l);

}

void EntityImporter::sendOperation(const Operation& op)
{
	if (!op->isDefaultSerialno()) {
		mAccount.getConnection()->getResponder()->await(op->getSerialno(), this, &EntityImporter::operation);
	}
	mAccount.getConnection()->send(op);
}

void EntityImporter::cancel()
{
	m_state = CANCEL;
}

const EntityImporter::Stats& EntityImporter::getStats() const
{
	return mStats;
}

void EntityImporter::operation(const Operation & op)
{
	if (m_state == CANCEL) {
		m_state = CANCELLED;
		return;
	}
	if (m_state == CANCELLED) {
		return;
	}
	OpVector res;
	if (op->getClassNo() == Atlas::Objects::Operation::INFO_NO) {
		infoArrived(op, res);
	} else if (op->getClassNo() == Atlas::Objects::Operation::ERROR_NO) {
		errorArrived(op, res);
	} else if (op->getClassNo() == Atlas::Objects::Operation::SIGHT_NO) {
		sightArrived(op, res);
	}

	for (auto& op : res) {
		sendOperation(op);
	}

}

std::vector<EntityImporter::ShortInfo> EntityImporter::getInfoFromDirectory(const std::string& directoryPath)
{
	std::vector<ShortInfo> infos;
	oslink::directory osdir(directoryPath);
	while (osdir) {
		const std::string filename = osdir.next();
		if (filename != "." && filename != "..") {
			try {
				ShortInfo info;

				std::fstream fileStream(directoryPath + "/" + filename, std::ios::in);
				AtlasObjectDecoder atlasLoader;

				Atlas::Codecs::XML codec(fileStream, atlasLoader);
				codec.poll(true);

				auto rootObj = atlasLoader.getLastObject();

				if (!rootObj.isValid()) {
					continue;
				}
				Atlas::Message::Element metaElem;
				if (rootObj->copyAttr("meta", metaElem) == 0 && metaElem.isMap()) {
					auto meta = metaElem.asMap();
					info.filename = directoryPath + "/" + filename;
					if (meta["name"].isString() && meta["name"] != "") {
						info.name = meta["name"].asString();
					} else {
						info.name = filename;
					}
					if (meta["description"].isString()) {
						info.description = meta["description"].asString();
					}

					Atlas::Message::Element entitiesElem;
					if (rootObj->copyAttr("entities", entitiesElem) == 0 && entitiesElem.isList()) {
						info.entityCount = entitiesElem.asList().size();
					}
					infos.push_back(info);
				}

			} catch (const std::exception& ex) {
				S_LOG_FAILURE("Error when trying to read import info from '" << filename << "'." << ex);
			}
		}

	}

	//Sort by name
	std::sort(infos.begin(), infos.end(), [](const ShortInfo& a, const ShortInfo& b) {
		return a.name < b.name;
	});
	return infos;
}
}
