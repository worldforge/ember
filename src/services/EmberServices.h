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



namespace Ember {

  // some forward declarations before we start
  class LoggingService;
  class ConfigService;
  class InputService;
  //class GuiService;
  class MetaserverService;
  class ServerService;
  class SoundService;
  class TestService;

/**
 * This is a singleton class that is used to access instances of all the
 * different Ember services.
 *
 * There's a getServiceName() method for each service. <p>
 *
 * TODO: Should this class also create the instances of the services,
 * or should it have set methods for them?  <p>
 *
 * Example: <p>
 * <code>
 *
 *   EmberServices.getInstance()->getLoggingService()->log( ... );  <br/>
 *   ... = EmberServices.getInstance()->getMetaServerService()->getMetaServerList();
 *
 * </code>
 *
 * @author Hans Häggström
 */
class EmberServices

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
    Ember::LoggingService *myLoggingService;

    /**
     * The instance of the ConfigService
     */
    Ember::ConfigService *myConfigService;

    /**
     * The instance of the InputService
     */
    Ember::InputService *myInputService;
	
    /**
     * The instance of the GuiService
     */
    //Ember::GuiService *myGuiService;

    /**
     * The instance of the MetaserverService
     */
    Ember::MetaserverService *myMetaserverService;

    /**
     * The instance of the ServerService
     */
    Ember::ServerService *myServerService;

    /**
     * The instance of the SoundService
     */
    Ember::SoundService *mySoundService;

    /**
     * The EmberServices singleton instance.
     */
    static EmberServices *theInstance;

    //======================================================================
    // Public Methods
    //======================================================================
    public:


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a EmberServices instance.
     */
    virtual ~EmberServices();
    


    //----------------------------------------------------------------------
    // Singleton

    /**
     * Returns the EmberServices instance.
     */
    static EmberServices *getInstance();
    


    //----------------------------------------------------------------------
    // Getters

    /**
     * Returns an instance of the TestService.
     */
    Ember::TestService *getTestService();
    

    /**
     * Returns an instance of the LoggingService
     */
    Ember::LoggingService *getLoggingService();

    /**
     * Returns an instance of the ConfigService
     */
    Ember::ConfigService *getConfigService();

    /**
     * Returns an instance of the InputService
     */
    Ember::InputService *getInputService();
	
    /**
     * Returns an instance of the GuiService
     */
    //Ember::GuiService *getGuiService();

    /**
     * Returns an instance of the MetaserverService
     */
    Ember::MetaserverService *getMetaserverService();

    /**
     * Returns an instance of the ServerService
     */
    Ember::ServerService *getServerService();

    /**
     * Returns an instance of the SoundService
     */
    Ember::SoundService *getSoundService();

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
    EmberServices();


    /**
     * Copy constructor not provided.
     */
    EmberServices( const EmberServices &source )
    {
    }


    /**
     * Assignment operator not provided.
     */
    EmberServices &operator= ( const EmberServices &source )
    {
        return *this;
    }


}; // End of class
} // End of application namespace

#endif


