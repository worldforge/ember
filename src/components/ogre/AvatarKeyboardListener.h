/*
	AvatarKeyboardListener.h by Miguel Guzman (Aglanor)
	Copyright (C) 2002 Miguel Guzman & The Worldforge Project

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __EmberOgre_AvatarKeyboardListener_H__
#define __EmberOgre_AvatarKeyboardListener_H__

// Inherits from Ogre Keyboard Listener
//#include <OgreEventListeners.h>


// forward declarations
//class Avatar
/*
namespace Ogre
{
	class KeyEvent;
	class Real;
}
*/
//class Ogre::KeyEvent;
//class Ogre::Real;

// the above is broken so we directly include Ogre.h
#include <Ogre.h>
#include <Avatar.h>

class AvatarKeyboardListener
{

	public:

	~AvatarKeyboardListener();

	static AvatarKeyboardListener & getSingleton(void);

	void setAvatar(Avatar* avatar);
	void act(Ogre::Real timePassed);

	private:

	/*
	 * Private constructor (for singleton)
	 */
	AvatarKeyboardListener(void);

	/**
	 * Instance variable for singleton console object implementation.
	 */
    static AvatarKeyboardListener* _instance;

	/**
	 * Avatar
	 */
	Avatar* mAvatar;

}; // End of class declaration

#endif
