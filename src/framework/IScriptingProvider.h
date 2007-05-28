//
// C++ Interface: IScriptingProvider
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#ifndef EMBERISCRIPTINGPROVIDER_H
#define EMBERISCRIPTINGPROVIDER_H

#include <string>

#include "IResourceProvider.h"

namespace Ember {

class ScriptingService;

/**
@author Erik Hjortsberg
*/
class IScriptingProvider{
public:
	
	virtual ~IScriptingProvider(){}

	/**
	 *    Loads the script.
	 * @param scriptName 
	 */
	virtual void loadScript(ResourceWrapper& resourceWrapper) = 0;
	
	/**
	 *    Executes the supplied string directly into the scripting environment.
	 * @param scriptCode 
	 */
	virtual void executeScript(const std::string& scriptCode) = 0;
	
	
	/**
	 *    Returns true if the provider will load the supplied script name. This is in most cases decided from the filename suffix.
	 * @param scriptName 
	 * @return 
	 */
	virtual bool willLoadScript(const std::string& scriptName) = 0;
	
	/**
	 *    Gets the unique name of the scripting provider.
	 * @return 
	 */
	virtual const std::string& getName() const = 0;
	
	/**
	 *    Register with  a service to allow for callbacks etc.
	 * @param service 
	 */
	virtual void _registerWithService(ScriptingService* service) = 0;
	
	/**
	 *    Forces a full garbage collection.
	 */
	virtual void forceGC() = 0;

};

}

#endif
