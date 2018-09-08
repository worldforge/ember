//
// C++ Interface: ConnectedAdapter
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBERCONNECTEDADAPTER_H
#define EMBERCONNECTEDADAPTER_H

#include "IServerAdapter.h"
#include <Atlas/Objects/ObjectsFwd.h>

#include <sigc++/trackable.h>
namespace Eris
{
class Avatar;
class Connection;
class Account;
}

namespace Ember
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 * @brief A server adapter used when the user is connected and have an entity in the world.
 */
class ConnectedAdapter: public virtual IServerAdapter, public virtual sigc::trackable
{
public:
    ConnectedAdapter(Eris::Account& account, Eris::Avatar& avatar);

	~ConnectedAdapter() override = default;

	void moveToPoint(const WFMath::Point<3>& dest) override;

	void moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation) override;

	void moveInDirection(const WFMath::Vector<3>& velocity) override;
	// 	virtual void teleportTo(const WFMath::Point<3>& dest);
	void say(const std::string &message) override;

	void sayTo(const std::string &message, const std::vector<std::string>& entities) override;

	void sayTo(const std::string &message, const Eris::Entity& entity) override;

	void touch(Eris::Entity* entity, WFMath::Point<3> pos) override;

	void emote(const std::string& emote) override;

	void drop(Eris::Entity* entity, const WFMath::Vector<3>& offset, const WFMath::Quaternion& orientation) override;

	void place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos) override;

	void place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos, const WFMath::Quaternion& orient) override;

	void wield(Eris::Entity* entity, const std::string& attachPoint) override;

	void take(Eris::Entity* entity) override;

	void use(Eris::Entity* entity, const std::string& operation, WFMath::Point<3> pos) override;

	void useStop() override;

	void actuate(Eris::Entity* entity, const std::string& action) override;

	void attack(Eris::Entity* entity) override;

	void eat(Eris::Entity* entity) override;

	void deleteEntity(Eris::Entity* entity) override;

	void setAttributes(Eris::Entity* entity, Atlas::Message::MapType& elements) override;

	void adminTell(const std::string& entityId, const std::string& attribute, const std::string &value) override;

	void createTypeInfo(const Atlas::Objects::Root& typeInfo) override;

	void setTypeInfo(const Atlas::Objects::Root& typeInfo) override;

private:
	Eris::Account& mAccount;
	Eris::Avatar& mAvatar;
	Eris::Connection& mConnection;

	/**
	 * @brief An op handler which will just ignore all ops.
	 *
	 * Use this when you want to do something as an admin and you don't want the resulting op to enter into the regular op handling.
	 * @param op
	 */
	void operationResultIgnored(const Atlas::Objects::Operation::RootOperation& op);
};

}

#endif
