//
// Copyright (C) 2009 Alistair Riddoch
// Copyright (C) 2012 Erik Ogenvik
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

//

#ifndef ENTITY_IMPORTER_H
#define ENTITY_IMPORTER_H

#include <Atlas/Objects/RootEntity.h>
#include <Atlas/Objects/SmartPtr.h>
#include <Atlas/Objects/ObjectsFwd.h>

#include <sigc++/trackable.h>
#include <sigc++/signal.h>

#include <vector>
#include <list>
#include <set>
#include <stack>
#include <fstream>

namespace Atlas
{
class Bridge;
class Codec;
class Encoder;
}

namespace Eris
{
class Account;
}
namespace Ember
{
typedef Atlas::Objects::Operation::RootOperation Operation;
typedef std::vector<Atlas::Objects::Operation::RootOperation> OpVector;

class StackEntry
{
public:
	const Atlas::Objects::Entity::RootEntity obj;
	std::string restored_id;
	std::list<std::string>::const_iterator child;

	StackEntry(const Atlas::Objects::Entity::RootEntity & o, const std::list<std::string>::const_iterator & c);
	explicit StackEntry(const Atlas::Objects::Entity::RootEntity & o);
};

/**
 * @brief Imports a previously exported entity.
 */
class EntityImporter: public virtual sigc::trackable
{
public:
	explicit EntityImporter(Eris::Account& account);
	virtual ~EntityImporter();

	virtual void start(const std::string& filename);

	/**
	 * @brief Cancels the loading.
	 */
	void cancel();

	/**
	 * @brief Emitted when the load has been completed.
	 */
	sigc::signal<void> EventCompleted;

	/**
	 * @brief Emitted when an entity has been updated or created.
	 *
	 * The argument denotes how many instances are left to load.
	 */
	sigc::signal<void, int> EventProgress;

protected:
	Eris::Account& mAccount;
	int m_count;
	int m_updateCount;
	int m_createCount;
	std::map<std::string, Atlas::Objects::Root> m_objects;
	enum
	{
		INIT, UPDATING, CREATING, WALKING, CANCEL, CANCELLED
	} m_state;

	std::deque<StackEntry> m_treeStack;
	std::set<std::string> m_newIds;

	void sendOperation(const Operation& op);

	void getEntity(const std::string & id, OpVector & res);
	void walk(OpVector & res);
	void create(const Atlas::Objects::Entity::RootEntity & obj, OpVector & res);

	void errorArrived(const Operation &, OpVector & res);
	void infoArrived(const Operation &, OpVector & res);
	void sightArrived(const Operation &, OpVector & res);

	void operation(const Operation& op);

};

}
#endif // TOOLS_WORLD_LOADER_H
