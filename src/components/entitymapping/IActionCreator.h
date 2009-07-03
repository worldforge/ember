//
// C++ Interface: IActionCreator
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
#ifndef EMBEROGRE_MODEL_MAPPTING_IACTIONCREATOR_H
#define EMBEROGRE_MODEL_MAPPTING_IACTIONCREATOR_H

#include "Definitions/CaseDefinition.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Cases {
class CaseBase;
}

class EntityMapping;

/**
Interface that allows for client specific actions to be created.

Clients that use the Mapping framework are required to implement this interface. This interface is used in EntityMappingManager::createMapping(...)
*/
class IActionCreator
{
public:
	virtual ~IActionCreator() {}
	
	/**
	Creates actions for the supplied case.
	Use Cases::CaseBase::addAction(...) to add the actions to the case.
	*/
	virtual void createActions(EntityMapping& modelMapping, Cases::CaseBase* aCase, Definitions::CaseDefinition& caseDefinition) = 0;
private:
};

}

}

}

#endif
