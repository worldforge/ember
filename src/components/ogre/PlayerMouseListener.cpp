/*
	ConsoleObjectImpl.cpp by Miguel Guzman (Aglanor)
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
#include "PlayerMouseListener.h"

//TODO: add a sequence for mediadeployer ids
//TODO: add several methods for adding media with different params

PlayerMouseListener* PlayerMouseListener::_instance = 0;

PlayerMouseListener & PlayerMouseListener::getSingleton(void)
{
	if(_instance == 0)
		_instance = new PlayerMouseListener;
	return *_instance;
}

PlayerMouseListener::PlayerMouseListener(void)
{
	//mSceneMgr = Ogre::Root::getSingleton().getSceneManager(Ogre::ST_EXTERIOR_CLOSE);
}

PlayerMouseListener::~PlayerMouseListener()
{

}

void PlayerMouseListener::mouseMoved(short newX, short newY, short oldX, short oldY)
{
//	AvatarController::getSingleton().setMouseCoords(newX, newY);		
	
/*	// TODO: very important! Take into account here the screen width and height
	// IF NOT, THE ROTATION WILL DEPEND ON SCREEN SIZE;
	float diffX = -(newX - oldX) * 0.13;
	float diffY = -(newY - oldY) * 0.13;
	//std::cout << "MOVED MOUSE: " << diffX << " " << diffY << std::endl;
	//mCamera->yaw(diffX);
	//mCamera->pitch(diffY);
	 * */
//	mAvatar->rotate(diffX,diffY);
}

void PlayerMouseListener::mousePressed(unsigned char button)
{
	fprintf(stderr, "PRESSED A MOUSE BUTTOn\n");
}

void PlayerMouseListener::mouseReleased(unsigned char button)
{
	fprintf(stderr, "RELEASED A MOUSE BUTTOn\n");
}

void PlayerMouseListener::setAvatar(Avatar* avatar)
{
	mAvatar = avatar;
}

// TODO: enough with this... shall be removed
void PlayerMouseListener::setCamera(Ogre::Camera* cam)
{
	mCamera = cam;
}

