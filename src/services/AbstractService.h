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

#ifndef ABSTRACTSERVICE_H
#define ABSTRACTSERVICE_H

// Include system headers here


// Include library headers here

#include "Service.h"
#include <string>

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
class AbstractService: public Service
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:
		
    //----------------------------------------------------------------------
    // Constructors & Destructor

	public:
	
    /** Creates a new AbstractService using default values. */
    AbstractService()
    {
    }


    /** Copy constructor. */
    AbstractService( const Service &source )
    {
    }


    /** Assignment operator. */
    AbstractService &operator= ( const Service &source )
    {
    }

    /** Deletes a Service instance. */
    ~AbstractService()
    {
    }

    //----------------------------------------------------------------------
    // Getters & Setters

    //----------------------------------------------------------------------
    // Methods
	
}; // Service

	} // namespace services
} // namespace dime

#endif


