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

#ifndef EMBER_SINGLEATTRIBUTEMATCH_H
#define EMBER_SINGLEATTRIBUTEMATCH_H

#include "AttributeMatch.h"

namespace Ember {

namespace EntityMapping {

namespace Cases {
class AttributeCase;
}
}
}

#include "../Cases/AttributeCase.h"


namespace Ember {

namespace EntityMapping {

namespace Matches {

class SingleAttributeMatch : public AttributeMatch {
public:

	/**
	Creates a new instance that watches for changes to the supplied attribute.
	*/
	explicit SingleAttributeMatch(const std::string& attributeName);


	void testAttribute(const Atlas::Message::Element& attribute, bool triggerEvaluation) override;

	/**
	Sets the Entity to watch.
	*/
	void setEntity(Eris::Entity* entity) override;

	/**
	Use the supplied observer to observe changes to the attribute. The supplied observer will be managed by this class.
	*/
	void setMatchAttributeObserver(Observers::MatchAttributeObserver* observer);

protected:

	std::unique_ptr<Observers::MatchAttributeObserver> mMatchAttributeObserver;
};

}
}
}

#endif //EMBER_SINGLEATTRIBUTEMATCH_H
