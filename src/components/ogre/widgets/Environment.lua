--[[ Environment
A widget for allowing interaction with the environment, such as setting the time of day and changing the fog distance.

]]--
Environment = {}

function Environment:buildWidget(environmentObject)

	self.environment = environmentObject
	self.widget = guiManager:createWidget()

	self.widget:loadMainSheet("Environment.layout", "Environment")

	self.timeSlider = CEGUI.toSlider(self.widget:getWindow("Time"))
	self.timeSlider:subscribeEvent("ValueChanged", function()
		local timeValue = self.timeSlider:getCurrentValue()

		self.environment:setTime(math.floor(86400 * timeValue))
		return true
	end)

	self.fogDensitySlider = CEGUI.toSlider(self.widget:getWindow("FogDistance"))
	self.fogDensitySlider:subscribeEvent("ValueChanged", function()
		local densityValue = self.fogDensitySlider:getCurrentValue()

		local fog = self.environment:getFog()
		if fog then
			fog:setDensity(0.01 * densityValue)
		end
		return true
	end)

	self.timeMultiplierSpinner = CEGUI.toSpinner(self.widget:getWindow("TimeMultiplier"))
	--	self.timeMultiplierSpinner:setCurrentValue(self.environment:getTimeMultiplier())
	self.timeMultiplierSpinner:setCurrentValue(1)
	self.timeMultiplierSpinner:subscribeEvent("ValueChanged", function()
		local multiplier = self.timeMultiplierSpinner:getCurrentValue()

		self.environment:setTimeMultiplier(multiplier)
		return true
	end
	)

	self.widget:registerConsoleVisibilityToggleCommand("environment")
	self.widget:enableCloseButton()
	self.widget:hide()
end

function Environment:shutdown()
	guiManager:destroyWidget(self.widget)
	disconnectAll(self.connectors)
	environment = nil
	collectgarbage()
end

Environment.gotViewConnection = emberOgre.EventWorldCreated:connect(function(world)
	local environmentObject = world:getEnvironment()
	environment = { connectors = {}, timeSlider = nil }
	setmetatable(environment, { __index = Environment })
	connect(environment.connectors, emberOgre.EventWorldDestroyed, Environment.shutdown, environment)
	environment:buildWidget(environmentObject)
end
)
