//
// C++ Implementation: MediaUpdater
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "MediaUpdater.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/wfut/WfutService.h"

namespace EmberOgre {

MediaUpdater::MediaUpdater()
{
}


MediaUpdater::~MediaUpdater()
{
}

void MediaUpdater::performUpdate()
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingleton().getConfigService();
	Ember::WfutService* wfutSrv= Ember::EmberServices::getSingleton().getWfutService();
	
	if (configSrv->itemExists("wfut", "server")) {
		std::string server(configSrv->getValue("wfut", "server"));
		if (configSrv->itemExists("wfut", "channel")) {
			std::string channel(configSrv->getValue("wfut", "channel"));
			
			wfutSrv->startUpdate(server, channel, configSrv->getHomeDirectory() , "");
			while (wfutSrv->poll()) {
			}
			
		}
	}

}


}
