--[[
Shows the inspect widget when the world has been entered.

Note that since the widget currently is coded in C++ we'll use the createWidget(...) method of creating it. 
]]--

Inspect = {}


--Show the inspect widget when the world has been entered
Inspect.createdWorldConnector = Ember.Lua.Connector:createConnector(emberOgre.EventWorldCreated):connect(function(world)
		local inspectWidget = guiManager:createWidget("InspectWidget")
		local connectors = {}
		
		connect(connectors, emberOgre.EventWorldDestroyed, function()
				guiManager:destroyWidget(inspectWidget)
				inspectWidget = nil
				disconnectAll(connectors)
				connectors = nil
			end
		)
	end
)
