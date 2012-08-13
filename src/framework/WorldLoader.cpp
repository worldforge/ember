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

#include "WorldLoader.h"

#include "MultiLineListFormatter.h"
#include "AtlasFileLoader.h"
#include "LoggingInstance.h"

#include <Eris/Account.h>
#include <Eris/Connection.h>
#include <Eris/Response.h>
#include <Eris/Avatar.h>

#include <Atlas/Objects/Anonymous.h>
#include <Atlas/Objects/Operation.h>

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
		obj(o), child(c)
{
}

StackEntry::StackEntry(const Atlas::Objects::Entity::RootEntity & o) :
		obj(o)
{
	child = obj->getContains().end();
}

void WorldLoader::getEntity(const std::string & id, OpVector & res)
{
	std::map<std::string, Root>::const_iterator I = m_objects.find(id);
	if (I == m_objects.end()) {
		S_LOG_FAILURE("Inconsistency in dump file: " << id << " missing.");
		return;
	}
	RootEntity obj = smart_dynamic_cast<RootEntity>(I->second);
	if (!obj.isValid()) {
		S_LOG_FAILURE("Corrupt dump - non entity found " << id << ".");
		return;
	}

	m_state = WALKING;
	m_treeStack.push_back(StackEntry(obj));

	Anonymous get_arg;
	get_arg->setId(id);
	get_arg->setObjtype("obj");

	Get get;
	get->setArgs1(get_arg);
	get->setFrom(mAccount.getId());
	get->setSerialno(Eris::getNewSerialno());
	res.push_back(get);
}

void WorldLoader::walk(OpVector & res)
{
	assert(!m_treeStack.empty());
	StackEntry & current = m_treeStack.back();
	if (current.obj->getContains().empty()) {
		// Pop: Go back to WALKING parent
		assert(!m_treeStack.empty());
		m_treeStack.pop_back();
		while (!m_treeStack.empty()) {
			StackEntry & se = m_treeStack.back();
			++se.child;
			if (se.child != se.obj->getContains().end()) {
				getEntity(*se.child, res);
				break;
			}
			m_treeStack.pop_back();
		}
		if (m_treeStack.empty()) {
			S_LOG_INFO("Restore done.");
			S_LOG_INFO("Restored " << m_count << ", created:" << m_createCount << ", updated:" << m_updateCount << ".");
			m_complete = true;
			EventCompleted.emit();
		}
	} else {
		// Start WALKING the current entity
		assert(current.child == current.obj->getContains().end());
		current.child = current.obj->getContains().begin();
		assert(current.child != current.obj->getContains().end());

		getEntity(*current.child, res);
	}
}

void WorldLoader::create(const RootEntity & obj, OpVector & res)
{
	++m_count;
	++m_createCount;

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

	res.push_back(create);
}

void WorldLoader::errorArrived(const Operation & op, OpVector & res)
{
	if (op->isDefaultRefno()) {
		return;
	}
	switch (m_state) {
	case WALKING:
	{
		assert(!m_treeStack.empty());
		StackEntry & current = m_treeStack.back();
		RootEntity obj = current.obj;

		assert(obj.isValid());

		create(obj, res);
	}
		break;
	case CREATING:
		S_LOG_FAILURE("Could not create.");
		m_complete = true;
		EventCompleted.emit();

		break;
	default:
		S_LOG_FAILURE("Unexpected state in state machine.");
		break;
	};
}

void WorldLoader::infoArrived(const Operation & op, OpVector & res)
{
	if (op->isDefaultRefno()) {
		return;
	}
	if (op->isDefaultArgs() || op->getArgs().empty()) {
		S_LOG_FAILURE("Info with no arg.");
		return;
	}

	if (m_state == CREATING) {
		Operation sub_op = op;
		if (!sub_op.isValid()) {
			return;
		}
		Root created = sub_op->getArgs().front();
		m_newIds.insert(created->getId());
		StackEntry & current = m_treeStack.back();
		current.restored_id = created->getId();
		S_LOG_VERBOSE("Created: " << created->getParents().front() << "(" << created->getId() << ")");
		walk(res);
	} else if (m_state == WALKING) {
		const Root & arg = op->getArgs().front();
		RootEntity ent = smart_dynamic_cast<RootEntity>(arg);
		if (!ent.isValid()) {
			S_LOG_FAILURE("Info response is not entity.");
			return;
		}
		if (arg->isDefaultId()) {
			S_LOG_FAILURE("Corrupted info response: no id.");
		}
		const std::string & id = arg->getId();

		StackEntry & current = m_treeStack.back();
		RootEntity obj = current.obj;

		assert(id == obj->getId());

		if (m_newIds.find(id) != m_newIds.end() || (m_treeStack.size() != 1 && ent->isDefaultLoc()) || ent->getParents().front() != obj->getParents().front()) {
			create(obj, res);
		} else {

			Root update = obj.copy();

			current.restored_id = id;

			S_LOG_VERBOSE("Updating: " << obj->getId() << "," << obj->getParents().front());

			// assert(id == "0");

			update->removeAttrFlag(Atlas::Objects::Entity::CONTAINS_FLAG);
			update->removeAttrFlag(Atlas::Objects::STAMP_FLAG);

			Set set;
			set->setArgs1(update);
			set->setFrom(mAccount.getActiveCharacters().begin()->second->getId());
			set->setTo(id);
			set->setSerialno(Eris::getNewSerialno());

			res.push_back(set);

			++m_count;
			++m_updateCount;

			m_state = UPDATING;
		}
	}
}

void WorldLoader::sightArrived(const Operation & op, OpVector & res)
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
		Operation sub_op = smart_dynamic_cast<Operation>(arg);
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

WorldLoader::WorldLoader(Eris::Account& account) :
		mAccount(account), m_count(0), m_updateCount(0), m_createCount(0), m_state(INIT), m_complete(false)
{
}

WorldLoader::~WorldLoader()
{
}

void WorldLoader::start(const std::string& filename)
{

	AtlasFileLoader loader(filename, m_objects);

	if (!loader.isOpen()) {
		m_complete = true;
		EventCompleted.emit();

		return;
	}
	loader.read();
	S_LOG_INFO("Starting loading of world. Number of objects: " << m_objects.size());

	Look l;

	l->setFrom(mAccount.getActiveCharacters().begin()->second->getId());
	l->setSerialno(Eris::getNewSerialno());

	sendOperation(l);

}

void WorldLoader::sendOperation(const Operation& op)
{
	if (!op->isDefaultSerialno()) {
		mAccount.getConnection()->getResponder()->await(op->getSerialno(), this, &WorldLoader::operation);
	}
	mAccount.getConnection()->send(op);
}

void WorldLoader::operation(const Operation & op)
{
	OpVector res;
	if (op->getClassNo() == Atlas::Objects::Operation::INFO_NO) {
		infoArrived(op, res);
	} else if (op->getClassNo() == Atlas::Objects::Operation::ERROR_NO) {
		errorArrived(op, res);
	} else if (op->getClassNo() == Atlas::Objects::Operation::SIGHT_NO) {
		sightArrived(op, res);
	}

	for (OpVector::const_iterator I = res.begin(); I != res.end(); ++I) {
		sendOperation(*I);
	}

}
}
