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

#ifndef GUISERVICE_H
#define GUISERVICE_H

// Included headers from the current project
#include <framework/Service.h>
#include <services/input/InputService.h>
#include "Container.h"
#include <services/platform/SDLDrawDevice.h>

// Included custom library headers

// Included system headers
#include <string>
#include <vector>


namespace dime {

/**
 * This service provides a GUI widget system.
 * It can create various basic GUI Widgets, and connect them to listeners / data sources (of Data Model type?).
 * It also holds the root window.
 *
 *
 * @author Hans Häggström
 * @author Adam Gregory
 */
class GuiService : public Service, public SigC::Object

{

	//======================================================================
	// Private Variables
	//======================================================================
    private:

    Container myRootWidget;

	InputService *myInputService;
	
	SDLDrawDevice *myDrawTarget;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
    * Creates a new GuiService using default values.
    */
    GuiService();
    

    //----------------------------------------------------------------------
    // Destructor

    /**
    * Deletes a GuiService instance.
    */
    virtual ~GuiService()
    {
    }
    

    //----------------------------------------------------------------------
    // Getters

	/**
	 * Returns the root widget
	 */
	Container& getRootWidget()
	{
		return myRootWidget;
	}
	
    //----------------------------------------------------------------------
    // Setters

    //----------------------------------------------------------------------
    // Other public methods
	
	/**
	 * Sets the target DrawDevice target for GuiService to render Widgets onto.
	 */
	void setDrawTarget(SDLDrawDevice *target)
	{
		myDrawTarget = target;
	}

	/**
	 * Draws the GUI
	 */
	void refresh();
	
	/**
	 * Passes mouse motion events down to widget tree
	 */
	void MouseMotion(InputDevice *mouse, const SDLKey &key, InputMapping::InputSignalType signaltype);
	
	/**
	 * Passes mouse motion events down to widget tree
	 */
	void MouseClick(InputDevice *mouse, const SDLKey &key, InputMapping::InputSignalType signaltype);


    //----------------------------------------------------------------------
    // Methods inherited from Service
    /**
    * This method is used to start the service.
    * It takes care of aquiring needed resources, initializing
    * data structures, and so on. <p>
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

        return 0;
    }


	/**
	 * This method stops the service, and frees any used resources.
	 *
	 * @ param code code which represents the cause of the service halt
	 */
	virtual void stop( int code )
    {
        setRunning( false );
    }



    //======================================================================
    // Protected Methods
    //======================================================================
    protected:


    //======================================================================
    // Private Methods
    //======================================================================
    private:


    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:

    /**
    * Copy constructor not provided.
    */
    GuiService( const GuiService &source )
    {
    }


    /**
    * Assignment operator not provided.
    */
    GuiService &operator= ( const GuiService &source )
    {
        return *this;
    }


}; // End of class

} // End of application namespace

#endif




