//
// C++ Implementation: MatchAttributeObserver
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "MatchAttributeObserver.h"

#include "../AttributeDependentMatch.h"
#include "../AttributeMatch.h"

namespace Ember {



namespace EntityMapping {

namespace Matches {

namespace Observers {

MatchAttributeObserver::MatchAttributeObserver(Matches::AttributeDependentMatch* match,const std::string& attributeName)
: mMatch(match), mAttributeName(attributeName)
{
}

MatchAttributeObserver::MatchAttributeObserver(Matches::AttributeMatch* match)
: mMatch(match), mAttributeName(match->getAttributeName())
{
}

void MatchAttributeObserver::attributeChanged( const Atlas::Message::Element& attributeValue)
{
	mMatch->testAttribute(attributeValue, true);
}

void MatchAttributeObserver::observeEntity(Eris::Entity* entity)
{
	mSlot.disconnect();
	if (entity) {
		mSlot = sigc::mem_fun(*this, &MatchAttributeObserver::attributeChanged);
		entity->observe(mAttributeName, mSlot);
		//If the attribute already exists, trigger a test now.
		if (entity->hasAttr(mAttributeName)) {
			attributeChanged(entity->valueOfAttr(mAttributeName));
		}
	}
}



}

}

}

}
