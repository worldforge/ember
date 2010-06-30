QuickHelp = {}

function QuickHelp.buildWidget()
	quickhelp = {
		helper = nil,
		timeToShowBlurb = 0,
		timeToFade = 0,
		timeBlurbShown = 0,
		updateText_connector = nil,
		updateAlpha_connector = nil
		}
	setmetatable(quickhelp, {__index = QuickHelp})
		
	quickhelp.helper = EmberOgre.Gui.QuickHelpCursor:new()
	
	quickhelp.timeToShowBlurb = 4
	quickhelp.timeToFade = 5
	quickhelp.timeBlurbShown = 0
	
	quickhelp.updateText_connector = EmberOgre.LuaConnector:new_local(quickhelp.helper.EventUpdateText):connect(quickhelp.updateText, quickhelp)
	
	quickhelp:buildCEGUIWidget()
end

function QuickHelp:buildCEGUIWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("QuickHelp.layout", "QuickHelp/")
	
	self.textWindow = self.widget:getWindow("HelpTextBox")
	self.enabledCheckbox = CEGUI.toCheckbox(self.widget:getWindow("Hide"))
	self.frameWindow = CEGUI.toFrameWindow(self.widget:getWindow("MainWindow"))
	
	self.frameWindow:setRollupEnabled(false)
	
	self.widget:enableCloseButton()
	
	self.widget:getWindow("Next"):subscribeEvent("Clicked", "QuickHelp.Next_Click", self)
	self.widget:getWindow("Previous"):subscribeEvent("Clicked", "QuickHelp.Back_Click", self)
	
	self.widget:show()
end

function QuickHelp:frameStarted(timeSinceLastUpdate)
	if self.widget:isActive() then 
		self:disableAlphaConnector()
	end
	self.timeBlurbShown = timeSinceLastUpdate + self.timeBlurbShown
	if self.timeBlurbShown > self.timeToShowBlurb then
		
		self.widget:getMainWindow():setAlpha(1.0 - ((self.timeBlurbShown-self.timeToShowBlurb) / self.timeToFade ))
		
		if self.timeBlurbShown > (self.timeToShowBlurb+self.timeToFade) then
			self.timeBlurbShown = 0
			self.widget:hide()
			self:disableAlphaConnector()
		end
	end
end

function QuickHelp:updateText(text)
	if not self.enabledCheckbox:isSelected() then
		if not self.widget:isVisible() then
			self.updateAlpha_connector = EmberOgre.LuaConnector:new(self.widget.EventFrameStarted):connect(self.frameStarted, self)
		end
		self.widget:show()
		self.widget:getMainWindow():setAlpha(1.0)
	end
	self.textWindow:setText(text)
end

function QuickHelp:disableAlphaConnector()
	self.updateAlpha_connector:disconnect()
	self.updateAlpha_connector:delete()
end

function QuickHelp:Next_Click(args)
	self.helper:nextMessage()
end

function QuickHelp:Back_Click(args)
	self.helper:previousMessage()
end

function QuickHelp:shutdown()
end

QuickHelp.buildWidget()