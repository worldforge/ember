--[[
The main icon bar is shown in the lower left of the screen and contains the most used icons.
By default these are an icon for showing the help, one of shutting down Ember and one which shows the current input mode ("gui" or "movement").
The movement icon will only be shown once we've connected to a server.

It's possible for other scripts to add other icons by accessing the self.iconBar variable, which is an instance of EmberOgre.Gui.IconBar.

This class is also responsible for changing the look of the mouse pointer between gui mode (a pointer) and movement mode (either a walking or running man depending on the speed of movement). This is here for historical reason and should perhaps be moved to it's own class/place.
]]--


MainIconBar = {singletonInstance = nil}

function MainIconBar.addExternalIcon(iconName, foregroundImage, tooltipText)
	if MainIconBar.singletonInstance then
		return MainIconBar.singletonInstance:addIcon(iconName, foregroundImage, tooltipText)
	end
	return nil
end

function MainIconBar.removeExternalIcon(icon)
	if MainIconBar.singletonInstance then
		return MainIconBar.singletonInstance:removeIcon(icon)
	end
end

--[[creates a new icon and adds it to the iconbar, using the default background and border images
@param iconName The name of the icon.
@param foregroundImage The CEGUI::Image to use as foreground image.
@param tooltipText The text to use for tooltip text.
@returns The newly created IconBase instance. This has already been added to the iconbar. Note that the iconbar is responsible for deleting the IconBase instance, so you don't have to do it yourself.
]]--
function MainIconBar:addIcon(iconName, foregroundImage, tooltipText)
	local iconBase = EmberOgre.Gui.IconBase:new(iconName, self.images.background, foregroundImage, self.images.borderinactive, self.images.borderactive)
	self.iconBar:addIcon(iconBase)
	iconBase:getButton():setTooltipText(tooltipText)
	return iconBase
end

function MainIconBar:removeIcon(icon)
	--Note that this won't delete the icon, it's up to the caller to do that.
	self.iconBar:removeIcon(icon)
end

function MainIconBar:buildWidget()

	self.iconBar = EmberOgre.Gui.IconBar:new("mainIcons")
	guiManager:getMainSheet():addChildWindow(self.iconBar:getWindow())
	
	--we'll use the same backgrounds for all icons
	self.images.background = EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "background_A")
	self.images.borderactive = EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "ring_over")
	self.images.borderinactive = EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "ring")
	local foreground = nil
	
	local helpIconBase = nil
	
	--start with the close icon
	foreground = EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "close2")
	helpIconBase = self:addIcon("close2", foreground, "Click here to exit Ember.")
	helpIconBase:getButton():subscribeEvent("Clicked", self.close_Clicked, self)
	
	--then the help icon
	foreground = EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "question")
	helpIconBase = self:addIcon("help", foreground, "Click here to access the help.")
	helpIconBase:getButton():subscribeEvent("Clicked", self.help_Clicked, self)
	
	--and the the movement icon
	
	--we need to dynamically switch the icon depending on the current movement mode, so let's load all of the movement status images
	self.movementImage_walk	= EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "walk")
	self.movementImage_run	= EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "run")
	self.movementImage_gui	= EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "abc")
	
	self.movementModeIcon = self:addIcon("movementmode", self.movementImage_gui, "This shows your current input mode.\nUse the right mouse button for movement mode.\nDouble click also switches modes. Press and hold shift to run.")
	--start out with the movement mode icon hidden, only show it when the user has an avatar
	self.movementModeIcon:getContainer():setVisible(false)
	
	self.movementModeIcon:getButton():subscribeEvent("Clicked", self.movement_Clicked, self)
	
	connect(self.connectors, Ember.Input:getSingleton().EventChangedInputMode, self.Input_InputModeChanged, self)
	
	--position it in the lower left of the screen
	local height = self.iconBar:getAbsoluteHeight()
	self.iconBar:getWindow():setPosition(CEGUI.UVector2:new_local(CEGUI.UDim(0,0), CEGUI.UDim(1, -height)))
	
	
	connect(self.connectors, emberOgre.EventMovementControllerCreated, self.EmberOgre_movementControllerCreated, self)
	connect(self.connectors, emberOgre.EventMovementControllerDestroyed, function()
			self.movementModeIcon:getContainer():setVisible(false)
		end)
		
	MainIconBar.singletonInstance = self
end

--just show the help window when the help button is clicked
function MainIconBar:help_Clicked(args)
	console:runCommand("/help");
	EmberOgre.Gui.QuickHelp:getSingleton():showWidget()
	return true
end

--show the softquit window when the close button is clicked
function MainIconBar:close_Clicked(args)
	--note that if the cegui system isn't working, the /softquit command knows how to shut down immediately instead
	console:runCommand("/softquit")
	return true
end

--toggle input mode when the input mode button is clicked
function MainIconBar:movement_Clicked(args)
	Ember.Input:getSingleton():toggleInputMode()
	return true
end

--when the input mode is changed, we need to update both the image on the movement mode icon and the mouse pointer
function MainIconBar:Input_InputModeChanged(inputMode)
	self.currentMode = inputMode
	if inputMode == Ember.Input.IM_GUI then
		self.movementModeIcon:setForeground(self.movementImage_gui)
		if self.originalCursorImage ~= nil then
			CEGUI.MouseCursor:getSingleton():setImage(self.originalCursorImage);
			self.originalCursorImage = nil;
		end
	else
		--delegate to another method since movement mode can either be running or walking (perhaps swimming or flying in the future?)
		self:checkMovementMode();
	end
end

--When the movement mode is changed (i.e. walking or running) we need to update the images on the movement mode icon as well as the cursor image
function MainIconBar:MovementController_MovementModeChanged(mode)
	if self.currentMode == Ember.Input.IM_MOVEMENT then
		self:checkMovementMode()
	end
end

--This method will update the cursor image as well as the image on the movement mode icon to reflect whether the avatar is walking or running
function MainIconBar:checkMovementMode()
	if self.originalCursorImage == nil then
		self.originalCursorImage = CEGUI.MouseCursor:getSingleton():getImage()
	end

	if emberOgre:getWorld():getMovementController():getMode() == EmberOgre.MovementControllerMode.MM_RUN then
		self.movementModeIcon:setForeground(self.movementImage_run)
		CEGUI.MouseCursor:getSingleton():setImage(self.movementImage_run)
	else
		self.movementModeIcon:setForeground(self.movementImage_walk)
		CEGUI.MouseCursor:getSingleton():setImage(self.movementImage_walk)
	end
end

--Only show the movement icon when we've actually connected to a server; it makes no sense before that
function MainIconBar:EmberOgre_movementControllerCreated()
	self.movementModeIcon:getContainer():setVisible(true)
	connect(self.connectors, emberOgre:getWorld():getMovementController().EventMovementModeChanged, 
		function(mode)
			if self.currentMode == Ember.Input.IM_MOVEMENT then
				self:checkMovementMode()
			end
		end)
end

function MainIconBar:shutdown()
	MainIconBar.singletonInstance = nil
	disconnectAll(self.connectors)

	guiManager:getMainSheet():removeChildWindow(self.iconBar:getWindow())

	self.iconBar:delete()
	
end


connect(connectors, emberOgre.EventGUIManagerInitialized, function(guiManager)
	local mainIconBar = {connectors={}, images={}}
	setmetatable(mainIconBar, {__index = MainIconBar})
	mainIconBar:buildWidget()
	
	connect(mainIconBar.connectors, emberOgre.EventGUIManagerBeingDestroyed, function()
		mainIconBar:shutdown()
		mainIconBar = nil
	end)
end)


