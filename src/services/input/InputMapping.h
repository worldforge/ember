/*
    Copyright (C) 2002 Hans Tim Enderling

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

#ifndef INPUT_MAPPING_H
#define INPUT_MAPPING_H

// Include other headers of the current program here

// Include library headers here
#include <sdl/SDL_keysym.h>
#include <sigc++/signal_system.h>


// Include system headers here
#include <string>

//General TODOs:
// - test if it compiles under Unix especially
// - test if it works
// - Is there a way for timing out handling of input events? (Just to avoid the 'user presses
// button serveral times, because he sees no reaction and afterwards he has to wait, because
// all events are really processed...')
// - Add debug assertions.

namespace dime {

/**
 * an input mapping, used to filter the users input and give signals to the listeners.
 *
 * In general input mapping is based on the combination of two principles: 
 * (Motion) events and (key/button) states.
 * 
 * Motion events include:
 * - mouse motion
 * - joystick axis motion
 * - joystick hat motion
 * - joystick ball motion
 *
 * Two special events, that are also available for convenience:
 *
 * - A 'void' event, that never occurs
 * - A repetition event, that occurs in equal distances of time
 *
 * All these motions are seen as changes of the some axis vectors, where it is possible to 
 * change more than one axis by one event. The axis values are scaled to a floating point
 * value in the range -1 to 1, where -1/1 correspond to some preset minimum/maximum of physical 
 * value. 
 *  
 * States include:
 * - keyboard key pressed?
 * - mouse button pressed?
 * - joystick button pressed?
 *
 * Further there is a way to combine some states by 'AND'. This you have a base state 
 * (arbitrary key/button) and some modifiers. Available modifiers are:
 * - capslock
 * - left/right control
 * - left/right shift
 * - left/right alt
 * - any of the shift keys
 * - any of the alt keys
 * - any of the control keys
 * 
 * HINT: If you want the user to press multiple modifiers, you have to combine them by bitwise
 * or |. An exception of this rule is connecting the left and the right version of the same key
 * In this case the user can press any of both keys, not both at the same time!
 * 
 * The result is, that input mapping can be done by providing:
 * a) A basic state and it's modifiers.
 * b) An event that has to occur 
 *
 * [TODO: Is a state only true, if the modifiers where true before the base key was pressed?]
 *
 * An input signal is fired when
 * - a) becomes true  (STATE_BECAME_TRUE)
 * - b) occurs, while a) is true. (EVENT_OCCURED) 
 * - a) becomes false (STATE_BECAME_FALSE) 
 *
 * The key codes and the modifiers are taken from SDL.
 *
 * @author Tim Enderling
 */

/*The joystick axis event just contains the new position of the axis (or several), and is sent to anybody listening to the joystick..
<zzorn_2> Btw, we should probably use a similar way to represent different axis, such as mouse x,y, joystick x,y, joystick throttle, hat, etc..
*** Grimicus (~grim@24.125.66.narf-34395) has joined #dime
<Grimicus> DON'T DO IT!
<Grimicus> AHHHHH!!!!
* Grimicus grins
<zzorn_2> a floating point number from 0 to 1, or perhaps -1 to 1 seems fine..
Also, at least in case of mice we should also record the delta from the actual latest position (mickeys), even if the mouse already is at the screen edge..
*/
	
//extensions of SDL key enum type	
//left mouse button
const SDLKey SDLK_LEFT_MB =  static_cast<SDLKey>( static_cast<SDLKey>(SDLK_LAST)+1); 
 //middle mouse button
const SDLKey SDLK_MIDDLE_MB = static_cast<SDLKey>( static_cast<SDLKey>(SDLK_LAST)+2);
//right mouse button
const SDLKey SDLK_RIGHT_MB = static_cast<SDLKey>( static_cast<SDLKey>(SDLK_LAST)+3); 

//joystick buttons 
const SDLKey SDLK_JB0      = static_cast<SDLKey>( static_cast<SDLKey>(SDLK_LAST)+4); 
const SDLKey SDLK_JB1      = static_cast<SDLKey>( static_cast<SDLKey>(SDLK_JB0)+1); 
const SDLKey SDLK_JB2      = static_cast<SDLKey>( static_cast<SDLKey>(SDLK_JB0)+2); 
const SDLKey SDLK_JB3      = static_cast<SDLKey>( static_cast<SDLKey>(SDLK_JB0)+3); 
const SDLKey SDLK_JB4      = static_cast<SDLKey>( static_cast<SDLKey>(SDLK_JB0)+4); 
const SDLKey SDLK_JB5      = static_cast<SDLKey>( static_cast<SDLKey>(SDLK_JB0)+5); 
const SDLKey SDLK_JB6      = static_cast<SDLKey>( static_cast<SDLKey>(SDLK_JB0)+6); 
const SDLKey SDLK_JB7      = static_cast<SDLKey>( static_cast<SDLKey>(SDLK_JB0)+7); 
const SDLKey SDLK_JB8      = static_cast<SDLKey>( static_cast<SDLKey>(SDLK_JB0)+8); 
const SDLKey SDLK_JB9      = static_cast<SDLKey>( static_cast<SDLKey>(SDLK_JB0)+9); 

typedef basic_string<float> FloatString;

class InputMapping

{
    //======================================================================
    // Inner classes and typedefs
    //======================================================================
    public:

	/**
	 * A signal takes the following params:
	 * - FloatString & axis
	 * - InputSignalType type (of event)
	 */

	typedef Signal2<void, FloatString &, InputSignalType> InputSignal;
	typedef Slot2<void, FloatString &, InputSignalType>   InputSlot;

    //======================================================================
    // Public Constants
    //======================================================================
    public:

	/**
	 * Input Signal Types
	 */

	enum InputSignalType
	{
		STATE_BECAME_TRUE  = 0x1,
		STATE_BECAME_FALSE = 0x2,
		EVENT_OCCURED	   = 0x4
	};
	
	enum InputEvent
	{
		MOUSE_MOTION,
		JOYSTICK_AXIS_MOTION,
		JOYSTICK_BALL_MOTION,
		JOYSTICK_HAT_MOTION,
		VOID_EVENT,
		REPETITION_EVENT
	};	

    //======================================================================
    // Private Variables
    //======================================================================
    private:

	InputEvent  myEvent;
	SDLKey	    myBaseKey;
	SDLMod      myModifiers;
	InputSignal mySignal;


    //======================================================================
    // Public methods
    //======================================================================
	public:
	
    //----------------------------------------------------------------------
    // Constructors & Destructor

    /** Creates a new InputMapping*/

	//TODO: Is there a practical case of multiple connections (= slots) to the signal?

    InputMapping(InputEvent event, SDLKey baseKey, SDLMod modifier, InputSlot & slot)
    {
		myEvent = event;
		myBaseKey = baseKey;
		myModifiers = modifier;
		mySignal.connect(slot);
	}


    /** Deletes a InputMapping instance. */
    virtual ~InputMapping()
    {
        // Free any allocated memory and resources
    }
	
    //======================================================================
    // Protected methods
    //======================================================================
    protected:


  

}; // InputMapping

} // namespace dime

#endif


