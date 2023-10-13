//
// C++ Interface: MatchDefinition
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
#ifndef EMBEROGRE_MODEL_MAPPINGMATCHDEFINITION_H
#define EMBEROGRE_MODEL_MAPPINGMATCHDEFINITION_H

namespace Ember::EntityMapping::Definitions {
struct CaseDefinition;
}


#include "DefinitionBase.h"
#include "CaseDefinition.h"
#include <vector>


namespace Ember::EntityMapping::Definitions {

/**
	A definition for a Match.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
struct MatchDefinition : public DefinitionBase {
	std::vector<CaseDefinition> Cases;
};
}


#endif
