//
// C++ Interface: ServerBrowser
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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
#ifndef DIMEOGRESERVERBROWSER_H
#define DIMEOGRESERVERBROWSER_H

#include <Eris/Metaserver.h>
#include <Eris/ServerInfo.h>
#include <Eris/Utils.h>
#include <Eris/Connection.h>

#include "Widget.h"

namespace DimeOgre {

class GUIManager;
/**
@author Erik Hjortsberg
*/
class ServerBrowserWidget : public Widget
{
public:
    ServerBrowserWidget(GUIManager* guiManager);

    ~ServerBrowserWidget();
	void buildWidget();

protected:

	CEGUI::MultiColumnList* mServerList;

	void gotServerCount(int count);
	
    void gotFailure(const std::string& msg);

    void receivedServerInfo(const Eris::ServerInfo& sInfo);
	
    void completedServerList();
	
	void connectToServer();
	
	Eris::Meta* metaServer;
	bool Refresh_Click(const CEGUI::EventArgs& args);
	bool Connect_Click(const CEGUI::EventArgs& args);

		
	void connectedToServer(Eris::Connection* connection);


};

}

#endif
