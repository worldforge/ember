--[[
A widget for allowing interaction with the environment, such as setting the time of day and changing the fog distance.

]]--
Environment = {}

function Environment:Time_ValueChanged(args)
	local timeValue = self.timeSlider:getCurrentValue()
	
	self.environment:setTime(86400 * timeValue)
	return true
end

function Environment:FogDistance_ValueChanged(args)
	local densityValue = self.fogDensitySlider:getCurrentValue()
	
	local fog = self.environment:getFog()
	if fog ~= nil then
		fog:setDensity(0.01 * densityValue)
	end
	return true
end

function Environment:TimeMultiplier_ValueChanged(args)
	local multiplier = self.timeMultiplierSpinner:getCurrentValue()
	
	self.environment:setTimeMultiplier(multiplier)
	return true
end


function Environment:buildWidget(environmentObject)

	self.environment = environmentObject
	self.widget = guiManager:createWidget()
	
	self.widget:loadMainSheet("Environment.layout", "Environment/")
	
	self.timeSlider = CEGUI.toSlider(self.widget:getWindow("Time"))
	self.timeSlider:subscribeEvent("ValueChanged", self.Time_ValueChanged, self) 
	
	self.fogDensitySlider = CEGUI.toSlider(self.widget:getWindow("FogDistance"))
	self.fogDensitySlider:subscribeEvent("ValueChanged", self.FogDistance_ValueChanged, self) 
	
	self.timeMultiplierSpinner = CEGUI.toSpinner(self.widget:getWindow("TimeMultiplier"))
--	self.timeMultiplierSpinner:setCurrentValue(self.environment:getTimeMultiplier())
	self.timeMultiplierSpinner:setCurrentValue(1)
	self.timeMultiplierSpinner:subscribeEvent("ValueChanged", self.TimeMultiplier_ValueChanged, self) 
	
	
	self.widget:registerConsoleVisibilityToggleCommand("environment")
	self.widget:enableCloseButton()
	self.widget:hide()
end

function Environment:shutdown()
	guiManager:destroyWidget(self.widget)
	environment = nil
end

Environment.gotViewConnection = createConnector(emberOgre.EventWorldCreated):connect(function(world)
	local environmentObject = world:getEnvironment()
	environment = {connectors={}, timeSlider = nil}
	setmetatable(environment, {__index = Environment})
	connect(environment.connectors, emberOgre.EventWorldDestroyed, Environment.shutdown, environment)
	environment:buildWidget(environmentObject)
end
)
