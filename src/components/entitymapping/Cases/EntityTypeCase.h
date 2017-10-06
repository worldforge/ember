//
// C++ Interface: EntityTypeCase
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
#ifndef EMBEROGRE_MODEL_MAPPING_CASESENTITYTYPECASE_H
#define EMBEROGRE_MODEL_MAPPING_CASESENTITYTYPECASE_H

namespace Ember {



namespace EntityMapping {

namespace Matches {
class EntityTypeMatch;
}
}
}

#include "../Matches/EntityTypeMatch.h"
#include "EntityBaseCase.h"
#include "Case.h"

namespace Ember {



namespace EntityMapping {

namespace Cases {

/**
	An entity type triggering case that is a child of an EntityTypeMatch
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class EntityTypeCase : public Case<Matches::EntityTypeMatch>, public EntityBaseCase
{
public:

protected:
	void _setState(bool state) override;
};


}

}

}

#endif
