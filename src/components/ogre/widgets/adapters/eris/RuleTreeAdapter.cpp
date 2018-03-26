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

using ::Atlas::Objects::Root;

namespace Ember
{
namespace OgreView
{

namespace Gui
{

namespace Adapters
{

namespace Eris
{

RuleTreeAdapter::RuleTreeAdapter(::Eris::Connection& connection, CEGUI::Tree& treeWidget) :
		mConnection(connection),
		mTreeWidget(treeWidget)

{
}

void RuleTreeAdapter::refresh(const std::string& rootRule)
{
	refreshRules({rootRule});
}

void RuleTreeAdapter::refreshRules(const std::vector<std::string>& rootRules)
{
	mFetchers.clear();

	for (auto& rootRule : rootRules) {
		auto entry = mFetchers.insert({rootRule, std::unique_ptr<Authoring::RulesFetcher>(new Authoring::RulesFetcher(mConnection))});
		entry.first->second->EventAllRulesReceived.connect([this, rootRule] {fetcherAllRulesReceived(rootRule);});
		entry.first->second->EventNewRuleReceived.connect(EventNewRuleReceived);
		entry.first->second->startFetching(rootRule);
	}
}

void RuleTreeAdapter::fetcherAllRulesReceived(std::string rootRule)
{
	auto I = mFetchers.find(rootRule);
	if (I != mFetchers.end()) {
		auto& fetcher = I->second;

		mRules.insert(fetcher->getRules().begin(), fetcher->getRules().end());
		const auto& root = getRule(rootRule);
		if (root.isValid()) {
			addToTree(root, nullptr, true);
		}

		mFetchers.erase(rootRule);
		if (mFetchers.empty()) {
			EventAllRulesReceived.emit();
		}
	}
}

::Atlas::Objects::Root RuleTreeAdapter::getRule(const std::string& id)
{
	auto I = mRules.find(id);
	if (I != mRules.end()) {
		return I->second;
	}
	return ::Atlas::Objects::Root();
}

void RuleTreeAdapter::extractChildren(const Root& op, std::list<std::string>& children)
{
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
}

void RuleTreeAdapter::addToTree(const Root& rule, CEGUI::TreeItem* parent, bool addRecursive)
{

	CEGUI::TreeItem* item = ColouredTreeItem::create(rule->getId());
	item->toggleIsOpen();
	if (!parent) {
		mTreeWidget.addItem(item);
	} else {
		parent->addItem(item);
	}

	if (addRecursive) {
		std::list<std::string> children;
		extractChildren(rule, children);

		for (auto& child : children) {
			const auto& childData = getRule(child);
			if (childData.isValid()) {
				addToTree(childData, item, addRecursive);
			}
		}
	}

}

Atlas::Objects::Root RuleTreeAdapter::getSelectedRule()
{
	CEGUI::TreeItem* item = mTreeWidget.getFirstSelectedItem();
	if (item) {
		auto itemData = getRule(item->getText().c_str());
		if (itemData.isValid()) {
			return itemData;
		}
	}
	return Atlas::Objects::Root();
}

}
}
}
}
}
