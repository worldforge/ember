//
// C++ Interface: NonConnectedAdapter
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
#ifndef EMBERNONCONNECTEDADAPTER_H
#define EMBERNONCONNECTEDADAPTER_H

#include "IServerAdapter.h"

namespace Ember
{

/**
 @author Erik Ogenvik <erik@ogenvik.org>
 */
class NonConnectedAdapter: public IServerAdapter
{
public:
	NonConnectedAdapter() = default;

	~NonConnectedAdapter() override = default;

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

	void wield(Eris::Entity* entity, const std::string& outfitSlot) override;

	void take(Eris::Entity* entity) override;

	void use(Eris::Entity* entity, WFMath::Point<3> pos, const std::string& operation) override;

	void useStop() override;

	void actuate(Eris::Entity* entity, const std::string& action) override;

	void attack(Eris::Entity* entity) override;

	void eat(Eris::Entity* entity) override;

	void deleteEntity(Eris::Entity* entity) override;

	void setAttributes(Eris::Entity* entity, Atlas::Message::MapType& elements) override;

	void adminTell(const std::string& entityId, const std::string& attribute, const std::string &value) override;

	void createTypeInfo(const Atlas::Objects::Root& typeInfo) override;

	void setTypeInfo(const Atlas::Objects::Root& typeInfo) override;

};

}

#endif
