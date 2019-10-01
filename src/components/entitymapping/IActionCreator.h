//
// C++ Interface: IActionCreator
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_MODEL_MAPPTING_IACTIONCREATOR_H
#define EMBEROGRE_MODEL_MAPPTING_IACTIONCREATOR_H

#include "Definitions/CaseDefinition.h"

namespace Ember {



namespace EntityMapping {

namespace Cases {
class CaseBase;
}

class EntityMapping;

/**
Interface that allows for client specific actions to be created.

Clients that use the Mapping framework are required to implement this interface. This interface is used in EntityMappingManager::createMapping(...)
*/
struct IActionCreator
{
	virtual ~IActionCreator() = default;

	/**
	Creates actions for the supplied case.
	Use Cases::CaseBase::addAction(...) to add the actions to the case.
	*/
	virtual void createActions(EntityMapping& modelMapping, Cases::CaseBase* aCase, Definitions::CaseDefinition& caseDefinition) = 0;
};

}

}

#endif
