/*
 Copyright (C) 2010 erik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "DirectAttributeObserver.h"
#include "framework/LoggingInstance.h"

namespace Ember
{

DirectAttributeObserver::DirectAttributeObserver(Eris::Entity& entity, sigc::signal<void, const Atlas::Message::Element&>& eventChanged, const std::string& attributeName) :
	mEntity(entity), mAttributeName(attributeName), mEventChanged(eventChanged), mSlot(sigc::mem_fun(*this, &DirectAttributeObserver::attributeChanged))
{
	entity.observe(attributeName, mSlot);
}

DirectAttributeObserver::~DirectAttributeObserver()
{
	mSlot.disconnect();
}

void DirectAttributeObserver::forceEvaluation()
{
	if (mEntity.hasAttr(mAttributeName)) {
		attributeChanged(mEntity.valueOfAttr(mAttributeName));
	}
}

void DirectAttributeObserver::attributeChanged(const Atlas::Message::Element& attributeValue)
{
	mEventChanged.emit(attributeValue);
}

}
