//
// C++ Interface: OutfitMatch
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
#ifndef EMBEROGRE_MODEL_MAPPING_MATCHESOUTFITMATCH_H
#define EMBEROGRE_MODEL_MAPPING_MATCHESOUTFITMATCH_H

#include "AbstractMatch.h"
#include "AttributeDependentMatch.h"
#include "../Cases/OutfitCase.h"
#include <Eris/View.h>
#include <Atlas/Message/Element.h>

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Matches {

namespace Observers {
class EntityCreationObserver;
}

/**
	Watches for changes to a specific outfit point, such as "body" or "feet". Whenever an entity is outfitted or removed this will trigger.
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class OutfitMatch : public AbstractMatch<Cases::OutfitCase>, public AttributeDependentMatch
{
friend class Observers::EntityCreationObserver;
public:

	OutfitMatch(const std::string& outfitName, Eris::View* view);

	virtual void testAttribute(const Atlas::Message::Element& attribute, bool triggerEvaluation = false);
	
	inline const std::string& getOutfitName();
	virtual void setEntity(Eris::Entity* entity);
	
	void setEntityCreationObserver(Observers::EntityCreationObserver* observer);

protected:

	void testEntity(Eris::Entity* entity);
	std::string mOutfitName;
	Eris::View* mView;
	std::auto_ptr<Observers::EntityCreationObserver> mEntityObserver;
};
}

}

}

}

#endif
