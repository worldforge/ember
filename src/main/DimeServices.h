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

#ifndef DIME_SERVICES_H
#define DIME_SERVICES_H

// Include headers from the current project here
#include <services/test/TestService.h>
#include <services/logging/LoggingService.h>
#include <services/input/InputService.h>

// Include custom library headers here

// Include system headers here

namespace dime {

/**
 * This is a singleton class that is used to access instances of all the
 * different Dime services.
 *
 * There's a getServiceName() method for each service. <p>
 *
 * TODO: Should this class also create the instances of the services,
 * or should it have set methods for them?  <p>
 *
 * Example: <p>
 * <code>
 *
 *   DimeServices.getInstance()->getLoggingService()->log( ... );  <br/>
 *   ... = DimeServices.getInstance()->getMetaServerService()->getMetaServerList();
 *
 * </code>
 *
 * @author Hans Häggström
 */
class DimeServices

{
    //======================================================================
    // Inner Classes
    //======================================================================


    //======================================================================
    // Public Constants
    //======================================================================
    public:


    //======================================================================
    // Private Constants
    //======================================================================
    private:


    //======================================================================
    // Private Variables
    //======================================================================
    private:

    
    /**
     * The instance of the LoggingService
     */
    dime::LoggingService *myLoggingService;

	/*
	 * The instance of the InputService
     */
    dime::InputService *myInputService;

    /**
     * The DimeServices singleton instance.
     */
    static DimeServices *theInstance;

    //======================================================================
    // Public Methods
    //======================================================================
    public:


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a DimeServices instance.
     */
    virtual ~DimeServices()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Singleton

    /**
     * Returns the DimeServices instance.
     */
    static DimeServices *getInstance()
    {
        if( theInstance )
        {
            return theInstance;
        }
        else
        {
            theInstance = new DimeServices();
            return theInstance;
        }
    }


    //----------------------------------------------------------------------
    // Getters

    /**
     * Returns an instance of the TestService.
     */
    dime::TestService *getTestService()
    {
        // TODO
        return NULL;
    }

    /**
     * Returns an instance of the LoggingService
     */
    dime::LoggingService *getLoggingService()
    {
        return myLoggingService;
    }

	/**
     * Returns an instance of the InputService
     */
    dime::InputService *getInputService()
    {
        return myInputService;
    }

    //----------------------------------------------------------------------
    // Setters

    //----------------------------------------------------------------------
    // Other public methods


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
     * The constructor is private, and only used to create the singleton instance.
     */
    DimeServices()
    {
        myLoggingService = new dime::LoggingService();
		myInputService	 = new dime::InputService();
    }


    /**
     * Copy constructor not provided.
     */
    DimeServices( const DimeServices &source )
    {
    }


    /**
     * Assignment operator not provided.
     */
    DimeServices &operator= ( const DimeServices &source )
    {
        return *this;
    }


}; // End of class
} // End of application namespace

#endif


