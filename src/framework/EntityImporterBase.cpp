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

#include "EntityImporterBase.h"

#include "LoggingInstance.h"

#include <Atlas/Objects/Anonymous.h>
#include <Atlas/Objects/Operation.h>

#include <fstream>
#include <utility>

using Atlas::Objects::Root;
using Atlas::Objects::smart_dynamic_cast;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Entity::RootEntity;
using Atlas::Objects::Operation::Create;
using Atlas::Objects::Operation::Get;
using Atlas::Objects::Operation::Look;
using Atlas::Objects::Operation::Set;
using Atlas::Message::Element;

StackEntry::StackEntry(Atlas::Objects::Entity::RootEntity  o, const std::vector<std::string>::const_iterator & c) :
		obj(std::move(o)), currentChildIterator(c)
{
}

StackEntry::StackEntry(Atlas::Objects::Entity::RootEntity  o) :
		obj(std::move(o))
{
	currentChildIterator = obj->getContains().end();
}

bool EntityImporterBase::getEntity(const std::string & id, OpVector & res)
{
	auto I = mPersistedEntities.find(id);
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

	m_state = ENTITY_WALKING;
	mTreeStack.emplace_back(obj);

	Anonymous get_arg;
	get_arg->setId(id);
	get_arg->setObjtype("obj");

	Get get;
	get->setArgs1(get_arg);
	get->setFrom(mAccountId);
	get->setSerialno(newSerialNumber());
	res.push_back(get);
	S_LOG_VERBOSE("EntityImporterBase: Getting entity with id " << id);
	return true;
}

void EntityImporterBase::extractChildren(const Root& op, std::list<std::string>& children)
{
	Element childElem;
	if (op->copyAttr("children", childElem) == 0) {
		if (childElem.isList()) {
			for (auto child : childElem.asList()) {
				if (child.isString()) {
					children.push_back(child.asString());
				}
			}
		}
	}
}

bool EntityImporterBase::getRule(const std::string & id, OpVector & res)
{
	auto I = mPersistedRules.find(id);
	if (I == mPersistedRules.end()) {
		S_LOG_WARNING("Could not find rule with id " << id << ".");
		return false;
	}

	auto definition = I->second;

	m_state = RULE_WALKING;

	std::list<std::string> children;
	extractChildren(definition, children);

	RuleStackEntry entry = { id, definition, children, {} };
	mRuleStack.push_back(entry);

	Get get;
	Anonymous arg;
	arg->setId(id);
	get->setArgs1(arg);
//  get->setObjtype("op");
	get->setSerialno(newSerialNumber());

	res.push_back(get);

	Anonymous get_arg;
	get_arg->setId(id);
	get_arg->setObjtype("obj");

	return true;
}

void EntityImporterBase::walkRules(OpVector & res)
{
	if (mRuleStack.empty()) {
		startEntityWalking();
	} else {
		auto & current = mRuleStack.back();
		auto definition = current.definition;
		//Check if there are any children. If not, we should pop the stack and

		if (current.children.empty()) {
			// Pop: Go back to WALKING parent
			assert(!mRuleStack.empty());
			mRuleStack.pop_back();
			while (!mRuleStack.empty()) {
				auto & se = mRuleStack.back();
				//Try to get the next child rule (unless we've reached the end of the list of children).
				for (; ++se.currentChildIterator, se.currentChildIterator != se.children.end();) {
					if (getRule(*se.currentChildIterator, res)) {
						//We've sent a request for a rule; we should break out and await a response from the server.
						return;
					}
				}

				//If we've reached the end of the contained rules we should pop the current stack entry.
				mRuleStack.pop_back();
			}
		} else {
			for (auto I = current.children.begin(); I != current.children.end(); ++I) {
				current.currentChildIterator = I;
				bool foundRule = getRule(*current.currentChildIterator, res);
				if (foundRule) {
					return;
				}
			}
		}
		if (mRuleStack.empty()) {
			startEntityWalking();
		}
	}
}

void EntityImporterBase::walkEntities(OpVector & res)
{
	if (mTreeStack.empty()) {
		sendResolvedEntityReferences();
	} else {
		StackEntry & current = mTreeStack.back();
		//Check if there are any children. If not, we should pop the stack and
		if (current.obj->getContains().empty()) {
			// Pop: Go back to WALKING parent
			assert(!mTreeStack.empty());
			mTreeStack.pop_back();
			while (!mTreeStack.empty()) {
				StackEntry & se = mTreeStack.back();
				//Try to get the next child entity (unless we've reached the end of the list of children).
				//Since some entities are references but not persisted we need to loop until we find one that we know of.
				for (; ++se.currentChildIterator, se.currentChildIterator != se.obj->getContains().end();) {
					if (getEntity(*se.currentChildIterator, res)) {
						//We've sent a request for an entity; we should break out and await a response from the server.
						return;
					}
				}

				//If we've reached the end of the contained entities we should pop the current stack entry.
				mTreeStack.pop_back();
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
		if (mTreeStack.empty()) {
			sendResolvedEntityReferences();
		}
	}
}

void EntityImporterBase::sendMinds()
{
	if (!mResolvedMindMapping.empty()) {
		Atlas::Objects::Factories factories;
		S_LOG_INFO("Sending minds.");
		for (const auto& mind : mResolvedMindMapping) {
			Atlas::Message::MapType message;
			mind.second->addToMessage(message);

			Atlas::Message::Element thoughtsElem = message["thoughts"];
			Atlas::Message::ListType thoughtArgs;

			if (thoughtsElem.isList()) {
				Atlas::Message::ListType thoughtList = thoughtsElem.List();

				for (auto& thought : thoughtList) {
					//If the thought is a list of things the entity owns, we should adjust it with the new entity ids.
					if (thought.isMap()) {
						auto& thoughtMap = thought.Map();
						if (thoughtMap.count("things") > 0) {
							auto& thingsElement = thoughtMap.find("things")->second;
							if (thingsElement.isMap()) {
								for (auto& thingI : thingsElement.asMap()) {
									if (thingI.second.isList()) {
										Atlas::Message::ListType newList;
										for (auto& thingId : thingI.second.asList()) {
											if (thingId.isString()) {
												const auto& entityIdLookupI = mEntityIdMap.find(thingId.asString());
												//Check if the owned entity has been created with a new id. If so, replace the data.
												if (entityIdLookupI != mEntityIdMap.end()) {
													newList.emplace_back(entityIdLookupI->second);
												} else {
													newList.emplace_back(thingId);
												}
											} else {
												newList.emplace_back(thingId);
											}
										}
										thingI.second = newList;
									}
								}
							}
						}

						if (thoughtMap.count("pending_things") > 0) {
							//things that the entity owns, but haven't yet discovered are expressed as a list of entity ids
							auto& pendingThingsElement = thoughtMap.find("pending_things")->second;
							if (pendingThingsElement.isList()) {
								Atlas::Message::ListType newList;
								for (auto& thingId : pendingThingsElement.asList()) {
									if (thingId.isString()) {
										const auto& entityIdLookupI = mEntityIdMap.find(thingId.asString());
										//Check if the owned entity has been created with a new id. If so, replace the data.
										if (entityIdLookupI != mEntityIdMap.end()) {
											newList.emplace_back(entityIdLookupI->second);
										} else {
											newList.emplace_back(thingId);
										}
									} else {
										newList.emplace_back(thingId);
									}
								}
								pendingThingsElement = newList;
							}
						}

						if (thoughtMap.count("object") > 0) {
							auto& objectElement = thoughtMap.find("object")->second;
							if (objectElement.isString()) {
								std::string& objectString = objectElement.String();
								//Other entities are referred to using the syntax "'$eid:...'".
								//For example, the entity with id 2 would be "'$eid:2'".
								auto pos = objectString.find("$eid:");
								if (pos != std::string::npos) {
									auto quotePos = objectString.find('\'', pos);
									if (quotePos != std::string::npos) {
										auto id = objectString.substr(pos + 5, quotePos - pos - 5);
										auto I = mEntityIdMap.find(id);
										if (I != mEntityIdMap.end()) {
											objectString.replace(pos + 5, quotePos - 7, I->second);
										}
									}
								}
							}
						}

					}
					thoughtArgs.push_back(thought);
				}
			}

			Atlas::Objects::Operation::RootOperation thinkOp;
			thinkOp->setParent("think");
			thinkOp->setTo(mind.first);
			//By setting it TO an entity and FROM our avatar we'll make the server deliver it as
			//if it came from the entity itself (the server rewrites the FROM to be of the entity).
			thinkOp->setFrom(mAvatarId);
			thinkOp->setSerialno(newSerialNumber());

			Atlas::Objects::Operation::Set setOp;
			setOp->setArgsAsList(thoughtArgs, &factories);
			thinkOp->setArgs1(setOp);

			mStats.mindsProcessedCount++;
			S_LOG_VERBOSE("Restoring mind of " << mind.first);
			mThoughtOpsInTransit++;

			sigc::slot<void(const Operation&)> slot = sigc::mem_fun(*this, &EntityImporterBase::operationThinkResult);
			sendAndAwaitResponse(thinkOp, slot);
			EventProgress.emit();
		}
		mResolvedMindMapping.clear();
	} else {
		complete();
	}
}

void EntityImporterBase::sendResolvedEntityReferences()
{
	if (!mEntitiesWithReferenceAttributes.empty()) {
		for (const auto& entryI : mEntitiesWithReferenceAttributes) {
			const auto& persistedEntityId = entryI.first;
			const auto& attributeNames = entryI.second;

			auto createdEntityI = mEntityIdMap.find(persistedEntityId);
			if (createdEntityI == mEntityIdMap.end()) {
				S_LOG_WARNING("Could not find final server side entity id for persisted entity " << persistedEntityId << " when doing entity ref resolving.");
				continue;
			}
			const auto& createdEntityId = createdEntityI->second;

			//This should not fail at this phase, so we're not doing any checks.
			auto& persistedEntity = mPersistedEntities.find(persistedEntityId)->second;

			RootEntity entity;

			for (const auto& attributeName : attributeNames) {
				Element element = persistedEntity->getAttr(attributeName);
				resolveEntityReferences(element);
				entity->setAttr(attributeName, element);
			}

			Set set;
			set->setFrom(mAvatarId);
			set->setSerialno(newSerialNumber());
			set->setTo(createdEntityId);
			set->setArgs1(entity);

			mSetOpsInTransit++;
			sigc::slot<void(const Operation&)> slot = sigc::mem_fun(*this, &EntityImporterBase::operationSetResult);
			sendAndAwaitResponse(set, slot);
		}
	} else {
		sendMinds();
	}
}

void EntityImporterBase::resolveEntityReferences(Atlas::Message::Element& element)
{
	if (element.isMap()) {
		auto entityRefI = element.asMap().find("$eid");
		if (entityRefI != element.asMap().end() && entityRefI->second.isString()) {
			auto I = mEntityIdMap.find(entityRefI->second.asString());
			if (I != mEntityIdMap.end()) {
				entityRefI->second = I->second;
			}
		}
		//If it's a map we need to process all child elements too
		for (auto& I : element.asMap()) {
			resolveEntityReferences(I.second);
		}
	} else if (element.isList()) {
		//If it's a list we need to process all child elements too
		for (auto& I : element.asList()) {
			resolveEntityReferences(I);
		}
	}
}

void EntityImporterBase::complete()
{
	S_LOG_INFO("Restore done.");
	S_LOG_INFO("Restored " << mStats.entitiesProcessedCount<< ", created: " << mStats.entitiesCreateCount << ", updated: " << mStats.entitiesUpdateCount << ", create errors: " << mStats.entitiesCreateErrorCount << " .");
	EventCompleted.emit();
}

void EntityImporterBase::createEntity(const RootEntity & obj, OpVector & res)
{
	++mStats.entitiesProcessedCount;
	++mStats.entitiesCreateCount;
	EventProgress.emit();

	m_state = ENTITY_CREATING;

	assert(mTreeStack.size() > 1);
	auto I = mTreeStack.rbegin();
	++I;
	assert(I != mTreeStack.rend());
	const std::string & loc = I->restored_id;

	RootEntity create_arg = obj.copy();

	create_arg->removeAttrFlag(Atlas::Objects::Entity::CONTAINS_FLAG);
	create_arg->removeAttrFlag(Atlas::Objects::Entity::VELOCITY_FLAG);
	create_arg->removeAttrFlag(Atlas::Objects::ID_FLAG);
	create_arg->setLoc(loc);

	//Remove any attribute which references another entity from the Create op.
	//This is because the attribute will at this time with certainty refer to the wrong or a non-existing entity.
	//The attribute will later on be set through a Set op in sendResolvedEntityReferences().
	auto referenceMapEntryI = mEntitiesWithReferenceAttributes.find(obj->getId());
	if (referenceMapEntryI != mEntitiesWithReferenceAttributes.end()) {
		for (const auto& attributeName : referenceMapEntryI->second) {
			create_arg->removeAttr(attributeName);
		}
	}

	Create create;
	create->setArgs1(create_arg);
	create->setFrom(mAvatarId);
	create->setSerialno(newSerialNumber());

	mCreateEntityMapping.insert(std::make_pair(create->getSerialno(), obj->getId()));

	res.push_back(create);
}

void EntityImporterBase::createRule(const Atlas::Objects::Root & obj, OpVector & res)
{
	m_state = RULE_CREATING;
	Atlas::Objects::Operation::Create createOp;
	createOp->setFrom(mAccountId);
	createOp->setArgs1(obj);

	createOp->setSerialno(newSerialNumber());
	S_LOG_INFO("Creating new rule '" << obj->getId() << "' on server.");
	res.push_back(createOp);
}

void EntityImporterBase::updateRule(const Root& existingDefinition, const Root& newDefinition, OpVector & res)
{

	m_state = RULE_UPDATING;
	Root updatedDefinition = newDefinition.copy();

	//If the existing definition had any children that aren't part of the import, we should preserve those
	std::list<std::string> existingChildren, newChildren;
	extractChildren(existingDefinition, existingChildren);
	extractChildren(newDefinition, newChildren);

	for (auto& child : newChildren) {
		existingChildren.remove(child);
	}

	for (auto& child : existingChildren) {
		if (mPersistedRules.find(child) == mPersistedRules.end()) {
			newChildren.push_back(child);
		}
	}

	if (!newChildren.empty() && !existingChildren.empty()) {
		Atlas::Message::ListType childrenElement;
		for (auto& child : newChildren) {
			childrenElement.emplace_back(child);
		}
		updatedDefinition->setAttr("children", childrenElement);
	}

	if (updatedDefinition->asMessage() != existingDefinition->asMessage()) {
		S_LOG_INFO("Updating server rule '"<< updatedDefinition->getId() << "'.");
		Atlas::Objects::Operation::Set setOp;
		setOp->setFrom(mAccountId);
		setOp->setArgs1(updatedDefinition);

		setOp->setSerialno(newSerialNumber());
		res.push_back(setOp);
	} else {
		mStats.rulesProcessedCount++;
		EventProgress.emit();
		S_LOG_VERBOSE("Not updating server rule '"<< updatedDefinition->getId() << "' as nothing is changed.");
		walkRules(res);
	}
}

void EntityImporterBase::errorArrived(const Operation & op, OpVector & res)
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
	case RULE_WALKING:
	{
		//An error here just means that the rule we asked for didn't exist on the server, and we need
		//to create it. This is an expected result.
		auto& current = mRuleStack.back();
		auto definition = current.definition;
		assert(definition.isValid());

		createRule(definition, res);

	}
		break;
	case RULE_CREATING:
	{
		mStats.rulesProcessedCount++;
		mStats.rulesCreateErrorCount++;
		//An error here means that something went wrong when trying to create a rule. This is wrong.
		//It probably means that there's something wrong with the data we're sending.
		auto& current = mRuleStack.back();

		std::string ruleId = current.definition->getId();
		S_LOG_FAILURE("Could not create rule with id '" << ruleId << "', continuing with next. Server message: " << errorMessage);
		EventProgress.emit();
		walkRules(res);
	}
		break;
	case RULE_UPDATING:
	{
		mStats.rulesProcessedCount++;
		//An error here means that something went wrong when trying to update a rule. This is wrong.
		//It probably means that there's something wrong with the data we're sending.
		auto& current = mRuleStack.back();

		std::string ruleId = current.definition->getId();
		S_LOG_FAILURE("Could not update rule with id '" << ruleId << "', continuing with next. Server message: " << errorMessage);
		mStats.rulesCreateErrorCount++;
		EventProgress.emit();
		walkRules(res);
	}
		break;
	case ENTITY_WALKING:
	{
		//An error here just means that the entity we asked for didn't exist on the server, and we need
		//to create it. This is an expected result.
		assert(!mTreeStack.empty());
		StackEntry & current = mTreeStack.back();
		const RootEntity& obj = current.obj;

		assert(obj.isValid());

		createEntity(obj, res);
	}
		break;
	case ENTITY_CREATING:
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
				entityType = entity->getParent();
			}
		}
		S_LOG_FAILURE("Could not create entity of type '" << entityType << "', continuing with next. Server message: " << errorMessage);
		mStats.entitiesCreateErrorCount++;
		EventProgress.emit();
		walkEntities(res);
	}
		break;
	default:
		S_LOG_FAILURE("Unexpected state in state machine. Server message: " << errorMessage);
		break;
	}
}

void EntityImporterBase::infoArrived(const Operation & op, OpVector & res)
{
	if (op->isDefaultRefno()) {
		return;
	}
	if (op->isDefaultArgs() || op->getArgs().empty()) {
		S_LOG_FAILURE("Info with no arg.");
		return;
	}
	const Root & arg = op->getArgs().front();

	if (m_state == RULE_WALKING) {
		auto& current = mRuleStack.back();
		if (arg->getId() != current.definition->getId()) {
			S_LOG_WARNING("Got info on rule " << arg->getId() << " when expecting " << current.definition->getId() << ".");
			return;
		}

		updateRule(arg, current.definition, res);

	} else if (m_state == RULE_CREATING) {
		mStats.rulesProcessedCount++;
		mStats.rulesCreateCount++;
		EventProgress.emit();
		walkRules(res);
	} else if (m_state == RULE_UPDATING) {
		mStats.rulesProcessedCount++;
		mStats.rulesUpdateCount++;
		EventProgress.emit();
		walkRules(res);
	} else if (m_state == ENTITY_CREATING) {
		if (!op.isValid()) {
			return;
		}
		mNewIds.insert(arg->getId());
		StackEntry & current = mTreeStack.back();
		current.restored_id = arg->getId();
		S_LOG_VERBOSE("Created: " << arg->getParent() << "(" << arg->getId() << ")");

		auto I = mCreateEntityMapping.find(op->getRefno());
		if (I != mCreateEntityMapping.end()) {
			//Check if there's a mind that we should send further on
			auto mindI = mPersistedMinds.find(I->second);
			if (mindI != mPersistedMinds.end()) {
				mResolvedMindMapping.emplace_back(arg->getId(), mindI->second);
			}
			mEntityIdMap.insert(std::make_pair(I->second, arg->getId()));
			mCreateEntityMapping.erase(op->getRefno());
		} else {
			S_LOG_WARNING("Got info about create for an entity which we didn't seem to have sent.");
		}

		walkEntities(res);
	} else if (m_state == ENTITY_WALKING) {
		const RootEntity& ent = smart_dynamic_cast<RootEntity>(arg);
		if (!ent.isValid()) {
			S_LOG_FAILURE("Info response is not entity.");
			return;
		}
		if (arg->isDefaultId()) {
			S_LOG_FAILURE("Corrupted info response: no id.");
		}
		const std::string & id = arg->getId();

		StackEntry & current = mTreeStack.back();
		const RootEntity& obj = current.obj;

		assert(id == obj->getId());

		if (mNewIds.find(id) != mNewIds.end() || (mTreeStack.size() != 1 && ent->isDefaultLoc()) || ent->getParent() != obj->getParent()) {
			createEntity(obj, res);
		} else {

			Root update = obj.copy();

			current.restored_id = id;

			S_LOG_VERBOSE("Updating: " << obj->getId() << " ," << obj->getParent());

			update->removeAttrFlag(Atlas::Objects::Entity::CONTAINS_FLAG);
			update->removeAttrFlag(Atlas::Objects::STAMP_FLAG);

			Set set;
			set->setArgs1(update);
			set->setFrom(mAvatarId);
			set->setTo(id);
			set->setSerialno(newSerialNumber());

			res.push_back(set);

			//Check if there's a mind, and if so put it in our map of resolved entity-to-mind mappings (since we know the entity id)
			auto mindI = mPersistedMinds.find(obj->getId());
			if (mindI != mPersistedMinds.end()) {
				mResolvedMindMapping.emplace_back(obj->getId(), mindI->second);
			}

			++mStats.entitiesProcessedCount;
			++mStats.entitiesUpdateCount;
			EventProgress.emit();

			m_state = ENTITY_UPDATING;
		}
	}
}

void EntityImporterBase::sightArrived(const Operation & op, OpVector & res)
{
	if (op->isDefaultArgs() || op->getArgs().empty()) {
		S_LOG_FAILURE("No arg");
		return;
	}
	const Root & arg = op->getArgs().front();
	switch (m_state) {
	case ENTITY_WALKSTART:
		if (op->isDefaultRefno()) {
			break;
		}
		if (arg->isDefaultId()) {
			S_LOG_WARNING("Corrupted top level entity: no id");
			cancel();
			return;
		} else {
			getEntity(arg->getId(), res);
		}

		// Expecting sight of world root
		break;
	case ENTITY_UPDATING:
	{
		const Operation& sub_op = smart_dynamic_cast<Operation>(arg);
		if (!sub_op.isValid()) {
			break;
		}
		if (sub_op->getClassNo() != Atlas::Objects::Operation::SET_NO || sub_op->getArgs().empty() || sub_op->isDefaultSerialno()) {
			S_LOG_FAILURE("This is not our entity update response.");
			break;
		}
		walkEntities(res);
	}
		break;
	default:
		S_LOG_WARNING("Unexpected state in state machine.");
		break;
	}
}

EntityImporterBase::EntityImporterBase(std::string accountId, std::string avatarId) :
		mAccountId(std::move(accountId)),
		mAvatarId(std::move(avatarId)),
		mStats( { }),
		m_state(INIT),
		mThoughtOpsInTransit(0),
		mSetOpsInTransit(0),
		mResumeWorld(false)
{
}

EntityImporterBase::~EntityImporterBase() = default;

void EntityImporterBase::start(const std::string& filename)
{
	Atlas::Objects::Factories factories;

	auto rootObj = loadFromFile(filename);

	if (!rootObj.isValid()) {
		EventCompleted.emit();
		return;
	}
	Atlas::Message::Element metaElem;
	Atlas::Message::Element entitiesElem;
	Atlas::Message::Element mindsElem;
	Atlas::Message::Element rulesElem;
	rootObj->copyAttr("meta", metaElem);
	rootObj->copyAttr("entities", entitiesElem);
	rootObj->copyAttr("minds", mindsElem);
	if (rootObj->copyAttr("rules", rulesElem) == 0) {
		if (!rulesElem.isList()) {
			S_LOG_WARNING("Rules element is not list.");
			EventCompleted.emit();
			return;
		} else {
			for (auto& ruleMessage : rulesElem.asList()) {
				if (ruleMessage.isMap()) {
					auto object = factories.createObject(ruleMessage.asMap());
					if (object.isValid()) {
						if (!object->isDefaultId()) {
							mPersistedRules.insert(std::make_pair(object->getId(), object));
						}
					}
				}
			}

		}
	}

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

	for (auto& entityMessage : entitiesElem.asList()) {
		if (entityMessage.isMap()) {
			auto& entityMap = entityMessage.asMap();
			auto object = factories.createObject(entityMap);
			if (object.isValid()) {
				if (!object->isDefaultId()) {
					registerEntityReferences(object->getId(), entityMap);
					mPersistedEntities.insert(std::make_pair(object->getId(), object));
				}
			}
		}
	}
	//If we should resume the world, check if the world has a "suspended" property,
	//and disable it if so.
	if (mResumeWorld) {
		auto I = mPersistedEntities.find("0");
		if (I != mPersistedEntities.end()) {
			if (I->second->hasAttr("suspended")) {
				I->second->setAttr("suspended", 0);
				S_LOG_INFO("Resuming suspended world.");
			}
		}
	}
	for (auto& mindMessage : mindsElem.asList()) {
		if (mindMessage.isMap()) {
			auto object = factories.createObject(mindMessage.asMap());
			if (object.isValid()) {
				if (!object->isDefaultId()) {
					mPersistedMinds.insert(std::make_pair(object->getId(), object));
				}
			}
		}
	}

	S_LOG_INFO("Starting loading of world. Number of entities: " << mPersistedEntities.size() << " Number of minds: " << mPersistedMinds.size() << " Number of rules: " << mPersistedRules.size());
	mStats.entitiesCount = static_cast<unsigned int>(mPersistedEntities.size());
	mStats.mindsCount = static_cast<unsigned int>(mPersistedMinds.size());
	mStats.rulesCount = static_cast<unsigned int>(mPersistedRules.size());

	EventProgress.emit();

	if (mPersistedRules.empty()) {
		startEntityWalking();
	} else {
		startRuleWalking();
	}

}

void EntityImporterBase::registerEntityReferences(const std::string& id, const Atlas::Message::MapType& element)
{
	for (const auto& I : element) {
		const auto& name = I.first;
		if (name == "id" || name == "parent" || name == "contains") {
			continue;
		}
		if (hasEntityReference(I.second)) {
			mEntitiesWithReferenceAttributes[id].push_back(name);
		}
	}
}

bool EntityImporterBase::hasEntityReference(const Element& element)
{
	if (element.isMap()) {
		auto entityRefI = element.asMap().find("$eid");
		if (entityRefI != element.asMap().end() && entityRefI->second.isString()) {
			return true;
		}
		//If it's a map we need to process all child elements too
		for (auto& I : element.asMap()) {
			return hasEntityReference(I.second);
		}
	} else if (element.isList()) {
		//If it's a list we need to process all child elements too
		for (auto& I : element.asList()) {
			return hasEntityReference(I);
		}
	}
	return false;
}

void EntityImporterBase::startEntityWalking()
{
	m_state = ENTITY_WALKSTART;
	Look l;

	l->setFrom(mAvatarId);
	l->setSerialno(newSerialNumber());

	sendOperation(l);

}

void EntityImporterBase::startRuleWalking()
{
	auto ruleI = mPersistedRules.find("root");
	if (ruleI == mPersistedRules.end()) {
		S_LOG_WARNING("Rules exist, but there's no root rule.");
		startEntityWalking();
	}
	m_state = RULE_WALKING;

	OpVector res;
	getRule("root", res);
	for (const auto& op : res) {
		sendOperation(op);
	}
}

void EntityImporterBase::sendOperation(const Operation& op)
{
	if (!op->isDefaultSerialno()) {
		sigc::slot<void(const Operation&)> slot = sigc::mem_fun(*this, &EntityImporterBase::operation);
		sendAndAwaitResponse(op, slot);
	} else {
		send(op);
	}
}

void EntityImporterBase::cancel()
{
	m_state = CANCEL;
}

const EntityImporterBase::Stats& EntityImporterBase::getStats() const
{
	return mStats;
}

void EntityImporterBase::setResume(bool enabled)
{
	mResumeWorld = enabled;
}

void EntityImporterBase::operationThinkResult(const Operation &)
{
	mThoughtOpsInTransit--;
	if (mThoughtOpsInTransit == 0) {
		complete();
	}
}

void EntityImporterBase::operationSetResult(const Operation &)
{
	mSetOpsInTransit--;
	if (mSetOpsInTransit == 0) {
		sendMinds();
	}
}

void EntityImporterBase::operation(const Operation & op)
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

	for (auto& resOp : res) {
		sendOperation(resOp);
	}

}

