/*
    Copyright (C) 2004  Erik Hjortsberg

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

#ifndef AVATARCONTROLLER_H
#define AVATARCONTROLLER_H

//include Ogre stuff
#include <Ogre.h>
#include <OgreFrameListener.h>
#include <OgreInput.h>
#include <OgreStringConverter.h>

class Avatar;
class DebugListener;
#include "InputManager.h"
//class InputManager;
//class InputManager::MouseListener;

class AvatarController : public InputManager::MouseListener
{
public:
	static AvatarController & getSingleton(void);

	virtual ~AvatarController();

	void setAvatar(Avatar* avatar);

	void frameStarted(const Ogre::FrameEvent & event, Ogre::InputReader* inputReader);
//	bool frameEnded(const Ogre::FrameEvent & event);

	//
	//InputManager::MouseListener methods
	void mouseMoved(short newX, short newY, short oldX, short oldY);
	void mousePressed(unsigned char button);
	void mouseReleased(unsigned char button);


protected:

	virtual void checkMouseMovement(const Ogre::FrameEvent & event, Ogre::InputReader* inputReader);
	virtual void checkMovementKeys(const Ogre::FrameEvent & event, Ogre::InputReader* inputReader);
	virtual void checkMouseClicks(const Ogre::FrameEvent & event, Ogre::InputReader* inputReader);

	
private:


	bool mMouseButton0Pressed;
	bool mMouseButton1Pressed;
	bool mMouseButton2Pressed;


	/**
	 * Instance variable for singleton avatar controller implementation.
	 */
    static AvatarController* _instance;
    
    /**
     * determines how often we should update to the server
     */
	float mUpdateInterval;
	
	/**
	 * rest until next update
	 */
	float mTimeToUpdate;
	
	/**
	 * Avatar
	 */
	Avatar* mAvatar;
	
	//======================================================================
    // Disabled constructors and operators
    //======================================================================
 
    /**
     * The constructor is private, and only used to create the singleton instance.
     */
    AvatarController();


    /**
     * Copy constructor not provided.
     */
    AvatarController( const AvatarController &source )
    {
    }


    /**
     * Assignment operator not provided.
     */
    AvatarController &operator= ( const AvatarController &source )
    {
        return *this;
    }
		
};

#endif // AvatarController_H
