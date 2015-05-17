-- -----------------------------------------
-- Start of handler functions
-----------------------------------------

-----------------------------------------
-- Script Entry Point
-----------------------------------------

ServerLogger = {connectors={}}


ServerLogger.widget = guiManager:createWidget()

function ServerLogger.buildWidget()
	
	ServerLogger.widget:loadMainSheet("ServerLogger.layout", "ServerLogger")
	
	--set up some useful values
	ServerLogger.logTextWidget = ServerLogger.widget:getWindow("LogText")
	
	--subscribe event
	ServerLogger.widget:getWindow("LoggingEnabled"):subscribeEvent("SelectStateChanged", ServerLogger.LoggingEnabled_SelectStateChanged)
	ServerLogger.widget:getWindow("ClearButton"):subscribeEvent("Clicked", ServerLogger.ClearButton_Clicked)
	
	
	ServerLogger.widget:registerConsoleVisibilityToggleCommand("serverLogger")
	ServerLogger.widget:enableCloseButton()
	ServerLogger.widget:hide()

end

function ServerLogger.server_SendingObject(obj)
	local newLogMessage = "Sending: \n" .. Ember.OgreView.Gui.AtlasHelper:serialize(obj, "presentation")
	
	log.info(newLogMessage)

	ServerLogger.logTextWidget:appendText(newLogMessage .. "\n")
end

function ServerLogger.server_ReceivedObject(obj)
	local newLogMessage = "Received: \n" .. Ember.OgreView.Gui.AtlasHelper:serialize(obj, "presentation")

	log.info(newLogMessage)

	ServerLogger.logTextWidget:appendText(newLogMessage .. "\n")

end

function ServerLogger.ClearButton_Clicked(args)
	ServerLogger.logTextWidget:setText("")
end

function ServerLogger.LoggingEnabled_SelectStateChanged(args)
	local checkBox = CEGUI.toToggleButton(ServerLogger.widget:getWindow("LoggingEnabled"))
	if checkBox ~= nil then
		ServerLogger.cleanup()
		if checkBox:isSelected() then
			ServerLogger.sendingConnection = createConnector(emberServices:getServerService().EventSendingObject)
			ServerLogger.sendingConnection:connect(ServerLogger.server_SendingObject)
			ServerLogger.receivedConnection = createConnector(emberServices:getServerService().EventReceivedObject)
			ServerLogger.receivedConnection:connect(ServerLogger.server_ReceivedObject)
		end
	end
	return true
end

function ServerLogger.cleanup()
	if ServerLogger.sendingConnection ~= nil then
		ServerLogger.sendingConnection:disconnect()
		ServerLogger.sendingConnection = nil
	end
	if ServerLogger.receivedConnection ~= nil then
		ServerLogger.receivedConnection:disconnect()
		ServerLogger.receivedConnection = nil
	end
end

ServerLogger.buildWidget()