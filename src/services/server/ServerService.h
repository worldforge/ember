/*
    Copyright (C) 2002  Miguel Guzman Miranda [Aglanor]
    Based on Silence-c++ by James and YUP code by Adam Wendt

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
#include <Eris/Metaserver.h>
#include <Eris/Utils.h>
#include <stdint.h>
#include <sigc++/object.h>
#include <Eris/Metaserver.h>
#include <Eris/ServerInfo.h>
#include <Eris/Utils.h>

namespace dime {

/**
 * Dime Server Service
 *
 * @author Miguel Guzman Miranda [Aglanor]
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

    Eris::Meta * msrv;
    //Eris::Serverlist serverlist;
    bool listed;

		
    //----------------------------------------------------------------------
    // Constructors & Destructor

	public:
	
    /** Creates a new ConfigService using default values. */
    ServerService();


    /** Deletes a ConfigService instance. */
    ~ServerService();

    //----------------------------------------------------------------------
    // Getters & Setters

    //----------------------------------------------------------------------
    // Methods
	
	int start();

	void stop(int code) ;

   	void GotServerCount(int count);
	
	void GotFailure(string msg);
	
	void poll();

}; //MetaserverService

} // namespace dime

#endif


