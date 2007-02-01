//
// C++ Interface: ModelMapping
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

namespace EmberOgre {

namespace Model {

namespace Mapping {

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
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class ModelMapping
{
public:
	typedef std::vector<Cases::CaseBase*> CaseBaseStore;
	typedef std::vector<Matches::Observers::AttributeObserver*> AttributeObserverStore;
    ModelMapping(Eris::Entity* entity);

    ~ModelMapping();
    
    Matches::EntityTypeMatch& getRootEntityMatch();
        
    void evaluateChanges();
    
    void addCase(Cases::CaseBase* aCase);
    
    void initialize();
    
    
protected:

	Matches::EntityTypeMatch mRootEntityMatch;
	
	Eris::Entity* mEntity;
	
	CaseBaseStore mCases;
};

}

}

}

#endif
