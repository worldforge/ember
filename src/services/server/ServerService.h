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
#include <Eris/Lobby.h>

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

  /**
   * Holds the lobby of this server
   */
    Eris::Lobby *myLobby;

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
	
	Service::Status start();

	void stop(int code);

    //----------------------------------------------------------------------
    // Callbacks from Eris

	// Connection Callbacks
	
	void gotFailure(const string& msg);

	void connected();

	bool disconnecting();

	void disconnected();

	void statusChanged(Eris::BaseConnection::Status);

	void timeout(Eris::BaseConnection::Status);

	// Lobby Callbacks

	void sightPerson(Eris::Person*);

	void privateTalk(const std::string&, const std::string&); 

	void loggedIn( const Atlas::Objects::Entity::Player& );
}; //ServerService

} // namespace dime

#endif
