//
// C++ Interface: EntityCreationObserver
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
#ifndef EMBEROGRE_MODEL_MAPPING_MATCHES_OBSERVERSENTITYCREATIONOBSERVER_H
#define EMBEROGRE_MODEL_MAPPING_MATCHES_OBSERVERSENTITYCREATIONOBSERVER_H

#include <sigc++/trackable.h>
#include "../OutfitMatch.h"
#include "../../ModelMapping.h"
#include <Eris/View.h>

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Matches {

namespace Observers {

/**
	Observes for the creation of a specific entity in the world and automatically trigger the Match the observer is attached to when that entity is created.
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class EntityCreationObserver : public sigc::trackable
{
public:

	EntityCreationObserver(OutfitMatch& outfitMatch);
	~EntityCreationObserver();
	
	void observeCreation(Eris::View* view, const std::string& entityId);

protected:

	OutfitMatch& mOutfitMatch;

	Eris::View::EntitySightSlot mSlot;
	
	void entitySeen(Eris::Entity* entity);
	
};

}

}

}

}

}

#endif
