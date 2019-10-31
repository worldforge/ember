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
#include "CEGUIOgreRenderer/Renderer.h"
#include <OgreRenderWindow.h>

namespace Ember {
namespace Cegui {


CEGUI::OgreRenderer& CEGUISetup::createRenderer(Ogre::RenderWindow* renderWindow) {
	auto& configService = EmberServices::getSingleton().getConfigService();
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

}
}