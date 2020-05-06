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

#ifndef EMBER_VIRTUALATTRIBUTEMATCH_H
#define EMBER_VIRTUALATTRIBUTEMATCH_H

#include "AbstractMatch.h"
#include "AttributeDependentMatch.h"

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

/**
	A Match that inspects a certain attribute.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class VirtualAttributeMatch : public AttributeMatch {
public:


	/**
	Creates a new instance that watches for changes to the supplied attribute. The attribute that is watched differs from the name of the attribute. (Such as when using a function comparer for "height", where the internal attribute watched is "bbox".)
	*/
	VirtualAttributeMatch(std::string attributeName, std::initializer_list<std::string> internalAttributeNames);

	void testAttribute(const Atlas::Message::Element& attribute, bool triggerEvaluation) override;

	/**
	Sets the Entity to watch.
	*/
	void setEntity(Eris::Entity* entity) override;

	/**
	Use the supplied observer to observe changes to the attribute. The supplied observer will be managed by this class.
	*/
	void addMatchAttributeObserver(std::unique_ptr<Observers::MatchAttributeObserver> observer);

protected:

	std::vector<std::string> mInternalAttributeNames;

	std::vector<std::unique_ptr<Observers::MatchAttributeObserver>> mMatchAttributeObservers;

};


}

}

}


#endif //EMBER_VIRTUALATTRIBUTEMATCH_H
