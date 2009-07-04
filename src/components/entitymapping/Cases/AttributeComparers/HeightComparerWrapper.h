//
// C++ Interface: HeightComparerWrapper
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
#ifndef EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSHEIGHTCOMPARERWRAPPER_H
#define EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSHEIGHTCOMPARERWRAPPER_H

#include <memory>
#include "AttributeComparerWrapper.h"

namespace Eris
{
class Entity;
}


namespace Ember {



namespace EntityMapping {

namespace Cases {

namespace AttributeComparers {

class NumericComparer;

/**
	Compares the height of an entity. The height is calculated from the bounding box.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class HeightComparerWrapper : public AttributeComparerWrapper
{
public:
	/**
	* Default constructor.
	* @param comparer The NumericComparer to use for comparison.
	* @param entity
	*/
	HeightComparerWrapper(NumericComparer* comparer, Eris::Entity* entity);
	virtual ~HeightComparerWrapper() {}

	/**
	Test the height. The attribute passed will be ignored.
	*/
	virtual bool testAttribute(const Atlas::Message::Element& attribute);

protected:
	std::auto_ptr<NumericComparer> mNumericComparer;
	Eris::Entity* mEntity;
};
}

}

}

}

#endif
