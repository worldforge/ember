/*
    Copyright (C) 2002  Miguel Guzman Miranda [Aglanor]
    Based on YUP::Metacmd code by Adam Wendt

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

#include "MetaserverService.h"

#include <iostream>
#include <sigc++/object_slot.h>
#include <stdint.h>
#include <sigc++/object.h>
#include <Eris/Metaserver.h>
#include <Eris/ServerInfo.h>
#include <Eris/Utils.h>
#include <stdint.h>
#include <list>

#include <algorithm>

namespace dime
{

	typedef std::list<Eris::ServerInfo> svrl;
	typedef svrl::iterator Iter;


	/* ctor */
	MetaserverService::MetaserverService()
	{
	    setName("Metaserver Service");
		setDescription("Service for Metaserver session");
		// TODO(zzorn, 2002-01-19): Set the status of the service to OK.
		//        setStatus( Service::Status::OK );
	}
	
	/* dtor */
	MetaserverService::~MetaserverService()
	{
	    delete msrv;
	}
	
	/* Method for starting this service 	*/
	int MetaserverService::start()
	{
		setStatus(1);
        setRunning( true );

		msrv = new Eris::Meta("dime", "metaserver.worldforge.org", 1);
    	listed = false;

    	// waiting for James to implement this
		// cout << "Servers: " << msrv->getGameServerCount() << "endl";
		//Eris::Serverlist whatever;
		svrl l = msrv -> getGameServerList ();
		
		cout << "listing hostnames" << endl;
		
		for(Iter i = l.begin(); i != l.end(); i++)
		{	
			//HINT: Always use .data() for compatibility to MSVC
			cout << "Hostname: " << (i)->getHostname().data() << endl;
	    }
		
		return 0;
	
	}

	/* Interface method for stopping this service 	*/
	void MetaserverService::stop(int code)
	{
		setStatus(0);
		setRunning( false );
	}

	void MetaserverService::GotServerCount(int count)
	{
   		char str[1024];
    	cout << "Got" << count << "game servers." << endl;
	}	
	
	void MetaserverService::GotFailure(string msg)
	{
    	cout << "Got Meta-server error: " << msg << endl;
	}	
	
} // namespace dime

