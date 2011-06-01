----------------------------------------------------
--A simple chat widget. It currently only shows chat output, input is handled by the console adapter.
----------------------------------------------------

Console = {}

--Set up the widget.
function Console:buildWidget()
	self.tabs = {self.gameTab, self.systemTab}
	
	--console has transitions, if we don't set this to false, every time it gets activated a 
	--visible "alpha pop" would happen and then the transition would continue
	self.widget:setIsActiveWindowOpaque(false)
	
	self.widget:loadMainSheet("Console.layout", "Console/")
	
	--these are used for coloured chat messages
	self.chatEntityColours = {}
	self.chatTotalColourUsage = {}
	for i = 1, 8 do
		self.chatTotalColourUsage[i] = 0
	end
	
	self.gameTab.textWindow = self.widget:getWindow("GameTextBox")
	self.gameTab.tabWindow = self.widget:getWindow("GamePanel")
	self.gameTab.prefix = self.gameTab.tabWindow:getText()
	self.gameTab.maxLines = 100
	self.gameTab.lines = 0
	
	self.systemTab.textWindow = self.widget:getWindow("SystemTextBox")
	self.systemTab.tabWindow = self.widget:getWindow("SystemPanel")
	self.systemTab.prefix = self.systemTab.tabWindow:getText()
	self.systemTab.maxLines = 100
	self.systemTab.lines = 0

	--When a tab has been selected and shown, the unread indicator should be reset	
	local tabControl = CEGUI.toTabControl(self.widget:getWindow("MainTabControl"))
	self.widget:getWindow("MainTabControl"):subscribeEvent("TabSelectionChanged", function(args)
			local selectedTab = self.tabs[tabControl:getSelectedTabIndex() + 1] --Lua uses 1 as the first index
			selectedTab.tabWindow:setText(selectedTab.prefix)
			selectedTab.unviewedCount = 0
			return true	
		end
	)
	
	
	self.consoleInputWindow = CEGUI.toEditbox(self.widget:getWindow("InputBox"))
	
	--this will bring console functionality to the editbox (such as history, tab completion etc.)
	self.consoleAdapter = Ember.OgreView.Gui.ConsoleAdapter:new_local(self.consoleInputWindow);
	
	
	connect(self.connectors, guiManager.AppendIGChatLine, self.appendIGChatLine, self)
	connect(self.connectors, guiManager.AppendOOGChatLine, self.appendIGChatLine, self)
	connect(self.connectors, guiManager.AppendAvatarImaginary, self.appendAvatarImaginary, self)
	connect(self.connectors, self.consoleAdapter.EventCommandExecuted, self.consoleAdapter_CommandExecuted, self)
	
	
	
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
		local i = 1
		
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
	return ret
end

--Convenience function that returns the exact colour to use for an entity name
--Returned format is the CEGUI AARRGGBB format, FF000000 is black
function Console:getColourForEntityName(entityName)
	if entityName == emberOgre:getWorld():getAvatar():getEmberEntity():getName() then
		--if it's the current player speaking, we always use the same colour for that
		return self.widget:getMainWindow():getProperty("ChatEntityColourSelf")
	end

	local index = self:getColourIndexForEntityName(entityName)
	return self.widget:getMainWindow():getProperty("ChatEntityColour" .. index)
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
		end
		
		--FIXME: we have decreased the total colour use count but chatEntityColours will endlessly grow with new
		--       and new entities
		--
		--       I haven't devised a strategy for this yet, purging it when entity use count drops to 0 doesn't
		--       sound right but maybe is the best in this case, especially if max message count is high
	end
end

--CEGUI uses [tag=something] as a formatting syntax, we have to make sure special character [ gets escaped out
function Console:escapeForCEGUI(message)
	--Only the starting [ character needs to be escaped for CEGUI, escaping ] will cause CEGUI to show \]
	return string.gsub(message, "%[", "\\%[")
end

function Console:appendChatMessage(line, entity, entityStartSymbol, entityEndSymbol)
	local messageColour = self.widget:getMainWindow():getProperty("ChatMessageColour")
	if line:find(emberOgre:getWorld():getAvatar():getEmberEntity():getName()) ~= nil then
		--if the message contains users name, lets make it stand out
		messageColour = self.widget:getMainWindow():getProperty("ChatMessageContainingSelfColour")
	end
	
	if entity ~= nil then
		entityNameColour = self:getColourForEntityName(entity:getName())
	
		self:appendLine("[colour='" .. entityNameColour .. "']" .. self:escapeForCEGUI(entityStartSymbol .. entity:getName() .. entityEndSymbol) .. " [colour='" .. messageColour .. "']" .. self:escapeForCEGUI(line), self.gameTab)
	else
		self:appendLine("[colour='" .. messageColour .. "']" .. self:escapeForCEGUI(line), self.gameTab)
	end
end

--handler for Out Of Game chat event
--adds messages to the top of the textbox
function Console:appendOOGChatLine(line, entity)
	self:appendChatMessage(line, entity, "{", "}")
end

--handler for In Game chat events
--adds messages to the top of the textbox
function Console:appendIGChatLine(line, entity)
	self:appendChatMessage(line, entity, "<", ">")
end

function Console:appendAvatarImaginary(line)
	self:appendLine(line, self.gameTab)
end

--Appends raw, preformatted, CEGUI string to given tab window
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
	
	window:setText(newText)
	--make sure that the newly added line is shown
	window:setProperty("VertScrollPosition", window:getProperty("VertExtent"))
	
	if not window:isVisible() then
		tab.unviewedCount = tab.unviewedCount + 1
		tab.tabWindow:setText(tab.prefix .. "(" .. tab.unviewedCount .. ")")
	end
	
	-- user might want to pay attention to the console, as text was appended in one of the tabs
	self.widget:getMainWindow():fireEvent("RequestAttention", CEGUI.WindowEventArgs:new(self.mainWindow))
end

function Console:consoleGotMessage(message)
	self:appendLine(self:escapeForCEGUI(message), self.systemTab)
	return true
end

function Console:shutdown()
	deleteSafe(self.consoleObject)
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end


connect(connectors, emberOgre.EventGUIManagerInitialized, function(guiManager)
	local console = {connectors={}, 
		widget = guiManager:createWidget(), 
		gameTab = {unviewedCount = 0},
		systemTab = {unviewedCount = 0},
		consoleAdapter = nil,
		consoleInputWindow = nil
	}
	setmetatable(console, {__index = Console})
	
	console:buildWidget()

	connect(console.connectors, emberOgre.EventGUIManagerBeingDestroyed, function()
		console:shutdown()
		console = nil
	end)
end)
