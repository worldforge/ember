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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "OgreWindowProvider.h"
#include "services/input/Input.h"
#include "framework/MainLoopController.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

namespace Ember {

namespace OgreView {

OgreWindowProvider::OgreWindowProvider(Ogre::RenderWindow& window) :
		mWindow(window) {
}

OgreWindowProvider::~OgreWindowProvider() = default;

std::string OgreWindowProvider::getWindowHandle() {
	std::ostringstream winHandleStr;
	size_t winHandle = 0;
	mWindow.getCustomAttribute("WINDOW", &winHandle);

	winHandleStr << winHandle;
	return winHandleStr.str();
}

bool OgreWindowProvider::isWindowVisible() {
	return mWindow.isVisible();
}

void OgreWindowProvider::processInput() {
}

void OgreWindowProvider::getWindowSize(unsigned int& width, unsigned int& height) {
	int left, top;
	mWindow.getMetrics(width, height, left, top);
}

}
}

