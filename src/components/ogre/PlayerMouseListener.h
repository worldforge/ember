/*
	PlayerMouseLister.h by Miguel Guzman (Aglanor)
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

#ifndef __DimeOgre_PlayerMouseLister_H__
#define __DimeOgre_PlayerMouseLister_H__

// Inherits from InputManager::MouseListener
#include "InputManager.h"
#include "Avatar.h"

class PlayerMouseListener : public InputManager::MouseListener
{
	public:

	~PlayerMouseListener();

	static PlayerMouseListener & getSingleton(void);
	void mouseMoved(short newX, short newY, short oldX, short oldY);
	void mousePressed(unsigned char button);
	void mouseReleased(unsigned char button);

	void setAvatar(Avatar* avatar);
	// TODO: hack... camera won't be here, we'll control player entity instead
	void setCamera(Ogre::Camera* cam);

	private:

	/**
	 * Avatar
	 */
	Avatar* mAvatar;

	// TODO: hack... camera won't be here, we'll control player entity instead
	Ogre::Camera* mCamera;

	/**
	 * Private constructor (for singleton)
	 */
	PlayerMouseListener(void);

	/**
	 * Instance variable for singleton console object implementation.
	 */
    static PlayerMouseListener* _instance;

	/**
	 * Scene manager to which deploy the media
	 */
	Ogre::SceneManager* mSceneMgr;

}; // End of class declaration

#endif
