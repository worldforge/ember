SettingsWidget = {}

function SettingsWidget:buildWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("Settings.layout", "Settings/")
	
	self.window = self.widget:getWindow("MainWindow")
	self.window:subscribeEvent("CloseClicked", self.CloseClicked, self)
end

function SettingsWidget:CloseClicked(args)
	self.widget:hide()
end

settingsWidget = {connectors={}}
setmetatable(settingsWidget, {__index = SettingsWidget})
settingsWidget:buildWidget()
