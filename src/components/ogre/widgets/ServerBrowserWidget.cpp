//
// C++ Implementation: ServerBrowser
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

#include "Widget.h"

#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIMultiColumnList.h> 

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "GUIManager.h"
#include "ServerBrowserWidget.h"

namespace EmberOgre {

class ServerBrowserWidgetListItem : public CEGUI::ListboxTextItem
{
public:
	ServerBrowserWidgetListItem(const CEGUI::String& text) : ListboxTextItem(text)
	{
		setSelectionBrushImage((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MultiListSelectionBrush");
	}
};

ServerBrowserWidget::ServerBrowserWidget(GUIManager* guiManager) :  Widget::Widget(guiManager)
{
}


ServerBrowserWidget::~ServerBrowserWidget()
{
	delete mMainWindow;
}

void ServerBrowserWidget::buildWidget()
{


	mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"widgets/ServerBrowserWidget.xml", "ServerBrowser/");

	mServerList = static_cast<CEGUI::MultiColumnList*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"ServerBrowser/ServerList"));

	mServerList->addColumn((CEGUI::utf8*)"Server Name", 0, 0.2f);
	mServerList->addColumn((CEGUI::utf8*)"Address", 1, 0.2f);
	mServerList->addColumn((CEGUI::utf8*)"Ping", 2, 0.1f);
	mServerList->addColumn((CEGUI::utf8*)"Clients", 3, 0.1f);
	mServerList->addColumn((CEGUI::utf8*)"Ruleset", 4, 0.2f);
	mServerList->addColumn((CEGUI::utf8*)"Server type", 5, 0.2f);
	mServerList->setSelectionMode(CEGUI::MultiColumnList::RowSingle);

	CEGUI::PushButton* refresh = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"ServerBrowser/Refresh"));
	refresh->subscribeEvent(CEGUI::ButtonBase::EventMouseClick, 
		boost::bind(&ServerBrowserWidget::Refresh_Click, this, _1));
	
	CEGUI::PushButton* connectButton = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"ServerBrowser/Connect"));
	BIND_CEGUI_EVENT(connectButton, CEGUI::ButtonBase::EventMouseClick, ServerBrowserWidget::Connect_Click)
	
	getMainSheet()->addChildWindow(mMainWindow); 

	
	Ember::EmberServices::getInstance()->getServerService()->GotConnection.connect(SigC::slot(*this, &ServerBrowserWidget::connectedToServer));

		
	connectToServer();
	//metaServer->refresh();


}


void ServerBrowserWidget::connectedToServer(Eris::Connection* connection) 
{
	mGuiManager->removeWidget(this);
	delete this;
}
	

bool ServerBrowserWidget::Refresh_Click(const CEGUI::EventArgs& args)
{
	mServerList->resetList();
	metaServer->refresh();
	return true;

}

bool ServerBrowserWidget::Connect_Click(const CEGUI::EventArgs& args)
{
	std::string serverName;	

	CEGUI::Window* manualNameWindow = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"ServerBrowser/ManualServerName");
	
	if (manualNameWindow->getText() != "") {
		serverName = std::string(manualNameWindow->getText().c_str());
	} else {
		uint selectedRowIndex = mServerList->getItemRowIndex(mServerList->getFirstSelectedItem());
	
	/*	uint selectedRowIndex = mServerList->getFirstSelectionRow();*/
		if (selectedRowIndex != -1) {
			CEGUI::ListboxItem* selectedItem = mServerList->getItemAtGridReference(CEGUI::MCLGridRef(selectedRowIndex, 0));
			serverName = std::string(selectedItem->getText().c_str());
		}
	}
		
	if (serverName != "") {
		Ember::EmberServices::getInstance()->getServerService()->connect(serverName);
	}
	return true;

}

void ServerBrowserWidget::connectToServer()
{
    metaServer = new Eris::Meta("metaserver.worldforge.org", 10);
//    metaServer->GotServerCount.connect(SigC::slot(*this, &ServerBrowserWidget::gotServerCount));
    metaServer->Failure.connect(SigC::slot(*this, &ServerBrowserWidget::gotFailure));
    metaServer->ReceivedServerInfo.connect(SigC::slot(*this, &ServerBrowserWidget::receivedServerInfo));
    metaServer->CompletedServerList.connect(SigC::slot(*this, &ServerBrowserWidget::completedServerList));
}

	
void ServerBrowserWidget::gotFailure(const std::string& msg)
{
}




void ServerBrowserWidget::receivedServerInfo(const Eris::ServerInfo& sInfo)
{
	mGuiManager->setDebugText("Got server info.");
	
	int rowNumber = mServerList->getRowCount();
	mServerList->addRow();
	
	ServerBrowserWidgetListItem* item = new ServerBrowserWidgetListItem(CEGUI::String(sInfo.getServername()));
//	item->setUserData(&sInfo);
	
	
	mServerList->setItem(item, 0, rowNumber);
	mServerList->setItem(new ServerBrowserWidgetListItem(CEGUI::String(sInfo.getHostname())), 1, rowNumber);
	std::stringstream ss_ping;
	ss_ping << sInfo.getPing();
	mServerList->setItem(new ServerBrowserWidgetListItem(CEGUI::String(ss_ping.str())), 2, rowNumber);
	std::stringstream ss_clientNum;
	ss_clientNum << sInfo.getNumClients();
	mServerList->setItem(new ServerBrowserWidgetListItem(CEGUI::String(ss_clientNum.str())), 3 ,rowNumber);
	mServerList->setItem(new ServerBrowserWidgetListItem(CEGUI::String(sInfo.getRuleset())), 4, rowNumber);
	mServerList->setItem(new ServerBrowserWidgetListItem(CEGUI::String(sInfo.getServer())), 5, rowNumber);
	
	
	
	//CEGUI::ListboxItem* item = new CEGUI::ListboxItem;
	//item->setUserData(sInfo);
}


void ServerBrowserWidget::completedServerList(int count)
{
	mGuiManager->setDebugText("Completet server list.");
}


}
