Performance = {}

Performance.widget = guiManager:createWidget()
Performance.mainText = nil

function Performance_buildWidget()
	Performance.widget:loadMainSheet("Performance.layout", "Performance/")
	
	local window = Performance.widget:getWindow("TextBox")
	Performance.mainText = CEGUI.toMultiLineEditbox(window)
	
--	EmberOgre::getSingleton().EventStartErisPoll.connect(sigc::mem_fun(*this, &Performance::startErisPolling));
--	EmberOgre::getSingleton().EventEndErisPoll.connect(sigc::mem_fun(*this, &Performance::endErisPolling));
	
	
	Performance.widget:registerConsoleVisibilityToggleCommand("performance")
	Performance.widget:enableCloseButton()
end

function Performance_framestarted(timeSinceLastFrame)
	if (Performance.widget:getMainWindow():isVisible()) then
		local statString
		local stats = emberOgre:getRenderWindow():getStatistics()
		
		--statString = "Current FPS: " .. string.format("%i", stats.lastFPS)
		statString = "FPS: " .. string.format("%i", stats.avgFPS)
		statString = statString .. "\nTriangle count: " .. string.format("%i", stats.triangleCount)
		if emberOgre:getMainView() ~= nil then
			statString = statString .. "\nLag: " .. emberOgre:getMainView():lookQueueSize()
		end
		local motionInfo = EmberOgre.MotionManager:getSingleton():getInfo()
		statString = statString .. "\nAnimated: " .. motionInfo.AnimatedEntities
		statString = statString .. "\nMoving: " .. motionInfo.MovingEntities
		statString = statString .. "\nAnimations: " .. motionInfo.Animations
		--ss << "Time in eris: " << getAverageErisTime() * 100 << "% \n"
	
		Performance.mainText:setText(statString)
	end

end

Performance_buildWidget()
EmberOgre.LuaConnector:new(guiManager.EventFrameStarted):connect("Performance_framestarted")

