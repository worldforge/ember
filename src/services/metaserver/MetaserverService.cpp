/*
 Copyright (C) 2002  Miguel Guzman Miranda [Aglanor]
 Copyright (C) 2005	Erik Hjortsberg
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "MetaserverService.h"

// Current project
#include "services/config/ConfigService.h"
#include "services/EmberServices.h"
#include "framework/LoggingInstance.h"
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"


#include <Eris/Metaserver.h>
#include <Eris/ServerInfo.h>
#include <Eris/Session.h>

// System headers
#include <iostream>
#include <list>
#include <algorithm>
#include <sstream>

// Libraries we are using

#include <sigc++/signal.h>


using namespace std;

namespace Ember
{

typedef std::list<Eris::ServerInfo> svrl;
typedef svrl::iterator Iter;

MetaserverService::MetaserverService(Eris::Session& session) :
	Service("Metaserver"), mSession(session), mMetaserver(nullptr), MetaRefresh("meta_refresh", this, "Refresh the meta server listing."), MetaAbort("meta_abort", this, "Abort the meta server update process.")
//   , MetaList("meta_list", this, "List all servers.")
{
	S_LOG_INFO("Metaserver Service created");
}

MetaserverService::~MetaserverService()
{
	delete mMetaserver;
}

/* Method for starting this service 	*/
bool MetaserverService::start()
{
	setRunning(true);

	ConfigService& configSrv = EmberServices::getSingleton().getConfigService();

	std::string metaserverHostname;
	if (configSrv.itemExists("metaserver", "server")) {
		metaserverHostname = std::string(configSrv.getValue("metaserver", "server"));
	} else {
		metaserverHostname = "metaserver.worldforge.org";
	}

	S_LOG_INFO("Connecting to meta server at address " << metaserverHostname << ".");
	mMetaserver = new Eris::Meta(mSession.getIoService(), mSession.getEventService(), metaserverHostname, 20);
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


	return true;
}

void MetaserverService::gotFailure(const string& msg)
{
	S_LOG_WARNING("Got Meta-server error: " << msg);
}

void MetaserverService::receivedServerInfo(const Eris::ServerInfo& sInfo)
{

	S_LOG_VERBOSE("Got serverinfo:\n"
			<< "Hostname: " <<sInfo.getHostname()
			<< "\nServerName: "<<sInfo.getServername()
			<< "\nRuleset: "<<sInfo.getRuleset()
			<< "\nServer Type: "<<sInfo.getServer()
			<< "\nClients: "<<sInfo.getNumClients()
			<< "\nPing: "<< sInfo.getPing()
			<< "\nUptime: "<< (int)sInfo.getUptime()
			<< "\nEntities: "<< sInfo.getEntities());
}

void MetaserverService::completedServerList(int count)
{
	S_LOG_INFO("Server List completed.");
	S_LOG_INFO("Servers: " << count);

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
	if (!mMetaserver)
		return;
	/*    if (MetaList == command){
	 } else */
	if (MetaAbort == command) {
		mMetaserver->cancel();
	} else if (MetaRefresh == command) {
		mMetaserver->refresh();
	}

	return;
}

int MetaserverService::compareVersions(const std::string& firstVersion, const std::string& secondVersion)
{
	std::vector<std::string> firstVersionStrings = Tokeniser::split(firstVersion, ".");
	std::vector<std::string> secondVersionStrings = Tokeniser::split(secondVersion, ".");

	std::vector<std::string>::const_iterator firstI = firstVersionStrings.begin();
	std::vector<std::string>::const_iterator secondI = secondVersionStrings.begin();

	while (firstI != firstVersionStrings.end() && secondI != secondVersionStrings.end()) {
		std::stringstream ss(*firstI);
		int firstNumber;
		if ((ss >> firstNumber).fail() || !(ss >> std::ws).eof()) {
			return 0;
		}
		std::stringstream ssSecond(*secondI);
		int secondNumber;
		if ((ssSecond >> secondNumber).fail() || !(ssSecond >> std::ws).eof()) {
			return 0;
		}
		if (firstNumber > secondNumber) {
			return 1;
		} else if (secondNumber > firstNumber) {
			return -1;
		}
		firstI++;
		secondI++;
	}
	return 0;
}

} // namespace Ember
