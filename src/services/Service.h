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

// Include system headers here
#include <string>


// Include library headers here

// Include other headers of the current program here

namespace dime {
	namespace services {

/**
 * Abstract class for interface common denominator of services.
 *
 * Stablishes the basic variables and methods to be shared by all services.
 *
 * @author Miguel Guzman Miranda
 *
 * @see OtherSubsystem::AnOtherRelatedClass
 * NOTE: Add other related classes here, doxygen will create links to them.
 */
class Service

{
    //======================================================================
    // Private Variables
    //======================================================================
    private:
    	/** Stores the unique name of the service */
		std::string myName;
		/** Stores the description of the service */
		std::string myDescription;
		/** Tells if the service is active or not */
		bool myActive;
		/** Current status code */
		int myStatus;
		

    //----------------------------------------------------------------------
    // Constructors & Destructor

	public:
	
    /** Creates a new Service using default values. */
    Service()
    {
    }


    /** Copy constructor. */
    Service( const Service &source )
    {
    }


    /** Assignment operator. */
    Service &operator= ( const Service &source )
    {
    }

    /** Deletes a Service instance. */
    ~Service()
    {
    }

    //----------------------------------------------------------------------
    // Getters & Setters

    /** Returns the name of this Service. */
    std::string getName() const
    {
        return myName;
    }

    /** Sets the name of this Service. */
    void setName( std::string name )
    {
    	myName = name;
    }

    /** Returns the description of this Service. */
    std::string getDescription() const
    {
        return myDescription;
    }

    /** Sets the description of this Service. */
    void setDescription( std::string description )
    {
    	myDescription = description;
    }

    /** Returns the status of this Service. */
    int getStatus() const
    {
        return myStatus;
    }

    /** Sets the description of this Service. */
    void setStatus( int status )
    {
    	myStatus = status;
    }

    /** Ask for active state */
    bool isActive()
    {
    	return myActive;
    }


    //----------------------------------------------------------------------
    // Methods
	
	/**
	 *	Interface method for starting the service
	 *
	 * @ returns success or error code
	 */
    virtual int start()=0;

	/**
	 *	Interface method for stopping the service
	 *
	 * @ param code code which represents the cause of the service halt
	 */
	virtual void stop(int code)=0;
	
	
}; // Service

	} // namespace services
} // namespace dime

#endif


