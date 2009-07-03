//
// C++ Interface: CaseDefinition
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
#ifndef EMBEROGRE_MODEL_MAPPING_DEFINITIONSCASEDEFINITION_H
#define EMBEROGRE_MODEL_MAPPING_DEFINITIONSCASEDEFINITION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Definitions {
class MatchDefinition;
}
}
}
}

#include "DefinitionBase.h"
#include "MatchDefinition.h"
#include "ActionDefinition.h"
#include <vector>

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Definitions {

/**
	A definition for a Case.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class CaseDefinition : public DefinitionBase
{
public:
	typedef std::vector<MatchDefinition> MatchStore;
	typedef std::vector<ActionDefinition> ActionStore;
	typedef std::pair<std::string, std::string> ParameterEntry;
	typedef std::vector<ParameterEntry> ParameterStore;

	MatchStore& getMatches();
	const MatchStore& getMatches() const;
	ActionStore& getActions();
	const ActionStore& getActions() const;
	ParameterStore& getCaseParameters();
	const ParameterStore& getCaseParameters() const;

protected:
	MatchStore mMatches;
	ActionStore mActions;
	ParameterStore mParameters;
};
}

}

}

}

#endif
