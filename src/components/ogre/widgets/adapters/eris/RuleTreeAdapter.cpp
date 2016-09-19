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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "RuleTreeAdapter.h"

#include "components/ogre/authoring/RulesFetcher.h"
#include "../../ColouredListItem.h"
#include "framework/LoggingInstance.h"

#include <CEGUI/widgets/Tree.h>
#include <CEGUI/widgets/TreeItem.h>

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
		mConnection(connection),mTreeWidget(treeWidget), mFetcher(nullptr)
{
}

RuleTreeAdapter::~RuleTreeAdapter()
{
	delete mFetcher;
}

void RuleTreeAdapter::refresh(const std::string& rootRule)
{

	if (!mFetcher) {
		mFetcher = new Authoring::RulesFetcher(mConnection);
		mFetcher->EventAllRulesReceived.connect(sigc::mem_fun(*this, &RuleTreeAdapter::fetcherAllRulesReceived));
		mFetcher->EventNewRuleReceived.connect(EventNewRuleReceived);
		mFetcher->startFetching(rootRule);
	}
}

void RuleTreeAdapter::fetcherAllRulesReceived()
{
	mRules = mFetcher->getRules();
	std::string rootRule = mFetcher->getRootRule();
	delete mFetcher;
	mFetcher = nullptr;

	const auto& root = getRule(rootRule);
	if (root.isValid()) {
		addToTree(root, nullptr, true);
	}
	EventAllRulesReceived.emit();
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
