//
// C++ Interface: EntityTypeMatch
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
#ifndef EMBEROGRE_MODEL_MAPPING_MATCHESENTITYTYPEMATCH_H
#define EMBEROGRE_MODEL_MAPPING_MATCHESENTITYTYPEMATCH_H

namespace Ember {



namespace EntityMapping {

namespace Cases {
class EntityTypeCase;
}
}
}

#include "../Cases/EntityTypeCase.h"
#include "AbstractMatch.h"
namespace Eris
{
class Entity;
}

namespace Ember {



namespace EntityMapping {

namespace Matches {

/**
	Watches for certain entity types.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class EntityTypeMatch : public AbstractMatch<Cases::EntityTypeCase>
{
public:

	void testEntity(Eris::Entity* entity);
	virtual void setEntity(Eris::Entity* entity);

protected:

};

}

}

}

#endif
