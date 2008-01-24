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

// #include "../EmberOgrePrerequisites.h"
#include <sigc++/slot.h>
#include <sigc++/signal.h>
#include <SDL_events.h>
#include "framework/ConsoleObject.h"
#include "framework/Singleton.h"

#include <set>
#include <list>
#include <map>

namespace EmberOgre {

class IInputAdapter;
class InputCommandMapper;

typedef std::set<SDLKey> KeysSet;
typedef std::list<IInputAdapter*> IInputAdapterStore;



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
	the relative movement measures as percentage of the total with of the window
	*/
	float xRelativeMovement; 
	float yRelativeMovement;
	
	/**
	the relative movement in pixels	
	*/
	int xRelativeMovementInPixels; 
	int yRelativeMovementInPixels;
	float timeSinceLastMovement;
};

/**
@author Erik Hjortsberg

This class takes care of all input and routes it to the correct place in Ember.
Right now that means that when in GUI mode, all input will be routed to CEGUI, and when in non-gui mode (ie. movement mode),
all input will be routed to Ember.

We use SDL for now. Perhaps in the future we'll add support for other input mechanisms (such as DirectInput)

Note that while keyboard input is buffered, mouse input is not.
*/
class Input : public Ember::ConsoleObject, public Ember::Singleton<Input>
{
friend class InputCommandMapper;

public:
	
	/**
	Command for binding keys to commands.
	*/
	static const std::string BINDCOMMAND;
	
	/**
	Command for unbinding keys to commands.
	*/
	static const std::string UNBINDCOMMAND;
	
	
	enum MouseButton
	{
		MouseButtonLeft,
		MouseButtonRight,
		MouseButtonMiddle,
		MouseWheelUp,
		MouseWheelDown,
	};
	
	/**
	Describes different input modes.
	*/
	enum InputMode
	{
		/**
		In gui mode, the mouse will move the cursor and allow interaction with the GUI system
		*/
		IM_GUI,
		
		/**
		In movement mode, the mouse will move the camera and the keys will move the player. Interaction with the gui is not possible.
		*/
		IM_MOVEMENT
	};
	
   Input();

   virtual ~Input();
	
	/**
	 *    Initializes the input object. Call this before you want to recieve input.
	 * @param width 
	 * @param heigh 
	 */
	void initialize(int width, int height);

	/**
	 * starts processing all input for a frame
	 * @param evt 
	 */
	void processInput();
	
	bool isApplicationVisible();
	
	/**emitted when a key has been pressed in movement mode
	@param the key event
	@param true if ember is in gui mode
	*/
 	sigc::signal<void, const SDL_keysym&, Input::InputMode> EventKeyPressed;
	
	/**emitted when a key has been released in movement mode
	@param the key event
	@param true if ember is in gui mode
	*/
 	sigc::signal<void, const SDL_keysym&, Input::InputMode> EventKeyReleased;
	
	/**emitted when the mouse has moved
	note that when in non-gui mode, the x and y position for the mouse will always be the same for consecutive signals
	although the relative position will have changed
	@param the mouse motion
	@param true if ember is in gui mode
	*/
	sigc::signal<void, const MouseMotion&, InputMode> EventMouseMoved;
	
	
	/**
		emitted when a mouse button is pressed
		@param the mouse button
		@param true if ember is in gui mode
	*/
	sigc::signal<void, MouseButton, InputMode> EventMouseButtonPressed;
	
	/**
		emitted when a mouse button is released
		@param the mouse button
		@param true if ember is in gui mode
	*/
	sigc::signal<void, MouseButton, InputMode> EventMouseButtonReleased;
	
	/**
		Emitted when the input mode has been changed.
		@param the new input mode
	*/
	sigc::signal<void, InputMode> EventChangedInputMode;
	
	/** 
	Emitted when the window is minimized or un-mininized.
	*/
	sigc::signal<void, bool> EventWindowActive;
	
	/**
	 * returns true if the supplied key is down 
	 * @param  
	 * @return 
	 */
	const bool isKeyDown(const SDLKey&) const;

	
	
	/**
	 *    Sets the window geometry. Call this whenever the size of the window has changed.
	 * @param width 
	 * @param heigh 
	 */
	void setGeometry(int width, int heigh);

	/**
	 *    Sets the new input mode.
	 * @param mode 
	 */
	void setInputMode(InputMode mode);
	
	/**
	 *    Gets the current input mode.
	 * @return 
	 */
	InputMode getInputMode() const;
	
	
	/**
	 *    Toggles between different input modes, returning the new mode.
	 * @return the new input mode
	 */
	InputMode toggleInputMode();
	
	
	/**
	 *    Adds an adaptor to which input event will be sent. Note that event will be sent to adapters added later first, allowing them to decide whether events should be sent to previous added adapters. This allows later added adapters to override current behaviour.
	 * @param adaptor 
	 */
	void addAdapter(IInputAdapter* adapter);
	
	
	/**
	 *    Removed an adaptor from the list of adaptors.
	 * @param adaptor 
	 */
	void removeAdapter(IInputAdapter* adapter);
	

	
	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command 
	 * @param args 
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);
	
	/**
	Suppress all further event handling of the current event. Call this inside event handling methods to prevent further event handling.
	*/
	void suppressFurtherHandlingOfCurrentEvent();
	
	/**
	Gets whether the movement mode is enabled, at which all mouse right click events will toggle between movement mode and mouse mode.
	*/
	bool getMovementModeEnabled() const;
	
	/**
	Sets whether the movement mode is enabled, at which all mouse right click events will toggle between movement mode and mouse mode.
	*/
	void setMovementModeEnabled(bool value);

protected:

	typedef std::map<std::string, InputCommandMapper*> InputCommandMapperStore;
	
	
	/**
	 *    Registers a command mapper.
	 * @param mapper 
	 */
	void registerCommandMapper(InputCommandMapper* mapper);
	
	/**
	 *    Deregisters a command mapper.
	 * @param mapper 
	 */
	void deregisterCommandMapper(InputCommandMapper* mapper);
	
	
	/**
	The current input mode.
	*/
	InputMode mCurrentInputMode;
	
	/**
	 * polls all input for the mouse
	 * @param evt 
	 */
	void pollMouse(float secondsSinceLast);
	void pollEvents(float secondsSinceLast);
	
	void keyChanged(const SDL_KeyboardEvent &keyEvent);
	
	void keyPressed(const SDL_KeyboardEvent &keyEvent);
	void keyReleased(const SDL_KeyboardEvent &keyEvent);


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
	
	/**
	the amount of time since the last right mouse click
	used for detecting double clicks
	*/
	uint32_t mTimeSinceLastRightMouseClick;
	
	uint32_t mLastTick;

	/**
	 *    gets the text in the clipboard and pastes it to the gui system
	 */
	void pasteFromClipboard();
	
	/**
		A store of adapters to which input event will be sent.
	*/
	IInputAdapterStore mAdapters;
	
	/**
		The dimensions of the window.
	*/
	float mScreenWidth, mScreenHeight;
	
	
	/**
	A store of InputCommandMappers with their state as the key.
	*/
	InputCommandMapperStore mInputCommandMappers;
	
	bool mSuppressForCurrentEvent;
	
	bool mMovementModeEnabled;
};

};

#endif
