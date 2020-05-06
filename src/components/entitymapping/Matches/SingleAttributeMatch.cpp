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

#include "SingleAttributeMatch.h"

#include <utility>

namespace Ember {

namespace EntityMapping {

namespace Matches {

SingleAttributeMatch::SingleAttributeMatch(std::string attributeName)
		: AttributeMatch(std::move(attributeName)) {
}


void SingleAttributeMatch::testAttribute(const Atlas::Message::Element& attribute, bool triggerEvaluation) {
	for (auto& aCase : mCases) {
		aCase->testMatch(attribute);
	}
	if (triggerEvaluation) {
		evaluateChanges();
	}
}

void SingleAttributeMatch::setEntity(Eris::Entity* entity) {
	AbstractMatch<Cases::AttributeCase>::setEntity(entity);
	if (mMatchAttributeObserver) {
		//observe the attribute by the use of an MatchAttributeObserver
		mMatchAttributeObserver->observeEntity(entity);
	}
	if (entity) {
		if (entity->hasProperty(mAttributeName)) {
			testAttribute(entity->valueOfProperty(mAttributeName), false);
//		} else {
//			testAttribute(Atlas::Message::Element(), false);
		}
	}
}

void SingleAttributeMatch::setMatchAttributeObserver(std::unique_ptr<Observers::MatchAttributeObserver> observer) {
	mMatchAttributeObserver = std::move(observer);
}

}

}

}
