//
// C++ Interface: NumericComparerWrapper
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
#ifndef EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSNUMERICCOMPARERWRAPPER_H
#define EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSNUMERICCOMPARERWRAPPER_H

#include "AttributeComparerWrapper.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Cases {

namespace AttributeComparers {

class NumericComparer;

/**
	An attribute comparer wrapper that handles numeric values.
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class NumericComparerWrapper : public AttributeComparerWrapper
{
public:
	NumericComparerWrapper(NumericComparer* comparer);
	virtual ~NumericComparerWrapper() {}
	virtual bool testAttribute(const Atlas::Message::Element& attribute);
private:
	std::auto_ptr<NumericComparer> mNumericComparer;
};


}

}

}

}

}

#endif
