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
#ifndef EMBEROGRE_MODEL_MAPPING_MATCHESENTITYREFMATCH_H
#define EMBEROGRE_MODEL_MAPPING_MATCHESENTITYREFMATCH_H

#include "AbstractMatch.h"
#include "AttributeDependentMatch.h"
#include "components/entitymapping/Cases/EntityRefCase.h"
#include <Eris/View.h>
#include <Atlas/Message/Element.h>

namespace Ember {


namespace EntityMapping {

namespace Matches {

namespace Observers {
class EntityCreationObserver;
}

/**
 *	@brief Watches for changes to an attribute which points to another entity.
 *  This is mainly used for attachments, such as "attached_hand_primary" or "attached_feet". Whenever an entity is outfitted or removed this will trigger.
 *	@author Erik Ogenvik <erik@ogenvik.org>
 */
class EntityRefMatch : public AbstractMatch<Cases::EntityRefCase>, public AttributeDependentMatch {
	friend class Observers::EntityCreationObserver;

public:

	EntityRefMatch(const std::string& attributeName, Eris::View* view);

	void testAttribute(const Atlas::Message::Element& attribute, bool triggerEvaluation) override;

	void setEntity(Eris::Entity* entity) override;

	void setEntityCreationObserver(std::unique_ptr<Observers::EntityCreationObserver> observer);

	/**
	Use the supplied observer to observe changes to the attribute. The supplied observer will be managed by this class.
	*/
	void setMatchAttributeObserver(std::unique_ptr<Observers::MatchAttributeObserver> observer);

protected:

	void testEntity(Eris::Entity* entity);

	std::string mAttributeName;
	Eris::View* mView;
	std::unique_ptr<Observers::EntityCreationObserver> mEntityObserver;
	std::unique_ptr<Observers::MatchAttributeObserver> mMatchAttributeObserver;

};


}

}

}

#endif
