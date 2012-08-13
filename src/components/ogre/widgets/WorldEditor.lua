WorldEditor = {}

function WorldEditor:buildWidget()

	self.widget = guiManager:createWidget()

	local setup = function()
		
		self.widget:getWindow("DumpWorld"):subscribeEvent("Clicked", function(args)
			local worldDumper = Ember.WorldDumper:new(emberServices:getServerService():getAccount())
			self.widget:getWindow("DumpStatus"):setText("Dumping...")
			createConnector(worldDumper.EventCompleted):connect(function()
				self.widget:getWindow("DumpStatus"):setText("Done dumping.")
				worldDumper:delete()			
			end)
			worldDumper:start(emberServices:getConfigService():getHomeDirectory() .. "/world.xml")
			return true
		end)

		self.widget:getWindow("LoadWorld"):subscribeEvent("Clicked", function(args)
			local worldLoader = Ember.WorldLoader:new(emberServices:getServerService():getAccount())
			self.widget:getWindow("DumpStatus"):setText("Loading...")
			createConnector(worldLoader.EventCompleted):connect(function()
				self.widget:getWindow("DumpStatus"):setText("Done loading.")
				worldLoader:delete()			
			end)
			worldLoader:start(emberServices:getConfigService():getHomeDirectory() .. "/world.xml")
			return true
		end)
		
			
		self.widget:enableCloseButton()
	end

	connect(self.connectors, self.widget.EventFirstTimeShown, setup)
	self.widget:loadMainSheet("WorldEditor.layout", "WorldEditor/")
	self.widget:registerConsoleVisibilityToggleCommand("worldEditor")

end


function WorldEditor:shutdown()
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

WorldEditor.createdWorldConnector = createConnector(emberOgre.EventWorldCreated):connect(function(world)
		worldEditor = {connectors={}, world=world}
		setmetatable(worldEditor, {__index = WorldEditor})
		
		worldEditor:buildWidget()
		connect(worldEditor.connectors, emberOgre.EventWorldDestroyed, function()
				worldEditor:shutdown()
				worldEditor = nil
			end
		)
	end
)


