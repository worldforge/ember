/*
    Copyright (C) 2002  Martin Pollard (Xmp)

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

#ifndef SERVERSERVICE_H
#define SERVERSERVICE_H

#include <framework/Service.h>

#include <Eris/Connection.h>

#include <sigc++/object.h>

namespace dime {

/**
 * Dime Server Service
 *
 * @author Martin Pollard (Xmp)
 *
 * @see dime::Service
 * @see dime::MetaserverService
 */
class ServerService: public Service, public SigC::Object
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:

  /** 
   * Holds our connection to the server
   */
    Eris::Connection *myConn;

    short myPort;
    string myHost;

		
    //----------------------------------------------------------------------
    // Constructors & Destructor

	public:
	
    /** Creates a new ServerService using default values. */
    ServerService(const std::string& host, short port = 6767);


    /** Deletes a ServerService instance. */
    ~ServerService();

    //----------------------------------------------------------------------
    // Getters & Setters

    //----------------------------------------------------------------------
    // Methods
	
	int start();

	void stop(int code);
	
	void GotFailure(const string& msg);

}; //ServerService

} // namespace dime

#endif
