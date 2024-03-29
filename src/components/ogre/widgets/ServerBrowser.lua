ServerBrowser = {}

function ServerBrowser:connectToMetaServer()
	local metaServer = emberServices:getMetaserverService():getMetaServer()

	connect(self.connectors, metaServer.ReceivedServerInfo, self.MetaServer_ReceivedServerInfo, self)
	--only refresh if it's enabled in the config
	if emberServices:getConfigService():isItemSet("metaserver", "enabled", "true") then
		metaServer:refresh()
	end
end

function ServerBrowser:buildWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("ServerBrowser.layout", "ServerBrowser")

	local wee
	wee = self.widget:getWindow("ServerList")
	self.serverList = CEGUI.toMultiColumnList(wee)
	self.serverList:subscribeEvent("MouseDoubleClick", self.ServerList_DoubleClick, self)

	wee = self.widget:getWindow("Refresh")
	local refreshButton = CEGUI.toPushButton(wee)
	refreshButton:subscribeEvent("Clicked", self.Refresh_Click, self)

	wee = self.widget:getWindow("Connect")
	local connectButton = CEGUI.toPushButton(wee)
	connectButton:subscribeEvent("Clicked", self.Connect_Click, self)

	wee = self.widget:getWindow("ManualServerName")
	self.manualServerNameTextbox = CEGUI.toEditbox(wee)
	self.manualServerNameTextbox:subscribeEvent("TextAccepted", self.manualServerNameTextbox_TextAcceptedEvent, self)

	wee = self.widget:getWindow("HideOldServers")
	self.hideOldServersCheckbox = CEGUI.toToggleButton(wee)
	--Only show the checkbox for filtering old servers if the metaserver:minimumversion value is set in the config
	if emberServices:getConfigService():itemExists("metaserver", "minimumversion") then
		local minimumversion = emberServices:getConfigService():getValue("metaserver", "minimumversion")
		if minimumversion then
			if minimumversion:is_string() then
				self.minimumVersion = minimumversion:as_string()
				self.hideOldServersCheckbox:setVisible(true)
				self.hideOldServersCheckbox:subscribeEvent("SelectStateChanged", self.hideOldServers_SelectStateChanged, self)
				self.hideOldServers = self.hideOldServersCheckbox:isSelected()
			end
		end
	end
	if emberServices:getConfigService():itemExists("metaserver", "minimumentitycount") then
		local minimumentitycount = emberServices:getConfigService():getValue("metaserver", "minimumentitycount")
		if minimumentitycount then
			if minimumentitycount:is_int() then
				self.minumumentitycount = minimumentitycount:as_int()
			end
		end
	end

	local serverService = emberServices:getServerService()
	connect(self.connectors, serverService.GotConnection, self.Server_GotConnection, self)

	connect(self.connectors, serverService.EventStatusChanged, self.Server_StatusChanged, self)

	self.widget:getWindow("CancelConnection"):subscribeEvent("Clicked", self.CancelConnection_Click, self)

	self:connectToMetaServer()
	local showWindow = function()
		self.widget:show()
		self.widget:getMainWindow():activate()

		--If the "autoconnect" value is set, try to connect to the specified server
		if emberServices:getConfigService():itemExists("metaserver", "autoconnect") then
			local serverConfAddress = emberServices:getConfigService():getValue("metaserver", "autoconnect")
			if serverConfAddress then
				if serverConfAddress:is_string() then
					local address = serverConfAddress:as_string()
					Ember.EmberServices.getSingleton():getServerService():connect(address)
				end
			end
		end
	end

	local showLocalServerSelector = function()
		self.widget:hide()
		local socketDetectedWindow = guiManager:createWidget()
		socketDetectedWindow:loadMainSheet("ServerSocketDetected.layout", "ServerSocketDetected")
		socketDetectedWindow:show();
		socketDetectedWindow:getMainWindow():activate()

		socketDetectedWindow:getWindow("DontConnect"):subscribeEvent("Clicked", function(args)
			guiManager:destroyWidget(socketDetectedWindow)
			socketDetectedWindow = nil
			showWindow()
			return true
		end)

		socketDetectedWindow:getWindow("ConnectNormally"):subscribeEvent("Clicked", function(args)
			guiManager:destroyWidget(socketDetectedWindow)
			socketDetectedWindow = nil
			serverService:connectLocal()
			return true
		end)

		socketDetectedWindow:getWindow("ConnectAsAdmin"):subscribeEvent("Clicked", function(args)
			guiManager:destroyWidget(socketDetectedWindow)
			socketDetectedWindow = nil

			local localServerAdminCreator = Ember.LocalServerAdminCreator.new(serverService)

			self.socketGotAvatarConnection = serverService.GotAvatar:connect(function()
				deleteSafe(localServerAdminCreator)
				if (self.socketGotAvatarConnection) then
					self.socketGotAvatarConnection:disconnect()
				end
			end)

			serverService:connectLocal()

			return true
		end)

	end

	self.widget:getWindow("ConnectLocal"):subscribeEvent("Clicked", function(args)
		showLocalServerSelector()
		return true
	end)

	connect(self.connectors, serverService.EventLocalSocketChanged, function()
		local hasSocket = serverService:hasLocalSocket()
		self.widget:getWindow("ConnectLocal"):setVisible(hasSocket)
	end, self)

	if serverService:hasLocalSocket() then
		self.widget:getWindow("ConnectLocal"):setVisible(true)

		showLocalServerSelector()

	else
		showWindow()
	end

end

function ServerBrowser:connectWithColumnList()
	local serverName
	if self.serverList:getFirstSelectedItem() then
		local selectedRowIndex = self.serverList:getItemRowIndex(self.serverList:getFirstSelectedItem())

		if selectedRowIndex ~= -1 then
			local selectedItem = self.serverList:getItemAtGridReference(CEGUI.MCLGridRef.new(selectedRowIndex, 7))
			if selectedItem then
				serverName = selectedItem:getText()
			end
		end
		if serverName ~= "" then
			Ember.EmberServices.getSingleton():getServerService():connect(serverName);
		end
	end
end

function ServerBrowser:doConnect()
	local serverName

	--first we check if there is text in the ManualServerName textbox
	--if so, we try to connect to the server specified there

	if self.manualServerNameTextbox:getText() ~= "" then
		serverName = self.manualServerNameTextbox:getText()
		--Try to separate the port number, if available.
		if serverName:find(":") then
			local port = serverName:sub(serverName:find(":") + 1, serverName:len())
			serverName = serverName:sub(0, serverName:find(":") - 1)
			Ember.EmberServices.getSingleton():getServerService():connect(serverName, port * 1)
		else
			Ember.EmberServices.getSingleton():getServerService():connect(serverName)
		end
	elseif self.serverList:getFirstSelectedItem() then
		--if ManualServerName is empty we try to connect to the server selected from the list
		self:connectWithColumnList()
	end
end

function ServerBrowser:Server_GotConnection(connection)
	self.widget:hide()
	connect(self.connectors, connection.Disconnected, function()
		self.widget:show()
		self.widget:getMainWindow():activate()
	end)
	connect(self.connectors, connection.Failure, function()
		self.widget:show()
		self.widget:getMainWindow():activate()
	end)

end

function ServerBrowser:Server_StatusChanged(status)
	local text = ""
	local showOverlay = true
	if status == Eris.BaseConnection.INVALID_STATUS then
		text = "Looking for server."
	elseif status == Eris.BaseConnection.NEGOTIATE then
		text = "Negotiating with server."
	elseif status == Eris.BaseConnection.CONNECTING then
		text = "Connecting to server."
	else
		text = "Connected to server"
		showOverlay = false
	end
	self.widget:getWindow("ConnectionOverlayStatus"):setText(text)
	self.widget:getWindow("ConnectionOverlay"):setVisible(showOverlay)

end

function ServerBrowser:CancelConnection_Click(args)
	Ember.EmberServices.getSingleton():getServerService():disconnect()
	return true
end

function ServerBrowser:Refresh_Click(args)
	self.serverList:resetList()
	emberServices:getMetaserverService():getMetaServer():refresh()
	return true
end

function ServerBrowser:Connect_Click(args)
	self:doConnect()
	return true
end

function ServerBrowser:manualServerNameTextbox_TextAcceptedEvent(args)
	self:doConnect()
	return true
end

function ServerBrowser:ServerList_DoubleClick(args)
	self:connectWithColumnList()
	return true
end

function ServerBrowser:hideOldServers_SelectStateChanged(args)
	self.hideOldServers = self.hideOldServersCheckbox:isSelected()
	self:refreshServerList()
	return true
end

function ServerBrowser:refreshServerList()
	local metaServer = emberServices:getMetaserverService():getMetaServer()
	self.serverList:resetList()
	local numberOfServerInfos = metaServer:getGameServerCount()

	for i = 0, numberOfServerInfos - 1 do
		local sInfo = metaServer:getInfoForServer(i)
		if sInfo.status == Eris.ServerInfo.VALID then
			self:addRow(sInfo)
		end
	end
end

function ServerBrowser:addRow(sInfo)
	if self.hideOldServers then
		if Ember.MetaserverService:compareVersions(self.minimumVersion, sInfo.version) > 0 then
			return
		end
	end
	if self.minimumentitycount >= sInfo.entities then
		return
	end

	local rowNumber = self.serverList:getRowCount()
	self.serverList:addRow()

	local item = Ember.OgreView.Gui.ColouredListItem.new(self:getSavedAccount(sInfo))
	self.serverList:setItem(item, 0, rowNumber);

	local item = Ember.OgreView.Gui.ColouredListItem.new(sInfo.name)
	self.serverList:setItem(item, 1, rowNumber)

	local item = Ember.OgreView.Gui.ColouredListItem.new(sInfo.ping)
	self.serverList:setItem(item, 2, rowNumber)

	local item = Ember.OgreView.Gui.ColouredListItem.new(sInfo.clients)
	self.serverList:setItem(item, 3, rowNumber)

	local item = Ember.OgreView.Gui.ColouredListItem.new(sInfo.ruleset)
	self.serverList:setItem(item, 4, rowNumber)

	local item = Ember.OgreView.Gui.ColouredListItem.new(sInfo.server)
	self.serverList:setItem(item, 5, rowNumber)

	local item = Ember.OgreView.Gui.ColouredListItem.new(sInfo.version)
	self.serverList:setItem(item, 6, rowNumber)

	local item = Ember.OgreView.Gui.ColouredListItem.new(sInfo.host)
	self.serverList:setItem(item, 7, rowNumber)
end

function ServerBrowser:getSavedAccount(sInfo)

	-- Essentially we try and fetch the 'hostname_<host>_servername_<server>' section
	-- get the 'username' key.  If this has a value, there is saved credentials
	-- We are always expecting a string ... even if it's empty.
	local serverService = emberServices:getServerSettingsService()
	local serverSettingCredentials = Ember.ServerSettingsCredentials.new(sInfo.host, sInfo.name)
	local savedUser = serverService:getItem(serverSettingCredentials, "username")
	local retFav = savedUser:as_string()
	if retFav ~= "" then
		retFav = "  ***" -- because centering text is seems impossible
	end

	return retFav
end

function ServerBrowser:MetaServer_ReceivedServerInfo(sInfo)
	--we got some server info, add it to the server list

	self:addRow(sInfo)

end

serverBrowser = { connectors = {}, hideOldServers = false, minimumVersion = '', minimumentitycount = 0, rows = {} }
setmetatable(serverBrowser, { __index = ServerBrowser })
serverBrowser:buildWidget()
connect(serverBrowser.connectors, scriptingService.EventShutdown, function()
	disconnectAll(serverBrowser.connectors)
	guiManager:destroyWidget(serverBrowser.widget)
	serverBrowser = nil
	collectgarbage()
end)
