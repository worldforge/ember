/*
	AvatarKeyboardListener.cpp by Miguel Guzman (Aglanor)
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

// config headers
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// system headers

// library headers
#include <Ogre.h>

// local headers
#include "AvatarKeyboardListener.h"
#include "InputManager.h"

//TODO: add a sequence for mediadeployer ids
//TODO: add several methods for adding media with different params

AvatarKeyboardListener* AvatarKeyboardListener::_instance = 0;

AvatarKeyboardListener& AvatarKeyboardListener::getSingleton(void)
{
	if(_instance == 0)
		_instance = new AvatarKeyboardListener;
	return *_instance;
}

AvatarKeyboardListener::AvatarKeyboardListener(void)
{
	//mSceneMgr = Ogre::Root::getSingleton().getSceneManager(Ogre::ST_EXTERIOR_CLOSE);
}

AvatarKeyboardListener::~AvatarKeyboardListener()
{

}


// TODO: hack... camera won't be here, we'll control player entity instead
void AvatarKeyboardListener::setAvatar(Avatar* avatar)
{
	mAvatar = avatar;
}

void AvatarKeyboardListener::act(Ogre::Real timePassed) // why was this const under DIE???
{
		Ogre::Vector3 movement = Ogre::Vector3::ZERO;
		bool send = false; // do this checking against zero or something

		// forwards / backwards
		if(InputManager::getSingleton().isKeyDown(Ogre::KC_UP))  // W also, and same for the rest
		{
			movement.z = -1; 	//scale this
			send = true;
		}
		else if(InputManager::getSingleton().isKeyDown(Ogre::KC_DOWN))
		{
			movement.z = 1;		//scale
			send = true;
		}

		// strafe
		if(InputManager::getSingleton().isKeyDown(Ogre::KC_LEFT))
		{
			movement.x = -1;
			send = true;
		}
		else if(InputManager::getSingleton().isKeyDown(Ogre::KC_RIGHT))
		{
			movement.x = 1;
			send = true;
		}

		if(send)
		{
			mAvatar->move(movement);
		}

		//else
				//mCar->setDrive(Car::DO_NONE);

				/*

		else
				mCar->setSteer(0);
				*/

}


