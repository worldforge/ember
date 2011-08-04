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
	-- we fire this to start the ShowTransition
	self.window:fireEvent("Shown", CEGUI.WindowEventArgs:new(self.window))
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
	-- we fire this to start the ShowTransition
	self.window:fireEvent("Shown", CEGUI.WindowEventArgs:new(self.window))
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
	self.connectors = {}
	
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
	
	self:hide()
	self.widget:registerConsoleVisibilityToggleCommand("settings")
	
	local configService = emberServices:getConfigService()
	local valueChangedCall = function(section, key)
		settingsWidget:EventChangedConfigItem(section, key)
	end
	connect(self.connectors, configService.EventChangedConfigItem, valueChangedCall)
end

function SettingsWidget:destroy()
	disconnectAll(self.connectors)
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
					
					representationFactory = function(value) return Representations.VarconfStringComboboxRepresentation:new(value, true) end,
					suggestions = {"low", "medium", "high"}
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
				--FIXME: not effective for now
				--[[{
					label = "Double buffered",
					helpString = "If set to true, the application will be double buffered, which means that everything is rendered into a separate backbuffer, which is then copied to the main screen buffer when appropriate. This will in some cases reduce tearing.",
					
					section = "graphics",
					key = "doublebuffered",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new(value) end,
				},--]]
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
				{
					label = "Use aggressive focus region",
					helpString = "Sets whether or not to use the more aggressive approach to deciding on the focus region or not.",
					
					section = "shadows",
					key = "useaggressivefocusregion",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new(value) end,
				},
				{
					label = "Preferred terrain technique",
					helpString = "The preferred terrain technique. Available values are: ShaderNormalMapped, Shader, Base",
					
					section = "terrain",
					key = "preferredtechnique",
					
					representationFactory = function(value) return Representations.VarconfStringComboboxRepresentation:new(value) end,
					suggestions = {"ShaderNormalMapped", "Shader", "Base"},
				},
				{
					label = "Desired FPS",
					helpString = "FPS capping - used to prevent using more excessive resources. 0 disables FPS cap.",
					
					section = "graphics",
					key = "desiredfps",
					
					representationFactory = function(value) return Representations.VarconfSliderRepresentation:new(value, 100) end,
				},
			},
		},
		{
			label = "Audio",
			description = "All settings related to audio output of the Ember client.",
			
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
					
					representationFactory = function(value) return Representations.VarconfStringComboboxRepresentation:new(value, true) end,
					suggestions = {"output", "test1", "test2"}
				},
			},
		},
		{
			label = "Input",
			description = "Groups various options regarding mouse and keyboard input devices.",
			
			contents =
			{
				{
					label = "Mouse sensitivity",
					helpString = "Affects mouse look only.",
					
					section = "input",
					key = "degreespermouseunit",
					
					representationFactory = function(value) return Representations.VarconfSliderRepresentation:new(value, 400) end,
				},
				{
					label = "Invert mouse look",
					helpString = "If true, the pitch of the camera will be inverted. Useful for people that are used to flight simulators.",
					
					section = "input",
					key = "invertcamera",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new(value) end,
				},
				{
					label = "Catch mouse",
					helpString = "If true, Ember will catch the mouse. This means that the mouse cursor won't be able to move beyond the Ember window. ALT+TAB releases the mouse.",
					
					section = "input",
					key = "catchmouse",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new(value) end,
				},
			},
		},
		{
			label = "Servers",
			description = "Options regarding connecting to various servers",
			
			contents =
			{
				{
					label = "Connect to meta server",
					helpString = "Ember will connect to the meta server given below on startup.",
					
					section = "metaserver",
					key = "enabled",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new(value) end,
				},
				{
					label = "Metaserver",
					helpString = "Which meta server should Ember connect to on startup",
					
					section = "metaserver",
					key = "server",
					
					representationFactory = function(value) return Representations.VarconfStringEditboxRepresentation:new(value) end,
				},
				{
					label = "Autoconnect",
					helpString = "If specified, Ember will connect to this server at startup automatically",
					
					section = "metaserver",
					key = "autoconnect",
					
					representationFactory = function(value) return Representations.VarconfStringEditboxRepresentation:new(value) end,
				},
				{
					label = "Minimum server version",
					helpString = "If set to something, the server browser will filter out all servers with versions lower than this. This is to prevent new users from connecting to old stale servers.",
					
					section = "metaserver",
					key = "minimumversion",
					
					representationFactory = function(value) return Representations.VarconfStringEditboxRepresentation:new(value) end,
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

function SettingsWidget:getDeclarationData(section, key)
	for _, category in ipairs(self.settings) do
		for _, data in ipairs(category.contents) do
			if (data.section == section) and (data.key == key) then
				return data
			end
		end
	end
	
	return nil
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
		label:setProperty("UnifiedSize", "{{0.4, 0.0}, {0.0, 30.0}}")
		label:setProperty("FrameEnabled", "False")
		hbox:addChildWindow(label)
		data.labelWnd = label
		
		for _, suggestion in ipairs(suggestions) do
			representation:addSuggestion(suggestion)
		end
		
		local representationGuiRoot = representation:getGuiRoot()
		representationGuiRoot:setProperty("UnifiedSize", "{{0.6, 0}, {0.0, 30.0}}")
		hbox:addChildWindow(representationGuiRoot)
		
		hbox:setTooltipText(data.helpString)
		
		--local helpStringLabel = CEGUI.WindowManager:getSingleton():createWindow("EmberLook/StaticText")
		--helpStringLabel:setText(data.helpString)
		--helpStringLabel:setProperty("UnifiedSize", "{{0.5, -1.0}, {0.0, 30.0}}")
		--helpStringLabel:setProperty("FrameEnabled", "False")
		--helpStringLabel:setProperty("HorzFormatting", "WordWrapLeftAligned")
		--hbox:addChildWindow(helpStringLabel)
		
		vbox:addChildWindow(hbox)
		
		-- store the representation in data so that we can query it later (to get current value)
		data.representation = representation

		local localSection = data.section
		local localKey = data.key
		
		local valueChangedCall = function()
			settingsWidget:RepresentationValueChanged(localSection, localKey)
		end
		connect(self.connectors, representation:getEventValueChangedSignal(), valueChangedCall)
	end
	
	ret:addChildWindow(vbox)
	return ret
end

settingsRestartDialogInstance = nil

function SettingsWidget:hasChanges()
	-- Checks whether there are changed values in the settings
	-- returns: true if there are changes
	
	local configService = emberServices:getConfigService()
	
	-- go through everything and check whether we have changes
	for _, category in ipairs(self.settings) do
		for _, data in ipairs(category.contents) do
			if data.representation:hasChanges() then
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
			if data.representation:hasChanges() then
				configService:setValue(data.section, data.key, data.representation:getEditedValue(), varconf.USER)
				
				-- notify the representation that we applied our changes and it should alter it's original value
				data.representation:applyChanges()
				
				-- we call this to notify the representation that the varconf value has changed,
				-- the representation can stop notifying that it has changes if applicable
				self:RepresentationValueChanged(data.section, data.key)
				
				-- if this value changed and a change requires restart, we have to tell the user
				requiresRestart = requiresRestart or data.requiresRestart
			end
		end
	end
	
	self.applyButton:setEnabled(false)
	
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
			data.representation:setEditedValue(data.representation:getOriginalValue())
		end
	end
end

function SettingsWidget:resetAllToDefault()
	local configService = emberServices:getConfigService()
	
	-- goes through representations and for each it puts the default value to it
	for _, category in ipairs(self.settings) do
		for _, data in ipairs(category.contents) do
			--local default = ...
			--data.representation:setEditedValue(default)
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

function SettingsWidget:RepresentationValueChanged(section, key)
	local data = self:getDeclarationData(section, key)
	
	-- only enable the apply button if we have changes
	self.applyButton:setEnabled(self:hasChanges())
	
	if data.representation:hasChanges() then
		data.labelWnd:setText(data.label .. "*")
	else
		data.labelWnd:setText(data.label)
	end
end

function SettingsWidget:EventChangedConfigItem(section, key)
	local data = self:getDeclarationData(section, key)
	-- if we don't have these data declared, return immediately
	if data == nil then
		return
	end
	
	local configService = emberServices:getConfigService()
	local value = configService:getValue(section, key)
	
	if data.representation:hasChanges() then
		local oldEdited = data.representation:getEditedValue()
		data.representation:setEditedValue(value)
		data.representation:applyChanges()
		data.representation:setEditedValue(oldEdited)
	else
		data.representation:setEditedValue(value)
		data.representation:applyChanges()
	end
	
	-- sync up the changed status of the representation
	self:RepresentationValueChanged(section, key)
end

setmetatable(settingsWidget, {__index = SettingsWidget})
settingsWidget:buildWidget()
