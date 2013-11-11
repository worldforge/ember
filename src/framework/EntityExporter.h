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

#ifndef EMBER_ENTITYEXPORTER_H
#define EMBER_ENTITYEXPORTER_H

#include <Atlas/Objects/ObjectsFwd.h>

#include <sigc++/trackable.h>
#include <sigc++/signal.h>

#include <list>
#include <vector>
#include <fstream>
#include <map>
#include <unordered_map>
#include <memory>

namespace Atlas
{
class Bridge;
class Codec;
class Encoder;
namespace Objects
{

class ObjectsEncoder;
}
namespace Message
{
class QueuedDecoder;
class Encoder;
class Element;
}
}

namespace Eris
{
class Account;
}

namespace Ember
{

/**
 * @author Alistair Riddoch
 * @author Erik Ogenvik
 *
 * @brief Exports an entity (along with all contained entities) to disk.
 *
 * The entity export format is in Atlas format, with one top map.
 *
 *
 * <atlas>
 *  <map>
 *   <map name="meta>
 *    <string name="name">An optional name for this dump.</string>
 *    <string name="description">An optional description of this dump.</string>
 *    <string name="timestamp">The timestamp, CET, when this dump was taken.</string>
 *    <int name="transients">Boolean, true if transients are persisted.</int>
 *   </map>
 *   <map name="server">
 *    <string name="host">The hostname of the server.</string>
 *    <string name="name">The name of the server.</string>
 *   <string name="ruleset">The ruleset of the server.</string>
 *   </map>
 *   <list name="entities">
 *    all entities in Message form
 *   </list>
 *   <list name="minds">
 *    all minds in Message form
 *   </list>
 *   <list name="types">
 *    optional types in Message form
 *   </list>
 *  <map>
 * </atlas>
 */
class EntityExporter: public virtual sigc::trackable
{
public:

	/**
	 * @brief Stats about the process.
	 *
	 * Meant to be used in a GUI or debug info.
	 */
	struct Stats
	{
		/**
		 * @brief The number of entities queried.
		 */
		unsigned int entitiesQueried;
		/**
		 * @brief The number of entities received.
		 */
		unsigned int entitiesReceived;
		/**
		 * @brief The number of entities for which the server didn't send correct data.
		 */
		unsigned int entitiesError;
		/**
		 * @brief The number of minds queried.
		 */
		unsigned int mindsQueried;
		/**
		 * @brief The number of minds received.
		 */
		unsigned int mindsReceived;
		/**
		 * @brief The number of minds for which the server didn't send correct data.
		 */
		unsigned int mindsError;
		/**
		 * @brief The number of rules queried.
		 */
		unsigned int rulesQueried;
		/**
		 * @brief The number of rules received.
		 */
		unsigned int rulesReceived;
		/**
		 * @brief The number of rules queried.
		 */
		unsigned int rulesError;
	};

	/**
	 * @brief Ctor.
	 * @param account The account connected to the server. Must be an admin account.
	 */
	explicit EntityExporter(Eris::Account& account);

	/**
	 * @brief Dtor.
	 */
	virtual ~EntityExporter();

	/**
	 * @brief Starts the dumping process.
	 * It will be carried out async, so you must listen to EventCompleted to know when it's done.
	 * @param filename The file name to where the dump should be written.
	 * @param entityId The entity id of the entity to dump. If none is specified the root entity ("0") will be dumped.
	 */
	void start(const std::string& filename, const std::string& entityId = "0");

	/**
	 * @brief Cancels the dumping.
	 */
	void cancel();

	/**
	 * @brief Sets the name of the export.
	 *
	 * Call this before you call start().
	 * @param name The name.
	 */
	void setName(const std::string& name);

	/**
	 * @brief Sets the description of the export.
	 *
	 * Call this before you call start().
	 * @param description The description.
	 */
	void setDescription(const std::string& description);

	/**
	 * @brief Sets whether we should also export transient entities.
	 *
	 * The default is not to export those entities.
	 * @param exportTransient Whether we should export transient entities.
	 */
	void setExportTransient(bool exportTransient);

	/**
	 * @brief Gets whether we should also export transient entities.
	 * @return Whether we should also export transient entities.
	 */
	bool getExportTransient() const;

	/**
	 * @brief Sets whether we should preserve ids.
	 *
	 * @param preserveIds Whether we should preserve ids.
	 */
	void setPreserveIds(bool preserveIds);

	/**
	 * @brief Gets whether we should preserve ids.
	 * @return Whether we should preserve ids.
	 */
	bool getPreserveIds() const;

	/**
	 * @brief Sets whether we should export rules.
	 *
	 * @param exportRules Whether we should export rules.
	 */
	void setExportRules(bool exportRules);

	/**
	 * @brief Gets whether we should export rules.
	 * @return Whether we should export rules.
	 */
	bool getExportRules() const;

	/**
	 * @brief Gets stats about the export process.
	 * @return Stats about the process.
	 */
	const Stats& getStats() const;

	/**
	 * @brief Emitted when the dump is complete.
	 */
	sigc::signal<void> EventCompleted;

	/**
	 * @brief Emitted when there's any progress.
	 *
	 */
	sigc::signal<void> EventProgress;

protected:
	typedef Atlas::Objects::Operation::RootOperation Operation;

	Eris::Account& mAccount;

	Stats mStats;

	std::list<std::string> mQueue;
	std::map<int, std::string> mThoughtsOutstanding;

	/**
	 * @brief Contains mapping between the id of entities they have on the server, and the id they will get in the dump.
	 *
	 * This is used both for mapping to ids when mPreserveIds is set to false, but also to cull out transient
	 * entities from CONTAINS attributes.
	 */
	std::unordered_map<std::string, std::string> mIdMapping;

	/**
	 * @brief All entities as received from the server.
	 */
	std::vector<Atlas::Message::Element> mEntities;

	/**
	 * @brief All minds as received from the server.
	 */
	std::vector<Atlas::Message::Element> mMinds;

	/**
	 * @brief All rules received from the server.
	 */
	std::vector<Atlas::Message::Element> mRules;

	/**
	 * @brief The full file name of the dump.
	 */
	std::string mFilename;

	/**
	 * @brief The id of the entity
	 */
	std::string mRootEntityId;

	bool mComplete;
	bool mCancelled;

	/**
	 * @brief An optional name of the dump.
	 */
	std::string mName;

	/**
	 * @brief An optional description of the dump.
	 */
	std::string mDescription;

	/**
	 * @brief Keeps track of the number of outstanding get requests.
	 */
	size_t mOutstandingGetRequestCounter;

	/**
	 * @brief True if we should also export transient entities.
	 * Default is "false" (as if an entity is marked as transient it's not meant to be persisted).
	 */
	bool mExportTransient;

	/**
	 * @brief True if the original ids of the entities should be preserved.
	 * Default is false.
	 */
	bool mPreserveIds;

	/**
	 * @brief True if rules also should be exported.
	 */
	bool mExportRules;

	/**
	 * @brief Starts the process of requesting entities and walking the entity hierarchy.
	 */
	void startRequestingEntities();

	void dumpRule(const Atlas::Objects::Entity::RootEntity& ent);
	void dumpEntity(const Atlas::Objects::Entity::RootEntity& ent);
	void dumpMind(const std::string& entityId, const Operation & op);
	void infoArrived(const Operation& op);
	void thoughtOpArrived(const Operation& op);
	void operationGetResult(const Operation& op);
	void operationGetThoughtResult(const Operation& op);
	void operationGetRuleResult(const Operation& op);
	void requestThoughts(const std::string& entityId, const std::string& persistedId);
	void requestRule(const std::string& rule);

	/**
	 * @brief Checks the queue if we should ask the server for more data on any entity, or if we're done.
	 *
	 * This is the main method which should be called whenever something is received from the server.
	 */
	void pollQueue();

	/**
	 * @brief Call this when the dumping is complete.
	 *
	 * This will write the file to disk.
	 */
	void complete();

	/**
	 * @brief Adjusts entity references.
	 *
	 * If mPreserveIds is set to false then new ids will be generated for all entities.
	 * We then need to also make sure that any references in minds are updated to use the new ids.
	 */
	void adjustReferencedEntities();

};

}
#endif
