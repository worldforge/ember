-----------------------------------------
-- Start of handler functions
-----------------------------------------

-----------------------------------------
-- Script Entry Point
-----------------------------------------
ServerBrowser = {connectors={}}


function ServerBrowser.connectToMetaServer()
	local metaServer = emberServices:getMetaserverService():getMetaServer()
	
--    EmberOgre.LuaConnector:new(metaServer:Failure.connect, "MetaServer_Failure")
	connect(ServerBrowser.connectors, metaServer.ReceivedServerInfo, "ServerBrowser.MetaServer_ReceivedServerInfo")
--    EmberOgre.LuaConnector:new(metaServer:CompletedServerList.connect, "MetaServer_CompletedServerList.connect")
	--only refresh if it's enabled in the config
	if emberServices:getConfigService():isItemSet("metaserver", "enabled", "true") then
    	metaServer:refresh()
    end
end


function ServerBrowser.buildWidget()
	ServerBrowser.widget = guiManager:createWidget()
	ServerBrowser.widget:loadMainSheet("ServerBrowser.layout", "ServerBrowser/")
	
	local wee
	wee = ServerBrowser.widget:getWindow("ServerList")
	ServerBrowser.serverList = CEGUI.toMultiColumnList(wee)
	ServerBrowser.serverList:subscribeEvent("MouseDoubleClick", "ServerBrowser.ServerList_DoubleClick")
	
	wee = ServerBrowser.widget:getWindow("Refresh")
	local refreshButton = CEGUI.toPushButton(wee)
	refreshButton:subscribeEvent("Clicked", "ServerBrowser.Refresh_Click")
	
	wee = ServerBrowser.widget:getWindow("Connect")
	local connectButton = CEGUI.toPushButton(wee)
	connectButton:subscribeEvent("Clicked", "ServerBrowser.Connect_Click")
		
	wee = ServerBrowser.widget:getWindow("ManualServerName")
	ServerBrowser.manualServerNameTextbox = CEGUI.toPushButton(wee)
	ServerBrowser.manualServerNameTextbox:subscribeEvent("TextAccepted", "ServerBrowser.manualServerNameTextbox_TextAcceptedEvent")
	
	
	local serverService = emberServices:getServerService()
	connect(ServerBrowser.connectors, serverService.GotConnection, "ServerBrowser.Server_GotConnection")
	
	ServerBrowser.connectToMetaServer()
	ServerBrowser.widget:show()
	ServerBrowser.widget:getMainWindow():activate()
	
	--If the "autoconnect" value is set, try to connect to the specified server
	if emberServices:getConfigService():itemExists("metaserver", "autoconnect") then
		local serverConfAddress = emberServices:getConfigService():getValue("metaserver", "autoconnect")
		if serverConfAddress ~= nil then
			if serverConfAddress:is_string() then
				local address = serverConfAddress:as_string()
				Ember.EmberServices:getSingleton():getServerService():connect(address)
			end
		end
    end

end


function ServerBrowser.connectWithColumnList()
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

function ServerBrowser.doConnect()
	local serverName

	--first we check if there is text in the ManualServerName textbox
	--if so, we try to connect to the server specified there
	
	if ServerBrowser.manualServerNameTextbox:getText() ~= "" then
		serverName = ServerBrowser.manualServerNameTextbox:getText()
		Ember.EmberServices:getSingleton():getServerService():connect(serverName)
	elseif ServerBrowser.serverList:getFirstSelectedItem() ~= nil then
		--if ManualServerName is empty we try to connect to the server selected from the list 
		ServerBrowser.connectWithColumnList()
	end
end

function ServerBrowser.Server_GotConnection()
	ServerBrowser.widget:hide()
end

function ServerBrowser.Refresh_Click(args)
	ServerBrowser.serverList:resetList()
	--Ember.EmberServices:getSingleton():getMetaserverService():getMetaserver():refresh()
end

function ServerBrowser.Connect_Click(args)
	ServerBrowser.doConnect()
end

function ServerBrowser.manualServerNameTextbox_TextAcceptedEvent(args)
	ServerBrowser.doConnect()
end

function ServerBrowser.ServerList_DoubleClick(args)
	ServerBrowser.connectWithColumnList()
end

function ServerBrowser.MetaServer_ReceivedServerInfo(sInfo)
	--we got some server info, add it to the server list

	--mGuiManager->setDebugText("Got server info.");
	
	local rowNumber = ServerBrowser.serverList:getRowCount()
	ServerBrowser.serverList:addRow()
	
	local item = EmberOgre.Gui.ColouredListItem:new(sInfo:getServername())
--	item->setUserData(&sInfo);
	
	
	ServerBrowser.serverList:setItem(item, 0, rowNumber)
	local ss_ping = sInfo:getPing()
	local ss_clientNum = sInfo:getNumClients()
	
	item = EmberOgre.Gui.ColouredListItem:new(ss_ping)
	ServerBrowser.serverList:setItem(item, 1, rowNumber)
	
	item = EmberOgre.Gui.ColouredListItem:new(ss_clientNum)
	ServerBrowser.serverList:setItem(item, 2 ,rowNumber)
	
	item = EmberOgre.Gui.ColouredListItem:new(sInfo:getRuleset())
	ServerBrowser.serverList:setItem(item, 3, rowNumber)
	
	item = EmberOgre.Gui.ColouredListItem:new(sInfo:getServer())
	ServerBrowser.serverList:setItem(item, 4, rowNumber)
	
	item = EmberOgre.Gui.ColouredListItem:new(sInfo:getVersion())
	ServerBrowser.serverList:setItem(item, 5, rowNumber)
	
	item = EmberOgre.Gui.ColouredListItem:new(sInfo:getHostname())
	ServerBrowser.serverList:setItem(item, 6, rowNumber)
	
	
	
	--CEGUI::ListboxItem* item = new CEGUI::ListboxItem;
	--item->setUserData(sInfo);
end


ServerBrowser.buildWidget()