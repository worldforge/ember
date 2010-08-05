--[[
Shows the inspect widget when the world has been entered.

Note that since the widget currently is coded in C++ we'll use the createWidget(...) method of creating it. 
]]--

Inspect = {}


--Show the inspect widget when the world has been entered
Inspect.createdWorldConnector = EmberOgre.LuaConnector:new_local(emberOgre.EventWorldCreated):connect(function(world)
		local inspectWidget = guiManager:createWidget("InspectWidget")
		
		local destroyedConnector = EmberOgre.LuaConnector:new_local(emberOgre.EventWorldDestroyed):connect(function()
				guiManager:destroyWidget(inspectWidget)
				inspectWidget = nil
				destroyedConnector:disconnect()
				destroyedConnector = nil
			end
		)
	end
)
