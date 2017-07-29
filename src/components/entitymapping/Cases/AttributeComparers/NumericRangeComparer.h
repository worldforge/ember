//
// C++ Interface: NumericRangeComparer
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
#ifndef EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSNUMERICRANGECOMPARER_H
#define EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSNUMERICRANGECOMPARER_H

#include "NumericComparer.h"
#include <memory>

namespace Ember {



namespace EntityMapping {

namespace Cases {

namespace AttributeComparers {

/**
	Test for values being in a certain range. The range is defined by two different NumericComparer instances.
	@author Erik Ogenvik <erik@ogenvik.org>
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
	std::unique_ptr<NumericComparer> mMinComparer;
	std::unique_ptr<NumericComparer> mMaxComparer;
};

}

}

}

}

#endif
