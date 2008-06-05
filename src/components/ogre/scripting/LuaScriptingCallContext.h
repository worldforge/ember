//
// C++ Interface: LuaScriptingCallContext
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
#ifndef EMBEROGRELUASCRIPTINGCALLCONTEXT_H
#define EMBEROGRELUASCRIPTINGCALLCONTEXT_H

#include "luaobject.h"
#include "framework/IScriptingProvider.h"

namespace EmberOgre {

/**
@brief A scripting call context for lua scripts.
Whenever you want to be able to inspect return values from calling lua scripts, you should use an instance of this class and submit it when calling executeCode(...).
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class LuaScriptingCallContext : public Ember::IScriptingCallContext
{
friend class LuaScriptingProvider;
public:
	LuaScriptingCallContext();
	virtual ~LuaScriptingCallContext();
	/**
	 *    Gets the return value of the lua call.
	 * @return 
	 */
	inline LuaRef* getReturnValue();
protected:
	/**
	 *    Sets the return value of the lua call. This is an internal method used only by the lua scripting provider.
	 * @param returnValue 
	 */
	inline void setReturnValue(LuaRef* returnValue);
	
	/**
	The return value from the lua call.
	*/
	LuaRef* mReturnValue;
};

void LuaScriptingCallContext::setReturnValue(LuaRef* returnValue)
{
	delete returnValue;
	mReturnValue = returnValue;
}

LuaRef* LuaScriptingCallContext::getReturnValue()
{
	return mReturnValue;
}

}

#endif
