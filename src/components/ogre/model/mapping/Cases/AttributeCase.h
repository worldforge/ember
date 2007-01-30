//
// C++ Interface: AttributeCase
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
#ifndef EMBEROGRE_MODEL_MAPPING_CASESATTRIBUTECASE_H
#define EMBEROGRE_MODEL_MAPPING_CASESATTRIBUTECASE_H

#include "Case.h"

#include <Atlas/Message/Element.h>
#include "AttributeComparers/AttributeComparerWrapper.h"
namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Matches {
class AttributeMatch;
}
}
}
}

#include "../Matches/AttributeMatch.h"


namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Cases {


/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class AttributeCase : public Case<Matches::AttributeMatch>
{
public:
	AttributeCase() {};
	~AttributeCase() {};
	bool testMatch(const Atlas::Message::Element& attribute);
	
	/**
	Default to a string comparer.
	*/
	void setAttributeValue(const std::string& value);
	void setAttributeComparerWrapper(AttributeComparers::AttributeComparerWrapper* comparerWrapper);
protected:
	//std::string mValue;
	std::auto_ptr<AttributeComparers::AttributeComparerWrapper> mComparerWrapper;
};

}

}

}

}

#endif
