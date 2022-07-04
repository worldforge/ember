Performance = { connectors = {}, widget = guiManager:createWidget() }

function Performance:buildWidget()
	self.widget:loadMainSheet("Performance.layout", "Performance")

	local window = self.widget:getWindow("TextBox")
	local mainText = CEGUI.toMultiLineEditbox(window)

	connect(self.connectors, emberOgre.EventTerrainManagerCreated, function(terrainManager)
		self.terrainManager = terrainManager
	end)
	connect(self.connectors, emberOgre.EventMotionManagerCreated, function(motionManager)
		self.motionManager = motionManager
	end)
	connect(self.connectors, emberOgre.EventTerrainManagerDestroyed, function()
		self.terrainManager = nil
	end)
	connect(self.connectors, emberOgre.EventMotionManagerDestroyed, function()
		self.motionManager = nil
	end)

	connect(self.connectors, emberServices:getServerService().GotView, function(view)
		self.view = view
	end)
	connect(self.connectors, emberServices:getServerService().DestroyedView, function()
		self.view = nil
	end)

	connect(self.connectors, self.widget.EventFrameStarted, function()
		if (self.widget:getMainWindow():isVisible()) then
			local statString

			local stats = emberOgre:getScreen():getFrameStats()

			--statString = "Current FPS: " .. string.format("%i", stats.lastFPS)
			statString = "FPS: " .. string.format("%i", math.floor(stats.avgFPS))
			statString = statString .. "\nTriangle count: " .. string.format("%i", stats.triangleCount)
			statString = statString .. "\nBatch count: " .. string.format("%i", stats.batchCount)
			if self.view then
				statString = statString .. "\nSightqueue: " .. self.view:lookQueueSize()
			end
			if self.motionManager then
				local motionInfo = self.motionManager:getInfo()
				statString = statString .. "\nAnimated: " .. motionInfo.AnimatedEntities
				statString = statString .. "\nMoving: " .. motionInfo.MovingEntities
			end
			--ss << "Time in eris: " << getAverageErisTime() * 100 << "% \n"

			-- NOTE: commented out because currently, it does not work and breaks the widget
			-- if self.terrainManager then
			-- 	statString = statString .. "\n" .. self.terrainManager:getAdapter():getDebugInfo()
			-- end

			mainText:setText(statString)
		end

	end)

	self.widget:registerConsoleVisibilityToggleCommand("performance")
	self.widget:enableCloseButton()
	self.widget:hide()
end

Performance:buildWidget()

