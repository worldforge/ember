----------------------------------------------------
--A simple chat widget. It currently only shows chat output, input is handled by the console adapter.
----------------------------------------------------

Console = {}

--Set up the widget.
function Console:buildWidget()
	self.tabs = {self.gameTab}
	
	--console has transitions, if we don't set this to false, every time it gets activated a 
	--visible "alpha pop" would happen and then the transition would continue
	self.widget:setIsActiveWindowOpaque(false)
	
	self.widget:loadMainSheet("Console.layout", "Console")
	
	--these are used for coloured chat messages
	self.chatEntityColours = {}
	self.chatEntityOccurences = {}
	self.chatTotalColourUsage = {}
	for i = 1, 8 do
		self.chatTotalColourUsage[i] = 0
	end
	
	self.gameTab.textWindow = self.widget:getWindow("GameTextBox")
	self.gameTab.tabWindow = self.widget:getWindow("GamePanel")
	self.gameTab.prefix = self.gameTab.tabWindow:getText()
	self.gameTab.maxLines = 100
	self.gameTab.lines = 0

	
	self.consoleInputWindow = CEGUI.toEditbox(self.widget:getWindow("InputBox"))
	
	--this will bring console functionality to the editbox (such as history, tab completion etc.)
	self.consoleAdapter = Ember.OgreView.Gui.ConsoleAdapter:new_local(self.consoleInputWindow);
	
	
	connect(self.connectors, guiManager.AppendIGChatLine, self.appendIGChatLine, self)
	connect(self.connectors, guiManager.AppendOOGChatLine, self.appendIGChatLine, self)
	connect(self.connectors, guiManager.AppendAvatarImaginary, self.appendAvatarImaginary, self)
	connect(self.connectors, self.consoleAdapter.EventCommandExecuted, self.consoleAdapter_CommandExecuted, self)
	
	self.avatarEntity = nil
	connect(self.connectors, emberOgre.EventCreatedAvatarEntity, function(avatarEntity) 
		self.avatarEntity = avatarEntity
		self.avatarEntityDeletionConnection = connect(nil, avatarEntity.BeingDeleted, function() self.avatarEntity = nil end)
	end)
	
	
	self.widget:show()
	connect(self.connectors, console.GotMessage, self.consoleGotMessage, self)
	
	self.consoleObject = Ember.Lua.LuaConsoleObject:new("console_focus", self.console_focus)
	self.consoleObject:setSelf(self)
end

function Console:consoleAdapter_CommandExecuted(command)
	--if the user presses enter with nothing entered, toggle to movement mode. This will allow for easy switching between chat and movement
	if command == "" then
		--remove focus from the console
		self.widget:getMainWindow():deactivate()
		Ember.Input:getSingleton():setInputMode(Ember.Input.IM_MOVEMENT)
	end
end

function Console:console_focus()
	self.consoleInputWindow:activate()
	--switch to gui mode, so that this command can be called even when the user is in movement mode
	Ember.Input:getSingleton():setInputMode(Ember.Input.IM_GUI)
end

--Determines the correct colour index for given entityName
function Console:getColourIndexForEntityName(entityName)
	--We have the following strategy:
	--1) once we assing a colour to an entity, we keep it
	--2) if we run out of colours, we always assign the least used ones first
	
	local ret = 1
	
	if self.chatEntityColours[entityName] ~= nil then
		--this entity already has had a colour assigned, use it
		ret = self.chatEntityColours[entityName]
	
	else
		--lets find a colour that's the least used one
		local min = self.chatTotalColourUsage[1]
		ret = 1

		--we start from index 2 because we have already set min to the first colour
		for i = 2, 8 do
			--if this particular colour is less used, use it instead
			if self.chatTotalColourUsage[i] < min then
				min = self.chatTotalColourUsage[i]
				ret = i
			end
		end
		
		--assign the colour to given entity
		self.chatEntityColours[entityName] = ret
	end
	
	--we increase the use count of this colour
	--NOTE: use counts are decreased when lines are purged from the history, see notifyLinePurged
	self.chatTotalColourUsage[ret] = self.chatTotalColourUsage[ret] + 1
	
	if self.chatEntityOccurences[entityName] == nil then
		self.chatEntityOccurences[entityName] = 0
	end
	self.chatEntityOccurences[entityName] = self.chatEntityOccurences[entityName] + 1
	
	return ret
end

--Convenience function that returns the exact colour to use for an entity name
--Returned format is the CEGUI AARRGGBB format, FF000000 is black
function Console:getColourForEntityName(entityName)
	local propertyName = ""
	
	if entityName == emberOgre:getWorld():getAvatar():getEmberEntity():getName() then
		--if it's the current player speaking, we always use the same colour for that
		propertyName = "ChatEntityColourSelf"
	else
		--it's another player speaking, ask for the colour
		propertyName = "ChatEntityColour" .. self:getColourIndexForEntityName(entityName)
	end
	
	if not self.widget:getMainWindow():isPropertyPresent(propertyName) then
		return "FF000000" --reasonable fallback
	end
	
	return self.widget:getMainWindow():getProperty(propertyName)
end

--Called when a line gets purged from the message history, be it game or system tab
function Console:notifyLinePurged(line, tab)
	if (tab == self.gameTab) then
		--Lua makes minimal matches so typing > or } into the chat message won't screw this up
		--FIXME: typing > or } into the character name will though
		local entityName = line:match("[<{](.*)[>}]")
		if (entityName ~= nil) and (entityName ~= emberOgre:getWorld():getAvatar():getEmberEntity():getName()) then
			local colourIndex = self.chatEntityColours[entityName]
			if colourIndex ~= nil then
				self.chatTotalColourUsage[colourIndex] = self.chatTotalColourUsage[colourIndex] - 1
			end
			
			-- we have decreased the total colour use count but chatEntityColours will endlessly grow with new
			-- and new entities
			
			-- we have to remedy this by purging the table from an entity if that entity doesn't occur in the chat history
			self.chatEntityOccurences[entityName] = self.chatEntityOccurences[entityName] - 1
			
			if self.chatEntityOccurences[entityName] <= 0 then
				-- the occurence has dropped to 0 so we want purge the entity from our
				-- chat colour tables
				self.chatEntityColours[entityName] = nil
				self.chatEntityOccurences[entityName] = nil
			end
		end
	end
end

--Checks whether given line contains name of the player that is currently playing
function Console:chatMessageContainsPlayerName(line)
	if self.avatarEntity then
		return line:find(self.avatarEntity:getName()) ~= nil
	end
	return false
end

function Console:getChatMessageColour(line, entity, entityTalk)
	local propertyName = "ChatMessageColour"
	local addressedToUs = false
	local addressedToOther = false
	
	--If it's something we've said ourselves, just use normal colour.
	if self.avatarEntity and entity and self.avatarEntity:getId() == entity:getId() then
		propertyName = "ChatMessageColour"
	else
		--check if the player is addressed
		if entityTalk then
			if self.avatarEntity then
				if entityTalk:isAddressedToEntity(self.avatarEntity:getId()) then
					addressedToUs = true
				end
				if not entityTalk:isAddressedToNone() and not addressedToUs then
					addressedToOther = true
				end
			end
		end
		if self:chatMessageContainsPlayerName(line) or addressedToUs then
			--if the message contains users name, or it's addressed to the player, lets make it stand out
			propertyName = "ChatMessageContainingSelfColour"
		elseif addressedToOther then
			--if the message is addressed to others, mark it 
			propertyName = "ChatMessageAddressedToOther"
		end
	end
	
	if not self.widget:getMainWindow():isPropertyPresent(propertyName) then
		return "FF000000" --reasonable fallback
	end
	
	return self.widget:getMainWindow():getProperty(propertyName)
end

function Console:appendChatMessage(line, entity, entityStartSymbol, entityEndSymbol, entityTalk)
	local messageColour = self:getChatMessageColour(line, entity, entityTalk)
	
	if entity then
		local entityNameColour = self:getColourForEntityName(entity:getName())
	
		self:appendLine("[colour='" .. entityNameColour .. "']" .. escapeForCEGUI(entityStartSymbol .. entity:getName() .. entityEndSymbol) .. " [colour='" .. messageColour .. "']" .. escapeForCEGUI(line), self.gameTab)
	else
		self:appendLine("[colour='" .. messageColour .. "']" .. escapeForCEGUI(line), self.gameTab)
	end
	
	if self:chatMessageContainsPlayerName(line) then
		--user might want to pay attention to the console, another player mentioned his/her name!
		self.widget:getMainWindow():fireEvent("RequestAttention", CEGUI.WindowEventArgs:new_local(self.mainWindow))
	end
end

--handler for Out Of Game chat event
--appends given text to the bottom of the Chat/Game tab
function Console:appendOOGChatLine(line, entity)
	self:appendChatMessage(line, entity, "{", "}")
end

--handler for In Game chat events
--appends given text to the bottom of the Chat/Game tab
function Console:appendIGChatLine(entityTalk, entity)
	self:appendChatMessage(entityTalk:getMessage(), entity, "<", ">", entityTalk)
end

function Console:appendAvatarImaginary(line)
	self:appendLine(line, self.gameTab)
end

--Appends raw, preformatted CEGUI string to given tab window
--This function does no escaping, you have to take care of that yourself!
function Console:appendLine(line, tab)
	local window = tab.textWindow
	
	local newText = window:getText() .. "\n" .. line
	tab.lines = tab.lines + 1

	--while is used to be extra safe
	while tab.lines >= tab.maxLines do
		local firstLineEnd = newText:find("\n")
		local purgedLine = newText:sub(1, firstLineEnd + 1)

		--basically strips the first line out
		newText = newText:sub(firstLineEnd + 1)
		tab.lines = tab.lines - 1
		
		self:notifyLinePurged(purgedLine, tab)
	end
	
	-- check whether the scrollbar is at its end, this means it's scrolled all the way down
	-- if it's not all the way down, we want to console to "stick"
	local atEnd = false
	-- the minus 1 is used as a measure against rounding errors and cases where the scroll position is very close but not right at the end
	local maxScroll = tonumber(window:getProperty("VertScrollDocumentSize")) - tonumber(window:getProperty("VertScrollPageSize")) - 1
	if (maxScroll <= 0) or (tonumber(window:getProperty("VertScrollPosition")) >= maxScroll) then
		atEnd = true
	end
	
	window:setText(newText)
	
	if atEnd then
		--make sure that the newly added line is shown since we were at the end before
		window:setProperty("VertScrollPosition", window:getProperty("VertExtent"))
	end

end

--Retrieves suitable AARRGGBB colour for console messages based on their tag
--tag can be empty (""), or for example "error", "warning", "help", etc...
function Console:getConsoleMessageColourForTag(tag)
	if tag == "" then
		tag = "unknown"
	end

	--makes the first letter uppercase so it meshes well with the CamelCase property convention that's
	--used in the looknfeels/layouts
	local propertyName = "ConsoleMessageColour" .. tag:sub(1, 1):upper() .. tag:sub(2)
	
	if not self.widget:getMainWindow():isPropertyPresent(propertyName) then
		return "FF000000" --reasonable fallback
	end
	
	return self.widget:getMainWindow():getProperty(propertyName)
end

--Called from C++ using signals
--message is the message string
--tag can be "" or "error", "warning" or such
function Console:consoleGotMessage(message, tag)
	self:appendLine("[colour='" .. self:getConsoleMessageColourForTag(tag) .. "']" .. escapeForCEGUI(message), self.gameTab)
	
	if tag == "error" or tag == "important" then
		--user might want to pay attention to the console, either error or an important message was appended
		self.widget:getMainWindow():fireEvent("RequestAttention", CEGUI.WindowEventArgs:new_local(self.mainWindow))
	end
	
	return true
end

function Console:shutdown()
	deleteSafe(self.consoleObject)
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end


local consoleInit = function()
	local console = {connectors={}, 
		widget = guiManager:createWidget(), 
		gameTab = {unviewedCount = 0, index = 0},
		consoleAdapter = nil,
		consoleInputWindow = nil
	}
	setmetatable(console, {__index = Console})
	
	console:buildWidget()

	connect(console.connectors, emberOgre.EventGUIManagerBeingDestroyed, function()
		console:shutdown()
		console = nil
	end)
end
consoleInit()
