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

#ifndef RULESFETCHER_H_
#define RULESFETCHER_H_

#include <Atlas/Objects/SmartPtr.h>
#include <Atlas/Objects/ObjectsFwd.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Objects/RootOperation.h>

#include <sigc++/signal.h>
#include <sigc++/trackable.h>

#include <unordered_map>
#include <deque>
#include <string>

namespace Eris
{
class Connection;
}

namespace Ember
{
namespace OgreView
{
namespace Authoring
{

/**
 * @brief Fetches rules from the server.
 *
 * Listen for the EventAllRulesReceived signal to know when all rules have been received.
 *
 */
class RulesFetcher: public virtual sigc::trackable
{
public:
	RulesFetcher(Eris::Connection& connection);
	virtual ~RulesFetcher();

	/**
	 * @brief Start the process of getting rules from the server.
	 * @param rootRule The name of the root rule. Use "root" if you want all rules.
	 */
	void startFetching(const std::string& rootRule);

	/**
	 * @brief Gets all received rules so far.
	 *
	 * Best usage is to only call this once EventAllRulesReceived has been emitted.
	 * @return All received rules.
	 */
	const std::unordered_map<std::string, Atlas::Objects::Root>& getRules() const;

	/**
	 * @brief Gets the name of the root rule, as specified in startFetching().
	 * @return The name of the root rule.
	 */
	const std::string& getRootRule() const;

	/**
	 * @brief Emitted when a new rule has been received.
	 *
	 * The first argument is the total of rules received so far.
	 */
	sigc::signal<void> EventAllRulesReceived;

	/**
	 * @brief Emitted when all rules have been received.
	 */
	sigc::signal<void, int> EventNewRuleReceived;

private:

	struct StackEntry
	{
		std::string id;
		std::list<std::string> children;
		std::string currentChild;
	};

	Eris::Connection& mConnection;

	std::unordered_map<std::string, Atlas::Objects::Root> mRules;

	std::deque<StackEntry> mRulesStack;

	std::string mRootRule;

	void fetchRule(const std::string& id);
	void operationGetRuleResult(const Atlas::Objects::Operation::RootOperation& op);

};

}
}
}
#endif /* RULESFETCHER_H_ */
