//
// C++ Interface: AbstractMatch
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
#ifndef EMBEROGRE_MODEL_MAPPING_MATCHESABSTRACTMATCH_H
#define EMBEROGRE_MODEL_MAPPING_MATCHESABSTRACTMATCH_H

#include "MatchBase.h"
#include <vector>

namespace Eris
{
class Entity;
}

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Matches {


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
template <class TCase>
class AbstractMatch : public MatchBase
{
public:

AbstractMatch();

virtual ~AbstractMatch(); 

/*	AbstractMatch();
	virtual ~AbstractMatch();*/
	void addCase(TCase* aCase);
	std::vector<TCase*>& getCases(); 
	virtual void setEntity(Eris::Entity* entity);
	
	
protected:
	 std::vector<TCase*> mCases;
};

template <class TCase>
AbstractMatch<TCase>::AbstractMatch()
{
}

template <class TCase>
AbstractMatch<TCase>::~AbstractMatch() 
{
	cleanVector(mCases);
}


template <class TCase>
void AbstractMatch<TCase>::setEntity(Eris::Entity* entity)
{
	typename std::vector< TCase* >::iterator I = mCases.begin();
	for ( ; I != mCases.end(); ++I) {
		(*I)->setEntity(entity);
	}
}
	
template <class TCase>
std::vector<TCase*>& AbstractMatch<TCase>::getCases()
{ 
	return mCases;
}

template <class TCase>
void AbstractMatch<TCase>::addCase(TCase* aCase) {
	mCases.push_back(aCase); 
	aCase->setParentCase(mParentCase);
}

}

}

}

}

#endif
