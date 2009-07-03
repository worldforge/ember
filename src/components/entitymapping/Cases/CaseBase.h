//
// C++ Interface: CaseBase
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
	@brief Base class for all Cases.
	A Case containes zero or many Actions, which will be activated when the Case is activated. A Case also contains zero or many child Matches.
	A Case is activated when it's true and all it's parent cases, all the way up to the root of the ModelMapping, also are true.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class CaseBase 
{
public:
	typedef std::vector<Actions::Action*> ActionStore;
	typedef std::vector<Matches::MatchBase*> MatchBaseStore;

	CaseBase();
	virtual ~CaseBase();
	
	void setParentCase(CaseBase* aCase);

	/**
	 * Returns whether this invidivual case is true. 
	 * @return 
	 */
	bool getIsTrue();
	
	/**
	 *    Returns whether this case is active.
	 * @return 
	 */
	bool getIsActive();
	
	/**
	 * Returns whether this case is true and all of its parent cases, all the way up to the root, also are true.
	 * @return 
	 */
	bool getIsTrueToRoot();
	
	/**
	Adds an Action to this case.
	*/
	void addAction(Actions::Action* action);
	/**
	Gets a list of all the actions contained by this Case.
	*/
	const ActionStore& getActions();
	
	/**
	Activates all actions in the Case. Also switches the mActive flag.
	*/
	void activateActions();
	/**
	Deactivates all actions in the Case. Also switches the mActive flag.
	*/
	void deactivateActions();

	/**
	Adds an child Match to this case.
	*/
	void addMatch(Matches::MatchBase* match);
	/**
	Gets a list of all the Matches contained by this Case.
	*/
	const MatchBaseStore& getMatches();

	/**
	Evaluates all changes, and if a change has occurred, adds itself to the supplied ChangeContext instance.
	*/
	void evaluateChanges(ChangeContext& changeContext);
	
	/**
	Sets the entity that this Case will attach itself to.
	*/
	virtual void setEntity(Eris::Entity* entity);
	
protected:
	ActionStore mActions;

	CaseBase* mParentCase;
	bool mIsTrue, mIsActive;
	
	void setState(bool state);
	
	MatchBaseStore mMatches;
	
};

inline void CaseBase::setState(bool state) 
{
	mIsTrue = state;
}

inline const CaseBase::MatchBaseStore& CaseBase::getMatches() 
{
	return mMatches;
}
	
inline const CaseBase::ActionStore& CaseBase::getActions() 
{ 
	return mActions; 
};

inline void CaseBase::setParentCase(CaseBase* aCase) {
	mParentCase = aCase; 
}

inline bool CaseBase::getIsTrue() 
{ 
	return mIsTrue; 
}

inline bool CaseBase::getIsActive() 
{ 
	return mIsActive;
}

inline bool CaseBase::getIsTrueToRoot() {
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
