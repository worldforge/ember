/*
 Copyright (C) 2009 Erik Ogenvik

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

#include "RuleTreeAdapter.h"

#include "../../ColouredListItem.h"
#include "framework/LoggingInstance.h"

#include <CEGUI/widgets/Tree.h>

#include <memory>

using ::Atlas::Objects::Root;

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Eris {

RuleTreeAdapter::RuleTreeAdapter(::Eris::Connection& connection, std::string mindId, CEGUI::Tree& treeWidget) :
		mConnection(connection),
		mMindId(std::move(mindId)),
		mTreeWidget(treeWidget) {
}

void RuleTreeAdapter::refresh(const std::string& rootRule) {
	refreshRules({rootRule});
}

void RuleTreeAdapter::refreshRules(const std::vector<std::string>& rootRules) {
	mFetchers.clear();

	for (auto& rootRule : rootRules) {
		auto entry = mFetchers.emplace(rootRule, std::make_unique<Authoring::RulesFetcher>(mConnection, mMindId));
		entry.first->second->EventAllRulesReceived.connect([this, rootRule] { fetcherAllRulesReceived(rootRule); });
		entry.first->second->EventNewRuleReceived.connect(EventNewRuleReceived);
		entry.first->second->startFetching(rootRule);
	}
}

void RuleTreeAdapter::fetcherAllRulesReceived(const std::string& rootRule) {
	auto I = mFetchers.find(rootRule);
	if (I != mFetchers.end()) {
		auto& fetcher = I->second;

		mRules.insert(fetcher->getRules().begin(), fetcher->getRules().end());
		const auto& root = getRule(rootRule);
		if (root.isValid()) {
			Entry entry{root};
			fillEntry(entry, root);
			mTopEntries.emplace_back(std::move(entry));
		}

		rebuildTree();

		mFetchers.erase(rootRule);
		if (mFetchers.empty()) {
			EventAllRulesReceived.emit();
		}
	}
}

::Atlas::Objects::Root RuleTreeAdapter::getRule(const std::string& id) {
	auto I = mRules.find(id);
	if (I != mRules.end()) {
		return I->second;
	}
	return ::Atlas::Objects::Root();
}

std::list<std::string> RuleTreeAdapter::extractChildren(const Root& op) {
	std::list<std::string> children;
	Atlas::Message::Element childElem;
	if (op->copyAttr("children", childElem) == 0) {
		if (childElem.isList()) {
			for (auto child : childElem.asList()) {
				if (child.isString()) {
					children.push_back(child.asString());
				}
			}
		}
	}
	return children;
}

void RuleTreeAdapter::fillEntry(Entry& entry, ::Atlas::Objects::Root rule) {
	auto children = extractChildren(rule);
	for (auto& child: children) {
		auto I = mRules.find(child);
		if (I != mRules.end()) {
			Entry childEntry{I->second};
			fillEntry(childEntry, I->second);
			entry.children.emplace_back(std::move(childEntry));
		}
	}
}


void RuleTreeAdapter::addToTree(const Entry& entry, CEGUI::TreeItem* parent) {

	auto& rule = entry.rule;
	CEGUI::TreeItem* item = ColouredTreeItem::create(rule->getId());
	item->toggleIsOpen();
	if (!parent) {
		mTreeWidget.addItem(item);
	} else {
		parent->addItem(item);
	}

	for (auto& child : entry.children) {
		addToTree(child, item);
	}
}

Atlas::Objects::Root RuleTreeAdapter::getSelectedRule() {
	CEGUI::TreeItem* item = mTreeWidget.getFirstSelectedItem();
	if (item) {
		auto itemData = getRule(item->getText().c_str());
		if (itemData.isValid()) {
			return itemData;
		}
	}
	return Atlas::Objects::Root();
}

void RuleTreeAdapter::setFilter(std::string filter) {
	if (!filter.empty()) {
		auto item = mTreeWidget.findFirstItemWithText(filter);
		if (item) {
			mTreeWidget.ensureItemIsVisible(item);
			mTreeWidget.clearAllSelections();
			mTreeWidget.setItemSelectState(item, true);
		}
	}
}

void RuleTreeAdapter::rebuildTree() {
	while (mTreeWidget.getChildCount()) {
		mTreeWidget.removeChild(mTreeWidget.getChildAtIdx(0));
	}
	for (auto& entry: mTopEntries) {
		addToTree(entry, nullptr);
	}
}

}
}
}
}
}
