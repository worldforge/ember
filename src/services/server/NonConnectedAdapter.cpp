//
// C++ Implementation: NonConnectedAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "NonConnectedAdapter.h"

namespace Ember {

NonConnectedAdapter::NonConnectedAdapter()
{
}


NonConnectedAdapter::~NonConnectedAdapter()
{
}

void NonConnectedAdapter::moveToPoint(const WFMath::Point<3>& dest) {


}

void NonConnectedAdapter::moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation) {

}


void NonConnectedAdapter::moveInDirection(const WFMath::Vector<3>& velocity) {

}


void NonConnectedAdapter::touch(Eris::Entity* entity) 
{

}   		

void NonConnectedAdapter::take(Eris::Entity* entity) 
{

}   		

void NonConnectedAdapter::drop(Eris::Entity* entity, const WFMath::Vector<3>& offset) 
{

}
	
void NonConnectedAdapter::place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos)
{

}

void NonConnectedAdapter::place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos, const WFMath::Quaternion& orient)
{

}   		

void NonConnectedAdapter::wield(Eris::Entity* entity)
{

}   		

void NonConnectedAdapter::use(Eris::Entity* entity, WFMath::Point<3> pos, const std::string& operation)
{

}   		

void NonConnectedAdapter::useStop()
{

}   
	
void NonConnectedAdapter::actuate(Eris::Entity* entity, const std::string& action)
{
}
	
	
void NonConnectedAdapter::attack(Eris::Entity* entity)
{

}   

void NonConnectedAdapter::eat(Eris::Entity* entity)
{
}


void NonConnectedAdapter::say(const std::string &message) 
{

}

void NonConnectedAdapter::deleteEntity(Eris::Entity* entity)
{
}

void NonConnectedAdapter::setAttributes(Eris::Entity* entity, Atlas::Message::MapType& attributes)
{
}

void NonConnectedAdapter::adminTell(const std::string& entityId, const std::string& attribute, const std::string &value)
{
}


}
