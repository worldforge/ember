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

#ifndef RULETREEADAPTER_H_
#define RULETREEADAPTER_H_

#include "components/ogre/authoring/RulesFetcher.h"

#include <Atlas/Objects/SmartPtr.h>
#include <Atlas/Objects/Root.h>

#include <unordered_map>
#include <sigc++/trackable.h>
#include <sigc++/signal.h>
#include <string>
#include <memory>

namespace Eris
{
class Connection;
}

namespace CEGUI
{
class Tree;
class TreeItem;
}

namespace Ember
{
namespace OgreView
{
namespace Authoring {
class RulesFetcher;
}
namespace Gui
{

namespace Adapters
{

namespace Eris
{
/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief An adapter which the server rule tree to a cegui tree widget.
 *
 * Make sure to call refresh(...) to activate the adapter.
 */
class RuleTreeAdapter: public virtual sigc::trackable
{
public:
	RuleTreeAdapter(::Eris::Connection& connection, CEGUI::Tree& treeWidget);

	virtual ~RuleTreeAdapter() = default;

	/**
	 * @brief Refreshes the tree with new data from the server.
	 * @param rootRule The name of the root rule.
	 */
	void refresh(const std::string& rootRule);

	/**
	 * @brief Refreshes the tree with new data from the server.
	 * @param rootRule The name of the root rule.
	 */
	void refreshRules(const std::vector<std::string>& rootRules);

	/**
	 * @brief Gets the currently selected rule, if any.
	 * @returns The currently selected rule, if any.
	 */
	::Atlas::Objects::Root getSelectedRule();

	/**
	 * @brief Emitted when a new rule has been received.
	 *
	 * The first argument is the total of rules received so far.
	 */
	sigc::signal<void, int> EventNewRuleReceived;

	/**
	 * @brief Emitted when all rules have been received.
	 */
	sigc::signal<void> EventAllRulesReceived;

private:

	::Eris::Connection& mConnection;
	CEGUI::Tree& mTreeWidget;
	std::map<std::string, std::unique_ptr<Authoring::RulesFetcher>> mFetchers;

	std::unordered_map<std::string, ::Atlas::Objects::Root> mRules;


	/**
	 * @brief Hooked up to the RuleFetcher::EventAllRulesReceived signal.
	 */
	void fetcherAllRulesReceived(std::string rootRule);

	/**
	 * @brief Adds a rule to the tree.
	 * @param rule The rule to add.
	 * @param parent The parent of the rule, or 0 if we should add to the top.
	 * @param addRecursive If true, all the current children of the rule will be added as well.
	 */
	void addToTree(const ::Atlas::Objects::Root& rule, CEGUI::TreeItem* parent, bool addRecursive = false);


	/**
	 * @brief Utility method for extracting a list of children from an Atlas op.
	 * @param op An atlas op.
	 * @param children A list which will be filled with children.
	 */
	void extractChildren(const ::Atlas::Objects::Root& op, std::list<std::string>& children);

	/**
	 * @brief Gets a rule from the internal store of rules.
	 * @param id The id of the rule.
	 * @return
	 */
	::Atlas::Objects::Root getRule(const std::string& id);

};

}
}
}
}

}

#endif /* RULETREEADAPTER_H_ */
