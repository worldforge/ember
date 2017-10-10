//
// C++ Implementation: OutfitMatch
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

#include "OutfitMatch.h"
#include "Observers/EntityCreationObserver.h"

namespace Ember {



namespace EntityMapping {

namespace Matches {

OutfitMatch::OutfitMatch(const std::string& outfitName, Eris::View* view)
: mOutfitName(outfitName), mView(view), mEntityObserver(nullptr)
{
}

void OutfitMatch::testAttribute(const Atlas::Message::Element& attribute, bool triggerEvaluation)
{
	if (attribute.isMap()) {
		Eris::Entity* entity(nullptr);
		const auto& tmap = attribute.asMap();
		auto I = tmap.find(mOutfitName);
		std::string entityId;
		if (I != tmap.end() && Eris::Entity::extractEntityId(I->second, entityId)) {
			entity = mView->getEntity(entityId);
			//the entity might not be available yet, so we need to create an observer for it
			if (!entity) {
				if (mEntityObserver) {
					mEntityObserver->observeCreation(mView, entityId);
				}
			} else {
				testEntity(entity);
			}
		} else {
			testEntity(entity);
		}
	}
	if (triggerEvaluation) {
		evaluateChanges();
	}
}

void OutfitMatch::setEntity(Eris::Entity* entity)
{
	//observe the attribute by the use of an MatchAttributeObserver
	mMatchAttributeObserver->observeEntity(entity);
}

void OutfitMatch::setEntityCreationObserver(Observers::EntityCreationObserver* observer)
{
	mEntityObserver.reset(observer);
}

void OutfitMatch::testEntity(Eris::Entity* entity)
{

	AbstractMatch<Cases::OutfitCase>::setEntity(entity);

	for (auto& aCase : mCases) {
		aCase->testMatch(entity);
	}
	evaluateChanges();

}

}

}

}
