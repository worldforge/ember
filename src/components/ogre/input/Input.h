//
// C++ Interface: Input
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#ifndef EMBEROGREINPUT_H
#define EMBEROGREINPUT_H

#include "../EmberOgrePrerequisites.h"
#include <CEGUISystem.h>
#include <CEGUIEventArgs.h>
#include <CEGUIInputEvent.h> 

#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif

//#include <SDL_keysym.h>
#include <SDL.h>

namespace CEGUI {
class System;
class OgreRenderer;

}

namespace EmberOgre {

TYPEDEF_STL_MAP(SDLKey, CEGUI::Key::Scan, SDLKeyMap);

TYPEDEF_STL_SET(SDLKey, KeysSet);

struct MouseMotion
{
	int xPosition;
	int yPosition;
	Ogre::Real xRelativeMovement;
	Ogre::Real yRelativeMovement;
	Ogre::Real timeSinceLastMovement;
};

/**
@author Erik Hjortsberg

This class takes care of all input and routes it to the correct place in Ember.
Right now that means that when in GUI mode, all input will be routed to CEGUI, and when in non-gui mode (ie. movement mode),
all input will be routed to Ember.

We use SDL for now. Perhaps in the future we'll add support for other input mechanisms (such as DirectInput)

Note that while keyboard input is buffered, mouse input is not.
*/
class Input
: virtual public SigC::Object
{
public:
    Input(CEGUI::System *system, CEGUI::OgreRenderer *renderer);

    ~Input();
	
	//starts processing all input for a frame
	void processInput(const Ogre::FrameEvent& evt);
	
	//emitted when a key has been pressed in movement mode
 	SigC::Signal1<void, const SDLKey&> KeyPressed;
	//emitted when a key has been released in movement mode
 	SigC::Signal1<void, const SDLKey&> KeyReleased;
	
	//emitted when the mouse has moved
	SigC::Signal1<void, const MouseMotion&> MouseMoved;
	
	//returns true if the supplied key is down
	const bool isKeyDown(const SDLKey&) const;

	
	//returns true if we're in gui mode
	const bool isInGUIMode() const { return mInGUIMode; }
	
	
protected:
	CEGUI::System *mGuiSystem;
	CEGUI::OgreRenderer *mGuiRenderer;
	
	//polls all input for the mouse
	void pollMouse(const Ogre::FrameEvent& evt);
	void pollKeyboard(const Ogre::FrameEvent& evt);
	
	void keyChanged(const SDL_KeyboardEvent &keyEvent);
	
	void keyPressed(const SDL_KeyboardEvent &keyEvent);
	void keyReleased(const SDL_KeyboardEvent &keyEvent);

	//mapping of SDL-keys to CEGUI keys
	SDLKeyMap mKeyMap;
	
	//keys which should not be injected as chars, ie. enter, backspace etc.
	KeysSet mNonCharKeys;
	

	//a set of the keys that are currently pressed
	KeysSet mKeysPressed;

	//saves the last mouse state
	unsigned int mMouseState;

	//the last positions of the mouse 
	int mMouseY, mMouseX;
	bool mInGUIMode;
	
	
};

};

#endif
