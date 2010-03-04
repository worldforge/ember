//
// C++ Interface: EntityMapping
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
#ifndef EMBEROGRE_MODEL_MAPPINGMODELMAPPING_H
#define EMBEROGRE_MODEL_MAPPINGMODELMAPPING_H


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <vector>
#include <iostream>
#include <memory>

#include <Atlas/Objects/Entity.h>

#include <Eris/Entity.h>

#include "Matches/EntityTypeMatch.h"


namespace Eris
{
class Entity;
class View;
}

namespace Ember {


/**
 * @brief Namespace for entity mapping, which is the main way entities are mapped to graphical representation.
 * The mapping framework is a rule based engine which evaluates entities and their attributes to determine what action should be taken for presenting the entity to the user.
 */
namespace EntityMapping {

namespace Cases {
class OutfitCase;
class CaseBase;
class EntityTypeCase;
class AttributeCase;

namespace AttributeComparers {
class AttributeComparerWrapper;
}

}
namespace Matches {
class EntityTypeMatch;
class MatchBase;
class Match;
class AttributeMatch;
class OutfitMatch;

namespace Observers {
class AttributeObserver;
class EntityCreationObserver;

}

}


/**
Utility method for deleting all object held in a std::vector.
*/
template <typename T>
static void cleanVector(T& theVector)
{
	typename T::iterator I = theVector.begin();
	for (;I != theVector.end();	++I) {
		delete *I;
	}
	theVector.clear();
}



/**
	Instances of this are connected to Eris::Entity instances, and listen for changes in attributes and contained entities. When a change occurs, a client defined action is activated or deactivated, leading to client side changes. A typucal case is the changing of a model's appearance.

	The EntityMapping contains three different types of object: Matches, Cases and Actions.
	A Match tells the framework what needs to be tested. Examples are specific attributes or entity types
	Each Match has one or many Cases, which defines valid cases of the value that the Match is set to test for. The Cases are not mutually exclusive, so one or many cases can be true at the same time.
	Each Case can then contain either Actions or more Matches, or both. Actions are client defined, and can be to display a certain model, or to change a texture.

	These elements are arranged in a node tree structure, contained by the EntityMapping. The EntityMapping is self contained and uses it's own oberservers to watch for changes in the values that will result in changes. At the top of the node tree is an EntityTypeMatch instance, held directly by the EntityMapping. From this instance's Cases the framework determines what EntityMapping to use for a certain Eris::Type.

	Instances of this class are normally not created directly by the application, instead EntityMappingManager::createMapping(...) is used.

	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class EntityMapping
{
public:
	typedef std::vector<Cases::CaseBase*> CaseBaseStore;
	typedef std::vector<Matches::Observers::AttributeObserver*> AttributeObserverStore;
    EntityMapping(Eris::Entity* entity);

    ~EntityMapping();

    /**
    Gets the root entity match instance.
    */
    Matches::EntityTypeMatch& getRootEntityMatch();

    /**
    Initializes the mapping. Clients are required to call this on all new instances.
    */
    void initialize();


protected:

	Matches::EntityTypeMatch mRootEntityMatch;

	Eris::Entity* mEntity;
};

}

}

#endif
