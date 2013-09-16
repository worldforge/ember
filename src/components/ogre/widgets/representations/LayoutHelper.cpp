//
// C++ Implementation: LayoutHelper
//
// Description: 
//
//
// Author: Martin Preisler <preisler.m@gmail.com>, (C) 2011
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "LayoutHelper.h"
#include "components/ogre/GUIManager.h"
#include <CEGUI/WindowManager.h>
#include <sstream>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Representations {

unsigned int LayoutHelper::mLastPrefixBase = 0;

CEGUI::Window* LayoutHelper::loadLayout(const std::string& layoutFile, CEGUI::String& prefix)
{
	CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();

	std::string finalFileName(GUIManager::getSingleton().getLayoutDir() + layoutFile);
	
	return windowManager.loadLayoutFromFile(finalFileName);
}

}
    
}

}

}
