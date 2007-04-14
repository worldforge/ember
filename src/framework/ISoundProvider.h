//
// C++ Interface: ISoundProvider
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
#ifndef EMBERISOUNDPROVIDER_H
#define EMBERISOUNDPROVIDER_H

#include <string>

namespace Ember {

class SoundService;

/**
@author Miguel Guzman
*/
class ISoundProvider{
public:
	
	virtual ~ISoundProvider() {}
	
	/**
	 *    Loads a sound
	 * @param soundName 
	 */
	virtual void loadSound(const std::string& soundName) = 0;
	
	/**
	 *    Returns true if the provider will load the supplied sound name. This is in most cases decided from the filename suffix.
	 * @param soundName 
	 * @return 
	 */
	// TODO: perhaps the Sound Provider doesn't need this
	virtual bool willLoadSound(const std::string& soundName) = 0;
	
	/**
	 *    Gets the unique name of the sound provider.
	 * @return 
	 */
	virtual const std::string& getName() const = 0;
	
	/**
	 *    Register with  a service to allow for callbacks etc.
	 * @param service 
	 */
	virtual void _registerWithService(SoundService* service) = 0;

};

}

#endif
