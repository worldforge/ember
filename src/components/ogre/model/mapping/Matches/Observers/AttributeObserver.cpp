//
// C++ Implementation: AttributeObserver
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "AttributeObserver.h"

#include "../../ModelMapping.h"
#include <Atlas/Message/Element.h>
#include <sigc++/slot.h>
#include "../AttributeDependentMatch.h"
#include "../AttributeMatch.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Matches {

namespace Observers {

AttributeObserver::AttributeObserver(Matches::AttributeDependentMatch* match,const std::string& attributeName) 
: mMatch(match), mAttributeName(attributeName)
{
}

AttributeObserver::AttributeObserver(Matches::AttributeMatch* match) 
: mMatch(match), mAttributeName(match->getAttributeName())
{
}

void AttributeObserver::attributeChanged( const Atlas::Message::Element& attributeValue)
{
	mMatch->testAttribute(attributeValue, true);
}

void AttributeObserver::observeEntity(Eris::Entity* entity)
{
	mSlot.disconnect();
	if (entity) {
		mSlot = sigc::mem_fun(*this, &AttributeObserver::attributeChanged);
		entity->observe(mAttributeName, mSlot);
	}
}



}

}

}

}

}
