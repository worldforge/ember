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

#ifndef TESTSERVICE_H
#define TESTSERVICE_H

#include "../AbstractService.h"
#include <string>

namespace dime {
	namespace services {
		namespace test {

/**
 * Basic test service for testing and debugging.
 *
 * @author Miguel Guzman Miranda [Aglanor]
 *
 * @see dime::Service
 * @see dime::AbstractService
 */
class TestService: public AbstractService
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:
		std::string myTestMessage;
		int myTestNumber;

		
    //----------------------------------------------------------------------
    // Constructors & Destructor

	public:
	
    /** Creates a new TestService using default values. */
    TestService();


    /** Deletes a Service instance. */
    ~TestService()
    {
    }


    //----------------------------------------------------------------------
    // Getters & Setters

    //----------------------------------------------------------------------
    // Methods
	
   int start();

   void stop(int code) ;

}; // TestService

		} // namespace testservice
	} // namespace services
} // namespace dime

#endif


