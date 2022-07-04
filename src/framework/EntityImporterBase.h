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

#ifndef ENTITY_IMPORTERBASE_H
#define ENTITY_IMPORTERBASE_H

#include <Atlas/Objects/RootEntity.h>
#include <Atlas/Objects/SmartPtr.h>
#include <Atlas/Objects/ObjectsFwd.h>

#include <sigc++/trackable.h>
#include <sigc++/signal.h>

#include <vector>
#include <list>
#include <set>
#include <deque>
#include <unordered_map>
#include <unordered_set>

namespace Atlas
{
class Bridge;
class Codec;
class Encoder;
}

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
	std::vector<std::string>::const_iterator currentChildIterator;

	StackEntry(Atlas::Objects::Entity::RootEntity  o, const std::vector<std::string>::const_iterator & c);
	explicit StackEntry(Atlas::Objects::Entity::RootEntity  o);
};

/**
 * @brief Imports a previously exported entity.
 */
class EntityImporterBase: public virtual sigc::trackable
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
		/**
		 * @brief The number of rules contained in the dump.
		 */
		int rulesCount;
		/**
		 * @brief The number of minds contained in the dump.
		 */
		int mindsCount;
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
		/**
		 * The total number of rules to process.
		 */
		unsigned int rulesCount;
		/**
		 * The number of rules processed so far.
		 */
		unsigned int rulesProcessedCount;
		/**
		 * The number of rule update ops sent.
		 */
		unsigned int rulesUpdateCount;
		/**
		 * The number of rule creation ops sent.
		 */
		unsigned int rulesCreateCount;
		/**
		 * The number of failed rule creation ops.
		 */
		unsigned int rulesCreateErrorCount;
	};

	/**
	 * One entry on the stack of rules processed.
	 */
	struct RuleStackEntry
	{
		/**
		 * @brief The id of the rule.
		 */
		std::string id;
		/**
		 * @brief The definition of the rule.
		 */
		Atlas::Objects::Root definition;
		/**
		 * @brief The ids of the children of the rule.
		 */
		std::list<std::string> children;
		/**
		 * @brief The current children iterator. This is an iterator of the "children" field.
		 */
		std::list<std::string>::const_iterator currentChildIterator;
	};

	/**
	 * @brief Ctor.
	 * @param accountId The id of the account.
	 * @param avatarId The id of the avatar.
	 */
	explicit EntityImporterBase(std::string accountId, std::string avatarId);
	virtual ~EntityImporterBase();

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
	 * @brief Sets if we also should resume any suspended world when importing.
	 * @param enabled True if we should resume.
	 */
	void setResume(bool enabled);

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

	/**
	 * @brief The id of the account.
	 */
	const std::string mAccountId;

	/**
	 * @brief The id of the avatar.
	 */
	const std::string mAvatarId;

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
	 * @brief All of the persisted rules.
	 *
	 * When processing these, we first check if there's a rule already on the server.
	 * If there's not, we create a new one.
	 * If there already is one, we then compare the existing rule with the new one
	 * and only send a SET operation if there's any difference.
	 */
	std::map<std::string, Atlas::Objects::Root> mPersistedRules;

	/**
	 * @brief Keeps track of the responses from the server for create operations.
	 *
	 * This is used to populate m_entityIdMap with mapping data between entity id values found in the dump, and their new id values once they've been created.
	 */
	std::map<Atlas::Message::IntType, std::string> mCreateEntityMapping;

	/**
	 * @brief Keeps track of minds belonging to entities.
	 *
	 * This is needed (instead of directly accessing mPersistedMinds) since the id
	 * of the entity as created on the server might differ from what's persisted.
	 */
	std::vector<std::pair<std::string, Atlas::Objects::Root>> mResolvedMindMapping;

	/**
	 * @brief Keeps track of any entities that have references to other entities in their attributes.
	 *
	 * The key of the map is the id of the entity, as found in mPersistedEntities.
	 * The value of the map is a vector containing the names of the attributes with entity references.
	 *
	 * Any attribute which references another entity can't be correctly sent when the entity is
	 * initially created, since the referred entity might not have been created yet. This is even
	 * the most usual case, since most entity references refer to things worn or wielded, and these
	 * entities aren't created until the owner entity has been created.
	 */
	std::map<std::string, std::vector<std::string>> mEntitiesWithReferenceAttributes;

	enum
	{
		INIT, RULE_WALKING, RULE_UPDATING, RULE_CREATING, ENTITY_WALKSTART, ENTITY_UPDATING, ENTITY_CREATING, ENTITY_WALKING, CANCEL, CANCELLED
	} m_state;

	/**
	 * @brief Keeps track of the hierarchy of entities that are to be created or updated.
	 */
	std::deque<StackEntry> mTreeStack;

	/**
	 * @brief Keeps track of the hierarchy of rules that are to be created or updated.
	 */
	std::deque<RuleStackEntry> mRuleStack;

	/**
	 * @brief Records all newly created ids.
	 *
	 * This is so that we can easily detect if a certain entity as persisted has the same id
	 * as another one we've just created. If so we shouldn't update the one on the server, instead we
	 * should create a new one.
	 */
	std::unordered_set<std::string> mNewIds;

	/**
	 * Contains a map between any id in the dump and the new id the entity has gotten when created.
	 *
	 * This can be used to update ownership information in minds.
	 */
	std::unordered_map<std::string, std::string> mEntityIdMap;

	/**
	 * @brief Keeps track of the number of thought ops in transit.
	 */
	int mThoughtOpsInTransit;

	/**
	 * @brief Keeps track of the number of Set ops in transit.
	 */
	int mSetOpsInTransit;

	/**
	 * @brief True if we also should resume any suspended world when importing.
	 */
	bool mResumeWorld;

	/**
	 * @brief Sends an operation to the server.
	 */
	void sendOperation(const Operation& op);

	/**
	 * @brief Gets an entity from the server.
	 * @param id
	 * @param res
	 * @return True if the entity id was found amongst the entities, else false. The latter case will occur for transient entities, as they might not have been exported, but are still references from their parent entity.
	 */
	bool getEntity(const std::string & id, OpVector & res);

	/**
	 * @brief Gets a rule from the server.
	 * @param id
	 * @param res
	 * @return
	 */
	bool getRule(const std::string & id, OpVector & res);

	/**
	 * @brief Start walking through the entities, updating or creating them.
	 */
	void startEntityWalking();
	/**
	 * @brief Start walking through the rules, updating or creating them.
	 */
	void startRuleWalking();

	/**
	 * @brief Walks on the next entity in line to be created or updated on the server.
	 * @param res
	 */
	void walkEntities(OpVector & res);

	/**
	 * @brief Walks on the next rule in line to be created or updated on the server.
	 * @param res
	 */
	void walkRules(OpVector & res);

	/**
	 * @brief Sends all minds.
	 */
	void sendMinds();

	/**
	 * @brief Creates a new entity on the server.
	 * @param obj The entity specification.
	 * @param res
	 */
	void createEntity(const Atlas::Objects::Entity::RootEntity & obj, OpVector & res);

	/**
	 * @brief Creates a new rule on the server.
	 * @param obj The rule specification.
	 * @param res
	 */
	void createRule(const Atlas::Objects::Root & obj, OpVector & res);

	/**
	 * @brief Updates an existing rule.
	 *
	 * Note that the rule will only be updated if there's a difference between the new rule and the existing one.
	 * @param existingDefinition The existing rule, as received from the server.
	 * @param newDefinition The new rule.
	 * @param res
	 */
	void updateRule(const Atlas::Objects::Root& existingDefinition, const Atlas::Objects::Root& newDefinition, OpVector & res);

	/**
	 * @brief Register any entity referencing attributes, if found, in mEntitiesWithReferenceAttributes.
	 * @param id The persisted id of the entity.
	 * @param element The top level entity element.
	 */
	void registerEntityReferences(const std::string& id, const Atlas::Message::MapType& element);

	/**
	 * @brief Checks if the element has any entity references in it. This acts recursively.
	 * @param element The element to check.
	 * @return True if any entity reference was found.
	 */
	bool hasEntityReference(const Atlas::Message::Element& element);

	/**
	 * @brief Resolves any entity references in the element.
	 *
	 * This is done recursively.
	 * @param element The element to resolve entity references in.
	 */
	void resolveEntityReferences(Atlas::Message::Element& element);

	/**
	 * @brief Walk through the list of entities which have entity references in their attributes, and send update ops.
	 */
	void sendResolvedEntityReferences();

	void errorArrived(const Operation &, OpVector & res);
	void infoArrived(const Operation &, OpVector & res);
	void sightArrived(const Operation &, OpVector & res);

	/**
	 * @brief Called when new data arrives from the server.
	 * @param op
	 */
	void operation(const Operation& op);

	/**
	 * @brief Called when the result of a Think op is received.
	 * @param op
	 */
	void operationThinkResult(const Operation& op);

	/**
	 * @brief Called when the result of a Set op is received.
	 * @param op
	 */
	void operationSetResult(const Operation& op);

	/**
	 * @brief Called when the import is complete.
	 */
	void complete();

	/**
	 * @brief Helper method for extracting a list of children from a definition.
	 * @param op
	 * @param children
	 */
	void extractChildren(const Atlas::Objects::Root& op, std::list<std::string>& children);

	typedef sigc::slot<void, const Atlas::Objects::Operation::RootOperation&> CallbackFunction;

	/**
	 * @brief Create a new unique serial number for operations.
	 * @return A new serial number.
	 */
	virtual long int newSerialNumber() = 0;

	/**
	 * @brief Send an object to the server.
	 * @param op An object which is to be sent.
	 */
	virtual void send(const Atlas::Objects::Operation::RootOperation& op) = 0;

	/**
	 * @brief Send an object to the server and await a response.
	 * @param op An object which is to be sent.
	 * @param callback A callback, called when a response is received.
	 */
	virtual void sendAndAwaitResponse(const Atlas::Objects::Operation::RootOperation& op, CallbackFunction& callback) = 0;

	/**
	 * @brief Loads from a file.
	 * @param filename The full path to the file.
	 * @return The top level object stored in the file.
	 */
	virtual Atlas::Objects::Root loadFromFile(const std::string& filename) = 0;

};

#endif // ENTITY_IMPORTERBASE_H
