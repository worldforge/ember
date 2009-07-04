//
// C++ Interface: ChangeContext
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
#ifndef EMBEROGRE_MODEL_MAPPINGCHANGECONTEXT_H
#define EMBEROGRE_MODEL_MAPPINGCHANGECONTEXT_H
#include <vector>

namespace Ember {



namespace EntityMapping {

namespace Cases {
class CaseBase;
}

/**
	Whenever something has changed in the values that affects the Model, cases will need to be reevaluated and actions may be activated or deactivated. A ChangeContext wraps these events into one class. This is an internal class which will be created and acted upon by the mapping framework.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class ChangeContext
{
public:

	typedef std::vector<Cases::CaseBase*> CaseStore;

    ChangeContext();

    ~ChangeContext();

	/**
	* Adds a Case that needs to be activated to the context. Activation will happen upon calls to performActions().
	* @param aCase
	*/
	void addCaseToActivate(Cases::CaseBase* aCase);
	/**
	* Adds a Case that needs to be deactivated to the context. Deactivation will happen upon calls to performActions().
	* @param aCase
	*/
	void addCaseToDeactivate(Cases::CaseBase* aCase);

	/**
	* Performs the actions that should be activated and deactivated.
	*/
	void performActions();

protected:

/**
A store of those cases that should be activated.
*/
CaseStore mActivateQueue;

/**
A store of those cases that should be deactivated.
*/
CaseStore mDeactivateQueue;

};

}

}

#endif
