/*
    Copyright (C) 2002 Tim Enderling

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

#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

// Include other headers of the current program here

// Include library headers here
#include <SDL/SDL_keysym.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_mouse.h>
#include <sigc++/signal_system.h>
#include <SDL/SDL_timer.h>


// Include system headers here
#include <string>
#include <vector>

//General TODOs
// - Add JoystickDevice, JoystickHatDevice and JoystickBallDevice
// - test if it compiles under Unix especially
// - test if it works
// - Add debug assertions

namespace dime {

/**
 * An input device used to hold the positions and to provide info on the available devices.
 * 
 * At the moment SDL only supports multiple joysticks (and only one mouse and keyboard). 
 *
 * @author Tim Enderling
 */

//SDL user event
const Uint8 TIMEREVENT = SDL_USEREVENT;

//extensions of SDL key enum type	
//left mouse button
const SDLKey SDLK_LEFT_MB =  static_cast<SDLKey>(SDL_BUTTON_LEFT); 
 //middle mouse button
const SDLKey SDLK_MIDDLE_MB = static_cast<SDLKey>(SDL_BUTTON_MIDDLE);
//right mouse button
const SDLKey SDLK_RIGHT_MB = static_cast<SDLKey>(SDL_BUTTON_RIGHT); 

//joystick buttons 
const SDLKey SDLK_JB0      = static_cast<SDLKey>(0); 
const SDLKey SDLK_JB1      = static_cast<SDLKey>(1); 
const SDLKey SDLK_JB2      = static_cast<SDLKey>(2); 
const SDLKey SDLK_JB3      = static_cast<SDLKey>(3); 
const SDLKey SDLK_JB4      = static_cast<SDLKey>(4); 
const SDLKey SDLK_JB5      = static_cast<SDLKey>(5); 
const SDLKey SDLK_JB6      = static_cast<SDLKey>(6); 
const SDLKey SDLK_JB7      = static_cast<SDLKey>(7); 
const SDLKey SDLK_JB8      = static_cast<SDLKey>(8); 
const SDLKey SDLK_JB9      = static_cast<SDLKey>(9); 


/**
 * General base class for any kind of input device.
 * 
 * An general an input device is a collection of an arbitrary number of axis (physical values
 * or scaled values from 0 to 1) and keys (in pressed or released state). 
 * Anything more special is (e.g. repetition) handled in the derived classes.
 *
 * Because sometimes there is more than one physical input device of the sime type
 * (multiple joysticks) and even multiple subdevices (multiple joystick hats) an
 * index and a subindex can be specified when creating an InputDevice of such kind.
 *
 * HINT: The InputDevices do not hold the InputMappings (means are not responsible
 * for freeing the memory), because there can be two different of them 
 * (a motional and a key state) having a pointer to the same InputMapping. Instead
 * InputMappings are generally added/removed/deallocated by the InputService.
 */

class InputMapping;

class InputDevice
{
	friend class InputService;
    //======================================================================
    // Inner classes and typedefs
    //======================================================================
    public:

	typedef std::vector<InputMapping*>	MappingVector;
	typedef MappingVector::iterator MappingIterator;

    //======================================================================
    // Public Constants
    //======================================================================
    public:

	/**
	 * Input Signal Types
	 */

	enum DeviceType
	{
		MOUSE,
		KEYBOARD,
		JOYSTICK_AXIS,
		JOYSTICK_BALL,
		JOYSTICK_HAT,
		REPETITOR
	};	

	enum KeyState
	{
		PRESSED,
		RELEASED,
		UNSUPPORTED_KEY
	};

    //======================================================================
    // Private Variables
    //======================================================================
    private:

	DeviceType			myType;
	int					myIndex;
	int					mySubIndex;


    //======================================================================
    // Protected Variables
    //======================================================================
	
	protected:

	std::vector<int>	myPhysicalPosition;
	std::vector<int>	myPhysicalMin;
	std::vector<int>	myPhysicalMax;

	MappingVector		myKeyMappings;
	MappingVector		myMotionMappings;


	//======================================================================
    // Public methods
    //======================================================================
	public:
	
    //----------------------------------------------------------------------
    // Constructors & Destructor

    /** 
	 * Creates a new InputDevice and registers it at the InputService.
	 *
	 * @param type Type of input device to represent.
	 * @param index Handling number used in SDL if multiple devices of the same type
	 *				  are available. 
	 * @param subIndex Subnumber when it is needed (multiple hats at the same joystick...).
	 */

    InputDevice(DeviceType type, int index, int subIndex);

    /** 
	 * Deletes a InputDevice instance and unregisters it from the InputService. 
	 */
    virtual ~InputDevice();
	
    //----------------------------------------------------------------------
    // Public properties

	DeviceType getType();
	int getAxisCount();
        int getAxis(int axis);
	int getAxisPosition(int axis);
	float getAxisScaledPosition(int axis);

	virtual KeyState getKeyState(SDLKey key);
	
    //======================================================================
    // Protected methods
    //======================================================================
    protected:
  
	/**
	 * Retrieves the axis(es)'s minima, maxima and current positions and saves
	 * them for internal handling of the input device.
     *
	 * ATTENTION: Since this is a virtual function, it cannot be called by the ctor
	 * of InputDevice (as it should). Thus every derived class has to call initAxis
	 * in it's ctor itself, if needed.
	 */	
	virtual void initAxis();

	/**
	 * Called whenever an SDL event ocurres. Should return TRUE only if it's _absolutely sure_,
	 * that no other event handler needs this event. Thus returning TRUE means that the
	 * event has been 'eaten up' by handleEvent.
	 */
	virtual bool handleEvent(SDL_Event & event)
	{
		//default implementation handles no events
		return false;
	}

	/**
	 * Helping functions called by all derived input devices to signal changes.
	 */
	
	void keyPressed(SDLKey & key);
	void keyReleased(SDLKey & key);
	void motion();

}; // InputMapping

/**
 * Special derivation of input device for a keyboard.
 */

class KeyboardDevice: public InputDevice
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:

	int myKeyCount;			//count of keys
	KeyState * myKeys;		//array of key states

	//======================================================================
    // Public methods
    //======================================================================
	public:
	
    //----------------------------------------------------------------------
    // Constructors & Destructor

    /** 
	 * Creates a new Keyboard device. Since SDL supports only one keyboard, 
	 * no index is supported.
	 */

	KeyboardDevice();

    /** 
	 * Deletes a KeyboardDevice instance. 
	 */
    virtual ~KeyboardDevice();

    //----------------------------------------------------------------------
    // Public properties

	virtual KeyState getKeyState(SDLKey key);

   //======================================================================
    // Protected methods
    //======================================================================
    protected:
 
	bool KeyboardDevice::handleEvent(SDL_Event & event);
};

/**
 * Special derivation of input device for a mouse.
 */

class MouseDevice: public InputDevice
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:
	bool myHasThreeButtons; //determines wether the mouse has three buttons or not

	//======================================================================
    // Public methods
    //======================================================================
	public:
	
    //----------------------------------------------------------------------
    // Constructors & Destructor

    /** 
	 * Creates a new mouse device. Since SDL supports only one mouse, 
	 * no index is supported.
	 */

	MouseDevice();

    /** 
	 * Deletes a MouseDevice instance. 
	 */
    virtual ~MouseDevice();

    //----------------------------------------------------------------------
    // Public properties

	virtual KeyState getKeyState(SDLKey key);
	
    //======================================================================
    // Protected methods
    //======================================================================
    protected:

	virtual bool handleEvent(SDL_Event & event);
	virtual void initAxis();
};

/**
 * Special derivation of input device for automatic repition. RepitionDevice 
 * Objects send motion events in constant delays. This can be useful for
 * key repetition.
 *
 * HINT: Because timer functions are called from other threads, the repetition
 * device uses the SDL message loop passing some SDL_USEREVENT to fix this.
 * 
 * TODO: Do we need some synchronizing between the pressing of a key and 
 * the starting of the delay? In this case every key would need it's own 
 * repetition device.
 */

class RepetitionDevice: public InputDevice
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:
	long unsigned int myDelay; //delay in milliseconds
	SDL_TimerID myTimerID;

	//======================================================================
    // Public methods
    //======================================================================
	public:
	
    //----------------------------------------------------------------------
    // Constructors & Destructor

    /** 
	 * Creates a repetition device. Since this is no actual SDL input device
	 * no index or subindex is needed.
	 */

	RepetitionDevice(long unsigned int delay);

    /** 
	 * Deletes a RepetitionDevice instance. 
	 */
    virtual ~RepetitionDevice();


    //----------------------------------------------------------------------
    // Public properties

	long unsigned int getDelay();

	//======================================================================
    // Protected methods
    //======================================================================
    protected:

	/**
	 * Is called back by the SDL and pushes a TIMEREVENT into the message loop.
	 */

	static Uint32 TimerCallback(Uint32 interval, void *param);
	virtual bool handleEvent(SDL_Event & event);
};



} // namespace dime

#endif



