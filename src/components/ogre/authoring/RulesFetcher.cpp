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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "RulesFetcher.h"
#include "framework/LoggingInstance.h"

#include <Eris/Connection.h>
#include <Eris/Response.h>
#include <Atlas/Objects/Entity.h>

#include <algorithm>

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

RulesFetcher::RulesFetcher(Eris::Connection& connection) :
		mConnection(connection)
{
}

RulesFetcher::~RulesFetcher()
{
}

const std::unordered_map<std::string, Atlas::Objects::Root>& RulesFetcher::getRules() const
{
	return mRules;
}

void RulesFetcher::startFetching(const std::string& rootRule)
{
	mRootRule = rootRule;
	fetchRule(rootRule);
}

void RulesFetcher::fetchRule(const std::string& id)
{
	Get get;
	Anonymous arg;
	arg->setId(id);
	get->setArgs1(arg);
	get->setObjtype("op");
	get->setSerialno(Eris::getNewSerialno());

	mConnection.getResponder()->await(get->getSerialno(), this, &RulesFetcher::operationGetRuleResult);
	mConnection.send(get);

}

void RulesFetcher::operationGetRuleResult(const Atlas::Objects::Operation::RootOperation& op)
{
	if (op->getArgs().empty()) {
		S_LOG_WARNING("Got response to GET for rule with no args.");
		return;
	}

	Root ent = smart_dynamic_cast<Root>(op->getArgs().front());
	if (!ent.isValid()) {
		S_LOG_WARNING("Malformed rule arg.");
		return;
	}

	std::list<std::string> children;
	if (ent->hasAttr("children")) {
		Element childrenElement;
		if (ent->copyAttr("children", childrenElement) == 0) {
			if (childrenElement.isList()) {
				ListType& childrenList = childrenElement.asList();
				for (auto& childElem : childrenList) {
					if (childElem.isString()) {
						children.push_back(childElem.asString());
					} else {
						S_LOG_WARNING("Child was not a string.");
					}
				}
			}
		}
	}

	mRules.insert(std::make_pair(ent->getId(), ent));
	EventNewRuleReceived.emit(mRules.size());

	if (!children.empty()) {
		mRulesStack.push_back(StackEntry({ent->getId(), children, children.front()}));
		fetchRule(children.front());
	} else {
		while (!mRulesStack.empty()) {
			auto& stackEntry = mRulesStack.back();
			auto childIterator = std::find(stackEntry.children.begin(), stackEntry.children.end(), stackEntry.currentChild);
			childIterator++;
			if (childIterator == stackEntry.children.end()) {
				mRulesStack.pop_back();
			} else {
				stackEntry.currentChild = *childIterator;
				fetchRule(stackEntry.currentChild);
				return;
			}
		}
		if (mRulesStack.empty()) {
			EventAllRulesReceived.emit();
		}
	}

}

const std::string& RulesFetcher::getRootRule() const
{
	return mRootRule;
}

}
}
}
