/*
 Copyright (C) 2018 Erik Ogenvik

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

#include "VirtualAttributeMatch.h"

#include <utility>

namespace Ember {

namespace EntityMapping {

namespace Matches {


VirtualAttributeMatch::VirtualAttributeMatch(std::string attributeName, std::initializer_list<std::string> internalAttributeNames)
		: AttributeMatch(std::move(attributeName)),
		  mInternalAttributeNames(internalAttributeNames) {
}


void VirtualAttributeMatch::testAttribute(const Atlas::Message::Element& attribute, bool triggerEvaluation) {
	for (auto& aCase : mCases) {
		aCase->testMatch(attribute);
	}
	if (triggerEvaluation) {
		evaluateChanges();
	}
}

void VirtualAttributeMatch::setEntity(Eris::Entity* entity) {
	AbstractMatch<Cases::AttributeCase>::setEntity(entity);
	for (auto& observer : mMatchAttributeObservers) {
		observer->observeEntity(entity);
	}
	if (entity) {

		for (const auto& attribute : mInternalAttributeNames) {
			if (entity->hasProperty(attribute)) {
				//Since the attribute is virtual we won't send any specific attribute.
				testAttribute(Atlas::Message::Element(), false);
				//After the first match we'll break out of the loop.
				break;
			}
		}
	}
}

void VirtualAttributeMatch::addMatchAttributeObserver(std::unique_ptr<Observers::MatchAttributeObserver> observer) {
	mMatchAttributeObservers.push_back(std::move(observer));
}

}

}

}