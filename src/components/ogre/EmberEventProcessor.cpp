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

#include <time.h>

#include <SDL.h>


#include "EmberEventProcessor.h"
namespace EmberOgre {


EmberEventProcessor::EmberEventProcessor()
{
/*	mUseMouse = false;*/
}


// void EmberEventProcessor::initialise(Ogre::RenderWindow* ren)
// {
// 	//return Ogre::EventProcessor::initialise(ren);
// 	mWin = ren;
// 	
// 	cleanup();
// 	 
//  
// 	mEventQueue = new Ogre::EventQueue();
//  	SDL_ShowCursor(0);
//  	createInputReader(mUseMouse);
// 	//addMouseListener(this);
//  
// }
// 
// void EmberEventProcessor::toggleMouse()
// {
// 	mUseMouse =! mUseMouse;
// 	Ogre::PlatformManager::getSingleton().destroyInputReader(mInputDevice);
// 	createInputReader(mUseMouse);
// //	mInputReader->initialise(Ogre::Root::getSingleton().getAutoCreatedWindow(), true, false);
// 
// }
// 
// void EmberEventProcessor::mousePressed (Ogre::MouseEvent *e)
// {
// 	if (!mUseMouse)
// 		toggleMouse();
// 	//removeMouseListener(this);
// }
// 
// 
// bool EmberEventProcessor::isMouseUsed()
// {
// 	return mUseMouse;
// }
// 
// void EmberEventProcessor::createInputReader(bool useMouse) 
// {
// 		mInputDevice = Ogre::PlatformManager::getSingleton().createInputReader();
// 
// 		//mInputDevice->useBufferedInput(mEventQueue, true, false);
// 		mInputDevice->initialise(mWin, true, useMouse);
// }

}


//EmberEventProcessor::~EmberEventProcessor()
//{}
