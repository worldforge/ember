//
// C++ Interface: CaseDefinition
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
#ifndef EMBEROGRE_MODEL_MAPPING_DEFINITIONSCASEDEFINITION_H
#define EMBEROGRE_MODEL_MAPPING_DEFINITIONSCASEDEFINITION_H


namespace Ember::EntityMapping::Definitions {
struct MatchDefinition;
}

#include "DefinitionBase.h"
#include "MatchDefinition.h"
#include "ActionDefinition.h"
#include <vector>


namespace Ember::EntityMapping::Definitions {

/**
	A definition for a Case.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
struct CaseDefinition : public DefinitionBase {
	typedef std::pair<std::string, std::string> ParameterEntry;

	std::vector<MatchDefinition> Matches;
	std::vector<ActionDefinition> Actions;
	std::vector<ParameterEntry> Parameters;
};
}





#endif
