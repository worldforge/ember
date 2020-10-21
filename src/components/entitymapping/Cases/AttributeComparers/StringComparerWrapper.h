//
// C++ Interface: StringComparerWrapper
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
#ifndef EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSSTRINGCOMPARERWRAPPER_H
#define EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSSTRINGCOMPARERWRAPPER_H

#include <memory>
#include <Atlas/Message/Element.h>
#include "StringComparer.h"
#include "AttributeComparerWrapper.h"

namespace Ember {


namespace EntityMapping {

namespace Cases {

namespace AttributeComparers {


/**
	Wraps a string comparison comparer.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class StringComparerWrapper : public AttributeComparerWrapper {
public:
	explicit StringComparerWrapper(std::unique_ptr<StringComparer> comparer);

	bool testAttribute(const Atlas::Message::Element& attribute) override;

private:
	std::unique_ptr<StringComparer> mStringComparer;
};

}

}

}

}

#endif
