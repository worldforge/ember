QuickHelp = {}

function QuickHelp.buildWidget()
	quickhelp = {
		helper = nil,
		timeToShowBlurb = 4,
		timeToFade = 5,
		timeBlurbShown = 0,
		timeBlurbLastUpdate = -1,
		hidden = false,
		updateText_connector = nil,
		updateAlpha_connector = nil
		}
	setmetatable(quickhelp, {__index = QuickHelp})
		
	quickhelp.helper = Ember.OgreView.Gui.QuickHelpCursor:new_local()
	
	quickhelp.updateText_connector = createConnector(quickhelp.helper.EventUpdateText):connect(quickhelp.updateText, quickhelp)
	quickhelp.toggleVisibility_connector = createConnector(Ember.OgreView.Gui.QuickHelp:getSingleton().EventToggleWidgetVisibility):connect(quickhelp.toggleVisibility, quickhelp)
	quickhelp:buildCEGUIWidget()
end

function QuickHelp:buildCEGUIWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("QuickHelp.layout", "QuickHelp/")
	
	self.textWindow = self.widget:getWindow("HelpTextBox")
	self.frameWindow = CEGUI.toFrameWindow(self.widget:getWindow("MainWindow"))
	self.messagePosition = self.widget:getWindow("PageNumber")
	self.timer = self.widget:getWindow("Timer")
	
	self.frameWindow:setRollupEnabled(false)
	
	self.widget:enableCloseButton()
	
	self.widget:getWindow("Hide"):subscribeEvent("Clicked", "QuickHelp.Hide_Click", self)
	self.widget:getWindow("Next"):subscribeEvent("Clicked", "QuickHelp.Next_Click", self)
	self.widget:getWindow("Previous"):subscribeEvent("Clicked", "QuickHelp.Back_Click", self)
	self:updateButtons()
	
	self.widget:hide()
end

function QuickHelp:frameStarted(timeSinceLastUpdate)
	if self.widget:isActive() then 
		self.timer:setText("")
		self:disableAlphaConnector()
		self.widget:getMainWindow():setAlpha(1.0)
	else
		self.timeBlurbShown = timeSinceLastUpdate + self.timeBlurbShown
		local timeLeft = math.floor(math.max(self.timeToShowBlurb-self.timeBlurbShown, 0))
		--only update the text when it has changed
		if self.timeBlurbLastUpdate ~= timeLeft then
			self.timer:setText("Hiding in " .. timeLeft .. " seconds.")
			self.timeBlurbLastUpdate = timeLeft
		end
		if self.timeBlurbShown > self.timeToShowBlurb then
			self.widget:getMainWindow():setAlpha(1.0 - ((self.timeBlurbShown-self.timeToShowBlurb) / self.timeToFade ))
			if self.timeBlurbShown > (self.timeToShowBlurb+self.timeToFade) then
				self.timer:setText("")
				self.timeBlurbShown = 0
				self.widget:hide()
				self:disableAlphaConnector()
			end
		end
	end
end

function QuickHelp:toggleVisibility()
	self.hidden = false
	self.widget:getMainWindow():setAlpha(1.0)
	self.widget:show()
end

function QuickHelp:updateText(helpMessage)
	local text = helpMessage:getHelp()
	if not self.hidden then
		if not self.widget:isVisible() then
			if self.updateAlpha_connector then
				self.updateAlpha_connector:disconnect()
			end
			self.updateAlpha_connector = createConnector(self.widget.EventFrameStarted):connect(self.frameStarted, self)
			self.timeBlurbShown = 0
			
			--Adapt the time the help widget is shown to how many words there are in the text
			local words = 0
			for word in string.gfind(text, "[^%s]+") do words=words+1 end
			self.timeToShowBlurb = math.max(words / 2.0, 4) --a normal human reads about 250 words per minute, which is ~4 words per second. We'll go for two words per second as the user might not concentrate on the help message at first. Show it no lesser than four seconds 
			self.timeBlurbLastUpdate = -1
		end
		self.widget:show()
		self.widget:getMainWindow():setAlpha(1.0)
	end
	self.textWindow:setText(text)
	self.widget:getMainWindow():setText("Help - " .. helpMessage:getTitle())
	self.messagePosition:setText(self.helper:getCursorLocation() .. "/" .. self.helper:getSize())
	self:updateButtons()
end

function QuickHelp:disableAlphaConnector()
	self.updateAlpha_connector:disconnect()
	self.updateAlpha_connector = nil
end

function QuickHelp:Next_Click(args)
	self.helper:nextMessage()
	return true
end

function QuickHelp:Back_Click(args)
	self.helper:previousMessage()
	return true
end

function QuickHelp:Hide_Click(args)
	self.hidden = true
	self.widget:hide()
	return true
end

function QuickHelp:updateButtons()
	self.widget:getWindow("Next"):setEnabled(self.helper:getCursorLocation() < self.helper:getSize())
	self.widget:getWindow("Previous"):setEnabled(self.helper:getCursorLocation() > 1)
end

QuickHelp.buildWidget()