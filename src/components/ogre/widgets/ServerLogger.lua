-- ServerLogger

ServerLogger = { connectors = {}, widget = guiManager:createWidget() }

function ServerLogger.buildWidget(widget)
	widget:loadMainSheet("ServerLogger.layout", "ServerLogger")

	--set up some useful values
	local logTextWidget = widget:getWindow("LogText")

	function cleanup()
		if ServerLogger.sendingConnection then
			ServerLogger.sendingConnection:disconnect()
			ServerLogger.sendingConnection = nil
		end
		if ServerLogger.receivedConnection then
			ServerLogger.receivedConnection:disconnect()
			ServerLogger.receivedConnection = nil
		end
	end
	function LoggingEnabled_SelectStateChanged(args)
		local checkBox = CEGUI.toToggleButton(widget:getWindow("LoggingEnabled"))
		if checkBox then
			cleanup()
			if checkBox:isSelected() then
				ServerLogger.sendingConnection = emberServices:getServerService().EventSendingObject:connect(function(obj)
					local newLogMessage = "Sending: \n" .. Ember.OgreView.Gui.AtlasHelper.serialize(obj, "presentation")

					log.info(newLogMessage)

					logTextWidget:appendText(newLogMessage .. "\n")
				end)
				ServerLogger.receivedConnection = emberServices:getServerService().EventReceivedObject:connect(function(obj)
					local newLogMessage = "Received: \n" .. Ember.OgreView.Gui.AtlasHelper.serialize(obj, "presentation")

					log.info(newLogMessage)

					logTextWidget:appendText(newLogMessage .. "\n")

				end)
			end
		end
		return true
	end



	--subscribe event
	widget:getWindow("LoggingEnabled"):subscribeEvent("SelectStateChanged", LoggingEnabled_SelectStateChanged)
	widget:getWindow("ClearButton"):subscribeEvent("Clicked", function(args)
		logTextWidget:setText("")
	end)

	widget:registerConsoleVisibilityToggleCommand("serverLogger")
	widget:enableCloseButton()
	widget:hide()

end

ServerLogger.buildWidget(ServerLogger.widget)