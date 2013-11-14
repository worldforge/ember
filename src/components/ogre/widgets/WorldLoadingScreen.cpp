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
#include "../GUIManager.h"
#include "../EmberOgre.h"

#include <sigc++/sigc++.h>
#include <CEGUI/Window.h>
#include <CEGUI/WindowManager.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

WorldLoadingScreen::WorldLoadingScreen()
{

	/*
	 * Get Everything setup
	 */

	// Black background with white text
	mLoadingWindow = CEGUI::WindowManager::getSingleton().createWindow("EmberLook/StaticText", "WorldLoadingScreen");
//	mLoadingWindow->setProperty("BackgroundColours","tl:00AAAAAA tr:00AAAAAA bl:00AAAAAA br:00AAAAAA");
	mLoadingWindow->setProperty("BackgroundColours", "FFFFFF");
	mLoadingWindow->setProperty("TextColours", "FFFFFFFF");
	mLoadingWindow->setProperty("BackgroundEnabled", "true");
	mLoadingWindow->setHorizontalAlignment(CEGUI::HorizontalAlignment::HA_CENTRE);
	mLoadingWindow->setVerticalAlignment(CEGUI::VerticalAlignment::VA_CENTRE);
	mLoadingWindow->setAlwaysOnTop(true);
	mLoadingWindow->setEnabled(true);
	mLoadingWindow->setFont("DejaVuSans-14");
	mLoadingWindow->setProperty("HorzFormatting", "CentreAligned");
//	// Defaults for CEGUI
//	mLoadingWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0.0f), CEGUI::UDim(0.0f, 0.0f)));
//	mLoadingWindow->setSize(CEGUI::USize(CEGUI::UDim(1.0f, 0), CEGUI::UDim(1.0f, 0)));
	mLoadingWindow->setText("Entering world, please wait...");

	// Same EmberLook feel as a window
//	mLoadingWindow = CEGUI::WindowManager::getSingleton().createWindow("EmberLook/FrameWindow", "WorldLoadingScreen");
//	mLoadingWindow->setAlwaysOnTop(true);
//	mLoadingWindow->setEnabled(true);
//	mLoadingWindow->setPosition( CEGUI::UVector2( CEGUI::UDim( 0.0f, 0.0f ), CEGUI::UDim( 0.0f, 0.0f ) ) );
//	mLoadingWindow->setSize( CEGUI::USize( CEGUI::UDim( 1.0f, 0 ), CEGUI::UDim( 1.0f, 0 ) ) );
//	mLoadingWindow->setText("Loading world, please wait ...");

	EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(sigc::hide(sigc::mem_fun(*this, &Ember::OgreView::Gui::WorldLoadingScreen::hideScreen)));
	EmberOgre::getSingleton().EventWorldCreated.connect(sigc::hide(sigc::mem_fun(*this, &Ember::OgreView::Gui::WorldLoadingScreen::showScreen)));

}

WorldLoadingScreen::~WorldLoadingScreen()
{
	CEGUI::WindowManager::getSingleton().destroyWindow(mLoadingWindow);
}

CEGUI::Window& WorldLoadingScreen::getWindow()
{
	return *mLoadingWindow;
}

void WorldLoadingScreen::showScreen()
{
	/*
	 * Add to the main sheet.  This is "turning on" the load screen
	 */
	GUIManager::getSingleton().getMainSheet()->addChild(mLoadingWindow);
}

void WorldLoadingScreen::hideScreen()
{
	/*
	 * Remove from the main sheet.  This is "turning off" the load screen
	 */
	GUIManager::getSingleton().getMainSheet()->removeChild(mLoadingWindow);
}

} // end namespace Gui
} // end namespace OgreView
} // end namespace Ember

