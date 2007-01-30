//
// C++ Interface: AttributeMatch
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
#ifndef EMBEROGRE_MODEL_MAPPING_MATCHESATTRIBUTEMATCH_H
#define EMBEROGRE_MODEL_MAPPING_MATCHESATTRIBUTEMATCH_H

#include "AbstractMatch.h"
#include "AttributeDependentMatch.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Cases {
class AttributeCase;
}
}
}
}

#include "../Cases/AttributeCase.h"


namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Matches {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class AttributeMatch : public AbstractMatch<Cases::AttributeCase>, public AttributeDependentMatch
{
public:

	AttributeMatch(const std::string& attributeName);
	AttributeMatch(const std::string& attributeName, const std::string& internalAttributeName);

	virtual void testAttribute(const Atlas::Message::Element& attribute);
	
	inline const std::string& getAttributeName();
	virtual void setEntity(Eris::Entity* entity);

protected:

	std::string mAttributeName, mInternalAttributeName;
};

const std::string& AttributeMatch::getAttributeName()
{
	return mAttributeName;
}

}

}

}

}

#endif
