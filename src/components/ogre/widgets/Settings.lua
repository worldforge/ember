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
		{
			label = "Audio",
			
			contents =
			{
				{
					label = "Enabled",
					representation = Representations.VarconfCheckboxRepresentation:new((configService:getValue("audio", "enabled"))),
					helpString = "Controls whether sounds are played"
				},
				{
					label = "Output",
					representation = Representations.VarconfStringComboboxRepresentation:new((configService:getValue("audio", "output"))),
					helpString = "Which sound output should be used",
					
					suggestions = {"output", "test1", "test2"}
				},
			},
		},
		{
			label = "Graphics",
			
			contents =
			{
				{
					label = "Level",
					representation = Representations.VarconfStringComboboxRepresentation:new((configService:getValue("graphics", "level"))),
					helpString = "General graphics level",
					
					suggestions = {"low", "medium", "high"}
				},
				{
					label = "Fresnel water",
					representation = Representations.VarconfCheckboxRepresentation:new((configService:getValue("graphics", "fresnelwater"))),
					helpString = "More realistic and performance demanding water"
				},
				{
					label = "Foliage",
					representation = Representations.VarconfCheckboxRepresentation:new((configService:getValue("graphics", "foliage"))),
					helpString = "Render foliage over the terrain"
				},
				{
					label = "Shadow texture size",
					representation = Representations.VarconfIntComboboxRepresentation:new((configService:getValue("shadows", "texturesize"))),
					helpString = "The bigger this number, the more accurate the shadows will be",
					
					suggestions = {"256", "512", "1024", "2048", "4096"}
				},
				{
					label = "Shadow far distance",
					representation = Representations.VarconfSliderRepresentation:new((configService:getValue("shadows", "fardistance")), 1000),
					helpString = "If the shadows are to be further than this value, they won't be rendered"
				},
			},
		},
	}
	
	for i, category in ipairs(self.settings) do
		local wnd = self:buildUiFor(category.contents)
		
		wnd:setText(category.label)
		self.tabs:addChildWindow(wnd)
	end
end

function SettingsWidget:buildUiFor(contents)
	local ret = CEGUI.WindowManager:getSingleton():createWindow("EmberLook/ScrollablePane")
	ret:setProperty("UnifiedPosition", "{{0.0, 0.0}, {0.0, 0.0}}")
	ret:setProperty("UnifiedSize", "{{1.0, 0.0}, {1.0, 0.0}}")
	
	local vbox = CEGUI.WindowManager:getSingleton():createWindow("VerticalLayoutContainer")
	
	for _, data in ipairs(contents) do
		local representation = data.representation
		local suggestions = data.suggestions
		
		-- if suggestions are omitted, make them "empty"
		if suggestions == nil then
			suggestions = {}
		end
		
		local hbox = CEGUI.WindowManager:getSingleton():createWindow("HorizontalLayoutContainer")
		
		local label = CEGUI.WindowManager:getSingleton():createWindow("EmberLook/StaticText")
		label:setText(data.label)
		label:setProperty("UnifiedSize", "{{0.2, 0.0}, {0.0, 30.0}}")
		label:setProperty("FrameEnabled", "False")
		hbox:addChildWindow(label)
		
		for _, suggestion in ipairs(suggestions) do
			representation:addSuggestion(suggestion)
		end
		
		local representationGuiRoot = representation:getGuiRoot()
		representationGuiRoot:setProperty("UnifiedSize", "{{0.3, 0}, {0.0, 30.0}}")
		hbox:addChildWindow(representationGuiRoot)
		
		local helpStringLabel = CEGUI.WindowManager:getSingleton():createWindow("EmberLook/StaticText")
		helpStringLabel:setText(data.helpString)
		helpStringLabel:setProperty("UnifiedSize", "{{0.5, -1.0}, {0.0, 30.0}}")
		helpStringLabel:setProperty("FrameEnabled", "False")
		helpStringLabel:setProperty("HorzFormatting", "WordWrapLeftAligned")
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
