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
#include <services/config/ConfigService.h>
#include <services/input/InputService.h>
#include <services/gui/GuiService.h>
#include <services/sound/SoundService.h>
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
#include <services/metaserver/MetaserverService.h>
#include <services/server/ServerService.h>
#endif

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

    /**
     * The instance of the ConfigService
     */
    dime::ConfigService *myConfigService;

    /**
     * The instance of the InputService
     */
    dime::InputService *myInputService;
	
    /**
     * The instance of the GuiService
     */
    dime::GuiService *myGuiService;

    /**
     * The instance of the MetaserverService
     */
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
    dime::MetaserverService *myMetaserverService;
#endif

    /**
     * The instance of the ServerService
     */
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
    dime::ServerService *myServerService;
#endif

    /**
     * The instance of the SoundService
     */
    dime::SoundService *mySoundService;

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
      delete myGuiService;
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
		delete myMetaserverService;
		delete myServerService;
#endif
    }


    //----------------------------------------------------------------------
    // Singleton

    /**
     * Returns the DimeServices instance.
     */
    static DimeServices *getInstance()
    {
        if( !theInstance )
        {
            theInstance = new DimeServices();
        }

	return theInstance;
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
        return dime::LoggingService::getInstance();
    }

    /**
     * Returns an instance of the ConfigService
     */
    dime::ConfigService *getConfigService()
      {
	return dime::ConfigService::getInstance();
      }

    /**
     * Returns an instance of the InputService
     */
    dime::InputService *getInputService()
    {
        return dime::InputService::getInstance();
    }
	
    /**
     * Returns an instance of the GuiService
     */
    dime::GuiService *getGuiService()
    {
        if (myGuiService == NULL)
	  myGuiService = new dime::GuiService();

	return myGuiService;
    }

    /**
     * Returns an instance of the MetaserverService
     */
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
    dime::MetaserverService *getMetaserverService()
    {
        if (myMetaserverService == NULL)
	  myMetaserverService = new dime::MetaserverService();
	return myMetaserverService;
    }
#endif

    /**
     * Returns an instance of the ServerService
     */
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
    dime::ServerService *getServerService()
    {
        if (myServerService == NULL)
	  myServerService = new dime::ServerService();
	return myServerService;
    }
#endif

    /**
     * Returns an instance of the SoundService
     */
    dime::SoundService *getSoundService()
    {
        if (mySoundService == NULL)
	  mySoundService = new dime::SoundService();
	return mySoundService;
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
		myGuiService = NULL;
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
		myMetaserverService = NULL;
		myServerService = NULL;
#endif
		mySoundService = NULL;
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


