//
// C++ Implementation: CaseBase
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CaseBase.h"

#include "../EntityMapping.h"
#include "../Actions/Action.h"
#include "../IVisitor.h"

namespace Ember {



namespace EntityMapping {

namespace Cases {

CaseBase::CaseBase()
: mParentCase(0), mIsTrue(false), mIsActive(false)
{
}

CaseBase::~CaseBase() {
	::Ember::EntityMapping::cleanVector(mActions);
	::Ember::EntityMapping::cleanVector(mMatches);
}

void CaseBase::addAction(Actions::Action* action) {
	mActions.push_back(action);
	action->setCase(this);
}

void CaseBase::evaluateChanges(ChangeContext& changeContext)
{
	///if we're true to the root, but not yet active, add ourself to the activation queue. If the opposite, i.e. we're no longer true to the root but active, we need to be deactivated.
	if (getIsTrueToRoot()) {
		if (!getIsActive()) {
			changeContext.addCaseToActivate(this);
		}
	} else {
		if (getIsActive()) {
			changeContext.addCaseToDeactivate(this);
		}
	}
	///recursively iterate over the child matches
	MatchBaseStore::iterator I = mMatches.begin();
	for ( ; I != mMatches.end(); ++I) {
		(*I)->evaluateChanges(changeContext);
	}

}

void CaseBase::accept(IVisitor& visitor)
{
	visitor.visit(*this);
	for (ActionStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
		(*I)->accept(visitor);
	}
	for (MatchBaseStore::iterator I = mMatches.begin(); I != mMatches.end(); ++I) {
		(*I)->accept(visitor);
	}
}

void CaseBase::activateActions(ChangeContext& context)
{
	for (ActionStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
		(*I)->activate(context);
	}
	mIsActive = true;
}
void CaseBase::deactivateActions(ChangeContext& context)
{
	for (ActionStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
		(*I)->deactivate(context);
	}
	mIsActive = false;
}

void CaseBase::addMatch(Matches::MatchBase* match) {
	mMatches.push_back(match);
}

void CaseBase::setEntity(Eris::Entity* entity)
{
	MatchBaseStore::iterator I = mMatches.begin();
	for ( ; I != mMatches.end(); ++I) {
		(*I)->setEntity(entity);
	}
}


}

}

}
