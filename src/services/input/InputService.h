/*
    Copyright (C) 2002  Hans Häggström

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

#ifndef SERVICE_H
#define SERVICE_H

// Include other headers of the current program here
#include "InputEvent.h"
#include "InputMapping.h"
#include "InputEventListener.h"

// Include library headers here

// Include system headers here
#include <string>
#include <vector>
#include <list>

namespace dime {

/**
 * A service for getting input from the user, and sending it to
 * different listeners.
 *
 * @author Hans Häggström
 */
class InputService

{
    //======================================================================
    // Inner classes and typedefs
    //======================================================================
    public:

    /**
     * The type of const iterators used to iterate through the input mappings table.
     */
    typedef vector< InputMapping * >::const_iterator InputMappingIterator;


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
    vector< InputMapping * > myInputMappings;

    /**
     * This is a list with all the input events.
     * New ones are added at the beginning, and old ones are removed at the end.
     */
    list< InputEvent * > myInputEvents;


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
        // Free any allocated memory and resources

        // TODO: Delete all input events and mappings??
        //       Who owns the mappings??
    }


    //----------------------------------------------------------------------
    // Input Mapping methods

    /**
     * Returns a const iterator that can be used to iterate through the input mappings
     */
    virtual InputMappingIterator getInputMappings() const
    {
        return myInputMappings.begin();
    }


    /**
     * Adds a new input mapping at the specified index.
     *
     * @param inputMapping reference to the input mapping to add.
     * @param index the index in the input mapping sequence to add it to
     *              ( 0 = start, before any other).  If the index is outside the range,
     *              then it is added to the end (or beginning).
     */
    virtual void addInputMapping( InputMapping &inputMapping, int index )
    {
        // TODO
    }


    /**
     * Removes the specified input mapping. (but doesn't delete it).
     */
    virtual void removeInputMapping( InputMapping &inputMapping )
    {
        // TODO
    }


    //----------------------------------------------------------------------
    // Event Listener methods
	
    /**
     * Returns the InputEventListener that handles the specified event.
     * Does not send the event to the event listener,
     * so this method can also be queried to get the event listeners
     * currently bound to different keys or mouse buttons.
     *
     * @param event The event to deterimen the listener for.
     * @return The listener that handles the given event, or null if nobody handles it.
     */
    virtual InputEventListener *getListenerForEvent( InputEvent &event )
    {
        // TODO
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
     * This method handles the next event from the input queue,
     * sending it off to a listener, removing it from the queue, and deleting it.
     *
     * TODO: Do we need a queue for incoming events at all, or
     * can we just call this from the SDL callback function directly?
     */
    virtual void handleEvent()
    {
        // TODO
    }


    /**
     * Adds a new event to the event queue.
     *
     * TODO: Add parameters for all necessary fields to compose the event.
     */
    virtual void addEvent( /* various parameters.. */ )
    {
        // TODO
    }


    // TODO: Callback method that SDL events are sent to from SDL. (has to be non C++ method...?)
    //       It should call addEvent(), or perhaps handleEvent(...) ?


    // Implementation note about events and threads:

    // If we add events to a queue, we need an event dispatcher thread that
    // goes through the queue and calls event listeners.
    // This thread would then be the thread in which much of the UI and other
    // user driven functionality would be executed.  There might be problems
    // if other services, or the data model, have own threads..

    // Alternatively we could just handle the event from the SDL callback
    // function.  IIRC, SDL is single threaded?
    // Then everything would run in this thread, except heavy calculation or
    // network communication type things, that would have their own threads in
    // any case.

    // Personally I like alternative two better, because it seems simpler,
    // and (judging from Java programs that have the GUI and program logic in
    // the same thread normally) also works well.
    // -- zzorn

    // Btw, the GUI will likely have just one InputEventListener, that
    // calls method such as onMouseMove, onKeyPress, etc. of the GUI widgets.



}; // InputService

} // namespace dime

#endif


