//
// C++ Interface: AttributeObserver
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGREATTRIBUTEOBSERVER_H
#define EMBEROGREATTRIBUTEOBSERVER_H

#include <sigc++/trackable.h>
#include <Eris/Entity.h>


namespace Eris {
class Entity;
}

namespace EmberOgre {

/**
	@brief Observes changes to a specific attribute and emits a signal.
	
	Note that it's only possible to observe changes to top level attributes.
	
	Eris::Entity already has functionality for easily listening to attribute changes through the Eris::Entity::observe method, but this method is hard to use from a scripting environment (such as lua), thus the need for this class.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class AttributeObserver : public sigc::trackable
{
public:

	/**
	 *    @brief Ctor. The entity to watch and the name of the attribute are supplied as parameters.
	 * @param entity The entity to watch.
	 * @param attributeName The name of the attribute to watch.
	 */
	AttributeObserver(Eris::Entity* entity, const std::string& attributeName);
	/**
	 *    Dtor.
	 */
	~AttributeObserver();
	
	/**
	@brief Emitted when the attribute which is watched changes.
	The value passed is the new element for the named attribute.
	*/
	sigc::signal<void, const Atlas::Message::Element&> EventChanged;
	
protected:

	/**
	We keep an internal reference to the slot which be activated when the attribute in the entity change. We then pass this change on through the EventChanged signal.
	*/
	Eris::Entity::AttrChangedSlot mSlot;
	
	/**
	 * This method is called by the watched entity whenever the attribute changes.
	 * @param attributeValue 
	 */
	void attributeChanged(const Atlas::Message::Element& attributeValue);
};

}

#endif
