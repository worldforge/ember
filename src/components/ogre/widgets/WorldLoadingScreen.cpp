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


#include "WorldLoadingScreen.h"

namespace Ember
{
namespace OgreView
{
namespace Gui
{

WorldLoadingScreen::WorldLoadingScreen()
{

	/*
	 * Main window, larger than screen to take up the full viewport
	 * NOTE: could not get a regular window to display properly so used
	 *       a frame window.  It should not be so obfuscated on how to put up
	 *       a simple window.  This is terrible.
	 */
//	mLoadingWindow = CEGUI::WindowManager::getSingleton().createWindow("Defaultwindow", "WorldLoadingScreen");
	mLoadingWindow = CEGUI::WindowManager::getSingleton().createWindow("EmberLook/FrameWindow", "WorldLoadingScreen");
	mLoadingWindow->setAlwaysOnTop(true);
	mLoadingWindow->setEnabled(true);
	mLoadingWindow->setPosition( CEGUI::UVector2( CEGUI::UDim( -0.2f, 0.0f ), CEGUI::UDim( -0.2f, 0.0f ) ) );
	mLoadingWindow->setSize( CEGUI::USize( CEGUI::UDim( 1.25f, 0 ), CEGUI::UDim( 1.25f, 0 ) ) );
	mLoadingWindow->setText("Loading world, please wait ...");

	/*
	 * Text, approximately centered
	 */
	mWindowTip = CEGUI::WindowManager::getSingleton().createWindow("EmberLook/StaticText", "WLSMessage");
	mWindowTip->setPosition( CEGUI::UVector2( CEGUI::UDim( 0.4f, 0.0f ), CEGUI::UDim( 0.4f, 0.0f ) ) );
	mWindowTip->setSize( CEGUI::USize( CEGUI::UDim( 0.25f, 0.0f ), CEGUI::UDim( 0.25f, 0.0f ) ) );
	mWindowTip->setFont("DejaVuSans-14");
	mWindowTip->setText("Loading world, please wait ...");

	mLoadingWindow->addChild(mWindowTip);

}

WorldLoadingScreen::~WorldLoadingScreen()
{
	delete mWindowTip;
	delete mLoadingWindow;
}


} // end namespace Gui
} // end namespace OgreView
} // end namespace Ember

