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
#include "services/logging/LoggingService.h"
#if 0
//#include "services/datamodel/DataModelService.h"
#include "services/datamodel/IntProvider.h"
#include "services/datamodel/DataObject.h"
#include "services/datamodel/FloatProvider.h"
#endif
#include "framework/ConsoleBackend.h"

// System headers
#include <iostream>
#include <list>
#include <algorithm>
#include <strstream>

// Libraries we are using
#include <sigc++/object.h>
#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/handle_system.h>
#include <sigc++/signal_system.h>
#else
#include <sigc++/signal.h>
#endif
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
	    ConsoleBackend::getMainConsole()->registerCommand(META_LIST,this);
	    ConsoleBackend::getMainConsole()->registerCommand(META_REFRESH,this);
	    ConsoleBackend::getMainConsole()->registerCommand(META_ABORT,this);
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

		std::string metaserver = "metaserver.worldforge.org";

		msrv = new Eris::Meta("dime", metaserver, 10);
		msrv->GotServerCount.connect(SigC::slot(*this, &MetaserverService::gotServerCount));
		msrv->Failure.connect(SigC::slot(*this, &MetaserverService::gotFailure));
		msrv->ReceivedServerInfo.connect(SigC::slot(*this, &MetaserverService::receivedServerInfo));
		msrv->CompletedServerList.connect(SigC::slot(*this, &MetaserverService::completedServerList));
		listed = false;

#if 0
		PDataObject root = DataObject::getRoot();
		PDataObject servers = root->addChild("servers");        
		servers->addChild("metaserver", 
			new StringProvider(metaserver, "Metaserver used to retrieve server infos."));	
		
		// @todo: Add time stamp servers->addChild("Update",
		// new TimeProvider(

		PDataObject list = servers->addChild("list"); 
		list->setDescription("List of all servers found.");

		myStateDMP = new StringProvider("Listing servers.", "State of MetaserverService");
		servers->addChild("state", myStateDMP);
#endif

		return Service::OK;
	}

	/* Interface method for stopping this service 	*/
	void MetaserverService::stop(int code)
	{
		setStatus(Service::OK);
		setRunning( false );
#if 0
		PDataObject servers = DataObject::getRoot("/servers");
		servers->remove(); //HINT: This also frees myStateDMP
#endif
	}

	void MetaserverService::gotServerCount(int count)
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Got " << count << " game servers." << ENDM;
	}
    
    void MetaserverService::gotFailure(const string& msg)
    {
        LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Meta-server error: " << msg << ENDM;
		myStateDMP->setValue("Failure: " + msg);
    }
    
    void MetaserverService::receivedServerInfo(Eris::ServerInfo sInfo)
    {
#if 0
		PDataObject list = DataObject::getRoot("/servers/list");

		PDataObject entry = list->addChild();

		entry->addChild("host", new StringProvider(sInfo.getHostname(), 
			"Hostname or dotted-decimal IP of the server"));
		entry->addChild("name", new StringProvider(sInfo.getServername(), 
			"Human readable name of the server (set by the operator)"));
		entry->addChild("ruleset", new StringProvider(sInfo.getRuleset(), 
			"The game system this server is running (e.g. 'Acorn')"));
		entry->addChild("server", new StringProvider(sInfo.getRuleset(), 
			"Server program (e.g. 'Cyphesis' or 'Stage')"));
		entry->addChild("clients", new IntProvider(sInfo.getNumClients(), 
			"Number of clients connected to the server"));
		entry->addChild("ping", new IntProvider(sInfo.getPing(),
			"Estimated round-trip-time (ping) in milliseconds"));
		entry->addChild("uptime", new FloatProvider(sInfo.getUptime(),
			"Server uptime in seconds"));
#endif

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

	  myStateDMP->setValue("Server list completed.");

		// waiting for James to implement this
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)<< "Servers: " << msrv->getGameServerCount() << ENDM;
		//Eris::Serverlist whatever;
		svrl l = msrv -> getGameServerList ();
		
		strstream out;
		out << "Listing hostnames..." << endl;
		
		for(Iter i = l.begin(); i != l.end(); i++)
		{	
			//HINT: Always use .c_str() for compatibility to MSVC
			Eris::ServerInfo inf = *i;

			out << "Hostname: " << (i)->getHostname().c_str() << endl;
		}

		out.put(0);

		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << out.str() << ENDM;

		return;
	}

  void MetaserverService::runCommand(const std::string &command, const std::string &args)
  {
    if (!msrv) return;
    if (command == META_LIST){
    } else if (command == META_ABORT) {
      msrv->cancel();
    } else if (command == META_REFRESH) {
      msrv->refresh();
    }

    return;
  }
} // namespace dime

