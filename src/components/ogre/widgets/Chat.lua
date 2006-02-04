-----------------------------------------
-- Start of handler functions
-----------------------------------------

-----------------------------------------
-- Script Entry Point
-----------------------------------------
Chat = {}
Chat.widget = guiManager:createWidget()

function Chat_buildWidget()
	Chat.widget:loadMainSheet("Chat.layout", "Chat/")
	Chat.widget:registerConsoleVisibilityToggleCommand("chat")
	Chat.widget:enableCloseButton();
	
	EmberOgre.LuaConnector:new(guiManager.AppendIGChatLine):connect("Chat_appendIGChatLine")
	EmberOgre.LuaConnector:new(guiManager.AppendOOGChatLine):connect("Chat_appendIGChatLine")
	
	--let's hide it to beging with
	Chat.widget:hide()
	--and show if when the avatar has been created (though this disallows out of game chat)
	EmberOgre.LuaConnector:new(emberOgre.EventCreatedAvatarEntity):connect("Chat_createdAvatarEmberEntity")
end

function Chat_createdAvatarEmberEntity(avatarEntity)
	--show the chat window upon creation of the avatarEntity
	--this should perhaps happen before, to allow for Out Of Game chat?
	Chat.widget:show()
end

--handler for Out Of Game chat event
--adds messages to the top of the textbox
function Chat_appendOOGChatLine(line, entity)
	local textWnd = Chat.widget:getWindow("TextBox")
	local chatString = textWnd:getText()
	chatString = "{" .. entity:getName() .. "}" .. line .. "\n" .. chatString
	textWnd:setText(chatString)
	
end
--handler for In Game chat events
--adds messages to the top of the textbox
function Chat_appendIGChatLine(line, entity)
	local textWnd = Chat.widget:getWindow("TextBox")
	local chatString = textWnd:getText()
	chatString = "<" .. entity:getName() .. ">" .. line .. "\n" .. chatString
	textWnd:setText(chatString)
end

Chat_buildWidget()
