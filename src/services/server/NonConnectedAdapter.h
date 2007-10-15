//
// C++ Interface: NonConnectedAdapter
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
#ifndef EMBERNONCONNECTEDADAPTER_H
#define EMBERNONCONNECTEDADAPTER_H

#include "IServerAdapter.h"

namespace Ember {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class NonConnectedAdapter : public IServerAdapter
{
public:
    NonConnectedAdapter();

    ~NonConnectedAdapter();
    
	virtual void moveToPoint(const WFMath::Point<3>& dest); 
	virtual void moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation);
	virtual void moveInDirection(const WFMath::Vector<3>& velocity);
	virtual void say(const std::string &message);
	virtual void touch(Eris::Entity* entity);
	virtual void drop(Eris::Entity* entity, const WFMath::Vector<3>& offset);
	virtual void place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos = WFMath::Point<3>(0, 0, 0));
	virtual void place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos, const WFMath::Quaternion& orient);
	virtual void wield(Eris::Entity* entity);
	virtual void take(Eris::Entity* entity);
	virtual void use(Eris::Entity* entity, WFMath::Point<3> pos = WFMath::Point<3>(0,0,0));
	virtual void useStop();
	virtual void attack(Eris::Entity* entity);    
	virtual void eat(Eris::Entity* entity);
	virtual void deleteEntity(Eris::Entity* entity);
	virtual void setAttributes(Eris::Entity* entity, Atlas::Message::MapType& attributes);

};

}

#endif
