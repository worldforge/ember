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

#ifndef INPUT_MAPPING_H
#define INPUT_MAPPING_H

// Include other headers of the current program here
#include "InputDevice.h"

// Include library headers here
#include <sigc++/signal_system.h>
#include <sigc++/generator.h>
#include <sigc++/rettype.h>
#include <sigc++/bind.h>
#include <sigc++/convert.h>


// Include system headers here
#include <string>
#include <set>

//General TODOs:
// - test if it compiles under Unix especially
// - test if it works
// - Add debug assertions
// - Is there a way for timing out handling of input events? (Just to avoid the 'user presses
// button serveral times, because he sees no reaction and afterwards he has to wait, because
// all events are really processed...')
// - Should InputMappings be autoregistring? (ATM: No.)
// - Is a state only true, if the modifiers where pressed _before_ the base key? (ATM: Yes.)
// - Should KEY_RELEASED also occur, 
//    if the modifiers are released _before_ the base key? (ATM: No.)
// - Should KEY_PRESSED events also occur, if there are more modifiers pressed than
//   specified in the mapping? (ATM: No.)

namespace dime {

/**
 * an input mapping, used to filter the users input and give signals to the listeners.
 *
 * In general input mapping is based on the combination of two principles: 
 * (Motion) events and (key/button) states.
 * 
 * All these motions are seen as changes of the some axis vectors, where it is possible to 
 * change more than one axis by one event. 
 * 
 * See InputDevice for more details on available motion events and key states.
 *
 * For convenience also a range of keys (of the same input device) can be used instead 
 * of one key. The range includes it's border item.
 *
 * Further there is a way to combine some states by 'AND'. This you have a base state 
 * (arbitrary key/button) and some modifiers (not only a modifier!). Available modifiers are
 * capslock, left/right control/shift/alt/meta and any of both control/shift/alt/meta keys.
 *  * 
 * HINT: If you want the user to press multiple modifiers, you have to combine them by bitwise
 * or |. An exception of this rule is connecting the left and the right version of the same key
 * In this case the user can press any of both keys, not only both at the same time.
 *
 * An input signal is fired when
 * - A key and it's modifiers were pressed synchronously. (KEY_PRESSED)
 * - The motion event occurs, while the key is pressed is true.  
 *   (one of the key's pressed is passed to the signal handler) (EVENT_OCCURED) 
 * - The key or one of it's modifiers was released. (KEY_RELEASED) 
 *
 * For a key range you get seperate a KEY_PRESSED/KEY_RELEASED signal for each key.
 *
 * The types of input signals can be or'd together to get only those types wanted, like
 * you would expect it.
 *
 * Same examples of adding InputMappings to InputDevice:
 *
 * <code>
 * //assuming your in dime namespace
 * InputService * inputService;
 * //and you have a pointer to the inputService
 *
 * //get the default input devices
 * InputDevice * keyboard = inputService->getDevice(InputDevice::KEYBOARD);
 * InputDevice * mouse = inputService->getDevice(InputDevice::MOUSE);
 * InputDevice * repetitor = inputService->getDevice(InputDevice::REPETITOR);
 * 
 * //Single action on return
 * inputService.addMapping(new 
 *   InputMapping(keyboard, SDLK_RETURN, FALSE, slot(dialog,&dialog::onDefault)))
 *
 * //Observing a key being pressed and released
 * inputService.addMapping(new
 *   InputMapping(keyboard, SDLK_CAPS, TRUE, 
 *
 * //Observing mouse movement only
 * inputService.addMapping(new 
 *   InputMapping(mouse, slot(dialog, &dialog::updateMousePosition));
 *
 * [TODO]
 * 
 * </code>
 *
 * @author Tim Enderling
 */

using namespace SigC;

class InputMapping
{
	friend class InputDevice;

 //======================================================================
    // Public Constants
    //======================================================================
    public:

	/**
	 * Input Signal Types
	 */

	enum InputSignalType
	{
		KEY_PRESSED  = 0x1,
		KEY_RELEASED = 0x2,
		EVENT_OCCURED = 0x4
	};
	
    //======================================================================
    // Inner classes and typedefs
    //======================================================================
    public:

	/**
	 * A signal takes the following params:
	 * - InputDevice * motionDevice 
	 * - const SDLKey & key (pressed)
	 * - InputSignalType type 
	 *
	 * HINT: With the help of sigc++'s 'bind' you can pass further constant
	 * params to your handling function (usually called 'cockie').
	 */

	typedef SigC::Signal3<void, InputDevice *, const SDLKey & , InputSignalType, Marshal<void> 
		> InputSignal;

	typedef SigC::Slot3<void, InputDevice *, const SDLKey &, InputSignalType> InputSlot;

   
    //======================================================================
    // Private Variables
    //======================================================================
    private:

	InputDevice   * myMotionDevice;
	InputDevice   *	myKeyDevice;
	SDLKey			myKeyRangeStart;
	SDLKey			myKeyRangeEnd;
	SDLMod		    myModifiers;
	InputSignal		mySignal;
	InputSignalType myTypes;

	std::set<SDLKey>myKeysPressed;

	

    //======================================================================
    // Public methods
    //======================================================================
	public:
	
    //----------------------------------------------------------------------
    // Constructors & Destructor

    /** 
	 * Creates a new InputMapping. Exists in various forms to fit everyone's needs.
	 *
	 * @param motionDevice Input device to observe motion of.
	 * @param keyDevice Input device to observe key state of.
	 * @param baseKey Key to listen to.
	 * @param keyRangeStart Start of the key range.
	 * @param keyRangeEnd End of the key range.
	 * @param modifiers Key modifiers that should be pressed synchronous to baseKey/ a key
	 *				    inner key range.
	 * @param slot A callback or some other signal slot.
	 * @param types Types of input signals to pass through the slot. Can be any OR-combination
	 *				of InputSignalType enum's items. 
	 * @param fireOnRelease Decides wether to fire a signal on key releasing, i.e. wether
	 *						include KEY_RELEASED into types or not.
	 */

	/**
	 * Version used to observe a motion only.
	 */
	InputMapping(InputDevice * motionDevice, InputSlot & slot)
	{
		myMotionDevice		= motionDevice;
		myKeyDevice			= NULL;
		myTypes				= EVENT_OCCURED;
		
		myKeysPressed.insert(SDLK_UNKNOWN); //adds a pseudo key, that is always 'pressed'

		mySignal.connect(slot);		
	}

	/**
	 * Version used to observe a key only.
	 */
    InputMapping(InputDevice * keyDevice, SDLKey baseKey, 
				  bool fireOnRelease, InputSlot & slot)
    {
		myMotionDevice		= NULL;
		myKeyDevice			= keyDevice;
		myKeyRangeStart		= baseKey;
		myKeyRangeEnd		= baseKey;
		myModifiers			= KMOD_NONE;
		myTypes				= static_cast<InputSignalType>
									(KEY_PRESSED | (fireOnRelease ? KEY_RELEASED : 0));

		mySignal.connect(slot);	
	}

	/**
	 * Version used to observe a key and some modifiers.
	 */
    InputMapping(InputDevice * keyDevice, SDLKey baseKey, SDLMod modifiers, 
				 bool fireOnRelease, InputSlot & slot)
    {
		myMotionDevice		= NULL;
		myKeyDevice			= keyDevice;
		myKeyRangeStart		= baseKey;
		myKeyRangeEnd		= baseKey;
		myModifiers			= modifiers;
		myTypes				= static_cast<InputSignalType>(
			KEY_PRESSED | (fireOnRelease ? KEY_RELEASED : 0));
		
		mySignal.connect(slot);
	}

	
	/**
	 * Version used to observe a key range.
	 */
    InputMapping(InputDevice * keyDevice, SDLKey keyRangeStart, SDLKey keyRangeEnd,
				   bool fireOnRelease, InputSlot & slot)
    {
		myMotionDevice		= NULL;
		myKeyDevice			= keyDevice;
		myKeyRangeStart		= keyRangeStart;
		myKeyRangeEnd		= keyRangeEnd;
		myModifiers			= KMOD_NONE;
		myTypes				= static_cast<InputSignalType>
			(KEY_PRESSED | (fireOnRelease ? KEY_RELEASED : 0));
		
		mySignal.connect(slot);		
	}

	/**
	 * Version used to observe a key range and some modifiers.
	 */
    InputMapping(InputDevice * keyDevice, SDLKey keyRangeStart, SDLKey keyRangeEnd, 
				SDLMod modifiers, bool fireOnRelease, InputSlot & slot)
    {
		myMotionDevice		= NULL;
		myKeyDevice			= keyDevice;
		myKeyRangeStart		= keyRangeStart;
		myKeyRangeEnd		= keyRangeEnd;
		myModifiers			= modifiers;
		myTypes				= static_cast<InputSignalType>
			(KEY_PRESSED | (fireOnRelease ? KEY_RELEASED : 0));
		
		mySignal.connect(slot);
	}

	/**
	 * Version used to observe a motion event and a key with modifiers.
	 */
	
	InputMapping(InputDevice * motionDevice, 
				InputDevice * keyDevice, SDLKey baseKey,  
				SDLMod modifiers, InputSignalType types, InputSlot & slot)
    {
		myMotionDevice		= motionDevice;
		myKeyDevice			= keyDevice;
		myKeyRangeStart		= baseKey;
		myKeyRangeEnd		= baseKey;
		myModifiers			= modifiers;
		myTypes				= types;

		mySignal.connect(slot);
	}

	/**
	 * Version used to observe a motion event and a key range with modifiers.
	 */
	
	InputMapping(InputDevice * motionDevice, 
				InputDevice * keyDevice, SDLKey keyRangeStart, SDLKey keyRangeEnd,  
				SDLMod modifiers, InputSignalType types, InputSlot & slot)
    {
		myMotionDevice		= motionDevice;
		myKeyDevice			= keyDevice;
		myKeyRangeStart		= keyRangeStart;
		myKeyRangeEnd		= keyRangeEnd;
		myModifiers			= modifiers;
		myTypes				= types;

		mySignal.connect(slot);
	}

    /** Deletes a InputMapping instance. */
    virtual ~InputMapping()
    {
        // Free any allocated memory and resources
    }
	
    //----------------------------------------------------------------------
    // Public properties
		
	InputDevice * getKeyDevice()
	{
		return myKeyDevice;
	}

	InputDevice * getMotionDevice()
	{
		return myMotionDevice;
	}

    //======================================================================
    // Protected methods
    //======================================================================
    protected:

	/**
	 * Is called by the InputDevice mapped as keyDevice, whenever a key
	 * was pressed.
	 *
	 * For performance reasons this should be inline in any case.
	 */

	inline void keyPressed(SDLKey & key, SDLMod modifier)
	{
		if (key >= myKeyRangeStart && key <= myKeyRangeEnd)
		{
			if (((KMOD_CTRL & myModifiers) == KMOD_CTRL) && (modifier & KMOD_CTRL))
			{
				((int&)modifier) |= KMOD_CTRL;
			}

			if (((KMOD_SHIFT & myModifiers) == KMOD_SHIFT) && (modifier & KMOD_SHIFT))
			{
				((int&)modifier) |= KMOD_SHIFT;
			}

			if (((KMOD_ALT & myModifiers) == KMOD_ALT) && (modifier & KMOD_ALT))
			{
				((int&)modifier) |= KMOD_ALT;
			}

			if (((KMOD_META & myModifiers) == KMOD_META) && (modifier & KMOD_META))
			{
				((int&)modifier) |= KMOD_META;
			}
			
			if ((modifier & myModifiers) == myModifiers)
			{
				myKeysPressed.insert(key);			

				if (myTypes & KEY_PRESSED)
				{
					mySignal(myMotionDevice, key, KEY_PRESSED);
				}
			}
		}
	}

	/**
	 * Is called by the InputDevice mapped as keyDevice, whenever a key
	 * was released.
	 *
	 * For performance reasons this should be inline in any case.
	 */

	inline void keyReleased(SDLKey & key)
	{
		if (key >= myKeyRangeStart && key <= myKeyRangeEnd)
		{
			//remove only keys that was pressed synchronous with their modifiers.
			if (myKeysPressed.erase(key))
			{
				if (myTypes & KEY_RELEASED)
				{
					mySignal(myMotionDevice, key, KEY_RELEASED);
				}
			}
		}
	}

	/**
	 * Is called by the InputDevice mapped as motionDevice, whenever at least one axis
	 * position changes.
	 *
	 * For performance reasons this should be inline in any case.
	 */

	inline void motion()
	{
		if (myTypes & EVENT_OCCURED)
		{
			for (std::set<SDLKey>::iterator i = myKeysPressed.begin(); i != myKeysPressed.end(); i++)
			{		
				mySignal(myMotionDevice, *i, EVENT_OCCURED);
			}
		}
	}
}; // InputMapping

} // namespace dime

#endif


