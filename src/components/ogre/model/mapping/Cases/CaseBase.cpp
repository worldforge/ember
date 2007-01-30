//
// C++ Implementation: CaseBase
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
#include "CaseBase.h"

#include "../ModelMapping.h"
#include "../Actions/Action.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Cases {

CaseBase::CaseBase() 
: mParentCase(0), mIsActive(false), mIsTrue(false) 
{
}

CaseBase::~CaseBase() {
	Mapping::cleanVector(mActions);
	Mapping::cleanVector(mMatches);
}

void CaseBase::addAction(Actions::Action* action) { 
	mActions.push_back(action);
	action->setCase(this);
}

void CaseBase::evaluateChanges(std::vector<CaseBase*>& activateQueue, std::vector<CaseBase*>& deactivateQueue) 
{
	if (getIsTrueToRoot()) {
		if (!getIsActive()) {
			activateQueue.push_back(this);
// 			activateActions();
		}
	} else {
		if (getIsActive()) {
			deactivateQueue.push_back(this);
// 			deactivateActions();
		}
	}
}
void CaseBase::activateActions()
{
	for (ActionStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
		(*I)->activate();
	}
	mIsActive = true;
}
void CaseBase::deactivateActions()
{
	for (ActionStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
		(*I)->deactivate();
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

}
