//
// C++ Interface: NumericRangeComparer
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
#ifndef EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSNUMERICRANGECOMPARER_H
#define EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSNUMERICRANGECOMPARER_H

#include "NumericComparer.h"
#include <memory>

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Cases {

namespace AttributeComparers {

/**
	Test for values being in a certain range. The range is defined by two different NumericComparer instances.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class NumericRangeComparer : public NumericComparer
{
public:
	NumericRangeComparer(NumericComparer* minComparer, NumericComparer* maxComparer);
	virtual ~NumericRangeComparer() {}
	/**
	Returns true if the supplied value is true for both the used comparers.
	*/
	virtual bool test(float value);

protected:
	std::auto_ptr<NumericComparer> mMinComparer;
	std::auto_ptr<NumericComparer> mMaxComparer;
};

}

}

}

}

}

#endif
