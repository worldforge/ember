/*
    Copyright (C) 2002 Martin Pollard (Xmp)

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

#include "ServerService.h"
#include "services/logging/LoggingService.h"

#include <sigc++/object_slot.h>
#include <sigc++/object.h>
#include <Eris/Connection.h>

#include <list>
#include <algorithm>
#include <iostream>

using namespace Eris;


namespace dime
{


	/* ctor */
  ServerService::ServerService(const std::string& host, short port ) : myHost(host), myPort(port)
	{
	    setName("Server Service");
		setDescription("Service for Server session");
		// TODO(zzorn, 2002-01-19): Set the status of the service to OK.
		//        setStatus( Service::Status::OK );
	}
	
	/* dtor */
	ServerService::~ServerService()
	{
	    delete myConn;
	}
	
	/* Method for starting this service 	*/
	int ServerService::start()
	{
		setStatus(1);
		setRunning( true );

		// Create new instance of myConn
	        // We are connected without debuging enabled thus the false
		myConn = new Connection("dime", false);

		// Bind failure signal
		myConn->Failure.connect(SigC::slot(*this, &ServerService::GotFailure));
		myConn->Connected.connect(SigC::slot(*this, &ServerService::Connected));
		myConn->Disconnected.connect(SigC::slot(*this, &ServerService::Disconnected));
		myConn->Disconnecting.connect(SigC::slot(*this, &ServerService::Disconnecting));
		myConn->StatusChanged.connect(SigC::slot(*this, &ServerService::StatusChanged));
		myConn->Timeout.connect(SigC::slot(*this, &ServerService::Timeout));

		try {
		  myConn->connect( myHost, myPort );
		}
		catch (...)
		  {
		    // If the connection fails here an exception is thrown
		    delete myConn;
		    myConn = NULL;
		    return -1;
		  }

		return 0;
	
	}

	/* Interface method for stopping this service 	*/
	void ServerService::stop(int code)
	{
		setStatus(0);
		setRunning( false );
		myConn->disconnect();
		delete myConn;
		myConn=NULL;
	}
	
	void ServerService::GotFailure(const string& msg)
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Server error: " << msg << ENDM;
	}	
	
	void Connected()
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Connected"<< ENDM;
	}

	bool Disconnecting()
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Disconnecting"<< ENDM;
	  return true;
	}

	void Disconnected()
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Disconnected"<< ENDM;
	}

	void StatusChanged(Eris::BaseConnection::Status status)
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Status Changed to: "<<status<<ENDM;
	}

	void Timeout(Eris::BaseConnection::Status status)
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Connection Timed Out"<< ENDM;
	}

	
} // namespace dime

