----------------------------------------------------
--A simple chat widget. It currently only shows chat output, input is handled by the console (is this wise?).
----------------------------------------------------


Chat = {}
Chat.widget = guiManager:createWidget()

--Set up the widget.
function Chat.buildWidget()
	Chat.widget:loadMainSheet("Chat.layout", "Chat/")
	Chat.widget:registerConsoleVisibilityToggleCommand("chat")
	Chat.widget:enableCloseButton();
	
	EmberOgre.LuaConnector:new(guiManager.AppendIGChatLine):connect("Chat.appendIGChatLine")
	EmberOgre.LuaConnector:new(guiManager.AppendOOGChatLine):connect("Chat.appendIGChatLine")
	
	--let's hide it to begin with
	Chat.widget:hide()
	--and show if when the avatar has been created (though this disallows out of game chat)
	EmberOgre.LuaConnector:new(emberOgre.EventCreatedAvatarEntity):connect("Chat.createdAvatarEmberEntity")
end

function Chat.createdAvatarEmberEntity(avatarEntity)
	--show the chat window upon creation of the avatarEntity
	--this should perhaps happen before, to allow for Out Of Game chat?
	Chat.widget:show()
end

--handler for Out Of Game chat event
--adds messages to the top of the textbox
function Chat.appendOOGChatLine(line, entity)
	Chat.appendLine("{" .. entity:getName() .. "}" .. line)
end

--handler for In Game chat events
--adds messages to the top of the textbox
function Chat.appendIGChatLine(line, entity)
	Chat.appendLine("<" .. entity:getName() .. ">" .. line)
end

function Chat.appendLine(line)
	local window = Chat.widget:getWindow("TextBox")
	local textWnd = CEGUI.toMultiLineEditbox(window)
--	chatString = "<" .. entity:getName() .. ">" .. line .. "\n" .. chatString
	textWnd:setText(textWnd:getText() .. line)
	--make sure that the newly added line is shown
	textWnd:setCaratIndex(string.len(textWnd:getText()))
	textWnd:ensureCaratIsVisible() 
end

Chat.buildWidget()
