/*
 Copyright (C) 2017 Erik Ogenvik

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

#include "CEGUISetup.h"

#ifdef _WIN32
#include "platform/platform_windows.h"
#endif

#include "framework/LoggingInstance.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/input/Input.h"
#include "CEGUIOgreRenderer/Renderer.h"
#include "CEGUIOgreRenderer/ResourceProvider.h"
#include "CEGUIOgreRenderer/ImageCodec.h"
#include "CEGUILogger.h"
#include <OgreRenderWindow.h>
#include <CEGUI/System.h>
#include <CEGUI/SchemeManager.h>
#include <framework/Exception.h>
#include <CEGUITinyXMLParser/XMLParser.h>

namespace Ember::Cegui {


CEGUI::OgreRenderer& CEGUISetup::createRenderer(Ogre::RenderWindow* renderWindow) {
	auto& configService = ConfigService::getSingleton();
	if (!configService.getPrefix().empty()) {

#ifndef _WIN32
		//The environment variable CEGUI_MODULE_DIR points to where CEGUI can find its modules. Lets check if they are under the prefix,
		//for relocatable packages. We need to check both "lib64" and "lib".
		std::string modulePath = configService.getPrefix() + "/lib64/cegui-0.8";
		if (std::ifstream(modulePath).good()) {
			setenv("CEGUI_MODULE_DIR", modulePath.c_str(), 1);
			S_LOG_INFO("Setting CEGUI_MODULE_DIR to " << modulePath);
		} else {
			modulePath = configService.getPrefix() + "/lib/cegui-0.8";
			if (std::ifstream(modulePath).good()) {
				setenv("CEGUI_MODULE_DIR", modulePath.c_str(), 1);
				S_LOG_INFO("Setting CEGUI_MODULE_DIR to " << modulePath);
			}
		}
#endif
		//We need to set the current directory to the prefix before trying to load CEGUI.
		if (chdir(configService.getPrefix().c_str())) {
			S_LOG_WARNING("Failed to change to the prefix directory '" << configService.getPrefix() << "'. Gui loading might fail.");
		}
	}

	return CEGUI::OgreRenderer::create(*renderWindow);
}

CEGUISetup::CEGUISetup(Ogre::RenderWindow& window)
		: mCEGUILogger(std::make_unique<Cegui::CEGUILogger>()),
		  mWindow(window) {
//Check that CEGUI is built with Freetype support. If not you'll get a compilation error here.
#ifndef CEGUI_HAS_FREETYPE
	CEGUI is not built with Freetype
#endif

	//The OgreCEGUIRenderer is the main interface between Ogre and CEGUI.
	mGuiRenderer = &Ember::Cegui::CEGUISetup::createRenderer(&window);

	//We'll do our own rendering, interleaved with Ogre's, so we'll turn off the automatic rendering.
	mGuiRenderer->setRenderingEnabled(false);
	mGuiRenderer->setFrameControlExecutionEnabled(false);

	mOgreResourceProvider = &CEGUI::OgreRenderer::createOgreResourceProvider();
	mOgreResourceProvider->setDefaultResourceGroup("UI");

	mOgreImageCodec = &CEGUI::OgreRenderer::createOgreImageCodec();

	mXmlParser = std::make_unique<CEGUI::TinyXMLParser>();

	mGuiSystem = &CEGUI::System::create(*mGuiRenderer, mOgreResourceProvider, mXmlParser.get(), mOgreImageCodec, nullptr, "cegui/datafiles/configs/cegui.config");
	auto schemeI = CEGUI::SchemeManager::getSingleton().getIterator();
	if (schemeI.isAtEnd()) {
		// 			S_LOG_FAILURE("Could not load any CEGUI schemes. This means that there's something wrong with how CEGUI is setup. Check the CEGUI log for more detail. We'll now exit Ember.");
		throw Exception("Could not load any CEGUI schemes. This means that there's something wrong with how CEGUI is setup. Check the CEGUI log for more detail. We'll now exit Ember.");
	}

	Input::getSingleton().EventSizeChanged.connect([this](int width, int height) { mGuiSystem->notifyDisplaySizeChanged(CEGUI::Sizef((float)width, (float)height)); });


}

CEGUISetup::~CEGUISetup() {
	CEGUI::System::destroy();
	if (mOgreResourceProvider) {
		CEGUI::OgreRenderer::destroyOgreResourceProvider(*mOgreResourceProvider);
	}
	if (mOgreImageCodec) {
		CEGUI::OgreRenderer::destroyOgreImageCodec(*mOgreImageCodec);
	}
	if (mGuiRenderer) {
		CEGUI::OgreRenderer::destroy(*mGuiRenderer);
	}
}

}