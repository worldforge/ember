/*
    Copyright (C) 2002  Miguel Guzman Miranda [Aglanor], Martin Pollard (Xmp)
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

#ifndef METASERVERSERVICE_H
#define METASERVERSERVICE_H

#include <framework/Service.h>
#include <framework/ConsoleObject.h>

#include <sigc++/object.h>
#include <Eris/Metaserver.h>
#include <Eris/ServerInfo.h>

#include <string>

namespace Ember {

/**
 * Ember Metaserver Service
 *
 * @author Miguel Guzman Miranda [Aglanor]
 *
 * @see Ember::Service
 * @see Ember::ServerService
 * @see Ember::ConsoleObject
 */
class MetaserverService: public Service,
  public ConsoleObject, virtual public SigC::Object
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:

    Eris::Meta * msrv;
    Eris::ServerList serverlist;
    bool listed;
    //StringProvider * myStateDMP;
    static const char * const META_REFRESH;
    static const char * const META_ABORT;
    static const char * const META_LIST;


		
    //----------------------------------------------------------------------
    // Constructors & Destructor

	public:
	
    /** Creates a new ConfigService using default values. */
    MetaserverService();


    /** Deletes a ConfigService instance. */
    ~MetaserverService();

    //----------------------------------------------------------------------
    // Getters & Setters

    //----------------------------------------------------------------------
    // Methods
	
    Service::Status start();

    void stop(int code) ;
	
	
    void gotFailure(const std::string& msg);

    void receivedServerInfo(const Eris::ServerInfo& sInfo);
	
    void completedServerList(int count);

    /**
     * This is the function that needs to be extended to use the console.
     * command is a command that has been previously registered with the console
     * args is the argument string that has been provided for the command
     */ 
    virtual void runCommand(const std::string &command, const std::string &args);

}; //MetaserverService

} // namespace Ember

#endif


