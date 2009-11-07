//
// C++ Interface: AbstractMatch
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
#ifndef EMBEROGRE_MODEL_MAPPING_MATCHESABSTRACTMATCH_H
#define EMBEROGRE_MODEL_MAPPING_MATCHESABSTRACTMATCH_H

#include "MatchBase.h"
#include "../ChangeContext.h"
#include "../IVisitor.h"
#include <vector>

namespace Eris
{
class Entity;
}

namespace Ember {



namespace EntityMapping {

namespace Matches {


/**
Utility method for cleaning a std::vector, deleting all instances held.
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
	Base class for all matches which includes templated definitions of the kind of Case it will hold.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
template <class TCase>
class AbstractMatch : public MatchBase
{
public:

	AbstractMatch();

	virtual ~AbstractMatch();

	/**
	* Adds a child case.
	* @param aCase
	*/
	void addCase(TCase* aCase);

	/**
	Gets all child cases.
	*/
	std::vector<TCase*>& getCases();

	 /**
	 Sets the Eris::Entity.
	 */
	virtual void setEntity(Eris::Entity* entity);

	/**
	Evaluate all changes starting from this Match.
	*/
	void evaluateChanges();

	/**
	Evaluate all changes and add changed Cases to the supplied ChangeContext.
	*/
	virtual void evaluateChanges(ChangeContext& changeContext);

	/**
	 * @brief Accepts a visitor.
	 * @param visitor The visitor instance.
	 */
	virtual void accept(IVisitor& visitor);

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

template <class TCase>
void AbstractMatch<TCase>::evaluateChanges(ChangeContext& changeContext) {

	///we want to make sure that we start with deactivating actions, and then after that activate those that should be activated

	typename std::vector<TCase*>::iterator endI = mCases.end();
	for (typename std::vector<TCase*>::iterator I = mCases.begin(); I != endI; ++I) {
		(*I)->evaluateChanges(changeContext);
	}
}

template <class TCase>
void AbstractMatch<TCase>::evaluateChanges() {

	///we want to make sure that we start with deactivating actions, and then after that activate those that should be activated

	ChangeContext changeContext;

	evaluateChanges(changeContext);

	changeContext.performActions();
}

template <class TCase>
void AbstractMatch<TCase>::accept(IVisitor& visitor)
{
	visitor.visit(*this);
	for (typename std::vector<TCase*>::iterator I = mCases.begin(); I != mCases.end(); ++I) {
		(*I)->accept(visitor);
	}
}



}

}

}

#endif
