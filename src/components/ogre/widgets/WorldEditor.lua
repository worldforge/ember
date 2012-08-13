WorldEditor = {}

function WorldEditor:buildWidget()

	self.widget = guiManager:createWidget()

	local setup = function()
		local cancelButton = self.widget:getWindow("Cancel")
		cancelButton:subscribeEvent("Clicked", function(args)
			if cancelButton.method then
				cancelButton.method()
				return true
			end
		end)

		self.widget:getWindow("DumpWorld"):subscribeEvent("Clicked", function(args)
			local worldDumper = Ember.WorldDumper:new(emberServices:getServerService():getAccount())
			self.widget:getWindow("Status"):setText("Dumping...")
			createConnector(worldDumper.EventCompleted):connect(function()
				self.widget:getWindow("Status"):setText("Done dumping.")
				cancelButton:setVisible(false)
				cancelButton.method = nil
				worldDumper:delete()			
			end)
			createConnector(worldDumper.EventProgress):connect(function(entitiesDumped)
				self.widget:getWindow("Status"):setText("Dumping, " .. entitiesDumped .. " dumped")
			end)
			cancelButton.method = function()
				worldDumper:cancel()
			end
			cancelButton:setVisible(true)
			worldDumper:start(emberServices:getConfigService():getHomeDirectory() .. "/world.xml")
			return true
		end)

		self.widget:getWindow("LoadWorld"):subscribeEvent("Clicked", function(args)
			local worldLoader = Ember.WorldLoader:new(emberServices:getServerService():getAccount())
			self.widget:getWindow("Status"):setText("Loading...")
			createConnector(worldLoader.EventCompleted):connect(function()
				self.widget:getWindow("Status"):setText("Done loading.")
				cancelButton:setVisible(false)
				cancelButton.method = nil
				worldLoader:delete()			
			end)
			createConnector(worldLoader.EventProgress):connect(function(entitiesToLoad)
				self.widget:getWindow("Status"):setText("Loading, " .. entitiesToLoad .. " left")
			end)
			cancelButton.method = function()
				worldLoader:cancel()
			end
			cancelButton:setVisible(true)
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


