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

#include "EmberEventProcessor.h"
namespace EmberOgre {


EmberEventProcessor::EmberEventProcessor() : Ogre::EventProcessor()
{
	mUseMouse = false;
}


void EmberEventProcessor::initialise(Ogre::RenderWindow* ren)
{
	return Ogre::EventProcessor::initialise(ren);
	mWin = ren;
	
	cleanup();
	 
 
	mEventQueue = new Ogre::EventQueue();
 
 	createInputReader(mUseMouse);
	addMouseListener(this);
 
}

void EmberEventProcessor::toggleMouse()
{
	mUseMouse =! mUseMouse;
	Ogre::PlatformManager::getSingleton().destroyInputReader(mInputDevice);
	createInputReader(mUseMouse);
//	mInputReader->initialise(Ogre::Root::getSingleton().getAutoCreatedWindow(), true, false);

}

void EmberEventProcessor::mousePressed (Ogre::MouseEvent *e)
{
	if (!mUseMouse)
		toggleMouse();
	//removeMouseListener(this);
}


// std::string EmberEventProcessor::takeScreenshot() 
// {
// 	// retrieve current time
// 	time_t rawtime;
// 	struct tm* timeinfo;
// 	time(&rawtime);
// 	timeinfo = localtime(&rawtime);
// 	
// 	// construct filename string
// 	// padding with 0 for single-digit values
// 	std::stringstream filename;
// 	filename << "screenshot_" << ((*timeinfo).tm_year + 1900); // 1900 is year "0"
// 	int month = ((*timeinfo).tm_mon + 1); // January is month "0"
// 	if(month <= 9) 
// 	{
// 		filename << "0";	
// 	}
// 	filename << month;
// 	int day = (*timeinfo).tm_mday;
// 	if(day <= 9) 
// 	{
// 		filename << "0";	
// 	}
// 	filename << day << "_";
// 	int hour = (*timeinfo).tm_hour;
// 	if(hour <= 9) 
// 	{
// 		filename << "0"; 
// 	}
// 	filename << hour;
// 	int min = (*timeinfo).tm_min;
// 	if(min <= 9) 
// 	{
// 		filename << "0";	 
// 	}
// 	filename << min;
// 	int sec = (*timeinfo).tm_sec;
// 	if(sec <= 9) 
// 	{
// 		filename << "0";
// 	} 
// 	filename << sec << ".png";
// 	
// 	// take screenshot
// 	mWin->writeContentsToFile(filename.str());
// 	mWin->setDebugText(std::string("Screenshot: ") + filename.str());
// }

bool EmberEventProcessor::isMouseUsed()
{
	return mUseMouse;
}

void EmberEventProcessor::createInputReader(bool useMouse) 
{
		mInputDevice = Ogre::PlatformManager::getSingleton().createInputReader();

		mInputDevice->useBufferedInput(mEventQueue, true, true);
		mInputDevice->initialise(mWin, true, useMouse);
}

}


//EmberEventProcessor::~EmberEventProcessor()
//{}
