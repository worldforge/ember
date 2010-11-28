----------------------------------------------------
--A simple chat widget. It currently only shows chat output, input is handled by the console adapter.
----------------------------------------------------


Console = {}

--Set up the widget.
function Console:buildWidget()
	self.tabs = {self.gameTab, self.systemTab}
	
	self.widget:loadMainSheet("Console.layout", "Console/")
	
	self.gameTab.textWindow = CEGUI.toMultiLineEditbox(self.widget:getWindow("GameTextBox"))
	self.gameTab.tabWindow = CEGUI.toMultiLineEditbox(self.widget:getWindow("GamePanel"))
	self.gameTab.prefix = self.gameTab.tabWindow:getText()
	
	self.systemTab.textWindow = CEGUI.toMultiLineEditbox(self.widget:getWindow("SystemTextBox"))
	self.systemTab.tabWindow = CEGUI.toMultiLineEditbox(self.widget:getWindow("SystemPanel"))
	self.systemTab.prefix = self.systemTab.tabWindow:getText()

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

--handler for Out Of Game chat event
--adds messages to the top of the textbox
function Console:appendOOGChatLine(line, entity)
	if entity ~= nil then
		self:appendLine("{" .. entity:getName() .. "}" .. line, self.gameTab)
	else 
		self:appendLine(line, self.gameTab)
	end
end

--handler for In Game chat events
--adds messages to the top of the textbox
function Console:appendIGChatLine(line, entity)
	if entity ~= nil then
		self:appendLine("<" .. entity:getName() .. ">" .. line, self.gameTab)
	else
		self:appendLine(line, self.gameTab)
	end
end

function Console:appendAvatarImaginary(line)
	self:appendLine(line, self.gameTab)
end

function Console:appendLine(line, tab)
	local window = tab.textWindow
--	chatString = "<" .. entity:getName() .. ">" .. line .. "\n" .. chatString
	window:setText(window:getText() .. line)
	--make sure that the newly added line is shown
	window:setCaratIndex(string.len(window:getText()))
	window:ensureCaratIsVisible()
	if not window:isVisible() then
		tab.unviewedCount = tab.unviewedCount + 1
		tab.tabWindow:setText(tab.prefix .. "(" .. tab.unviewedCount .. ")")
	end 
end

function Console:consoleGotMessage(message)
	self:appendLine(message, self.systemTab)
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
