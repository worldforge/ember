//
// C++ Interface: WfutService
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
#ifndef EMBERWFUTSERVICE_H
#define EMBERWFUTSERVICE_H

#include "framework/Service.h"
#include <sigc++/object.h>
#include <sigc++/signal.h>

#include <memory>

namespace Ember {


class WfutSession;

/**
	Provides abilities to update media through the WFUT tool.

	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class WfutService : public Service
{
public:
    WfutService();

    virtual ~WfutService();
    
    virtual Service::Status start();
    
    
    void startUpdate(const std::string &serverRoot,
		const std::string &channelName,
		const std::string &localPath,
		const std::string &systemPath);

	/** The poll function is used to perform a chunk of downloading. This means
	* that it needs to be called frequently. It returns the number of files still
	* being downloaded. It will return 0 when all files have finished
	* downloading.
	*/
	int poll();

	/** The DownloadComplete signal is fired every time a file is successfully 
		* downloaded. The first argument is the source url and the second argument
		* is the filename from the FileObject.
		*/
	sigc::signal<void, const std::string&, const std::string&> DownloadComplete;

	/** The DownloadFailed signal is fired when there was a problem downloading 
		* a file. This could happen due to a broken url, or a problem saving the
		* file in the temp location, or copying it from the tmp location to the
		* destination. The first argument is the source url and the second argument
		* is the filename from the FileObject. The third argument is a message 
		* indicating a general reason why the download failed.
		*/
	sigc::signal<void, const std::string&, const std::string&, const std::string&> DownloadFailed;

	/** The AllDownloadsComplete is emitted when all files have been downloaded.
	*/
	sigc::signal<void> AllDownloadsComplete;

	/** 
	The DownloadingServerList is emitted when the file list (most often wfut.xml) is being downloaded from the server.
	@param the path to the file
	*/
	sigc::signal<void, const std::string&> DownloadingServerList;
	
	/**
	The UpdatesCalculated is emitted when all local files and server side files have been calculated, and a list of those that needs updates have been put together.
	@param the number of files to update
	*/
	sigc::signal<void, size_t> UpdatesCalculated;

protected:
	std::auto_ptr<WfutSession> mSession;

	void wfutSession_DownloadComplete(const std::string &url, const std::string &filename);
	void wfutSession_DownloadFailed(const std::string &url, const std::string &filename, const std::string &reason);
	void wfutSession_ServerListDownloading(const std::string &url);
	void wfutSession_UpdatesCalculated(size_t numberOfFilesToUpdate);
	sigc::slot<void, const std::string&, const std::string&> mDownloadCompleteSlot;
	sigc::slot<void, const std::string&, const std::string&, const std::string&> mDownloadFailureSlot;
	sigc::slot<void, const std::string&> mServerListDownloadingSlot;
	sigc::slot<void, size_t> mUpdatesCalculatedSlot;
};

}

#endif
