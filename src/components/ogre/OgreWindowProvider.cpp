/*
 * Copyright (C) 2012 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "OgreWindowProvider.h"
#include "services/input/Input.h"
#include "framework/MainLoopController.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

namespace Ember
{

namespace OgreView
{

OgreWindowProvider::OgreWindowProvider(Ogre::RenderWindow& window) :
		mWindow(window)
{
	Ogre::WindowEventUtilities::addWindowEventListener(&mWindow, this);

}

OgreWindowProvider::~OgreWindowProvider()
{
	Ogre::WindowEventUtilities::removeWindowEventListener(&mWindow, this);
}

std::string OgreWindowProvider::getWindowHandle()
{
	std::ostringstream winHandleStr;
	size_t winHandle = 0;
	mWindow.getCustomAttribute("WINDOW", &winHandle);

	winHandleStr << winHandle;
	return winHandleStr.str();
}

bool OgreWindowProvider::isWindowVisible()
{
	return mWindow.isVisible();
}

void OgreWindowProvider::processInput()
{
	Ogre::WindowEventUtilities::messagePump();
}

void OgreWindowProvider::getWindowSize(unsigned int& width, unsigned int& height)
{
	unsigned int depth;
	int left, top;
	mWindow.getMetrics(width, height, depth, left, top);
}

void OgreWindowProvider::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height;
	getWindowSize(width, height);
	Input::getSingleton().setGeometry(width, height);
}

bool OgreWindowProvider::windowClosing(Ogre::RenderWindow* rw)
{
	MainLoopController::getSingleton().requestQuit();

	//returning false means the window should not be destroyed.
	return false;
}

void OgreWindowProvider::windowClosed(Ogre::RenderWindow* rw)
{
	//last chance to detach OIS from the window.
	Input::getSingleton().shutdownInteraction();
}

void OgreWindowProvider::windowFocusChange(Ogre::RenderWindow* rw)
{
	Input& input = Input::getSingleton();
	input.setInputMode(Input::IM_GUI);
	input.setMouseGrab(false);
}
}
}

