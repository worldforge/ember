//
// C++ Implementation: OutfitMatch
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
#include "OutfitMatch.h"
#include "Observers/EntityCreationObserver.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Matches {

OutfitMatch::OutfitMatch(const std::string& outfitName, Eris::View* view)
: mOutfitName(outfitName), mView(view), mEntityObserver(0)
{
}

void OutfitMatch::testAttribute(const Atlas::Message::Element& attribute, bool triggerEvaluation)
{
	if (attribute.isMap()) {
		Eris::Entity* entity(0);
		const Atlas::Message::MapType & tmap = attribute.asMap();
		Atlas::Message::MapType::const_iterator I = tmap.find(mOutfitName);
		if (I != tmap.end() && I->second.isString()) {
			entity = mView->getEntity(I->second.asString());
			///the entity might not be available yet, so we need to create an observer for it
			if (!entity) {
				if (mEntityObserver.get()) {
					mEntityObserver->observeCreation(mView, I->second.asString());
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
	///observe the attribute by the use of an AttributeObserver
	mAttributeObserver->observeEntity(entity);
}

void OutfitMatch::setEntityCreationObserver(Observers::EntityCreationObserver* observer)
{
	mEntityObserver = std::auto_ptr<Observers::EntityCreationObserver>(observer);
}

void OutfitMatch::testEntity(Eris::Entity* entity)
{
// 	if (entity) {
// 		mEntityObserver.release();
// 	}
	
	AbstractMatch<Cases::OutfitCase>::setEntity(entity);	
	
	for (std::vector<Cases::OutfitCase*>::iterator I = mCases.begin(); I != mCases.end(); ++I) {
		(*I)->testMatch(entity);
	}
	evaluateChanges();
	
}

}

}

}

}
