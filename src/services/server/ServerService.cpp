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
  ServerService::ServerService(const std::string& host, short port ) : myConn(NULL), myLobby(NULL),
                                                                        myPort(port), myHost(host)
	{
	    setName("Server Service");
		setDescription("Service for Server session");
	}
	
	/* dtor */
	ServerService::~ServerService()
	{
//	    delete myConn;
	}
	
	/* Method for starting this service 	*/
	Service::Status ServerService::start()
	{
		setStatus(Service::OK);
		setRunning( true );

		// Create new instance of myConn the constructor sets the
		// singleton instance up.  Do _not_ use Connection::Instance()
		// this does not create a new connection.
	  // We are connected without debuging enabled thus the false
		myConn = new Connection("dime",false);

		// Bind failure signal
		myConn->Failure.connect(SigC::slot(*this, &ServerService::gotFailure));
		myConn->Connected.connect(SigC::slot(*this, &ServerService::connected));
		myConn->Disconnected.connect(SigC::slot(*this, &ServerService::disconnected));
		myConn->Disconnecting.connect(SigC::slot(*this, &ServerService::disconnecting));
		myConn->StatusChanged.connect(SigC::slot(*this, &ServerService::statusChanged));
		myConn->Timeout.connect(SigC::slot(*this, &ServerService::timeout));

		try {
		    // If the connection fails here an exception is thrown
		    myConn->connect( myHost, myPort );
		}
		catch (...)
		  {
		    //delete myConn;
		    myConn = NULL;
		    return Service::FAILURE;
		  }

		return Service::OK;
	
	}

	/* Interface method for stopping this service 	*/
	void ServerService::stop(int code)
	{
		setStatus(Service::OK);
		setRunning( false );
		myConn->disconnect();
		//delete myConn;
		myConn=NULL;
	}
	
	void ServerService::gotFailure(const string& msg)
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Server error: " << msg << ENDM;
	}	
	
	void ServerService::connected()
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Connected"<< ENDM;
    // Set up the lobby
	  myLobby=Lobby::instance();
	  myLobby->SightPerson.connect(SigC::slot(*this,&ServerService::sightPerson));
	  myLobby->PrivateTalk.connect(SigC::slot(*this,&ServerService::privateTalk));
	  myLobby->LoggedIn.connect(SigC::slot(*this,&ServerService::loggedIn));
	}

	bool ServerService::disconnecting()
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Disconnecting"<< ENDM;
	  // NULL out lobby so noone gets tempted to play with an unconnected lobby
	  myLobby=NULL;
	  return true;
	}

	void ServerService::disconnected()
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Disconnected"<< ENDM;
	}

	void ServerService::statusChanged(Eris::BaseConnection::Status status)
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Status Changed to: "<<status<<ENDM;
	}

	void ServerService::timeout(Eris::BaseConnection::Status status)
	{
	  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Connection Timed Out"<< ENDM;
	}

	void ServerService::sightPerson(Eris::Person* person)
	{
	}

	void ServerService::privateTalk(const std::string& str1, const std::string& str2)
	{
	}

	void ServerService::loggedIn( const Atlas::Objects::Entity::Player& player )
	{
	}
	
} // namespace dime

