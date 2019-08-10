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

#include "RuleEditor.h"

#include "framework/LoggingInstance.h"


#include <Eris/Connection.h>
#include <Eris/Account.h>
#include <Eris/Avatar.h>
#include <Eris/Response.h>

#include <Atlas/Objects/Entity.h>

using Atlas::Objects::Root;
using Atlas::Objects::smart_dynamic_cast;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Entity::RootEntity;
using Atlas::Objects::Operation::Get;
using Atlas::Message::Element;
using Atlas::Message::ListType;
using Atlas::Message::MapType;


namespace Ember
{
namespace OgreView
{
namespace Authoring
{

RuleEditor::RuleEditor(Eris::Avatar& avatar) :
		mAvatar(avatar)
{

}

long int RuleEditor::updateOrCreateRule(Atlas::Objects::Root& rule)
{
	auto serial = Eris::getNewSerialno();

	mQueuedRules.insert(std::make_pair(serial, rule));

	//We must first check if the rule exists or not
	Get get;
	Anonymous arg;
	arg->setId(rule->getId());
	get->setArgs1(arg);
	get->setObjtype("op");
	get->setSerialno(serial);
	get->setFrom(mAvatar.getId());

	mAvatar.getConnection()->getResponder()->await(get->getSerialno(), this, &RuleEditor::operationGetRuleResult);
	mAvatar.getConnection()->send(get);

	return serial;
}

void RuleEditor::operationGetRuleResult(const Atlas::Objects::Operation::RootOperation& op)
{
	auto refno = op->getRefno();
	auto queueI = mQueuedRules.find(refno);
	if (queueI == mQueuedRules.end()) {
		S_LOG_WARNING("Got response to GET for rule which wasn't registered in our queue.");
		return;
	}

	if (op->getClassNo() == Atlas::Objects::Operation::ERROR_NO) {
		createRule(refno, queueI->second);
	} else if (op->getClassNo() == Atlas::Objects::Operation::INFO_NO) {
		updateRule(refno, queueI->second);
	}
}

void RuleEditor::operationCreateRuleResult(const Atlas::Objects::Operation::RootOperation& op)
{
	auto refno = op->getRefno();
	auto queueI = mQueuedRules.find(refno);
	if (queueI == mQueuedRules.end()) {
		S_LOG_WARNING("Got response to SET or CREATE for rule which wasn't registered in our queue.");
		return;
	}
	mQueuedRules.erase(queueI);

	if (op->getClassNo() == Atlas::Objects::Operation::ERROR_NO) {
		EventRuleEditError.emit(refno);
	} else if (op->getClassNo() == Atlas::Objects::Operation::INFO_NO) {
		EventRuleCreated.emit(refno);
	}
}



void RuleEditor::operationUpdateRuleResult(const Atlas::Objects::Operation::RootOperation& op)
{
	auto refno = op->getRefno();
	auto queueI = mQueuedRules.find(refno);
	if (queueI == mQueuedRules.end()) {
		S_LOG_WARNING("Got response to SET or CREATE for rule which wasn't registered in our queue.");
		return;
	}
	mQueuedRules.erase(queueI);

	if (op->getClassNo() == Atlas::Objects::Operation::ERROR_NO) {
		EventRuleEditError.emit(refno);
	} else if (op->getClassNo() == Atlas::Objects::Operation::INFO_NO) {
		EventRuleUpdated.emit(refno);
	}
}

void RuleEditor::updateRule(long int serial, Atlas::Objects::Root& rule)
{
	Atlas::Objects::Operation::Set op;
	op->setFrom(mAvatar.getId());
	op->setArgs1(rule);
	op->setSerialno(serial);

	mAvatar.getConnection()->getResponder()->await(op->getSerialno(), this, &RuleEditor::operationUpdateRuleResult);
	mAvatar.getConnection()->send(op);
}

void RuleEditor::createRule(long int serial, Atlas::Objects::Root& rule)
{
	Atlas::Objects::Operation::Create op;
	op->setFrom(mAvatar.getId());
	op->setArgs1(rule);
	op->setSerialno(serial);

	mAvatar.getConnection()->getResponder()->await(op->getSerialno(), this, &RuleEditor::operationCreateRuleResult);
	mAvatar.getConnection()->send(op);
}

}
}
}
