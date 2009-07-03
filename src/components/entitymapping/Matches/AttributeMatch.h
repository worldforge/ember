//
// C++ Interface: AttributeMatch
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
	A Match that inspects a certain attribute.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class AttributeMatch : public AbstractMatch<Cases::AttributeCase>, public AttributeDependentMatch
{
public:

	/**
	Creates a new instance that watches for changes to the supplied attribute.
	*/
	AttributeMatch(const std::string& attributeName);
	
	/**
	Creates a new instance that watches for changes to the supplied attribute. The attribute that is watched differs from the name of the attribute. (Such as when using a function comparer for "height", where the internal attribute watched is "bbox".)
	*/
	AttributeMatch(const std::string& attributeName, const std::string& internalAttributeName);

	virtual void testAttribute(const Atlas::Message::Element& attribute, bool triggerEvaluation = false);
	
	/**
	Gets the name of the attribute that is watched. 
	*/
	const std::string& getAttributeName();
	
	/**
	Sets the Entity to watch.
	*/
	virtual void setEntity(Eris::Entity* entity);

protected:

	std::string mAttributeName, mInternalAttributeName;
};

inline const std::string& AttributeMatch::getAttributeName()
{
	return mAttributeName;
}

}

}

}

}

#endif
