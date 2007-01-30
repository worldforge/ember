//
// C++ Interface: NumericEqualsOrGreaterComparer
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
#ifndef EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSNUMERICEQUALSORGREATERCOMPARER_H
#define EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSNUMERICEQUALSORGREATERCOMPARER_H

#include "NumericComparer.h"
#include "NumericValueHolder.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Cases {

namespace AttributeComparers {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class NumericEqualsOrGreaterComparer : public NumericComparer, private NumericValueHolder
{
public:
	NumericEqualsOrGreaterComparer(float value);
	virtual bool test(float value);

protected:
};


}

}

}

}

}

#endif
