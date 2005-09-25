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
class OgreCEGUIRenderer;

}

namespace EmberOgre {

TYPEDEF_STL_MAP(SDLKey, CEGUI::Key::Scan, SDLKeyMap);

TYPEDEF_STL_SET(SDLKey, KeysSet);



/**
struct for a mouse movement
*/
struct MouseMotion
{
	/**
	the position of the mouse
	*/
	int xPosition; 
	int yPosition;
	/**
	the relative movement measures as percentage of the totoal with of the window
	*/
	Ogre::Real xRelativeMovement; 
	Ogre::Real yRelativeMovement;
	
	/**
	the relative movement in pixels	
	*/
	int xRelativeMovementInPixels; 
	int yRelativeMovementInPixels;
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
	enum MouseButton
	{
		MouseButtonLeft,
		MouseButtonRight,
		MouseButtonMiddle
	};
	
   Input(CEGUI::System *system, CEGUI::OgreCEGUIRenderer *renderer);

    ~Input();
	

	/**
	 * starts processing all input for a frame
	 * @param evt 
	 */
	void processInput(const Ogre::FrameEvent& evt);
	
	/**emitted when a key has been pressed in movement mode
	@parameter the key event
	@parameter true if ember is in gui mode
	*/
 	SigC::Signal2<void, const SDL_keysym&, bool> EventKeyPressed;
	
	/**emitted when a key has been released in movement mode
	@parameter the key event
	@parameter true if ember is in gui mode
	*/
 	SigC::Signal2<void, const SDL_keysym&, bool> EventKeyReleased;
	
	/**emitted when the mouse has moved
	note that when in non-gui mode, the x and y position for the mouse will always be the same for consecutive signals
	although the relative position will have changed
	@parameter the mouse motion
	@parameter true if ember is in gui mode
	*/
	SigC::Signal2<void, const MouseMotion&, bool> EventMouseMoved;
	
	
	/**
		emitted when a mouse button is pressed
		@parameter the mouse button
		@parameter true if ember is in gui mode
	*/
	SigC::Signal2<void, const MouseButton&, bool> EventMouseButtonPressed;
	
	/**
		emitted when a mouse button is released
		@parameter the mouse button
		@parameter true if ember is in gui mode
	*/
	SigC::Signal2<void, const MouseButton&, bool> EventMouseButtonReleased;
	
	/**
	 * returns true if the supplied key is down 
	 * @param  
	 * @return 
	 */
	const bool isKeyDown(const SDLKey&) const;

	
	
	/**
	 *    returns true if we're in gui mode
	 * @return 
	 */
	const bool isInGUIMode() const { return mInGUIMode; }
	
	
	/**
	 *    Sets whether or not to use the "locked" moevment mode, in which the input system always will be in movement mode, even though the right mouse button isn't pressed.
	 * @param  
	 */
	void setIsInLockedMovementMode(bool value);
	
	/**
	 *    Returns whether or not we're in the "locked" movement mode, in which the input system always will be in movement mode, even though the right mouse button isn't pressed.
	 * @return 
	 */
	bool getIsInLockedMovementMode();
	
	
protected:
	CEGUI::System *mGuiSystem;
	CEGUI::OgreCEGUIRenderer *mGuiRenderer;
	
	/**
	 * polls all input for the mouse
	 * @param evt 
	 */
	void pollMouse(const Ogre::FrameEvent& evt);
	void pollKeyboard(const Ogre::FrameEvent& evt);
	
	void keyChanged(const SDL_KeyboardEvent &keyEvent);
	
	void keyPressed(const SDL_KeyboardEvent &keyEvent);
	void keyReleased(const SDL_KeyboardEvent &keyEvent);

	/**
	mapping of SDL-keys to CEGUI keys
	*/
	SDLKeyMap mKeyMap;
	
	/**
	keys which should not be injected as chars, ie. enter, backspace etc.
	*/
	KeysSet mNonCharKeys;
	

	/**
	a set of the keys that are currently pressed
	*/
	KeysSet mKeysPressed;

	/**
	saves the last mouse state
	*/
	unsigned int mMouseState;

	/**
	the last positions of the mouse 
	*/
	int mMouseY, mMouseX;
	bool mInGUIMode;
	
	/**
	the amount of time since the last right mouse click
	used for detecting double clicks
	*/
	Ogre::Real mTimeSinceLastRightMouseClick;
	
	/**
	if true, we're in "locked" movement mode, no matter if the right mouse button is pressed or not
	*/
	bool mInLockedMovementMode;
	
};

};

#endif
