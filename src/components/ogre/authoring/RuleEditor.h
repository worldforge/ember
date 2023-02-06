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

#ifndef EMBER_OGRE_AUTHORING_RULEEDITOR_H_
#define EMBER_OGRE_AUTHORING_RULEEDITOR_H_

#include <Atlas/Objects/SmartPtr.h>
#include <Atlas/Objects/ObjectsFwd.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Objects/RootOperation.h>

#include <sigc++/signal.h>
#include <sigc++/trackable.h>

#include <unordered_map>
#include <string>

namespace Eris
{
class Avatar;
}

namespace Ember
{
namespace OgreView
{
namespace Authoring
{

/**
 * @brief Allows for easy editing of rules on the server.
 */
class RuleEditor: public virtual sigc::trackable
{
public:

	explicit RuleEditor(Eris::Avatar& avatar);
	virtual ~RuleEditor() = default;

	/**
	 * @brief Updates or create a rule.
	 * @param rule The rule definition.
	 * @return A serial number which can be matched to the Even* signals.
	 */
	Atlas::Message::IntType updateOrCreateRule(Atlas::Objects::Root& rule);

	/**
	 * @brief Emitted when a rule has been created.
	 * The argument is a serial number.
	 */
	sigc::signal<void, Atlas::Message::IntType> EventRuleCreated;

	/**
	 * @brief Emitted when a rule has been updated.
	 * The argument is a serial number.
	 */
	sigc::signal<void, Atlas::Message::IntType> EventRuleUpdated;

	/**
	 * @brief Emitted when there was an error updating or creating a rule.
	 * The argument is a serial number.
	 */
	sigc::signal<void, Atlas::Message::IntType> EventRuleEditError;

private:

	Eris::Avatar& mAvatar;

	std::unordered_map<Atlas::Message::IntType, Atlas::Objects::Root> mQueuedRules;

	void operationGetRuleResult(const Atlas::Objects::Operation::RootOperation& op);
	void operationCreateRuleResult(const Atlas::Objects::Operation::RootOperation& op);
	void operationUpdateRuleResult(const Atlas::Objects::Operation::RootOperation& op);

	void updateRule(Atlas::Message::IntType serial, Atlas::Objects::Root& rule);
	void createRule(Atlas::Message::IntType serial, Atlas::Objects::Root& rule);
};

}
}
}

#endif /* EMBER_OGRE_AUTHORING_RULEEDITOR_H_ */
