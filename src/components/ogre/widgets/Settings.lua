SettingsWidget = {}

function SettingsWidget:buildWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("Settings.layout", "Settings/")
	
	self.window = self.widget:getWindow("MainWindow")
	self.window:subscribeEvent("CloseClicked", self.CloseClicked, self)
	
	self.tabs = self.widget:getWindow("MainWindow/Tabs")
	
	self:buildSettingsUi()
end

function SettingsWidget:buildSettingsUi()
	--This is a place where the settings contents are declared and (later) used to construct the GUI
	--to manipulace them
	
	local settings =
	{
		Audio =
		{
		},
		Graphics =
		{
		}
	}
	
	for category, representations in pairs(settings) do
		local wnd = self:buildUiFor(representations)
		
		wnd:setText(category)
		self.tabs:addChildWindow(wnd)
	end
end

function SettingsWidget:buildUiFor(representations)
	local ret = CEGUI.WindowManager:getSingleton():createWindow("DefaultWindow")
	
	return ret
end

function SettingsWidget:CloseClicked(args)
	self.widget:hide()
end

settingsWidget = {connectors={}}
setmetatable(settingsWidget, {__index = SettingsWidget})
settingsWidget:buildWidget()
