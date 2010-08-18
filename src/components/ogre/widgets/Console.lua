----------------------------------------------------
--A simple chat widget. It currently only shows chat output, input is handled by the console adapter.
----------------------------------------------------


Console = {
connectors={}, 
widget = guiManager:createWidget(), 
gameTextWindow = nil,
systemTextWindow = nil,
consoleAdapter = nil,
consoleInputWindow = nil
}

--Set up the widget.
function Console.buildWidget()
	Console.widget:loadMainSheet("Console.layout", "Console/")
	
	Console.gameTextWindow = CEGUI.toMultiLineEditbox(Console.widget:getWindow("GameTextBox"))
	Console.systemTextWindow = CEGUI.toMultiLineEditbox(Console.widget:getWindow("SystemTextBox"))
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
		Console.appendLine("{" .. entity:getName() .. "}" .. line, Console.gameTextWindow)
	else 
		Console.appendLine(line, Console.gameTextWindow)
	end
end

--handler for In Game chat events
--adds messages to the top of the textbox
function Console.appendIGChatLine(line, entity)
	if entity ~= nil then
		Console.appendLine("<" .. entity:getName() .. ">" .. line, Console.gameTextWindow)
	else
		Console.appendLine(line, Console.gameTextWindow)
	end
end

function Console.appendAvatarImaginary(line)
	Console.appendLine(line, Console.gameTextWindow)
end

function Console.appendLine(line, window)
--	chatString = "<" .. entity:getName() .. ">" .. line .. "\n" .. chatString
	window:setText(window:getText() .. line)
	--make sure that the newly added line is shown
	window:setCaratIndex(string.len(window:getText()))
	window:ensureCaratIsVisible() 
end

function Console.consoleGotMessage(message)
	Console.appendLine(message, Console.systemTextWindow)
	return true
end

Console.buildWidget()
