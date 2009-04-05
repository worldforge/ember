//
// C++ Interface: Action
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
#ifndef EMBEROGRE_MODEL_MAPPING_ACTIONSACTION_H
#define EMBEROGRE_MODEL_MAPPING_ACTIONSACTION_H

namespace EmberOgre {

namespace Model {

namespace Cases
{
class CaseBase;
}

namespace Mapping {

namespace Actions {

/**
	@brief Base class for actions. Applications are requested to implement their own subclasses.
	
	When an action is activated the model mapping engine will call activate(). If an already activated action is deactivated, as a result of another action being activated, the engine will first call deactivate() on the previous active action.
	By extending this in a subclass and providing your own implementation of activate() and deactivate() you can provide application specific behaviour for entity changes.
	
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class Action 
{
public:
	virtual ~Action() {};
	
	/**
	* Activate the action.
	*/
	virtual void activate() = 0;
	
	/**
	* Deactivate the action.
	*/
	virtual void deactivate() = 0;
	

	/**
	 * Sets the case this action belongs to.
	 * @param ownerCase The owning case.
	 */
	void setCase(Cases::CaseBase* ownerCase);
	
	/**
	 * Gets the case that this action belongs to.
	 * @return The owner case. 
	 */
	Cases::CaseBase* getCase() const;

protected:
	Cases::CaseBase* mOwnerCase;
};

inline void Action::setCase(Cases::CaseBase* ownerCase)
{
	mOwnerCase = ownerCase;
}

inline Cases::CaseBase* Action::getCase() const
{
	return mOwnerCase;
}


}

}

}

}

#endif
