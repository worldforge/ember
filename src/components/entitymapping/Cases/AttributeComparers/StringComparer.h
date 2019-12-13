//
// C++ Interface: StringComparer
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
#ifndef EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSSTRINGCOMPARER_H
#define EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSSTRINGCOMPARER_H

#include <string>

namespace Ember {


namespace EntityMapping {

namespace Cases {

namespace AttributeComparers {

/**
	Interface for string comparison.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
struct StringComparer {

	/**
	Returns true if the supplied value equals the stored value. The comparison is case sensitive.
	@param value
	*/
	virtual bool test(const std::string& value) = 0;

};

/**
 * Tests for string equality. This is case sensitive.
 */
class StringValueComparer : public StringComparer {
public:
	explicit StringValueComparer(std::string value);

	bool test(const std::string& value) override;

protected:
	std::string mValue;

};

/**
 * Checks that the string isn't empty.
 */
class StringNotEmptyComparer : public StringComparer {
public:

	bool test(const std::string& value) override;

};
}


}

}

}

#endif
