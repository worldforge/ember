----------------------------------------------------
--A simple chat widget. It currently only shows chat output, input is handled by the console (is this wise?).
----------------------------------------------------


Chat = {
connectors={}, 
widget = guiManager:createWidget(), 
gameTextWindow = nil,
systemTextWindow = nil,
consoleAdapter = nil,
consoleInputWindow = nil
}

--Set up the widget.
function Chat.buildWidget()
	Chat.widget:loadMainSheet("Chat.layout", "Chat/")
-- 	Chat.widget:registerConsoleVisibilityToggleCommand("chat")
	Chat.widget:enableCloseButton();
	
	Chat.gameTextWindow = CEGUI.toMultiLineEditbox(Chat.widget:getWindow("GameTextBox"))
	Chat.systemTextWindow = CEGUI.toMultiLineEditbox(Chat.widget:getWindow("SystemTextBox"))
	Chat.consoleInputWindow = CEGUI.toEditbox(Chat.widget:getWindow("InputBox"))
	
	--this will bring console functionality to the editbox (such as history, tab completion etc.)
	Chat.consoleAdapter = EmberOgre.Gui.ConsoleAdapter:new_local(Chat.consoleInputWindow);
	
	
	connect(Chat.connectors, guiManager.AppendIGChatLine, "Chat.appendIGChatLine")
	connect(Chat.connectors, guiManager.AppendOOGChatLine, "Chat.appendIGChatLine")
	connect(Chat.connectors, guiManager.AppendAvatarImaginary, "Chat.appendAvatarImaginary")
	connect(Chat.connectors, Chat.consoleAdapter.EventCommandExecuted, "Chat.consoleAdapter_CommandExecuted")
	
	
	
	--let's hide it to begin with
--	Chat.widget:hide()
	Chat.widget:show()
	--and show if when the avatar has been created (though this disallows out of game chat)
	Chat.connectors.createdAvatarEmberEntity = EmberOgre.LuaConnector:new_local(emberOgre.EventCreatedAvatarEntity):connect("Chat.createdAvatarEmberEntity")
	connect(Chat.connectors, console.GotMessage, "Chat.consoleGotMessage")
	
	Chat.consoleObject = EmberOgre.LuaConsoleObject:new_local("console_focus", "Chat.console_focus")
	

end

function Chat.consoleAdapter_CommandExecuted(command)
	--if the user presses enter with nothing entered, toggle to movement mode. This will allow for easy switching between chat and movement
	if command == "" then
		--remove focus from the console
		Chat.widget:getMainWindow():deactivate()
		guiManager:getInput():setInputMode(EmberOgre.Input.IM_MOVEMENT)
	end
end

function Chat.console_focus()
	Chat.consoleInputWindow:activate()
	--switch to gui mode, so that this command can be called even when the user is in movement mode
	guiManager:getInput():setInputMode(EmberOgre.Input.IM_GUI)
end

function Chat.createdAvatarEmberEntity(avatarEntity)
	--show the chat window upon creation of the avatarEntity
	--this should perhaps happen before, to allow for Out Of Game chat?
	Chat.widget:show()
end

--handler for Out Of Game chat event
--adds messages to the top of the textbox
function Chat.appendOOGChatLine(line, entity)
	if entity ~= nil then
		Chat.appendLine("{" .. entity:getName() .. "}" .. line, Chat.gameTextWindow)
	else 
		Chat.appendLine(line, Chat.gameTextWindow)
	end
end

--handler for In Game chat events
--adds messages to the top of the textbox
function Chat.appendIGChatLine(line, entity)
	if entity ~= nil then
		Chat.appendLine("<" .. entity:getName() .. ">" .. line, Chat.gameTextWindow)
	else
		Chat.appendLine(line, Chat.gameTextWindow)
	end
end

function Chat.appendAvatarImaginary(line)
	Chat.appendLine(line, Chat.gameTextWindow)
end

function Chat.appendLine(line, window)
--	chatString = "<" .. entity:getName() .. ">" .. line .. "\n" .. chatString
	window:setText(window:getText() .. line)
	--make sure that the newly added line is shown
	window:setCaratIndex(string.len(window:getText()))
	window:ensureCaratIsVisible() 
end

function Chat.consoleGotMessage(message)
	Chat.appendLine(message, Chat.systemTextWindow)
	return true
end

Chat.buildWidget()
