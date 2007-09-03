-----------------------------------------


-----------------------------------------
Environment = {}
Environment.timeSlider = nil
Environment.widget = guiManager:createWidget()

function Environment.buildWidget()
	
	Environment.widget:loadMainSheet("Environment.layout", "Environment/")
	
	Environment.timeSlider = Environment.widget:getWindow("Time")
	Environment.timeSlider = CEGUI.toSlider(Environment.timeSlider)
	
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

Environment.buildWidget()