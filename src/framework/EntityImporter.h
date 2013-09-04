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
#include <unordered_set>

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

/**
 * @brief Represents one entry on the hierarchy of entities which are to be created on the server.
 */
class StackEntry
{
public:
	/**
	 * @brief The persisted entity data.
	 *
	 * This should be sent to the server.
	 */
	const Atlas::Objects::Entity::RootEntity obj;

	/**
	 * @brief The id of the entity on the server, after it's either been created or updated.
	 */
	std::string restored_id;

	/**
	 * @brief Keeps track of the what child we're currently handling.
	 *
	 * As we walk through the children of entity this is updated, thus keeping track
	 * of what child needs to be updated next.
	 */
	std::list<std::string>::const_iterator currentChildIterator;

	StackEntry(const Atlas::Objects::Entity::RootEntity & o, const std::list<std::string>::const_iterator & c);
	explicit StackEntry(const Atlas::Objects::Entity::RootEntity & o);
};

/**
 * @brief Imports a previously exported entity.
 */
class EntityImporter: public virtual sigc::trackable
{
public:

	/**
	 * @brief Short info about a entity dump file.
	 */
	struct ShortInfo
	{
		/**
		 * @brief The full name of the file of the dump.
		 */
		std::string filename;

		/**
		 * @brief An optional name of the dump.
		 */
		std::string name;

		/**
		 * @brief An optional description of the dump.
		 */
		std::string description;

		/**
		 * @brief The number of entities contained in the dump.
		 */
		int entityCount;
	};

	/**
	 * @brief Contains various stats about the import process, to be shown in an UI or help with debugging.
	 */
	struct Stats
	{
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

	/**
	 * @brief Reads a directory and gets information on any entity dumps found.
	 *
	 * This is not recursive.
	 * @param directoryPath The path to the directory.
	 * @return Information on any entity dumps found.
	 */
	static std::vector<ShortInfo> getInfoFromDirectory(const std::string& directoryPath);

	/**
	 * @brief Starts importing entities from the specified file.
	 * @param filename A path to an entity dump file.
	 */
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

	/**
	 * @brief Stats about the import process.
	 */
	Stats mStats;

	/**
	 * @brief All of the persisted entities, which are to be created on the server.
	 */
	std::map<std::string, Atlas::Objects::Root> mPersistedEntities;

	/**
	 * @brief All minds, which are connected to some of the mPersistedEntities.
	 *
	 * These are sent to the server after all of the entities are restored.
	 * Note that the mind data (i.e. the value) is first transferred to mResolvedMindMapping
	 * before it's sent to the server. This is because the id of the entity might
	 * be different from what's stored if it has to be created first.
	 */
	std::map<std::string, Atlas::Objects::Root> mPersistedMinds;

	/**
	 * @brief Keeps track of the responses from the server for create operations.
	 *
	 * This is used to populate m_entityIdMap with mapping data between entity id values found in the dump, and their new id values once they've been created.
	 */
	std::map<long, std::string> mCreateEntityMapping;

	/**
	 * @brief Keeps track of minds belonging to entities.
	 *
	 * This is needed (instead of directly accessing mPersistedMinds) since the id
	 * of the entity as created on the server might differ from what's persisted.
	 */
	std::vector<std::pair<std::string, Atlas::Objects::Root>> mResolvedMindMapping;

	enum
	{
		INIT, UPDATING, CREATING, WALKING, CANCEL, CANCELLED
	} m_state;

	/**
	 * @brief Keeps track of the hierarchy of entities that are to be created or updated.
	 */
	std::deque<StackEntry> m_treeStack;

	/**
	 * @brief Records all newly created ids.
	 *
	 * This is so that we can easily detect if a certain entity as persisted has the same id
	 * as another one we've just created. If so we shouldn't update the one on the server, instead we
	 * should create a new one.
	 */
	std::unordered_set<std::string> m_newIds;

	/**
	 * Contains a map between any id in the dump and the new id the entity has gotten when created.
	 *
	 * This can be used to update ownership information in minds.
	 */
	std::unordered_map<std::string, std::string> m_entityIdMap;

	/**
	 * @brief Sends an operation to the server.
	 */
	void sendOperation(const Operation& op);

	/**
	 * Gets an entity from the server.
	 * @param id
	 * @param res
	 * @return True if the entity id was found amongst the entities, else false. The latter case will occur for transient entities, as they might not have been exported, but are still references from their parent entity.
	 */
	bool getEntity(const std::string & id, OpVector & res);

	/**
	 * @brief Walks on the next entity in line to be created or updated on the server.
	 * @param res
	 */
	void walk(OpVector & res);

	/**
	 * @brief Sends all minds.
	 */
	void sendMinds();

	/**
	 * @brief Creates a new entity on the server.
	 * @param obj The entity specification.
	 * @param res
	 */
	void create(const Atlas::Objects::Entity::RootEntity & obj, OpVector & res);

	void errorArrived(const Operation &, OpVector & res);
	void infoArrived(const Operation &, OpVector & res);
	void sightArrived(const Operation &, OpVector & res);

	/**
	 * @brief Called when new data arrives from the server.
	 * @param op
	 */
	void operation(const Operation& op);

};

}
#endif // TOOLS_WORLD_LOADER_H
