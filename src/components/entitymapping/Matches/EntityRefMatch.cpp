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

#include "EntityRefMatch.h"

#include <utility>
#include "Observers/EntityCreationObserver.h"

namespace Ember {


namespace EntityMapping {

namespace Matches {

EntityRefMatch::EntityRefMatch(std::string attributeName, Eris::View* view)
		: mAttributeName(std::move(attributeName)),
		  mView(view),
		  mEntityObserver(nullptr) {
}

void EntityRefMatch::testAttribute(const Atlas::Message::Element& attribute, bool triggerEvaluation) {

	auto entityId = Eris::Entity::extractEntityId(attribute);
	if (entityId) {
		auto entity = mView->getEntity(*entityId);
		//the entity might not be available yet, so we need to create an observer for it
		if (!entity) {
			if (mEntityObserver) {
				mEntityObserver->observeCreation(mView, *entityId);
			}
		} else {
			testEntity(entity);
		}
	} else {
		testEntity(nullptr);
	}

	if (triggerEvaluation) {
		evaluateChanges();
	}
}

void EntityRefMatch::setEntity(Eris::Entity* entity) {
	//observe the attribute by the use of an MatchAttributeObserver
	mMatchAttributeObserver->observeEntity(entity);
}

void EntityRefMatch::setEntityCreationObserver(std::unique_ptr<Observers::EntityCreationObserver> observer) {
	mEntityObserver = std::move(observer);
}

void EntityRefMatch::testEntity(Eris::Entity* entity) {

	AbstractMatch<Cases::EntityRefCase>::setEntity(entity);

	for (auto& aCase : mCases) {
		aCase->testMatch(entity);
	}
	evaluateChanges();

}

void EntityRefMatch::setMatchAttributeObserver(std::unique_ptr<Observers::MatchAttributeObserver> observer) {
	mMatchAttributeObserver = std::move(observer);
}

}

}

}
