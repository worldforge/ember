//
// C++ Implementation: WfutService
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
//         Simon Goodall, (C) 2005 - 2007
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
#include "WfutService.h"
#include "WfutSession.h"

using namespace WFUT;

namespace Ember {



WfutService::WfutService()
{
	mDownloadCompleteSlot = sigc::mem_fun(this, &WfutService::wfutSession_DownloadComplete);
	mDownloadFailureSlot = sigc::mem_fun(this, &WfutService::wfutSession_DownloadFailed);
	mServerListDownloadingSlot = sigc::mem_fun(this, &WfutService::wfutSession_ServerListDownloading);
	mUpdatesCalculatedSlot = sigc::mem_fun(this, &WfutService::wfutSession_UpdatesCalculated);
	
	mSession = std::auto_ptr<WfutSession>(new WfutSession(mDownloadCompleteSlot, mDownloadFailureSlot, mServerListDownloadingSlot, mUpdatesCalculatedSlot));
}


WfutService::~WfutService()
{
}

Service::Status WfutService::start()
{
	mSession->init();
}


void WfutService::startUpdate(const std::string &serverRoot,
const std::string &channelName,
const std::string &localPath,
const std::string &systemPath
)
{
	mSession->startUpdate(serverRoot, channelName, localPath, systemPath);
	
}

int WfutService::poll()
{
	int result = mSession->poll();
	if (!result) {
		AllDownloadsComplete.emit();
	}
	return result;
}

void WfutService::wfutSession_DownloadComplete(const std::string &url, const std::string &filename)
{
	DownloadComplete.emit(url, filename);
}

void WfutService::wfutSession_DownloadFailed(const std::string &url, const std::string &filename, const std::string &reason)
{
	DownloadFailed.emit(url, filename, reason);
}

void WfutService::wfutSession_ServerListDownloading(const std::string &url)
{
	DownloadingServerList.emit(url);
}

void WfutService::wfutSession_UpdatesCalculated(unsigned int numberOfFilesToUpdate)
{
	UpdatesCalculated.emit(numberOfFilesToUpdate);
}

}
