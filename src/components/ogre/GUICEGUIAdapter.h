//
// C++ Interface: GUICEGUIAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
#ifndef EMBEROGREGUICEGUIADAPTER_H
#define EMBEROGREGUICEGUIADAPTER_H

#include "EmberOgrePrerequisites.h"
#include <CEGUI/System.h>
#include <CEGUI/EventArgs.h>
#include <CEGUI/InputEvent.h> 

#include "services/input/IInputAdapter.h"
#include "services/input/Input.h"

#include <map>

namespace CEGUI {
class System;
class OgreRenderer;

}

namespace Ember {
namespace OgreView {

typedef std::map<SDLKey, CEGUI::Key::Scan> SDLKeyMap;

/**
 * @author Erik Hjortsberg
 *
 * @brief Provides an adapter between the input system and CEGUI.
 */
class GUICEGUIAdapter : public IInputAdapter {
public:

    /**
     * Creates a new instance.
     * @param system A valid CEGUI::System
     * @param renderer A valid CEGUI::OgreCEGUIRenderer
     * @return 
     */
    GUICEGUIAdapter(CEGUI::System *system, CEGUI::OgreRenderer *renderer);

    virtual ~GUICEGUIAdapter();
	
	virtual bool injectMouseMove(const MouseMotion& motion, bool& freezeMouse);
	virtual bool injectMouseButtonUp(const Input::MouseButton& button);
	virtual bool injectMouseButtonDown(const Input::MouseButton& button);
	virtual bool injectChar(int character);
	virtual bool injectKeyDown(const SDLKey& key);
	virtual bool injectKeyUp(const SDLKey& key);
    
private:
	CEGUI::System *mGuiSystem;
	CEGUI::OgreRenderer *mGuiRenderer;
	CEGUI::GUIContext &mGuiContext;
	
	/**
	mapping of SDL-keys to CEGUI keys
	*/
	SDLKeyMap mKeyMap;
	
};

}

}

#endif
