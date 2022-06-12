--[[ MainIconBar
The main icon bar is shown in the lower left of the screen and contains the most used icons.
By default these are an icon for showing the help, one of shutting down Ember and one which shows the current input mode ("gui" or "movement").
The movement icon will only be shown once we've connected to a server.

It's possible for other scripts to add other icons by accessing the self.iconBar variable, which is an instance of Ember.OgreView.Gui.IconBar.

This class is also responsible for changing the look of the mouse pointer between gui mode (a pointer) and movement mode (either a walking or running man depending on the speed of movement). This is here for historical reason and should perhaps be moved to it's own class/place.
]]--

MainIconBar = {  }

function MainIconBar:buildWidget()

	--[[creates a new icon and adds it to the iconbar, using the default background and border images
@param iconName The name of the icon.
@param foregroundImage The CEGUI::Image to use as foreground image.
@param tooltipText The text to use for tooltip text.
@returns The newly created IconBase instance. This has already been added to the iconbar. Note that the iconbar is responsible for deleting the IconBase instance, so you don't have to do it yourself.
]]--
	function addIcon(iconName, foregroundImage, tooltipText)
		local iconBase = Ember.OgreView.Gui.IconBase.new(iconName, self.images.background, foregroundImage, self.images.borderinactive, self.images.borderactive)
		self.iconBar:addIcon(iconBase)
		iconBase:getButton():setTooltipText(tooltipText)
		return iconBase
	end
	MainIconBar.addIcon = addIcon

	function removeIcon(icon)
		--Note that this won't delete the icon, it's up to the caller to do that.
		self.iconBar:removeIcon(icon)
	end
	MainIconBar.removeIcon = removeIcon

	--This method will update the cursor image as well as the image on the movement mode icon to reflect whether the avatar is walking or running
	function checkMovementMode()
		if emberOgre:getWorld() then
			if emberOgre:getWorld():getMovementController():getMode() == Ember.OgreView.MovementControllerMode.MM_RUN then
				self.movementModeIcon:setForeground(self.movementImage_run)
			else
				self.movementModeIcon:setForeground(self.movementImage_walk)
			end
		end
	end

	self.crossHair = CEGUI.WindowManager.getSingleton():loadLayoutFromFile(guiManager:getLayoutDir() .. "Crosshair.layout")
	guiManager:getMainSheet():addChild(self.crossHair)
	self.crossHair:setVisible(false)

	self.iconBar = Ember.OgreView.Gui.IconBar.new("mainIcons")
	guiManager:getMainSheet():addChild(self.iconBar:getWindow())

	--we'll use the same backgrounds for all icons
	self.images.background = Ember.OgreView.Gui.IconBase.loadImageFromImageset("iconset_standard", "background_A")
	self.images.borderactive = Ember.OgreView.Gui.IconBase.loadImageFromImageset("iconset_standard", "ring_over")
	self.images.borderinactive = Ember.OgreView.Gui.IconBase.loadImageFromImageset("iconset_standard", "ring")
	local foreground = nil


	--start with the close icon
	foreground = Ember.OgreView.Gui.IconBase.loadImageFromImageset("iconset_standard", "close2")
	self.closeIcon = addIcon("close2", foreground, "Click here to exit Ember.")
	self.closeIcon:getButton():subscribeEvent("Clicked",
	--show the softquit window when the close button is clicked
			function(args)
				--note that if the cegui system isn't working, the /softquit command knows how to shut down immediately instead
				console:runCommand("/softquit")
				return true
			end)

	--then the help icon
	foreground = Ember.OgreView.Gui.IconBase.loadImageFromImageset("iconset_standard", "question")
	self.helpIcon = addIcon("help", foreground, "Click here to access the help.")
	self.helpIcon:getButton():subscribeEvent("Clicked",
	--just show the help window when the help button is clicked
			function()
				console:runCommand("/help");
				Ember.OgreView.Gui.QuickHelp.getSingleton():showWidget()
				return true
			end)

	--and the the movement icon

	--we need to dynamically switch the icon depending on the current movement mode, so let's load all of the movement status images
	self.movementImage_walk = Ember.OgreView.Gui.IconBase.loadImageFromImageset("iconset_standard", "walk")
	self.movementImage_run = Ember.OgreView.Gui.IconBase.loadImageFromImageset("iconset_standard", "run")
	self.movementImage_gui = Ember.OgreView.Gui.IconBase.loadImageFromImageset("iconset_standard", "abc")

	self.movementModeIcon = addIcon("movementmode", self.movementImage_gui, "This shows your current input mode.\nUse the right mouse button for movement mode.\nDouble click also switches modes. Press and hold shift to run.")
	--start out with the movement mode icon hidden, only show it when the user has an avatar
	self.movementModeIcon:getContainer():setVisible(false)

	self.movementModeIcon:getButton():subscribeEvent("Clicked",
	--toggle input mode when the input mode button is clicked
			function()
				Ember.Input.getSingleton():toggleInputMode()
				return true
			end)

	connect(self.connectors, Ember.Input.getSingleton().EventChangedInputMode,
	--when the input mode is changed, we need to update both the image on the movement mode icon and the mouse pointer
			function(inputMode)
				self.currentMode = inputMode
				if inputMode == Ember.Input.IM_GUI then
					self.crossHair:setVisible(false)
					CEGUI.System.getSingleton():getDefaultGUIContext():getMouseCursor():setVisible(true)
					self.movementModeIcon:setForeground(self.movementImage_gui)
				else
					self.crossHair:setVisible(true)
					CEGUI.System.getSingleton():getDefaultGUIContext():getMouseCursor():setVisible(false)

					--delegate to another method since movement mode can either be running or walking (perhaps swimming or flying in the future?)
					checkMovementMode();
				end
			end)

	--position it in the lower left of the screen
	local height = self.iconBar:getAbsoluteHeight()
	self.iconBar:getWindow():setPosition(CEGUI.UVector2.new(CEGUI.UDim.new(0, 0), CEGUI.UDim.new(1, -height)))

	--and settings toggle icon
	foreground = Ember.OgreView.Gui.IconBase.loadImageFromImageset("iconset_standard", "settings")
	self.settingsIcon = addIcon("settings", foreground, "Toggles visibility of the settings window")
	self.settingsIcon:getButton():subscribeEvent("Clicked",
	--toggle settings window when clicked
			function()
				console:runCommand("/toggleVisibility_settings")
				return true
			end)

	--and inventory toggle icon
	foreground = Ember.OgreView.Gui.IconBase.loadImageFromImageset("iconset_standard", "inventory")
	self.inventoryIcon = addIcon("inventory", foreground, "Toggles visibility of the inventory")
	--start out with the inventory icon hidden, only show it when the user has an avatar
	self.inventoryIcon:getContainer():setVisible(false)
	self.inventoryIcon:getButton():subscribeEvent("Clicked",
	--toggle inventory window when clicked
			function(args)
				console:runCommand("/toggleVisibility_inventory")
				return true
			end
	)

	connect(self.connectors, emberOgre.EventMovementControllerCreated, function()
		--Only show the movement and inventory icons when we've actually connected to a server; it makes no sense before that
		self.movementModeIcon:getContainer():setVisible(true)
		connect(self.connectors, emberOgre:getWorld():getMovementController().EventMovementModeChanged,
				function(mode)
					--When the movement mode is changed (i.e. walking or running) we need to update the images on the movement mode icon as well as the cursor image
					if self.currentMode == Ember.Input.IM_MOVEMENT then
						checkMovementMode()
					end
				end)

		--Don't show the inventory for the admin avatar
		if emberOgre:getWorld():getAvatar():isAdmin() == false then
			self.inventoryIcon:getContainer():setVisible(true)
		end
	end)

	connect(self.connectors, emberOgre.EventMovementControllerDestroyed, function()
		self.movementModeIcon:getContainer():setVisible(false)
		self.inventoryIcon:getContainer():setVisible(false)
	end)

end

local mainIconBarInit = function()
	local mainIconBar = { connectors = {}, images = {} }
	setmetatable(mainIconBar, { __index = MainIconBar })

	connect(mainIconBar.connectors, emberOgre.EventGUIManagerBeingDestroyed, function()
		disconnectAll(mainIconBar.connectors)

		guiManager:getMainSheet():removeChild(mainIconBar.iconBar:getWindow())

		mainIconBar.iconBar:delete()

		mainIconBar = nil
	end)
	mainIconBar:buildWidget()
end
mainIconBarInit()

