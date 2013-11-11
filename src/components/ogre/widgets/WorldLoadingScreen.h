/*
 Copyright (C) 2013 Sean Ryan <sryan@evercrack.com>

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

#ifndef WORLDLOADINGSCREEN_H_
#define WORLDLOADINGSCREEN_H_

#include <string>
#include <CEGUI/Window.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Image.h>
#include <CEGUI/BasicImage.h>
#include <CEGUI/ColourRect.h>

namespace Ember
{
namespace OgreView
{

class WorldLoadingScreen
{
public:
	WorldLoadingScreen();
	~WorldLoadingScreen();

	CEGUI::Window& getWindow() { return *mLoadingWindow; }
	CEGUI::Window* getWindowPtr() { return mLoadingWindow; }

protected:

	CEGUI::Window* mLoadingWindow;
	CEGUI::ColourRect* mColourArea;
	CEGUI::Window* mWindowTip;

};

}
}
#endif /* WORLDWORLDLOADINGSCREEN_H_ */
