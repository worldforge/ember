//
// C++ Interface: OgreSoundProvider
//
// Description: 
//
//
// Author: Miguel Guzman <aglanor@gmail.com>, (C) 2006
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
#ifndef EMBEROGRESOUNDPROVIDER_H
#define EMBEROGRESOUNDPROVIDER_H

#include "framework/ISoundProvider.h"

namespace Ember
{
	class SoundService;
}

namespace EmberOgre {

/**
@author Miguel Guzman
*/
class OgreSoundProvider : public Ember::ISoundProvider
{
public:
    OgreSoundProvider();

    ~OgreSoundProvider();

	/**
	 *    Loads the script.
	 * @param soundName 
	 */
	virtual void loadSound(const std::string& scriptName);
	
	/**
	 *    Returns true if the provider will load the supplied script name. This is in most cases decided from the filename suffix.
	 * @param scriptName 
	 * @return 
	 */

	virtual bool willLoadSound(const std::string& scriptName);


	/**
	 *    Gets the unique name of the sound provider.
	 * @return 
	 */
	virtual const std::string& getName() const;
	
	/**
	 *    Register with  a service to allow for callbacks etc.
	 * @param service 
	 */
	virtual void _registerWithService(Ember::SoundService* service);
	
	// TODO: here go non-interface methods

private:

	Ember::SoundService* service; 
	
	
	
};

}

#endif
