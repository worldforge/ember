//
// C++ Interface: CaseBase
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
#ifndef EMBEROGRE_MODEL_MAPPING_CASESCASEBASE_H
#define EMBEROGRE_MODEL_MAPPING_CASESCASEBASE_H

#include <vector>
#include "../ChangeContext.h"

namespace Eris {
class Entity;
}


namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Actions {
class Action;
}

namespace Matches {
class MatchBase;
}

namespace Cases {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class CaseBase 
{
public:
	typedef std::vector<Actions::Action*> ActionStore;
	typedef std::vector<Matches::MatchBase*> MatchBaseStore;

	CaseBase();
	virtual ~CaseBase();
	
	inline void setParentCase(CaseBase* aCase);

	inline bool getIsTrue();
	inline bool getIsActive();
	inline bool getIsTrueToRoot();
	
	void addAction(Actions::Action* action);
	inline const ActionStore& getActions();
	
	void activateActions();
	void deactivateActions();

	void addMatch(Matches::MatchBase* match);
	inline const MatchBaseStore& getMatches();

	void evaluateChanges(ChangeContext& changeContext);
	
	virtual void setEntity(Eris::Entity* entity);
	
protected:
	ActionStore mActions;

	CaseBase* mParentCase;
	bool mIsTrue, mIsActive;
	
	inline void setState(bool state);
	
	MatchBaseStore mMatches;
	
};

void CaseBase::setState(bool state) 
{
	mIsTrue = state;
}

const CaseBase::MatchBaseStore& CaseBase::getMatches() 
{
	return mMatches;
}
	
const CaseBase::ActionStore& CaseBase::getActions() 
{ 
	return mActions; 
};

void CaseBase::setParentCase(CaseBase* aCase) {
	mParentCase = aCase; 
}

bool CaseBase::getIsTrue() 
{ 
	return mIsTrue; 
}

bool CaseBase::getIsActive() 
{ 
	return mIsActive;
}

bool CaseBase::getIsTrueToRoot() {
	if (!mParentCase) {
		return mIsTrue;
	} else {
		return mIsTrue && mParentCase->getIsTrueToRoot();
	}
}
	

}

}

}

}

#endif
