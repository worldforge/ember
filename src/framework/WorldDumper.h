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
	 */
	void start(const std::string& filename);

	/**
	 * @brief Emitted when the dump is complete.
	 */
	sigc::signal<void> EventCompleted;

protected:
	typedef Atlas::Objects::Operation::RootOperation Operation;

	Eris::Account& mAccount;
	int mLastSerialNo;
	std::list<std::string> mQueue;
	int mCount;
	std::fstream mFile;
	Atlas::Codec * mCodec;
	Atlas::Objects::ObjectsEncoder * mEncoder;
	Atlas::Bridge * mFormatter;
	bool mComplete;

	void dumpEntity(const Atlas::Objects::Entity::RootEntity& ent);
	void infoArrived(const Operation& op);
	void operation(const Operation& op);

};

}
#endif
