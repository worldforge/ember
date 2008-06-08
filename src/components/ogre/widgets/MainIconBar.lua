--[[
The main icon bar is shown in the lower left of the screen and contains the most used icons.
By default these are an icon for showing the help, one of shutting down Ember and one which shows the current input mode ("gui" or "movement").
The movement icon will only be shown once we've connected to a server.

It's possible for other scripts to add other icons by accessing the MainIconBar.iconBar variable, which is an instance of EmberOgre.Gui.IconBar.

This class is also responsible for changing the look of the mouse pointer between gui mode (a pointer) and movement mode (either a walking or running man depending on the speed of movement). This is here for historical reason and should perhaps be moved to it's own class/place.
]]--


MainIconBar = {connectors={}}



function MainIconBar.buildWidget()

	MainIconBar.iconBar = EmberOgre.Gui.IconBar:new_local("mainIcons")
	guiManager:getMainSheet():addChildWindow(MainIconBar.iconBar:getWindow())
	
	--we'll use the same backgrounds for all icons
	local background = EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "background_A")
	local borderactive = EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "ring_over")
	local borderinactive = EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "ring")
	local foreground = nil
	
	local helpIconBase = nil
	
	--start with the close icon
	foreground = EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "close2")
	helpIconBase = EmberOgre.Gui.IconBase:new("close2", background, foreground, borderinactive, borderactive)
	MainIconBar.iconBar:addIcon(helpIconBase)
	helpIconBase:getButton():setTooltipText("Click here to exit Ember.")
	helpIconBase:getButton():subscribeEvent("MouseClick", "MainIconBar.close_MouseClick")
	
	--then the help icon
	foreground = EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "question")
	helpIconBase = EmberOgre.Gui.IconBase:new("help", background, foreground, borderinactive, borderactive)
	MainIconBar.iconBar:addIcon(helpIconBase)
	helpIconBase:getButton():setTooltipText("Click here to access the help.");
	helpIconBase:getButton():subscribeEvent("MouseClick", "MainIconBar.help_MouseClick")
	
	--and the the movement icon
	
	--we need to dynamically switch the icon depending on the current movement mode, so let's load all of the movement status images
	MainIconBar.movementImage_walk	= EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "walk")
	MainIconBar.movementImage_run	= EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "run")
	MainIconBar.movementImage_gui	= EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "abc")
	
	MainIconBar.movementModeIcon = EmberOgre.Gui.IconBase:new("movementmode", background, MainIconBar.movementImage_gui, borderinactive, borderactive)
	MainIconBar.iconBar:addIcon(MainIconBar.movementModeIcon)
	MainIconBar.movementModeIcon:getButton():setTooltipText("This shows your current input mode.\nUse the right mouse button for movement mode.\nDouble click also switches modes. Press and hold shift to run.")
	--start out with the movement mode icon hidden, only show it when the user has an avatar
	MainIconBar.movementModeIcon:getContainer():setVisible(false)
	
	MainIconBar.movementModeIcon:getButton():subscribeEvent("MouseClick", "MainIconBar.movement_MouseClick")
	
	connect(MainIconBar.connectors, EmberOgre.Input:getSingleton().EventChangedInputMode, "MainIconBar.Input_InputModeChanged")
	connect(MainIconBar.connectors, emberOgre:getAvatarController().EventMovementModeChanged, "MainIconBar.AvatarController_MovementModeChanged")
	
	--position it in the lower left of the screen
	local height = MainIconBar.iconBar:getAbsoluteHeight()
	MainIconBar.iconBar:getWindow():setPosition(CEGUI.UVector2:new_local(CEGUI.UDim(0,0), CEGUI.UDim(1, -height)))
	
	
	connect(MainIconBar.connectors, emberOgre.EventCreatedAvatarEntity, "MainIconBar.EmberOgre_createdAvatarEmberEntity")

end

--just show the help window when the help button is clicked
function MainIconBar.help_MouseClick(args)
	console:runCommand("/help");
	return true
end

--show the softquit window when the close button is clicked
function MainIconBar.close_MouseClick(args)
	--note that if the cegui system isn't working, the /softquit command knows how to shut down immediately instead
	console:runCommand("/softquit")
	return true
end

--toggle input mode when the input mode button is clicked
function MainIconBar.movement_MouseClick(args)
	guiManager:getInput():toggleInputMode()
	return true
end

--when the input mode is changed, we need to update both the image on the movement mode icon and the mouse pointer
function MainIconBar.Input_InputModeChanged(inputMode)
	MainIconBar.currentMode = inputMode
	if inputMode == EmberOgre.Input.IM_GUI then
		MainIconBar.movementModeIcon:setForeground(MainIconBar.movementImage_gui)
		if MainIconBar.originalCursorImage ~= nil then
			CEGUI.MouseCursor:getSingleton():setImage(MainIconBar.originalCursorImage);
			MainIconBar.originalCursorImage = nil;
		end
	else
		--delegate to another method since movement mode can either be running or walking (perhaps swimming or flying in the future?)
		MainIconBar.checkMovementMode();
	end
end

--When the movement mode is changed (i.e. walking or running) we need to update the images on the movement mode icon as well as the cursor image
function MainIconBar.AvatarController_MovementModeChanged(mode)
	if MainIconBar.currentMode == EmberOgre.Input.IM_MOVEMENT then
		MainIconBar.checkMovementMode()
	end
end

--This method will update the cursor image as well as the image on the movement mode icon to reflect whether the avatar is walking or running
function MainIconBar.checkMovementMode()
	if MainIconBar.originalCursorImage == nil then
		MainIconBar.originalCursorImage = CEGUI.MouseCursor:getSingleton():getImage()
	end
	
	if emberOgre:getAvatarController():getCurrentMovement().mode == EmberOgre.AvatarMovementMode.MM_RUN then
		MainIconBar.movementModeIcon:setForeground(MainIconBar.movementImage_run)
		CEGUI.MouseCursor:getSingleton():setImage(MainIconBar.movementImage_run)
	else
		MainIconBar.movementModeIcon:setForeground(MainIconBar.movementImage_walk)
		CEGUI.MouseCursor:getSingleton():setImage(MainIconBar.movementImage_walk)
	end
end

--Only show the movement icon when we've actually connected to a server; it makes no sense before that
function MainIconBar.EmberOgre_createdAvatarEmberEntity(entity)
	MainIconBar.movementModeIcon:getContainer():setVisible(true)
end


MainIconBar.buildWidget()