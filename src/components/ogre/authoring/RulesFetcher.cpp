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

#include "RulesFetcher.h"
#include "framework/LoggingInstance.h"

#include <Eris/Connection.h>
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

namespace Ember {
namespace OgreView {
namespace Authoring {

RulesFetcher::RulesFetcher(Eris::Connection& connection, std::string mindId) :
		mConnection(connection), mMindId(std::move(mindId)) {
}

const std::unordered_map<std::string, RulesFetcher::RuleEntry>& RulesFetcher::getRules() const {
	return mRules;
}

void RulesFetcher::startFetching(const std::string& rootRule) {
	mRootRule = rootRule;
	fetchRule(rootRule);
}

void RulesFetcher::fetchRule(const std::string& id) {
	Get get;
	Anonymous arg;
	arg->setId(id);
	get->setArgs1(arg);
	get->setObjtype("op");
	get->setSerialno(Eris::getNewSerialno());
	get->setFrom(mMindId);

	mConnection.getResponder().await(get->getSerialno(), this, &RulesFetcher::operationGetRuleResult);
	mConnection.send(get);

}

void RulesFetcher::operationGetRuleResult(const Atlas::Objects::Operation::RootOperation& op) {
	if (op->getArgs().empty()) {
		S_LOG_WARNING("Got response to GET for rule with no args.");
		return;
	}

	Root arg = op->getArgs().front();

	std::vector<std::string> children;
	if (arg->hasAttr("children")) {
		Element childrenElement;
		if (arg->copyAttr("children", childrenElement) == 0) {
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

	mRules.insert(std::make_pair(arg->getId(), RuleEntry{arg, children}));
	EventNewRuleReceived.emit((int) mRules.size());

	if (!children.empty()) {
		mRulesStack.push_back(StackEntry({arg->getId(), children, children.front()}));
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

const std::string& RulesFetcher::getRootRule() const {
	return mRootRule;
}

}
}
}
