-----------------------------------------


-----------------------------------------
Environment = {
timeSlider = nil
, widget = guiManager:createWidget()
}


function Environment.buildWidget()
	
	Environment.widget:loadMainSheet("Environment.layout", "Environment/")
	
	Environment.timeSlider = Environment.widget:getWindow("Time")
	Environment.timeSlider = CEGUI.toSlider(Environment.timeSlider)
	
	Environment.fogDensitySlider = Environment.widget:getWindow("FogDistance")
	Environment.fogDensitySlider = CEGUI.toSlider(Environment.fogDensitySlider)
	
	Environment.widget:registerConsoleVisibilityToggleCommand("environment")
	Environment.widget:enableCloseButton()
	Environment.widget:hide()
	
end

function ModelEdit_zoom_ValueChanged(args)
	local timeValue = Environment.timeSlider:getCurrentValue()
	ModelEdit.renderer:setCameraDistance(zoomValue)
end

function Environment.Time_ValueChanged(args)
	local timeValue = Environment.timeSlider:getCurrentValue()
	
	local entityFactory = emberOgre:getEntityFactory()
	if entityFactory ~= nil then
		local worldEntity = entityFactory:getWorld()
		if worldEntity ~= nil then
			local environment = worldEntity:getEnvironment()
			if environment ~= nil then
				environment:setTime(86400 * timeValue)
			end
		end
	end
end

function Environment.FogDistance_ValueChanged(args)
	local densityValue = Environment.fogDensitySlider:getCurrentValue()
	
	local entityFactory = emberOgre:getEntityFactory()
	if entityFactory ~= nil then
		local worldEntity = entityFactory:getWorld()
		if worldEntity ~= nil then
			local environment = worldEntity:getEnvironment()
			if environment ~= nil then
				local fog = environment:getFog()
				if fog ~= nil then
					fog:setDensity(0.01 * densityValue)
				end
			end
		end
	end
	
end

Environment.buildWidget()