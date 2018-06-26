//
// C++ Interface: AttributeDependentMatch
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
#ifndef EMBEROGRE_MODEL_MAPPING_MATCHESATTRIBUTEDEPENDENTMATCH_H
#define EMBEROGRE_MODEL_MAPPING_MATCHESATTRIBUTEDEPENDENTMATCH_H

#include <Atlas/Message/Element.h>
#include "Observers/MatchAttributeObserver.h"

namespace Ember {



namespace EntityMapping {

namespace Matches {


/**
	Base class for all Matches that are dependent on an attribute.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class AttributeDependentMatch
{
public:
	AttributeDependentMatch() = default;
	virtual ~AttributeDependentMatch() = default;

	/**
	Test changes to the attribute.
	*/
	virtual void testAttribute(const Atlas::Message::Element& attribute, bool triggerEvaluation) = 0;


};
}

}

}

#endif
