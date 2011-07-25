settingsWidget = {connectors={}}

SettingsRestartDialog = {}

function SettingsRestartDialog:new()
	ret = {}
	setmetatable(ret, {__index = SettingsRestartDialog})
	
	ret:buildWidget()
	
	return ret
end

function SettingsRestartDialog:buildWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("SettingsRestartDialog.layout", "SettingsRestartDialog/")
	
	self.window = self.widget:getWindow("MainWindow")
	self.window:subscribeEvent("CloseClicked", self.CloseClicked, self)
	
	self.okButton = self.widget:getWindow("MainWindow/OKButton")
	self.okButton:subscribeEvent("Clicked", self.CloseClicked, self)
	
	-- make it a modal window to prevent user from missing the info
	self.window:setModalState(true)
end

function SettingsRestartDialog:CloseClicked(agrs)
	guiManager:destroyWidget(self.widget)
	self.widget = nil
end

SettingsUnappliedChangesDialog = {}

function SettingsUnappliedChangesDialog:new()
	ret = {}
	setmetatable(ret, {__index = SettingsUnappliedChangesDialog})
	
	ret:buildWidget()
	
	return ret
end

function SettingsUnappliedChangesDialog:buildWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("SettingsUnappliedChangesDialog.layout", "SettingsUnappliedChangesDialog/")
	
	self.window = self.widget:getWindow("MainWindow")
	
	self.applyButton = self.widget:getWindow("MainWindow/ApplyButton")
	self.applyButton:subscribeEvent("Clicked", self.ApplyClicked, self)
	
	self.discardButton = self.widget:getWindow("MainWindow/DiscardButton")
	self.discardButton:subscribeEvent("Clicked", self.DiscardClicked, self)
	
	-- make it a modal window to prevent user from missing the info
	self.window:setModalState(true)
end

function SettingsUnappliedChangesDialog:destroy()
	guiManager:destroyWidget(self.widget)
	self.widget = nil
end

function SettingsUnappliedChangesDialog:ApplyClicked(agrs)
	self:destroy()

	settingsWidget:applyAllValues()
	settingsWidget:hide()
	
	return true
end

function SettingsUnappliedChangesDialog:DiscardClicked(agrs)
	self:destroy()
	
	settingsWidget:discardAllValues()
	settingsWidget:hide()
	
	return true
end

SettingsWidget = {}

function SettingsWidget:buildWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("Settings.layout", "Settings/")
	
	self.window = self.widget:getWindow("MainWindow")
	self.window:subscribeEvent("CloseClicked", self.CloseClicked, self)
	
	self.okButton = self.widget:getWindow("MainWindow/OKButton")
	self.okButton:subscribeEvent("Clicked", self.OkClicked, self)
	
	self.applyButton = self.widget:getWindow("MainWindow/ApplyButton")
	self.applyButton:subscribeEvent("Clicked", self.ApplyClicked, self)
	
	self.cancelButton = self.widget:getWindow("MainWindow/CancelButton")
	self.cancelButton:subscribeEvent("Clicked", self.CloseClicked, self)
	
	self.tabs = self.widget:getWindow("MainWindow/Tabs")
	
	self:buildSettingsUi()
end

function SettingsWidget:buildSettingsUi()
	--This is a place where the settings contents are declared and (later) used to construct the GUI
	--to manipulace them
	
	local Representations = Ember.OgreView.Gui.Representations
	
	self.settings =
	{
		{
			label = "Graphics",
			description = "Groups visual settings affecting rendering quality. Many of these can deteriorate performance quite a lot so be careful.",
			
			contents =
			{
				{
					label = "Level",
					helpString = "General graphics level",
					
					section = "graphics",
					key = "level",
					
					representationFactory = function(value) return Representations.VarconfStringComboboxRepresentation:new(value) end,
					suggestions = {"low", "medium", "high"},
					requiresRestart = true,
				},
				{
					label = "Fresnel water",
					helpString = "More realistic and performance demanding water",
					
					section = "graphics",
					key = "fresnelwater",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new(value) end,
				},
				{
					label = "Foliage",
					helpString = "Render foliage over the terrain",
					
					section = "graphics",
					key = "foliage",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new(value) end,
				},
				{
					label = "Shadow texture size",
					helpString = "The bigger this number, the more accurate the shadows will be",
					
					section = "shadows",
					key = "texturesize",
					
					representationFactory = function(value) return Representations.VarconfIntComboboxRepresentation:new(value) end,
					suggestions = {"256", "512", "1024", "2048", "4096"},
				},
				{
					label = "Shadow far distance",
					helpString = "If the shadows are to be further than this value, they won't be rendered",
					
					section = "shadows",
					key = "fardistance",
					
					representationFactory = function(value) return Representations.VarconfSliderRepresentation:new(value, 1000) end,
				},
			},
		},
		{
			label = "Audio",
			description = "",
			
			contents =
			{
				{
					label = "Enabled",
					helpString = "Controls whether sounds are played",
					
					section = "audio",
					key = "enabled",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new(value) end,
				},
				{
					label = "Output",
					helpString = "Which sound output should be used",
					
					section = "audio",
					key = "output",
					
					representationFactory = function(value) return Representations.VarconfStringComboboxRepresentation:new(value) end,
					suggestions = {"output", "test1", "test2"}
				},
			},
		},
	}
	
	for _, category in ipairs(self.settings) do
		if category.requiresRestart == nil then
			category.requiresRestart = false
		end
		
		local wnd = self:buildUiFor(category)
		self.tabs:addChildWindow(wnd)
	end
end

function SettingsWidget:buildUiFor(category)
	local ret = CEGUI.WindowManager:getSingleton():createWindow("EmberLook/ScrollablePane")
	ret:setText(category.label)
	ret:setProperty("UnifiedPosition", "{{0.0, 0.0}, {0.0, 0.0}}")
	ret:setProperty("UnifiedSize", "{{1.0, 0.0}, {1.0, 0.0}}")
	
	local vbox = CEGUI.WindowManager:getSingleton():createWindow("VerticalLayoutContainer")
	local description = CEGUI.WindowManager:getSingleton():createWindow("EmberLook/StaticText")
	description:setText(category.description)
	description:setProperty("UnifiedSize", "{{1.0, -1.0}, {0.0, 50.0}}")
	description:setProperty("HorzFormatting", "WordWrapLeftAligned")
	vbox:addChildWindow(description)
	
	local configService = emberServices:getConfigService()
	
	for _, data in ipairs(category.contents) do
		local value = configService:getValue(data.section, data.key)
		
		local representation = data.representationFactory(value)
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
		
		-- store the representation in data so that we can query it later (to get current value)
		data.representation = representation
	end
	
	ret:addChildWindow(vbox)
	return ret
end

settingsRestartDialogInstance = nil

function SettingsWidget:hasChanges()
	-- Checks whether there are changed values in the settings
	-- returns: true if there are changes
	
	local configService = emberServices:getConfigService()
	
	-- go through everything and apply the new values
	for _, category in ipairs(self.settings) do
		for _, data in ipairs(category.contents) do
			if configService:getValue(data.section, data.key) ~= data.representation:getEditedValue() then
				return true
			end
		end
	end
	
	return false
end

function SettingsWidget:applyAllValues()
	-- Applies all values
	-- returns: true if the changes required restart of Ember, false otherwise
	
	local configService = emberServices:getConfigService()
	local requiresRestart = false
	
	-- go through everything and apply the new values
	for _, category in ipairs(self.settings) do
		for _, data in ipairs(category.contents) do
			if configService:getValue(data.section, data.key) ~= data.representation:getEditedValue() then
				--configService:setValue(data.section, data.key, data.representation:getEditedValue(), varconf.USER)
				
				-- if this value changed and a change requires restart, we have to tell the user
				requiresRestart = requiresRestart or data.requiresRestart
			end
		end
	end
	
	if requiresRestart then
		settingsRestartDialogInstance = SettingsRestartDialog:new()
	end
	
	return requiresRestart
end

function SettingsWidget:discardAllValues()
	-- Discards all edited values
	
	local configService = emberServices:getConfigService()
	
	-- go through everything and discard values
	for _, category in ipairs(self.settings) do
		for _, data in ipairs(category.contents) do
			data.representation:resetToOriginal()
		end
	end
end

function SettingsWidget:hide()
	self.widget:hide()
end

function SettingsWidget:OkClicked(args)
	self:applyAllValues()
	self:hide()
	
	return true
end

function SettingsWidget:ApplyClicked(args)
	self:applyAllValues()
	
	return true
end

settingsUnappliedChangesDialogInstance = nil

function SettingsWidget:CloseClicked(args)
	if self:hasChanges() then
		-- unapplied changes dialog takes care of discarding or applying the settings
		settingsUnappliedChangesDialogInstance = SettingsUnappliedChangesDialog:new()
	else
		self:hide()
	end
	
	return true
end

setmetatable(settingsWidget, {__index = SettingsWidget})
settingsWidget:buildWidget()
