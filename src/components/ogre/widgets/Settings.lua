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
	
	local configService = emberServices:getConfigService()
	local Representations = Ember.OgreView.Gui.Representations
	
	self.settings =
	{
		Audio =
		{
			{
				"Enabled",
				Representations.VarconfCheckboxRepresentation:new((configService:getValue("audio", "enabled"))),
				"Controls whether sounds are played"
			},
			{
				"Output",
				Representations.VarconfStringComboboxRepresentation:new((configService:getValue("audio", "output"))),
				"Which sound output should be used",
				
				{"output", "test1", "test2"}
			},
		},
		Graphics =
		{
			{
				"Level",
				Representations.VarconfStringComboboxRepresentation:new((configService:getValue("graphics", "level"))),
				"General graphics level",
				
				{"low", "medium", "high"}
			},
			{
				"Fresnel water",
				Representations.VarconfCheckboxRepresentation:new((configService:getValue("graphics", "fresnelwater"))),
				"More realistic and performance demanding water"
			},
			{
				"Foliage",
				Representations.VarconfCheckboxRepresentation:new((configService:getValue("graphics", "foliage"))),
				"Render foliage over the terrain"
			},
		},
	}
	
	for category, representations in pairs(self.settings) do
		local wnd = self:buildUiFor(representations)
		
		wnd:setText(category)
		self.tabs:addChildWindow(wnd)
	end
end

function SettingsWidget:buildUiFor(representations)
	local ret = CEGUI.WindowManager:getSingleton():createWindow("EmberLook/ScrollablePane")
	ret:setProperty("UnifiedPosition", "{{0.0, 0.0}, {0.0, 0.0}}")
	ret:setProperty("UnifiedSize", "{{1.0, 0.0}, {1.0, 0.0}}")
	
	local vbox = CEGUI.WindowManager:getSingleton():createWindow("VerticalLayoutContainer")
	
	for i, data in ipairs(representations) do
		local name = data[1]
		local representation = data[2]
		local helpString = data[3]
		local suggestions = {}
		
		-- check if suggestions are appended
		if table.getn(data) == 4 then
			suggestions = data[4]
		end
		
		local hbox = CEGUI.WindowManager:getSingleton():createWindow("HorizontalLayoutContainer")
		
		local label = CEGUI.WindowManager:getSingleton():createWindow("EmberLook/StaticText")
		label:setText(name)
		label:setProperty("UnifiedSize", "{{0.3, 0.0}, {0.0, 30.0}}")
		label:setProperty("FrameEnabled", "False")
		hbox:addChildWindow(label)
		
		for j, suggestion in ipairs(suggestions) do
			representation:addSuggestion(suggestion)
		end
		
		local representationGuiRoot = representation:getGuiRoot()
		representationGuiRoot:setProperty("UnifiedSize", "{{0.3, 0}, {0.0, 30.0}}")
		hbox:addChildWindow(representationGuiRoot)
		
		local helpStringLabel = CEGUI.WindowManager:getSingleton():createWindow("EmberLook/StaticText")
		helpStringLabel:setText(helpString)
		helpStringLabel:setProperty("UnifiedSize", "{{0.4, -1.0}, {0.0, 30.0}}")
		helpStringLabel:setProperty("FrameEnabled", "False")
		hbox:addChildWindow(helpStringLabel)
		
		vbox:addChildWindow(hbox)
	end
	
	ret:addChildWindow(vbox)
	return ret
end

function SettingsWidget:CloseClicked(args)
	self.widget:hide()
end

settingsWidget = {connectors={}}
setmetatable(settingsWidget, {__index = SettingsWidget})
settingsWidget:buildWidget()
