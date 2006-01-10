-----------------------------------------
-- Start of handler functions
-----------------------------------------

-----------------------------------------
-- Script Entry Point
-----------------------------------------
ServerBrowser = {}


function ServerBrowser_connectToMetaServer()
	local metaServer = Ember.EmberServices:getSingleton():getMetaserverService():getMetaServer()
	
--    EmberOgre.LuaConnector:new(metaServer:Failure.connect, "MetaServer_Failure")
    EmberOgre.LuaConnector:new(metaServer.ReceivedServerInfo):connect("ServerBrowser_MetaServer_ReceivedServerInfo")
--    EmberOgre.LuaConnector:new(metaServer:CompletedServerList.connect, "MetaServer_CompletedServerList.connect")
    metaServer:refresh()
end


function ServerBrowser_buildWidget()
	ServerBrowser.widget = guiManager:createWidget()
	ServerBrowser.widget:loadMainSheet("ServerBrowser.layout", "ServerBrowser/")
	
	local wee
	wee = ServerBrowser.widget:getWindow("ServerList")
	ServerBrowser.serverList = CEGUI.toMultiColumnList(wee)
	ServerBrowser.serverList:subscribeEvent("MouseDoubleClick", "ServerBrowser_ServerList_DoubleClick")
	
	wee = ServerBrowser.widget:getWindow("Refresh")
	local refreshButton = CEGUI.toPushButton(wee)
	refreshButton:subscribeEvent("MouseClick", "ServerBrowser_Refresh_Click")
	
	wee = ServerBrowser.widget:getWindow("Connect")
	local connectButton = CEGUI.toPushButton(wee)
	connectButton:subscribeEvent("MouseClick", "ServerBrowser_Connect_Click")
		
	wee = ServerBrowser.widget:getWindow("ManualServerName")
	ServerBrowser.manualServerNameTextbox = CEGUI.toPushButton(wee)
	ServerBrowser.manualServerNameTextbox:subscribeEvent("TextAccepted", "ServerBrowser_manualServerNameTextbox_TextAcceptedEvent")
	
	
	local serverService = emberServices:getServerService()
	EmberOgre.LuaConnector:new(serverService.GotConnection):connect("ServerBrowser_Server_GotConnection")
	
	ServerBrowser_connectToMetaServer()

end


function ServerBrowser_connectWithColumnList()
	local serverName
	if ServerBrowser.serverList:getFirstSelectedItem() ~= nil then
		local selectedRowIndex = ServerBrowser.serverList:getItemRowIndex(ServerBrowser.serverList:getFirstSelectedItem())
	
		if selectedRowIndex ~= -1 then
			local selectedItem = ServerBrowser.serverList:getItemAtGridReference(CEGUI.MCLGridRef:new_local(selectedRowIndex, 6))
			if selectedItem ~= nil then
				serverName = selectedItem:getText()
			end
		end
		if serverName ~= "" then
			Ember.EmberServices:getSingleton():getServerService():connect(serverName);
		end
	end
end

function ServerBrowser_doConnect()
	local serverName

	--first we check if there is text in the ManualServerName textbox
	--if so, we try to connect to the server specified there
	
	if ServerBrowser.manualServerNameTextbox:getText() ~= "" then
		serverName = ServerBrowser.manualServerNameTextbox:getText()
		Ember.EmberServices:getSingleton():getServerService():connect(serverName)
	elseif ServerBrowser.serverList:getFirstSelectedItem() ~= nil then
		--if ManualServerName is empty we try to connect to the server selected from the list 
		ServerBrowser_connectWithColumnList()
	end
end

function ServerBrowser_Server_GotConnection()
	ServerBrowser.widget:hide()
end

function ServerBrowser_Refresh_Click(args)
	ServerBrowser.serverList:resetList()
	--Ember.EmberServices:getSingleton():getMetaserverService():getMetaserver():refresh()
end

function ServerBrowser_Connect_Click(args)
	ServerBrowser_doConnect()
end

function ServerBrowser_manualServerNameTextbox_TextAcceptedEvent(args)
	ServerBrowser_doConnect()
end

function ServerBrowser_ServerList_DoubleClick(args)
	ServerBrowser_connectWithColumnList()
end

function ServerBrowser_MetaServer_ReceivedServerInfo(sInfo)
	--we got some server info, add it to the server list

	--mGuiManager->setDebugText("Got server info.");
	
	local rowNumber = ServerBrowser.serverList:getRowCount()
	ServerBrowser.serverList:addRow()
	
	local item = EmberOgre.ColoredListItem:new_local(sInfo:getServername())
--	item->setUserData(&sInfo);
	
	
	item = tolua.cast(item, "CEGUI::ListboxItem")
	ServerBrowser.serverList:setItem(item, 0, rowNumber)
	local ss_ping = sInfo:getPing()
	local ss_clientNum = sInfo:getNumClients()
	
	item = EmberOgre.ColoredListItem:new_local(ss_ping)
	item = tolua.cast(item, "CEGUI::ListboxItem")
	ServerBrowser.serverList:setItem(item, 1, rowNumber)
	
	item = EmberOgre.ColoredListItem:new_local(ss_clientNum)
	item = tolua.cast(item, "CEGUI::ListboxItem")
	ServerBrowser.serverList:setItem(item, 2 ,rowNumber)
	
	item = EmberOgre.ColoredListItem:new_local(sInfo:getRuleset())
	item = tolua.cast(item, "CEGUI::ListboxItem")
	ServerBrowser.serverList:setItem(item, 3, rowNumber)
	
	item = EmberOgre.ColoredListItem:new_local(sInfo:getServer())
	item = tolua.cast(item, "CEGUI::ListboxItem")
	ServerBrowser.serverList:setItem(item, 4, rowNumber)
	
	item = EmberOgre.ColoredListItem:new_local(sInfo:getVersion())
	item = tolua.cast(item, "CEGUI::ListboxItem")
	ServerBrowser.serverList:setItem(item, 5, rowNumber)
	
	item = EmberOgre.ColoredListItem:new_local(sInfo:getHostname())
	item = tolua.cast(item, "CEGUI::ListboxItem")
	ServerBrowser.serverList:setItem(item, 6, rowNumber)
	
	
	
	--CEGUI::ListboxItem* item = new CEGUI::ListboxItem;
	--item->setUserData(sInfo);
end


ServerBrowser_buildWidget()