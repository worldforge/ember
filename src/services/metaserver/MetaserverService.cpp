/*
 Copyright (C) 2002  Miguel Guzman Miranda [Aglanor]
 Copyright (C) 2005	Erik Ogenvik
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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "MetaserverService.h"

#include "services/config/ConfigService.h"
#include "framework/LoggingInstance.h"
#include "framework/Tokeniser.h"


#include "framework/Session.h"


using namespace std;

namespace Ember {

MetaserverService::MetaserverService(Session& session, ConfigService& configSrv) :
		Service("Metaserver"),
		mSession(session),
		MetaRefresh("meta_refresh", this, "Refresh the meta server listing."),
		MetaAbort("meta_abort", this, "Abort the meta server update process.")
//   , MetaList("meta_list", this, "List all servers.")
{
	std::string metaserverHostname;
	if (configSrv.itemExists("metaserver", "server")) {
		metaserverHostname = std::string(configSrv.getValue("metaserver", "server"));
	} else {
		metaserverHostname = "metaserver.worldforge.org";
	}

	S_LOG_INFO("Connecting to meta server at address " << metaserverHostname << ".");
	mMetaserver = std::make_unique<Eris::Meta>(mSession.m_io_service, mSession.m_event_service, metaserverHostname, 20);
	mMetaserver->Failure.connect(sigc::mem_fun(*this, &MetaserverService::gotFailure));
	mMetaserver->ReceivedServerInfo.connect(sigc::mem_fun(*this, &MetaserverService::receivedServerInfo));
	mMetaserver->CompletedServerList.connect(sigc::mem_fun(*this, &MetaserverService::completedServerList));

}

MetaserverService::~MetaserverService() {
	mMetaserver->cancel();
}


void MetaserverService::gotFailure(const string& msg) {
	S_LOG_WARNING("Got Meta-server error: " << msg);
}

void MetaserverService::receivedServerInfo(const Eris::ServerInfo& sInfo) {

	S_LOG_VERBOSE("Got serverinfo:\n"
						  << "Hostname: " << sInfo.host
						  << "\nServerName: " << sInfo.name
						  << "\nRuleset: " << sInfo.ruleset
						  << "\nServer Type: " << sInfo.server
						  << "\nClients: " << sInfo.clients
						  << "\nPing: " << sInfo.ping
						  << "\nUptime: " << (int) sInfo.uptime
						  << "\nEntities: " << sInfo.entities);
}

void MetaserverService::completedServerList(int count) {
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

Eris::Meta& MetaserverService::getMetaServer() const {
	return *mMetaserver;
}

void MetaserverService::runCommand(const std::string& command, const std::string& args) {
	if (!mMetaserver)
		return;
	/*    if (MetaList == command){
	 } else */
	if (MetaAbort == command) {
		mMetaserver->cancel();
	} else if (MetaRefresh == command) {
		mMetaserver->refresh();
	}
}

int MetaserverService::compareVersions(const std::string& firstVersion, const std::string& secondVersion) {
	auto firstVersionStrings = Tokeniser::split(firstVersion, ".");
	auto secondVersionStrings = Tokeniser::split(secondVersion, ".");

	auto firstI = firstVersionStrings.begin();
	auto secondI = secondVersionStrings.begin();

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
		++firstI;
		++secondI;
	}
	return 0;
}

} // namespace Ember
