SettingsRestartDialog = {}

function SettingsRestartDialog:new()
	local ret = {}
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
	self.window:fireEvent("Shown", CEGUI.WindowEventArgs:new_local(self.window))
end

function SettingsRestartDialog:CloseClicked(agrs)
	self:destroy()
	return true
end

function SettingsRestartDialog:destroy()
	guiManager:destroyWidget(self.widget)
	self.widget = nil
end

SettingsUnappliedChangesDialog = {}

function SettingsUnappliedChangesDialog:new(settingsWidget)
	local ret = {}
	setmetatable(ret, {__index = SettingsUnappliedChangesDialog})
	
	ret:buildWidget()
	ret.settingsWidget = settingsWidget
	
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
	self.window:fireEvent("Shown", CEGUI.WindowEventArgs:new_local(self.window))
end

function SettingsUnappliedChangesDialog:destroy()
	guiManager:destroyWidget(self.widget)
	self.widget = nil
end

function SettingsUnappliedChangesDialog:ApplyClicked(agrs)
	self:destroy()

	self.settingsWidget:applyAllValues()
	self.settingsWidget:hide()
	
	return true
end

function SettingsUnappliedChangesDialog:DiscardClicked(agrs)
	self:destroy()
	
	self.settingsWidget:discardAllValues()
	self.settingsWidget:hide()
	
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
	
	self:hide()
	self.widget:registerConsoleVisibilityToggleCommand("settings")
	
	local configService = emberServices:getConfigService()
	local valueChangedCall = function(section, key)
		self:EventChangedConfigItem(section, key)
	end
	connect(self.connectors, configService.EventChangedConfigItem, valueChangedCall)
end

function SettingsWidget:shutdown()
	if self.settingsRestartDialogInstance then
		self.settingsRestartDialogInstance:destroy()
		self.settingsRestartDialogInstance = nil
	end
	if self.settingsUnappliedChangesDialogInstance then
		self.settingsUnappliedChangesDialogInstance:destroy()
		self.settingsUnappliedChangesDialogInstance = nil
	end

	self.settings = nil
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

function SettingsWidget:buildSettingsUi()
	--This is a place where the settings contents are declared and (later) used to construct the GUI
	--to manipulace them
	
	local Representations = Ember.OgreView.Gui.Representations
	
	-- The declaration syntax described:
	-- *********************************
	--
	-- self.settings is an ordered array of tables, each of these tables represents a settings tab
	--
	-- keys of each settings tab
	-- label           - the text displayed in the tab's button
	-- helpString      - the text displayed at the top in the tab (when it's activated)
	--
	-- contents        - each settings tab contains ordered set of entries in its "contents" key
	
	-- meaning of various entry keys:
	-- label           - simply the text displayed to the left of the representation showing that particular settings entry
	-- helpString      - text that appears as a tooltip when you hold your mouse over the line containing the settings entry
	-- section and key - both varconf specific values, refer to varconf for a more detailed explanation
	-- representationFactory - a lua function that will construct a representation using passed data
	-- requiresRestart - if true user will be warned upon changing this that Ember should be restarted for the changes
	--                 - to take effect (optional and defaults to false)
	--
	-- Quick troubleshooting tip: Make sure the representationFactory anonymous function RETURNS the created representation
	--                            instance. I spent 15 minutes assuming it does and couldn't understand why it was being nil
	self.settings =
	{
		{
			label = "General",
			description = "Settings that don't fit into other category.",
			
			contents =
			{
				{
					label = "Startup help",
					helpString = "if true, the startup help window will be shown at startup",
					
					section = "general",
					key = "startuphelp",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
					requiresRestart = true,
				},
				{
					label = "Log chat messages",
					helpString = "If true, all chat message will be logged",
					
					section = "general",
					key = "logchatmessages",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
				},
				{
					label = "Chat bubbles time shown",
					helpString = "How long are chat messages shown (in seconds)",
					
					section = "ingamechatwidget",
					key = "timeshown",
					
					representationFactory = function(value) return Representations.VarconfSliderRepresentation:new_local(value, 60) end,
				},
				{
					label = "Chat bubbles distance shown",
					helpString = "How far do you have to go before chat messages disappear",
					
					section = "ingamechatwidget",
					key = "distanceshown",
					
					representationFactory = function(value) return Representations.VarconfSliderRepresentation:new_local(value, 100) end,
				},
				{
					label = "Desired FPS",
					helpString = "FPS capping - used to prevent using more excessive resources. 0 disables FPS cap.",
					
					section = "general",
					key = "desiredfps",
					
					representationFactory = function(value) return Representations.VarconfSliderRepresentation:new_local(value, 100) end,
				},
				{
					label = "Suppress Ogre settings window",
					helpString = "Suppress the showing of the Ogre settings window at startup.",
					
					section = "ogre",
					key = "suppressconfigdialog",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
				},
			},
		},
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
					
					representationFactory = function(value) return Representations.VarconfStringComboboxRepresentation:new_local(value, true) end,
					suggestions = {"low", "medium", "high"}
				},
				--FIXME: Removed from Ember, not effective
				--[[{
					label = "Fresnel water",
					helpString = "More realistic and performance demanding water",
					
					section = "graphics",
					key = "fresnelwater",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new(value) end,
				},--]]
				{
					label = "Foliage",
					helpString = "Render foliage over the terrain",
					
					section = "graphics",
					key = "foliage",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
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
					
					representationFactory = function(value) return Representations.VarconfIntComboboxRepresentation:new_local(value, true) end,
					suggestions = {"256", "512", "1024", "2048", "4096"},
				},
				{
					label = "Shadow far distance",
					helpString = "If the shadows are to be further than this value, they won't be rendered",
					
					section = "shadows",
					key = "fardistance",
					
					representationFactory = function(value) return Representations.VarconfSliderRepresentation:new_local(value, 1000) end,
				},
				{
					label = "Use aggressive focus region",
					helpString = "Sets whether or not to use the more aggressive approach to deciding on the focus region or not.",
					
					section = "shadows",
					key = "useaggressivefocusregion",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
				},
				{
					label = "Preferred terrain technique",
					helpString = "The preferred terrain technique. Available values are: ShaderNormalMapped, Shader, Base",
					
					section = "terrain",
					key = "preferredtechnique",
					
					representationFactory = function(value) return Representations.VarconfStringComboboxRepresentation:new_local(value, true) end,
					suggestions = {"ShaderNormalMapped", "Shader", "Base"},
				}
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
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
				},
				{
					label = "Output",
					helpString = "Which sound output should be used",
					
					section = "audio",
					key = "output",
					
					representationFactory = function(value) return Representations.VarconfStringComboboxRepresentation:new_local(value, true) end,
					suggestions = {"surround"}
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
					
					representationFactory = function(value) return Representations.VarconfSliderRepresentation:new_local(value, 400) end,
				},
				{
					label = "Invert mouse look",
					helpString = "If true, the pitch of the camera will be inverted. Useful for people that are used to flight simulators.",
					
					section = "input",
					key = "invertcamera",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
				},
				{
					label = "Catch mouse",
					helpString = "If true, Ember will catch the mouse. This means that the mouse cursor won't be able to move beyond the Ember window. ALT+TAB releases the mouse.",
					
					section = "input",
					key = "catchmouse",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
				},
				{
					label = "Adjust camera to terrain",
					helpString = "If true, the camera will be adjusted to the terrain so that it never dips below the surface",
					
					section = "input",
					key = "adjusttoterrain",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
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
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
				},
				{
					label = "Metaserver",
					helpString = "Which meta server should Ember connect to on startup",
					
					section = "metaserver",
					key = "server",
					
					representationFactory = function(value) return Representations.VarconfStringEditboxRepresentation:new_local(value) end,
				},
				{
					label = "Autoconnect",
					helpString = "If specified, Ember will connect to this server at startup automatically",
					
					section = "metaserver",
					key = "autoconnect",
					
					representationFactory = function(value) return Representations.VarconfStringEditboxRepresentation:new_local(value) end,
				},
				{
					label = "Minimum server version",
					helpString = "If set to something, the server browser will filter out all servers with versions lower than this. This is to prevent new users from connecting to old stale servers.",
					
					section = "metaserver",
					key = "minimumversion",
					
					representationFactory = function(value) return Representations.VarconfStringEditboxRepresentation:new_local(value) end,
				},
			},
		},
		-- TODO: It might be good to hide this in client final builds but for now it was decided to always show it
		{
			label = "Developer",
			description = "Only useful if you are troubleshooting or doing development.",
			
			contents =
			{
				{
					label = "Logging level",
					helpString = "",
					
					section = "general",
					key = "logginglevel",
					
					representationFactory = function(value) return Representations.VarconfStringComboboxRepresentation:new_local(value, true) end,
					suggestions = {"verbose", "info", "warning", "failure", "critical"}
				},
				{
					label = "Logging detailed",
					helpString = "",
					
					section = "general",
					key = "loggingdetailed",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
				},
				{
					label = "CEGUI Logging level",
					helpString = "",
					
					section = "cegui",
					key = "minimumlogginglevel",
					
					representationFactory = function(value) return Representations.VarconfStringComboboxRepresentation:new_local(value, true) end,
					suggestions = {"insane", "informative", "standard", "warnings", "errors"}
				},
				{
					label = "Enable Lua debugging",
					helpString = "",
					
					section = "lua",
					key = "debug",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
				},
				{
					label = "Use debug LOD colours for terrain",
					helpString = "",
					
					section = "terrain",
					key = "usedebuglodcolors",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
				},
				{
					label = "Show 'inspect' menu option",
					helpString = "Turning this on will display the 'inspect' menu option for all entities, even when the user isn't logged in as ad admin character.",
					
					section = "authoring",
					key = "showinspectforall",
					
					representationFactory = function(value) return Representations.VarconfCheckboxRepresentation:new_local(value) end,
				},
			},
		},
	}
	
	-- chew through settings declaration and construct UI for it
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
	
	-- you can think of the vertical layout container as a VBoxLayout in GTK or Vertical Layout in Qt4
	-- it simply puts its elements as tightly packed as possible after each other vertically
	-- in this case it represents category tab's list of entries
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
		
		-- and each line in the entry vertical list is a horizontal layout (serves as a wrapper in this case)
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
		
		-- This commented out portion of the code makes help string appear right to the representation
		-- In the end I decided to show help strings as a tooltip to save space
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
			self:RepresentationValueChanged(localSection, localKey)
		end
		connect(self.connectors, representation:getEventValueChangedSignal(), valueChangedCall)
	end
	
	ret:addChildWindow(vbox)
	return ret
end

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
		self.settingsRestartDialogInstance = SettingsRestartDialog:new()
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

function SettingsWidget:CloseClicked(args)
	if self:hasChanges() then
		-- unapplied changes dialog takes care of discarding or applying the settings
		self.settingsUnappliedChangesDialogInstance = SettingsUnappliedChangesDialog:new(self)
	else
		self:hide()
	end
	
	return true
end

function SettingsWidget:RepresentationValueChanged(section, key)
	local data = self:getDeclarationData(section, key)
	
	-- only enable the apply button if we have changes
	self.applyButton:setEnabled(self:hasChanges())
	
	-- FIXME: For now I use an asterisk to show a settings entry has changes, this might be changed
	--        to something more artistic in the future
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
	
	-- If we had changes before the variable was externally changed we save them and reapply them
	-- to prevent loosing them
	
	-- This is a compromise I came up with to the problem, it seems to me that if the entry doesn't have any changes in it
	-- it doesn't make sense to preserve the old value but if user changes it and something else is changing it at the same
	-- time it changes the original value but the edited value is preserved.
	
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


connect(connectors, emberOgre.EventGUIManagerInitialized, function(guiManager)

	local settingsWidget = {connectors={}}
	setmetatable(settingsWidget, {__index = SettingsWidget})
	settingsWidget:buildWidget()

	connect(settingsWidget.connectors, emberOgre.EventGUIManagerBeingDestroyed, function()
		settingsWidget:shutdown()
		settingsWidget = nil
	end)
end)
