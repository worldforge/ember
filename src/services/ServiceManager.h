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

#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include "Service.h"

#include <string>
#include <vector>

namespace dime {
   namespace services {

/**
 * Service Manager: keeps track and controls the dime services.
 *
 * Maintains a list of services and performs operations on them.
 *
 * @author Miguel Guzman Miranda [Aglanor]
 *
 */
class ServiceManager

{
    //======================================================================
    // Private Variables
    //======================================================================
    private:
        /** Dime Service Vector */
        vector < Service* > myServiceVector;

    //----------------------------------------------------------------------
    // Constructors & Destructor

	public:
	
    /** Creates a new Service using default values. */
    ServiceManager();

    /** Deletes an instance. */
    ~ServiceManager();


    //----------------------------------------------------------------------
    // Getters & Setters


    //----------------------------------------------------------------------
    // Methods

   /**
    * Starts all services.
    * 
    */
    void startAll()
    {
        // TODO..
    }
   
   /**
    * Stops all services.
    * 
    * @param code Cause of the halt
    */
    void stopAll(int code);

   /**
    * Lists all services to the standard output.
    *
    */
    void listAll();
     
   /**
    * Returns pointers to all registered service instances.
    *
    * @return iterator to go through a collection of pointers to services.
    */
    std::vector< Service* >::iterator getServices()
    {
        return myServiceVector.begin();
    }
     
   /** 
    * Deletes all the services, freeing the memory.
    * TODO(zzorn): I don't think we want to delete the services from here, as
    *              we don't own them (they are just registered here with a pointer,
    *              they are created elsewhere and for clarity should probably be
    *              deleted in the same place where they were created.
    */
   void deleteAll();

   
   /**
    * Registers a new service and adds it to the list of registered services, if it isn't
    * already registered.
    * @return true if successfully added & registered, false otherwise.
    */
   bool addService( Service *service );

   
   /**
    * Unregisters a service and removes it from the list of registered services,
    * if it was registered.  If it was removed sucessfully, true is returned.
    * If it wasn't registered or there was som other problem, false is returned.
    * @return true if successfully removed and unregistered, false otherwise.
    */
   bool removeService( Service *service );


    //======================================================================
    // Unimplemented copy constructor and assignment operator
    //======================================================================
    private:

    /** Copy constructor not provided. */
    ServiceManager( const ServiceManager &source )
    {
    }

    /** Assignment operator not provided. */
    ServiceManager &operator= ( const ServiceManager &source )
    {
    }

   
}; // Service

	} // namespace services
} // namespace dime

#endif


