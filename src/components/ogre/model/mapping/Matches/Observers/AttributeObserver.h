//
// C++ Interface: AttributeObserver
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
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
#ifndef EMBEROGRE_MODEL_MAPPING_MATCHES_OBSERVERSATTRIBUTEOBSERVER_H
#define EMBEROGRE_MODEL_MAPPING_MATCHES_OBSERVERSATTRIBUTEOBSERVER_H

#include <sigc++/trackable.h>
#include "../../ModelMapping.h"
#include "../AbstractMatch.h"

#include <Eris/Entity.h>

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Matches {

class AttributeMatch;
class AttributeDependentMatch;

namespace Observers {

/**
	Observes changes to a specific attribute and automatically pass changes on the Match that the observer is attached to.
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class AttributeObserver : public sigc::trackable
{
public:

	AttributeObserver(AttributeMatch* match) ;
	AttributeObserver(AttributeDependentMatch* match, const std::string& attributeName);
	
	/**
	Sets the entity to observe.
	*/
	void observeEntity(Eris::Entity* entity);
	
protected:
	Eris::Entity::AttrChangedSlot mSlot;
	
	void attributeChanged(const Atlas::Message::Element& attributeValue);
	
	AttributeDependentMatch* mMatch;
	
	std::string mAttributeName;
	
};

}

}

}

}

}

#endif
