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

#include "DimeEventProcessor.h"

DimeEventProcessor::DimeEventProcessor() : Ogre::EventProcessor()
{
	mUseMouse = false;
}


void DimeEventProcessor::initialise(Ogre::RenderWindow* ren)
{
	mWin = ren;
	
	cleanup();
	 
 
	mEventQueue = new Ogre::EventQueue();
 
 	createInputReader(mUseMouse);
 
}

void DimeEventProcessor::toggleMouse()
{
	mUseMouse =! mUseMouse;
	Ogre::PlatformManager::getSingleton().destroyInputReader(mInputDevice);
	createInputReader(mUseMouse);
//	mInputReader->initialise(Ogre::Root::getSingleton().getAutoCreatedWindow(), true, false);

}

bool DimeEventProcessor::isMouseUsed()
{
	return mUseMouse;
}

void DimeEventProcessor::createInputReader(bool useMouse) 
{
		mInputDevice = Ogre::PlatformManager::getSingleton().createInputReader();
		mInputDevice->useBufferedInput(mEventQueue, true, useMouse);
		mInputDevice->initialise(mWin, true, useMouse);
}


//DimeEventProcessor::~DimeEventProcessor()
//{}
