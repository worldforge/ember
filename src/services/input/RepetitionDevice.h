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

#ifndef REPETITION_DEVICE_H
#define REPETITION_DEVICE_H

// Include other headers of the current program here
#include "InputMapping.h"

// Include library headers here
#include <SDL/SDL_keysym.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_mouse.h>
#include <sigc++/signal_system.h>
#include <SDL/SDL_timer.h>


// Include system headers here
#include <string>
#include <vector>

namespace dime {

/**
 * Special derivation of input device for automatic repetition. RepetitionDevice 
 * Objects send motion events in constant delays. This can be useful for
 * key repetition.
 *
 * HINT: Because timer functions are called from other threads, the repetition
 * device uses the SDL message loop passing some SDL_USEREVENT to fix this.
 */

class RepetitionDevice: public InputDevice, public SigC::Object
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:
	long unsigned int myInitialDelay; //initial delay in milliseconds
	long unsigned int myDelay; //delay in milliseconds
	SDL_TimerID myTimerID;

	//======================================================================
    // Public methods
    //======================================================================
	public:
	
	/**
	 * Connect this to the input mapping of your choice. If a KEY_PRESSED event
	 * is found, the repetition device synchronizes it's calles to this event.
	 */

	void switchOn(InputDevice * motionDevice, InputDevice * keyDevice, 
			const SDLKey & Key, InputMapping::InputSignalType type);

    //----------------------------------------------------------------------
    // Constructors & Destructor

    /** 
	 * Creates a repetition device. Since this is no actual SDL input device
	 * no index or subindex is needed.
	 */
	RepetitionDevice(long unsigned int initialDelay, long unsigned int delay,
		bool alwaysRunning = false);

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



