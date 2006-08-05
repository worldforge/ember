//
// C++ Interface: Sun
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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
#ifndef DIMEOGRESUN_H
#define DIMEOGRESUN_H

#include "components/ogre/EmberOgrePrerequisites.h"


#include <sigc++/signal.h>

#include "framework/ConsoleObject.h"

namespace EmberOgre {

/**
@author Erik Hjortsberg
*/
class Sun : public Ember::ConsoleObject
{
public:
    Sun(Ogre::Camera* camera, Ogre::SceneManager* sceneMgr);

    ~Sun();
	
	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command 
	 * @param args 
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);
	
	/**
	* emitted when the sun changes position
	*/
	sigc::signal<void, Sun*, Ogre::Vector3> EventUpdatedSunPosition;
	
	/**
	* emitted when the sun changes colour
	*/
	sigc::signal<void, Sun*, Ogre::ColourValue> EventUpdatedSunColour;
	
	/**
	* emitted when the world ambient light is changed
	*/
	sigc::signal<void, Sun*, Ogre::ColourValue> EventUpdatedAmbientLight;
	
	const Ember::ConsoleCommandWrapper SetSunPosition;
	const Ember::ConsoleCommandWrapper SetSunColour;
	const Ember::ConsoleCommandWrapper SetAmbientLight;
	
	
	/**
	 *    changes the sun's position
	 * @param position a new world position
	 */
	void setSunPosition(const Ogre::Vector3& position);
	
	
	/**
	 *    changes the colour of the sun light
	 * @param colour 
	 */
	void setSunColour(const Ogre::ColourValue& colour);
	
	
	/**
	 *    changes the ambient light
	 * @param colour 
	 */
	void setAmbientLight(const Ogre::ColourValue& colour);
	
protected:
	Ogre::Light* mSun;
	
	Ogre::SceneNode* mSunNode;

};

};

#endif
