//
// C++ Interface: EntityBaseCase
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
#ifndef EMBEROGRE_MODEL_MAPPING_CASESENTITYBASECASE_H
#define EMBEROGRE_MODEL_MAPPING_CASESENTITYBASECASE_H

#include <vector>
namespace Eris
{
class Entity;
class TypeInfo;
}


namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Cases {

/**
	A Case which triggers on the type of the entity.
	Entity type matching takes into account inheritance, so for example, if the type Settler is a child of the type Character, and this case is set to be valid for the type Character, it will also be valid for all Settlers. 
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class EntityBaseCase
{
public:

	virtual ~EntityBaseCase() {}

	bool testMatch(Eris::Entity* entity);
	
	/**
	Adds an entity type to the list of valid entity types for this instance.
	*/
	void addEntityType(Eris::TypeInfo* typeInfo);
	
protected:
	std::vector<Eris::TypeInfo*> mEntityTypes;
	virtual void _setState(bool state) = 0;
};


}

}

}

}

#endif
