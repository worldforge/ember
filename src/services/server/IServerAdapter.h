//
// C++ Interface: IServerAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBERISERVERADAPTER_H
#define EMBERISERVERADAPTER_H

#include <Atlas/Objects/Entity.h>

#include <wfmath/atlasconv.h>
#include <wfmath/point.h>

namespace Eris
{
	class Entity;
}

namespace Ember {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class IServerAdapter{
public:

	virtual ~IServerAdapter() {}

	virtual void moveToPoint(const WFMath::Point<3>& dest) = 0; 
	virtual void moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation) = 0;
	virtual void moveInDirection(const WFMath::Vector<3>& velocity) = 0;
	virtual void say(const std::string &message) = 0;
	virtual void touch(Eris::Entity* entity) = 0;
	virtual void drop(Eris::Entity* entity, const WFMath::Vector<3>& offset) = 0;
	virtual void place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos = WFMath::Point<3>(0, 0, 0)) = 0;
	virtual void place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos, const WFMath::Quaternion& orient) = 0;
	virtual void wield(Eris::Entity* entity) = 0;
	virtual void take(Eris::Entity* entity) = 0;
	virtual void use(Eris::Entity* entity, WFMath::Point<3> pos = WFMath::Point<3>(0,0,0)) = 0;
	virtual void useStop() = 0;
	virtual void attack(Eris::Entity* entity) = 0;
	virtual void deleteEntity(Eris::Entity* entity) = 0;
};

}

#endif
