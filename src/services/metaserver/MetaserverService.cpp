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
#include "services/config/ConfigService.h"
#include "services/EmberServices.h"
#include "framework/ConsoleBackend.h"

// System headers
#include <iostream>
#include <list>
#include <algorithm>
#include <sstream>

// Libraries we are using

#include <sigc++/signal.h>

#include <Eris/Metaserver.h>
#include <Eris/ServerInfo.h>


using namespace std;

namespace Ember
{

  typedef std::list<Eris::ServerInfo> svrl;
  typedef svrl::iterator Iter;

  MetaserverService::MetaserverService() :mMetaserver(0)
  , MetaRefresh("meta_refresh", this, "Refresh the meta server listing.")
  , MetaAbort("meta_abort", this, "Abort the meta server update process.")
//   , MetaList("meta_list", this, "List all servers.")
  {
    setName("Metaserver Service");
    setDescription("Service for Metaserver session");
    // TODO(zzorn, 2002-01-19): Set the status of the service to OK.
    //        setStatus( Service::Status::OK );
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Metaserver Service created" << ENDM;
  }

  /* dtor */
  MetaserverService::~MetaserverService()
  {
    delete mMetaserver;
  }

  /* Method for starting this service 	*/
  Service::Status MetaserverService::start()
  {
    setStatus(Service::OK);
    setRunning( true );

	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();

	std::string metaserverHostname;
	if (configSrv->itemExists("servers", "metaserver")) {
		metaserverHostname = std::string(configSrv->getValue("servers", "metaserver"));
	} else {
		metaserverHostname = "metaserver.worldforge.org";
	}
	
    mMetaserver = new Eris::Meta(metaserverHostname, 10);
    mMetaserver->Failure.connect(sigc::mem_fun(*this, &MetaserverService::gotFailure));
    mMetaserver->ReceivedServerInfo.connect(sigc::mem_fun(*this, &MetaserverService::receivedServerInfo));
    mMetaserver->CompletedServerList.connect(sigc::mem_fun(*this, &MetaserverService::completedServerList));
   
    
//     std::string metaserver = "metaserver.worldforge.org";
// 
//     msrv = new Eris::Meta(metaserver, 10);
//     msrv->Failure.connect(SigC::slot(*this, &MetaserverService::gotFailure));
//     msrv->ReceivedServerInfo.connect(SigC::slot(*this, &MetaserverService::receivedServerInfo));
//     msrv->CompletedServerList.connect(SigC::slot(*this, &MetaserverService::completedServerList));
//     listed = false;


    return Service::OK;
  }

  /* Interface method for stopping this service 	*/
  void MetaserverService::stop(int code)
  {
    setStatus(Service::OK);
    setRunning( false );
  }

    
  void MetaserverService::gotFailure(const string& msg)
  {
  	S_LOG_WARNING("Got Meta-server error: " << msg);
  }
    
  void MetaserverService::receivedServerInfo(const Eris::ServerInfo& sInfo)
  {

    S_LOG_INFO("Got serverinfo:\n\r"
      << "Hostname: " <<sInfo.getHostname()
      << "\n\rServerName: "<<sInfo.getServername()
      << "\n\rRuleset: "<<sInfo.getRuleset()
      << "\n\rServer Type: "<<sInfo.getServer()
      << "\n\rClients: "<<sInfo.getNumClients()
      << " Ping: "<< sInfo.getPing()
      << " Uptime: "<< (int)sInfo.getUptime());
  }

  void MetaserverService::completedServerList(int count)
  {
    S_LOG_INFO("Server List completed.");
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)<< "Servers: " << count << ENDM;
	
//     stringstream out;
//     out << "Listing hostnames..." << endl;
// 		
//     for(int i = 0; i < count; i++)
//       {	
// 	//HINT: Always use .c_str() for compatibility to MSVC
// 	Eris::ServerInfo inf = mMetaserver->getInfoForServer(i);
// 	
// 
// 	out << "Hostname: " << inf.getHostname().c_str() << endl;
//       }
// 
// 
//     S_LOG_INFO( out.str());

  }

Eris::Meta& MetaserverService::getMetaServer() const
{
	return *mMetaserver;
}


  void MetaserverService::runCommand(const std::string &command, const std::string &args)
  {
    if (!mMetaserver) return;
/*    if (MetaList == command){
    } else */
    if (MetaAbort == command) {
      mMetaserver->cancel();
    } else if (MetaRefresh == command) {
      mMetaserver->refresh();
    }

    return;
  }
} // namespace Ember
