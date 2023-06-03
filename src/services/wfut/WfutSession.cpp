//
// C++ Implementation: WfutSession
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "WfutSession.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include "framework/LoggingInstance.h"

using namespace WFUT;

namespace Ember {

static bool fileExists(const std::string& fileName) 
{
	return boost::filesystem::exists(fileName);
}


WfutSession::WfutSession(sigc::slot<void(const std::string&, const std::string&)>& downloadCompleteSlot
, sigc::slot<void(const std::string&, const std::string&, const std::string&)>& downloadFailedSlot
, sigc::slot<void(const std::string&)>& serverListDownloadingSlot
, sigc::slot<void(size_t)>& updatesCalculatedSlot
) 
: mServerListDownloadingSlot(serverListDownloadingSlot)
, mUpdatesCalculatedSlot(updatesCalculatedSlot)
{
	mWfutClient.DownloadComplete.connect(downloadCompleteSlot);
	mWfutClient.DownloadFailed.connect(downloadFailedSlot);
}


WfutSession::~WfutSession()
{
	mWfutClient.shutdown();
}

void WfutSession::init()
{
	mWfutClient.init();
}

void WfutSession::startUpdate(const std::string &serverRoot,
const std::string &channelName,
const std::string &localPath,
const std::string &systemPath
)
{
	
	S_LOG_VERBOSE("Channel name: " << channelName);
	// This is the base path for all files that will be downloaded
	const std::string local_root = localPath + "/" + channelName + "/";
	
	const std::string& channel = channelName;
	std::string channel_file = "wfut.xml";
	std::string tmpfile = "tempwfut.xml";
	
	// Look for mLocal mWfutClient file.
	mLocalWfut = std::string(localPath  + "/" + channelName + "/" + channel_file);
	S_LOG_VERBOSE("Local wfut: " << mLocalWfut);
	if (fileExists(mLocalWfut)) {
		if (mWfutClient.getLocalList(mLocalWfut, mLocal)) {
			S_LOG_WARNING("Error reading local wfut.xml file.");
		}
	}
	
	// Look for tmpwfut file. If it exists, update the mLocal files list.
	const std::string tmp_wfut = localPath  + "/" + tmpfile;
	S_LOG_VERBOSE("Tmp wfut: " << tmp_wfut);
	
	if (fileExists(tmp_wfut)) {
		if (mWfutClient.getLocalList(tmp_wfut, mTmplist)) {
			S_LOG_WARNING("Error reading local tmpwfut.xml file.");
		} else {
			const FileMap &fm = mTmplist.getFiles();
			auto I = fm.begin();
			auto Iend = fm.end();
			for (; I != Iend; ++I) {
				mLocal.addFile(I->second);
			}
		}
	}
	
	
	// Look for a mSystem mWfutClient file
	if (!systemPath.empty()) {
		const std::string system_wfut = systemPath + "/" + channel + "/" + channel_file; 
		S_LOG_VERBOSE("System wfut: " << system_wfut);
		
		if (fileExists(system_wfut)) {
			if (mWfutClient.getLocalList(system_wfut, mSystem)) {
				S_LOG_WARNING("Error reading system wfut.xml file.");
			}
		}
	}
	
	// By now we should have a proper channel name. If not, then there is nothing 
	// we can do to find the mServer mUpdates.
	if (channel.empty() || channel == ".") {
		S_LOG_WARNING("Unable to determine channel name.");
		return;
	}
	
	const std::string server_wfut = serverRoot + "/" + channel + "/" + channel_file; 
	S_LOG_VERBOSE("Server wfut: " << server_wfut);
	
	mServerListDownloadingSlot(server_wfut);
	if (mWfutClient.getFileList(server_wfut, mServer)) {
		S_LOG_WARNING("Error downloading server channel file.");
		return;
	}
	
	S_LOG_VERBOSE("Local Root: " << local_root);
	
	S_LOG_INFO("Updating Channel: " << channel);
	
	// Now we have loaded all our data files, lets find out what we really need
	// to download
	if (mWfutClient.calculateUpdates(mServer, mSystem, mLocal, mUpdates, local_root)) {
		S_LOG_WARNING("Error determining file to update.");
		return;
	}
	
	mUpdatesCalculatedSlot(mUpdates.getFiles().size());
	
	// Make sure the mLocal file has the correct channel name
	mLocal.setName(mServer.getName());
	
	// Queue the list of files to download
	mWfutClient.updateChannel(mUpdates, serverRoot + "/" + channel, local_root);
	
}

int WfutSession::poll()
{
	int result = mWfutClient.poll();
	if (!result) {
		// Save the completed download list
		mWfutClient.saveLocalList(mServer, mLocalWfut);
	}
	return result;
}


}
