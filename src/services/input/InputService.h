/*
    Copyright (C) 2002 Hans Häggström, Tim Enderling

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

#ifndef INPUT_SERVICE_H
#define INPUT_SERVICE_H

// Include other headers of the current program here
#include "InputMapping.h"
#include "InputDevice.h"

// Include library headers here
#include <SDL/SDL_events.h>

// Include system headers here
#include <string>
#include <vector>
#include <list>


namespace dime {


//General TODOs:
// - test if it compiles under Unix especially
// - test if it works
// - Add debug assertions
// - No eating up of events till now. Add it if we need it.

/**
 * A service for getting input from the user, and sending it to
 * different listeners. 
 *
 * Thus it is the owner of all input mappings. If an input mapping
 * is not removed from the list before the dtor of InputService is called, it is automatically 
 * deleteted.
 * 
 * It also owns a list of input devices, that handles their input events. 
 * 
 * See InputDevice and InputMapping for more details.
 *
 * @author Hans Häggström/Tim Enderling
 */

class InputService: public Service
{
    //======================================================================
    // Inner classes and typedefs
    //======================================================================
    public:

    /**
     * The type of iterators used to iterate through the input mappings table.
     */
	typedef vector<InputMapping *>::iterator InputMappingIterator;
    typedef vector<InputMapping *>::const_iterator ConstInputMappingIterator;

	/**
	 * They type of iterators used to iterate through the input devices table.
	 */

	typedef vector<InputDevice*>::iterator InputDeviceIterator;
	typedef vector<InputDevice*>::const_iterator ConstInputDeviceIterator;

    //======================================================================
    // Public Constants
    //======================================================================
    public:


    //======================================================================
    // Private Variables
    //======================================================================
    private:

    /**
     * Stores a list of input mappings that are searched in order to find
     * one that handles a certain input event.
     */
    vector<InputMapping *> myInputMappings;
	vector<InputDevice  *> myInputDevices;

    //======================================================================
    // Public methods
    //======================================================================
	public:
	
    //----------------------------------------------------------------------
    // Constructors & Destructor

    /** Creates a new InputService using default values. */
    InputService()
    {
		setName( "InputService" );
		setDescription( "Maps user input to different actions." );
    }


    /** Deletes a InputService instance. */
    virtual ~InputService()
    {
        //delete all mappings
		for (InputMappingIterator i = myInputMappings.begin(); i != myInputMappings.end(); i++)
		{
			delete *i;
		}
    }


    //----------------------------------------------------------------------
    // Input Mapping methods

    /**
     * Returns a const iterator that can be used to iterate through the input mappings
     */
    virtual ConstInputMappingIterator getInputMappings() const
    {
        return myInputMappings.begin();
    }

    /**
     * Adds a new input mapping.
     *
     * @param inputMapping pointer to the input mapping to add.
     */
    virtual void addInputMapping( InputMapping * inputMapping)
    {
		myInputMappings.push_back(inputMapping);

		InputDevice * device = inputMapping->getMotionDevice();

		if (device != NULL)
		{
			device->myMotionMappings.push_back(inputMapping);
		}

		device = inputMapping->getKeyDevice();

		if (device != NULL)
		{
			device->myKeyMappings.push_back(inputMapping);
		}
    }

    /**
     * Removes the specified input mapping. (but doesn't delete it).
     */
    virtual void removeInputMapping( InputMapping * inputMapping )
    {
		for (InputMappingIterator i = myInputMappings.begin(); i != myInputMappings.end(); i++)
		{
			if (*i == inputMapping)
			{
				myInputMappings.erase(i);

				InputDevice * device = inputMapping->getMotionDevice();

				if (device != NULL)
				{
					for (i = device->myMotionMappings.begin(); 
								i != myMotionMappings.end(); i++)
					{
						if (i * == inputMapping)
						{
							device->myMotionMappings.erase(i);
							break;
						}
					}
				}

				device = inputMapping->getKeyDevice();

				if (device != NULL)
				{
					for (i = device->myKeyMappings.begin(); 
								i != myKeyMappings.end(); i++)
					{
						if (i * == inputMapping)
						{
							device->myKeyMappings.erase(i);
							break;
						}
					}
				}

				return;
			}
		}		
    }
    

    //----------------------------------------------------------------------
    // Methods inherited from Service
	
	/**
	 * This method is used to start the service.
     * It takes care of aquiring needed resources, initializing
     * data structures, and so on. <p>
     *
     * If the initialization suceeds, it should also call setRunning( true )
     * to indicate that the service is running.  <p>
     *
     * If initialization fails, it should set appropriate status code and
     * status text.  It could also write an entry into a log through the logging
     * service.  <p>
     *
	 * @returns success or error code
	 */
    virtual int start()
    {
        setRunning( true );

        // Start listening to SDL events.
        // TODO

        return 0;
    }


	/**
	 * This method stops the service, and frees any used resources.
	 *
	 * @ param code code which represents the cause of the service halt
     * TODO(zzorn): What do we need it for?
	 */
	virtual void stop( int code )
    {
        // Stop listening to SDL events.
        // TODO

        setRunning( false );
    }

	
    //======================================================================
    // Protected methods
    //======================================================================
    protected:


    //----------------------------------------------------------------------
    // Event handling methods
	
    /**
     * This method handles the next event from the SDL event queue, 
     * sending it off to a listener.
     *
	 * Should return TRUE only if it's _absolutely sure_, that no other event handler needs 
	 * this event. Thus returning TRUE means that the event has been 'eaten up' by handleEvent.
	 */

    virtual bool handleEvent(SDLEvent & event)
    {
        for (InputDeviceIterator i = myInputDevices.begin(); i != myInputDevices.end(); i++)
		{
			if ((*i)->handleEvent(event)) return TRUE;
		}

		return FALSE;
    }

}; // InputService

} // namespace dime

#endif


