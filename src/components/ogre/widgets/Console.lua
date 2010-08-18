----------------------------------------------------
--A simple chat widget. It currently only shows chat output, input is handled by the console adapter.
----------------------------------------------------


Console = {
connectors={}, 
widget = guiManager:createWidget(), 
gameTab = {unviewedCount = 0},
systemTab = {unviewedCount = 0},
consoleAdapter = nil,
consoleInputWindow = nil
}

--Set up the widget.
function Console.buildWidget()
	Console.tabs = {Console.gameTab, Console.systemTab}
	
	Console.widget:loadMainSheet("Console.layout", "Console/")
	
	Console.gameTab.textWindow = CEGUI.toMultiLineEditbox(Console.widget:getWindow("GameTextBox"))
	Console.gameTab.tabWindow = CEGUI.toMultiLineEditbox(Console.widget:getWindow("GamePanel"))
	Console.gameTab.prefix = Console.gameTab.tabWindow:getText()
	
	Console.systemTab.textWindow = CEGUI.toMultiLineEditbox(Console.widget:getWindow("SystemTextBox"))
	Console.systemTab.tabWindow = CEGUI.toMultiLineEditbox(Console.widget:getWindow("SystemPanel"))
	Console.systemTab.prefix = Console.systemTab.tabWindow:getText()

	--When a tab has been selected and shown, the unread indicator should be reset	
	local tabControl = CEGUI.toTabControl(Console.widget:getWindow("MainTabControl"))
	Console.widget:getWindow("MainTabControl"):subscribeEvent("TabSelectionChanged", function(args)
			local selectedTab = Console.tabs[tabControl:getSelectedTabIndex() + 1] --Lua uses 1 as the first index
			selectedTab.tabWindow:setText(selectedTab.prefix)
			selectedTab.unviewedCount = 0
			return true	
		end
	)
	
	
	Console.consoleInputWindow = CEGUI.toEditbox(Console.widget:getWindow("InputBox"))
	
	--this will bring console functionality to the editbox (such as history, tab completion etc.)
	Console.consoleAdapter = EmberOgre.Gui.ConsoleAdapter:new_local(Console.consoleInputWindow);
	
	
	connect(Console.connectors, guiManager.AppendIGChatLine, "Console.appendIGChatLine")
	connect(Console.connectors, guiManager.AppendOOGChatLine, "Console.appendIGChatLine")
	connect(Console.connectors, guiManager.AppendAvatarImaginary, "Console.appendAvatarImaginary")
	connect(Console.connectors, Console.consoleAdapter.EventCommandExecuted, "Console.consoleAdapter_CommandExecuted")
	
	
	
	Console.widget:show()
	connect(Console.connectors, console.GotMessage, "Console.consoleGotMessage")
	
	Console.consoleObject = Ember.Lua.LuaConsoleObject:new_local("console_focus", "Console.console_focus")
	

end

function Console.consoleAdapter_CommandExecuted(command)
	--if the user presses enter with nothing entered, toggle to movement mode. This will allow for easy switching between chat and movement
	if command == "" then
		--remove focus from the console
		Console.widget:getMainWindow():deactivate()
		Ember.Input:getSingleton():setInputMode(Ember.Input.IM_MOVEMENT)
	end
end

function Console.console_focus()
	Console.consoleInputWindow:activate()
	--switch to gui mode, so that this command can be called even when the user is in movement mode
	Ember.Input:getSingleton():setInputMode(Ember.Input.IM_GUI)
end

--handler for Out Of Game chat event
--adds messages to the top of the textbox
function Console.appendOOGChatLine(line, entity)
	if entity ~= nil then
		Console.appendLine("{" .. entity:getName() .. "}" .. line, Console.gameTab)
	else 
		Console.appendLine(line, Console.gameTab)
	end
end

--handler for In Game chat events
--adds messages to the top of the textbox
function Console.appendIGChatLine(line, entity)
	if entity ~= nil then
		Console.appendLine("<" .. entity:getName() .. ">" .. line, Console.gameTab)
	else
		Console.appendLine(line, Console.gameTab)
	end
end

function Console.appendAvatarImaginary(line)
	Console.appendLine(line, Console.gameTab)
end

function Console.appendLine(line, tab)
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

function Console.consoleGotMessage(message)
	Console.appendLine(message, Console.systemTab)
	return true
end

Console.buildWidget()
