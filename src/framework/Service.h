/*
    Copyright (C) 2002  Miguel Guzman Miranda [Aglanor]

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

// Include library headers here

// Include system headers here
#include <string>

namespace dime {

/**
 * Abstract class for interface common denominator of services.
 *
 * Establishes the basic variables and methods to be shared by all services.
 *
 * @author Miguel Guzman Miranda
 *
 * @see OtherSubsystem::AnOtherRelatedClass
 * NOTE: Add other related classes here, doxygen will create links to them.
 */
class Service

{
    //======================================================================
    // Public Constants
    //======================================================================
    public:

    /**
     * Contains the different error states that a service can be in.  <p>
     * <ul>
     * <li> OK          - Service working normally.  </li>
     * <li> NOTE        - Service working, but there is something that may need user attention.  </li>
     * <li> WARNING     - Service is working to some degree, but there was some problems encountered, or non-critical parts that are not working.  </li>
     * <li> FAILURE     - Service is not working.  A resource it depends on is not present, or the service is unimplemented.  </li>
     * <li> CRITICAL    - Service detected internal errors in itself or the system that can lead to data loss or other serious problems.  </li>
     * </ul>
     *
     * (The difference between FAILURE and CRITICAL is basically that Errors
     * mean that the rest of the systm may still work fine, while critical means
     * that the whole program is likely to be unstable.)
     */
	enum Status
	{
		OK       = 0,
		NOTE     = 1,
		WARNING  = 2,
		FAILURE  = 3,
		CRITICAL = 4
	};


    //======================================================================
    // Private Variables
    //======================================================================
    private:

    	/** Stores the unique name of the service */
	std::string myName;

	/** Stores the description of the service */
	std::string myDescription;

	/** Tells if the service is running or not */
	bool myRunning;

	/** Current status code */
	Status myStatus;

	/** Textual description of the current status, especially if it is some problem. */
	std::string myStatusText;
		

    //======================================================================
    // Public methods
    //======================================================================
	public:
	
    //----------------------------------------------------------------------
    // Constructors & Destructor

    /** Creates a new Service using default values. */
    Service()
    {
      setName( "" );
      setDescription( "" );
      setStatusText( "" );
      myStatus       = OK;
      myRunning      = false;
    }


    /** Copy constructor. */
    Service( const Service &source )
    {
        // Use the assignment operator to initialize the new class.
        *this = source;
    }


    /** Assignment operator. */
    virtual Service &operator= ( const Service &source )
    {
      myName         = source.myName;
      myDescription  = source.myDescription;

      myRunning      = false;    // The new service has not been initialized...
      myStatus       = OK;
      myStatusText   = "";       // Initial value

      // Return reference to this instance.
      return *this;
    }

    /** Deletes a Service instance. */
    virtual ~Service()
    {
    }


    //----------------------------------------------------------------------
    // Getters

    /** Returns the name of this Service. */
    virtual std::string getName() const
    {
        return myName;
    }


    /** Returns the description of this Service. */
    virtual std::string getDescription() const
    {
        return myDescription;
    }


    /** Returns the status of this Service. */
    virtual Service::Status getStatus() const
    {
        return myStatus;
    }


    /** Returns true if the service is currently running. */
    virtual bool isRunning()
    {
    	return myRunning;
    }


    /**
     * Returns the textual status message for this Service.
     * It is a description of the current status,
     * especially if there is some problem.
     * (If everything is fine, then this can be empty, as the status code can be
     * used to determine this.)
     */
    virtual std::string getStatusText()
    {
    	return myStatusText;
    }


    //----------------------------------------------------------------------
    // Methods
	
    /**
     * This method is used to start the service.
     * It should take care of aquiring needed resources, initializing
     * data structures, and so on. <p>
     *
     * If the initialization suceeds, it should also call setRunning( true )
     * to indicate that the service is running.  <p>
     *
     * If initialization fails, it should set appropriate status code and
     * status text.  It could also write an entry into a log through the logging
     * service.  <p>
     *
     * This method must be implemented by all inheriting classes.  <p>
     *
     *
     * @returns success or error code
     */
    virtual Status start() = 0;


    /**
     * This method stops the service, and frees any used resources.
     * If the service has no special resources that need to be freed,
     * or de-initialization to be done, this
     * method can be left to the default implementation (which just sets the
     * running state to false).  <p>
     *
     * Otherwise this method should be overridden, and setRunning( false )
     * should be called if the service was stopped.  <p>
     *
     * @ param code code which represents the cause of the service halt
     * TODO(zzorn): What do we need it for?
     */
    virtual void stop( int code )
    {
        setRunning( false );
    }

	
    //======================================================================
    // Protected methods
    //======================================================================
    protected:
	
    //----------------------------------------------------------------------
    // Setters

    /** Sets the name of this Service. */
    virtual void setName( const std::string& name )
    {
    	myName = name;
    }


    /** Sets the description of this Service. */
    virtual void setDescription( const std::string& description )
    {
    	myDescription = description;
    }


    /* *
     * Sets the description of this Service.
     */
    virtual void setStatus( Service::Status status )
    {
        myStatus = status;
    }


    /** Specifies wether this service is currently running or not. */
    virtual void setRunning( bool running )
    {
    	myRunning = running;
    }


    /**
     * Sets the textual status message for this Service.
     * It is a description of the current status,
     * especially if there is some problem.
     * (If everything is fine, then this can be empty, as the status code can be
     * used to determine this.)
     */
    virtual void setStatusText( const std::string& statusText )
    {
    	myStatusText = statusText;
    }


}; // Service

} // namespace dime

#endif


