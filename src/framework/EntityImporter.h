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
#include <unordered_map>

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

	struct ShortInfo
	{
		std::string filename;
		std::string name;
		std::string description;
		int entityCount;
	};

	/**
	 * @brief Contains various stats about the import process, to be shown in an UI or help with debugging.
	 */
	struct Stats {
		/**
		 * The total number of entities to process.
		 */
		unsigned int entitiesCount;
		/**
		 * The number of entities processed so far.
		 */
		unsigned int entitiesProcessedCount;
		/**
		 * The number of entity update ops sent.
		 */
		unsigned int entitiesUpdateCount;
		/**
		 * The number of entity creation ops sent.
		 */
		unsigned int entitiesCreateCount;
		/**
		 * The number of failed entity creation ops.
		 */
		unsigned int entitiesCreateErrorCount;
		/**
		 * The total number of minds to restore.
		 */
		unsigned int mindsCount;
		/**
		 * The total number of minds processed so far.
		 */
		unsigned int mindsProcessedCount;
	};


	explicit EntityImporter(Eris::Account& account);
	virtual ~EntityImporter();

	static std::vector<ShortInfo> getInfoFromDirectory(const std::string& directoryPath);


	virtual void start(const std::string& filename);

	/**
	 * @brief Cancels the loading.
	 */
	void cancel();

	/**
	 * @brief Gets stats about the importing process.
	 * @return Stats about the process.
	 */
	const Stats& getStats() const;

	/**
	 * @brief Emitted when the load has been completed.
	 */
	sigc::signal<void> EventCompleted;

	/**
	 * @brief Emitted when an entity has been updated or created.
	 *
	 */
	sigc::signal<void> EventProgress;

protected:
	Eris::Account& mAccount;
	Stats mStats;
	std::map<std::string, Atlas::Objects::Root> m_objects;
	std::map<std::string, Atlas::Objects::Root> mMinds;

	/**
	 * @brief Keeps track of the responses from the server for create operations.
	 *
	 * This is used to populate m_entityIdMap with mapping data between entity id values found in the dump, and their new id values once they've been created.
	 */
	std::map<long, std::string> mCreateEntityMapping;
	std::vector<std::pair<std::string, Atlas::Objects::Root>> mResolvedMindMapping;
	enum
	{
		INIT, UPDATING, CREATING, WALKING, CANCEL, CANCELLED
	} m_state;

	std::deque<StackEntry> m_treeStack;
	std::set<std::string> m_newIds;
	/**
	 * Contains a map between any id in the dump and the new id the entity has gotten when created.
	 *
	 * This can be used to update ownership information in minds.
	 */
	std::unordered_map<std::string, std::string> m_entityIdMap;

	void sendOperation(const Operation& op);

	/**
	 * Gets an entity from the server.
	 * @param id
	 * @param res
	 * @return True if the entity id was found amongst the entities, else false. The latter case will occur for transient entities, as they might not have been exported, but are still references from their parent entity.
	 */
	bool getEntity(const std::string & id, OpVector & res);
	void walk(OpVector & res);
	void sendMinds(OpVector & res);
	void create(const Atlas::Objects::Entity::RootEntity & obj, OpVector & res);

	void errorArrived(const Operation &, OpVector & res);
	void infoArrived(const Operation &, OpVector & res);
	void sightArrived(const Operation &, OpVector & res);

	void operation(const Operation& op);

};

}
#endif // TOOLS_WORLD_LOADER_H
