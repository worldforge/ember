//
// C++ Interface: EntityTypeCase
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
#ifndef EMBEROGRE_MODEL_MAPPING_CASESENTITYTYPECASE_H
#define EMBEROGRE_MODEL_MAPPING_CASESENTITYTYPECASE_H

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Matches {
class EntityTypeMatch;
}
}
}
}

#include "../Matches/EntityTypeMatch.h"
#include "EntityBaseCase.h"
#include "Case.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Cases {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class EntityTypeCase : public Case<Matches::EntityTypeMatch>, public EntityBaseCase
{
public:
// 	bool testMatch(Eris::Entity* entity);
	
// 	void addEntityType(Eris::TypeInfoPtr typeInfo);
	
protected:
// 	std::vector<Eris::TypeInfoPtr> mEntityTypes;
	virtual void _setState(bool state);
};


}

}

}

}

#endif
