//
// C++ Implementation: AttributeMatch
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
#include "AttributeMatch.h"
#include "../Cases/AttributeCase.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Matches {

AttributeMatch::AttributeMatch(const std::string& attributeName) 
: mAttributeName(attributeName), mInternalAttributeName(attributeName) 
{
}
	
AttributeMatch::AttributeMatch(const std::string& attributeName, const std::string& internalAttributeName)
: mAttributeName(attributeName), mInternalAttributeName(internalAttributeName) 
{
}


void AttributeMatch::testAttribute(const Atlas::Message::Element& attribute, bool triggerEvaluation)
{
	for (std::vector<Cases::AttributeCase*>::iterator I = mCases.begin(); I != mCases.end(); ++I) {
		(*I)->testMatch(attribute);
	}
	if (triggerEvaluation) {
		evaluateChanges();
	}
}

void AttributeMatch::setEntity(Eris::Entity* entity)
{
	AbstractMatch<Cases::AttributeCase>::setEntity(entity);
	///observe the attribute by the use of an AttributeObserver
	mAttributeObserver->observeEntity(entity);
	if (entity) {
		if (entity->hasAttr(mInternalAttributeName)) {
			testAttribute(entity->valueOfAttr(mInternalAttributeName), false);
		} else {
			testAttribute(Atlas::Message::Element(), false);
		}
	}
}


}

}

}

}
