//
// C++ Interface: MatchDefinition
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
#ifndef EMBEROGRE_MODEL_MAPPINGMATCHDEFINITION_H
#define EMBEROGRE_MODEL_MAPPINGMATCHDEFINITION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Definitions {


class CaseDefinition;
}
}
}
}
#include "DefinitionBase.h"
#include "CaseDefinition.h"
#include <vector>

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Definitions {


/**
	A definition for a Match.
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class MatchDefinition  : public DefinitionBase
{
public:
	typedef std::vector<CaseDefinition> CaseStore;
	
	CaseStore& getCases();
	const CaseStore& getCases() const;
	
protected:
	CaseStore mCases;
};
}

}

}

}


#endif
