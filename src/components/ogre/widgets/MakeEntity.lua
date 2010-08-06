--[[
A simple widget for creating new entities.

Note that since the widget currently is coded in C++ we'll use the createWidget(...) method of creating it. 
]]--

MakeEntity = {}


--Show the make entity widget when the world has been entered
MakeEntity.createdWorldConnector = EmberOgre.LuaConnector:new_local(emberOgre.EventWorldCreated):connect(function(world)
		local makeEntityWidget = guiManager:createWidget("MakeEntityWidget")
		local connectors = {}
		connect(connectors, emberOgre.EventWorldDestroyed, function()
				guiManager:destroyWidget(makeEntityWidget)
				makeEntityWidget = nil
				disconnectAll(connectors)
				connectors = nil
			end
		)
	end
)
