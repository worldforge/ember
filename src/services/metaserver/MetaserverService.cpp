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

// Current project
#include <services/logging/LoggingService.h>

// System headers
#include <iostream>
#include <list>
#include <algorithm>
#include <strstream>

// Libraries we are using
#include <sigc++/object_slot.h>
#include <sigc++/object.h>
#include <Eris/Metaserver.h>
#include <Eris/ServerInfo.h>
#include <Eris/Utils.h>


using namespace std;

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
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Metaserver Service created" << ENDM;
		msrv = NULL;
	}

	/* dtor */
	MetaserverService::~MetaserverService()
	{
	    delete msrv;
	}
	
	/* Method for starting this service 	*/
	Service::Status MetaserverService::start()
	{
		setStatus(Service::OK);
		setRunning( true );

		msrv = new Eris::Meta("dime", "metaserver.worldforge.org", 1);
		msrv->GotServerCount.connect(SigC::slot(*this, &MetaserverService::gotServerCount));
		msrv->Failure.connect(SigC::slot(*this, &MetaserverService::gotFailure));
		msrv->ReceivedServerInfo.connect(SigC::slot(*this, &MetaserverService::receivedServerInfo));
		msrv->CompletedServerList.connect(SigC::slot(*this, &MetaserverService::completedServerList));
		listed = false;

		return Service::OK;
	}

	/* Interface method for stopping this service 	*/
	void MetaserverService::stop(int code)
	{
		setStatus(Service::OK);
		setRunning( false );
	}

	void MetaserverService::gotServerCount(int count)
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Got " << count << " game servers." << ENDM;
	}
    
    void MetaserverService::gotFailure(const string& msg)
    {
        LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Meta-server error: " << msg << ENDM;
    }
    
    void MetaserverService::receivedServerInfo(Eris::ServerInfo sInfo)
    {
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Got serverinfo:\n\r"
										  << "Hostname: " <<sInfo.getHostname()
										  << "\n\rServerName: "<<sInfo.getServername()
										  << "\n\rRuleset: "<<sInfo.getRuleset()
										  << "\n\rServer Type: "<<sInfo.getServer()
										  << "\n\rClients: "<<sInfo.getNumClients()
										  << " Ping: "<< sInfo.getPing()
										  << " Uptime: "<< sInfo.getUptime()
										  << ENDM;
    return;
  }
  void MetaserverService::completedServerList()
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Server List completed." << ENDM;
	  listed = true;

		// waiting for James to implement this
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)<< "Servers: " << msrv->getGameServerCount() << ENDM;
		//Eris::Serverlist whatever;
		svrl l = msrv -> getGameServerList ();
		
		strstream out;
		out << "Listing hostnames..." << endl;
		
		for(Iter i = l.begin(); i != l.end(); i++)
		{	
			//HINT: Always use .data() for compatibility to MSVC
			out << "Hostname: " << (i)->getHostname().data() << endl;
		}
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << out.str() << ENDM;

		return;
	}

} // namespace dime

