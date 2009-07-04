//
// C++ Interface: EntityTypeMatch
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
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
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
