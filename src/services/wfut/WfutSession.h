//
// C++ Interface: WfutSession
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
#ifndef EMBEROGREWFUTSESSION_H
#define EMBEROGREWFUTSESSION_H

#include <libwfut/WFUT.h>
#include <sigc++/object.h>

namespace Ember {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class WfutSession : public sigc::trackable
{
public:
    WfutSession(sigc::slot<void, const std::string&, const std::string&>& downloadCompleteSlot 
    , sigc::slot<void, const std::string&, const std::string&, const std::string&>& downloadFailedSlot 
    , sigc::slot<void, const std::string&>& serverListDownloadingSlot
    , sigc::slot<void, unsigned int>& updatesCalculatedSlot);

    virtual ~WfutSession();
    
    void init();
    
    int poll();
    
	void startUpdate(const std::string &serverRoot,
		const std::string &channelName,
		const std::string &localPath,
		const std::string &systemPath
		);
private:
	WFUT::ChannelFileList mLocal, mSystem, mServer, mUpdates, mTmplist;
	WFUT::WFUTClient mWfutClient;
	std::string mLocalWfut;
	sigc::slot<void, const std::string&>& mServerListDownloadingSlot;
	sigc::slot<void, unsigned int>& mUpdatesCalculatedSlot;
};

}

#endif
