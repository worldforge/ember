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

#ifndef TOOLS_WORLD_DUMPER_H
#define TOOLS_WORLD_DUMPER_H

#include <Atlas/Objects/ObjectsFwd.h>

#include <sigc++/trackable.h>
#include <sigc++/signal.h>

#include <list>
#include <vector>
#include <fstream>

class TiXmlDocument;

namespace Atlas
{
class Bridge;
class Codec;
class Encoder;
namespace Objects
{

class ObjectsEncoder;
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
 * @brief Dumps the complete world to disk.
 *
 * The entity export format is as follows:
 *
 * <entityexport>
 *  <name>An optional name for this dump</name>
 *  <description>An optional description of this dump.</description>
 *  <timestamp>The timestampe, CET, when this dump was taken.</timestamp>
 *  <server>
 *   <host>The hostname of the server.</host>
 *   <name>The name of the server.</name>
 *   <ruleset>The ruleset of the server.</ruleset>
 *  </server>
 *  <entities></entities>
 *  <minds></minds>
 * </entityexport>
 */
class WorldDumper: public virtual sigc::trackable
{
public:

	/**
	 * @brief Ctor.
	 * @param account The account connected to the server. Must be an admin account.
	 */
	explicit WorldDumper(Eris::Account& account);

	/**
	 * @brief Dtor.
	 */
	virtual ~WorldDumper();

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
	 * @brief Emitted when the dump is complete.
	 */
	sigc::signal<void> EventCompleted;

	/**
	 * @brief Emitted when an entity has been dumped.
	 *
	 * The argument denotes how many instances have been dumped.
	 * Note that it's not really possible (in a clean way) to in advance know how many instances there are on the server.
	 */
	sigc::signal<void, int> EventProgress;

protected:
	typedef Atlas::Objects::Operation::RootOperation Operation;

	Eris::Account& mAccount;
	std::list<std::string> mQueue;
	int mCount;
	std::fstream mFile;
	Atlas::Codec * mCodec;
	Atlas::Objects::ObjectsEncoder * mEncoder;
	Atlas::Bridge * mFormatter;


	/**
	 * @brief The resulting xml document.
	 */
	TiXmlDocument* mXmlDocument;

	/**
	 * @brief Contains the atlas output stream of the entities.
	 */
	std::stringstream* mEntityStream;

	/**
	 * @brief Contains the atlas output stream of the minds.
	 */
	std::stringstream* mMindStream;
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

	void dumpEntity(const Atlas::Objects::Entity::RootEntity& ent);
	void infoArrived(const Operation& op);
	void operation(const Operation& op);

	/**
	 * @brief Call this when the dumping is complete.
	 *
	 * This will write the file to disk.
	 */
	void complete();

};

}
#endif
