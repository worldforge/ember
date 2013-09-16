QuickHelp = {}

function QuickHelp.buildWidget()
	quickhelp = {
		helper = nil,
		timeToShowBlurb = 4,
		timeBlurbShown = 0,
		timeBlurbLastUpdate = -1,
		showTransitionStarted = false,
		hideTransitionStarted = false,
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
	self.widget:loadMainSheet("QuickHelp.layout", "QuickHelp")
	
	self.textWindow = self.widget:getWindow("HelpTextBox")
	self.frameWindow = CEGUI.toFrameWindow(self.widget:getMainWindow())
	self.messagePosition = self.widget:getWindow("PageNumber")
	self.timer = self.widget:getWindow("Timer")
	
	self.frameWindow:setRollupEnabled(false)
	
	--self.widget:enableCloseButton()
	self.frameWindow:subscribeEvent("CloseClicked", "QuickHelp.CloseClicked", self)
	
	self.widget:getWindow("Hide"):subscribeEvent("Clicked", "QuickHelp.Hide_Click", self)
	self.widget:getWindow("Next"):subscribeEvent("Clicked", "QuickHelp.Next_Click", self)
	self.widget:getWindow("Previous"):subscribeEvent("Clicked", "QuickHelp.Back_Click", self)
	self:updateButtons()
	
	self.widget:hide()
end

-- Shows the QuickHelp widget immediately, no transitions
function QuickHelp:show()
	self.frameWindow:fireEvent("InterruptTransitions", CEGUI.WindowEventArgs:new_local(self.frameWindow))
	self.showTransitionStarted = false
	self.hideTransitionStarted = false
	
	self.widget:show()
	-- ensure a "valid" alpha is set
	self.frameWindow:setAlpha(1.0)
end

function QuickHelp:showWithTransition()
	-- make sure no other transitions are running
	self.frameWindow:fireEvent("InterruptTransitions", CEGUI.WindowEventArgs:new_local(self.frameWindow))
	
	self.showTransitionStarted = true
	self.hideTransitionStarted = false

	self.widget:show()
	self.frameWindow:fireEvent("StartShowTransition", CEGUI.WindowEventArgs:new_local(self.frameWindow))
end

-- Hides the QuickHelp widget immediately, no transitions
function QuickHelp:hide()
	self.frameWindow:fireEvent("InterruptTransitions", CEGUI.WindowEventArgs:new_local(self.frameWindow))
	self.showTransitionStarted = false
	self.hideTransitionStarted = false
	
	self.timer:setText("")
	self.timeBlurbShown = 0
	self.widget:hide()
	self:disableAlphaConnector()
end

function QuickHelp:hideWithTransition()
	-- make sure no other transitions are running
	self.frameWindow:fireEvent("InterruptTransitions", CEGUI.WindowEventArgs:new_local(self.frameWindow))
	
	self.showTransitionStarted = false
	-- make sure we store that we already sent the event, otherwise a never ending event firing starts
	self.hideTransitionStarted = true
	
	-- by firing this event, we allow looknfeel of the QuickHelp window to act accordingly
	-- (at the time of writing this, it fades the FrameWindow out over 5 seconds)
	self.frameWindow:fireEvent("StartHideTransition", CEGUI.WindowEventArgs:new_local(self.frameWindow))
end

function QuickHelp:hideWithSlowTransition()
	-- make sure no other transitions are running
	self.frameWindow:fireEvent("InterruptTransitions", CEGUI.WindowEventArgs:new_local(self.frameWindow))
	
	self.showTransitionStarted = false
	-- make sure we store that we already sent the event, otherwise a never ending event firing starts
	self.hideTransitionStarted = true
	
	-- by firing this event, we allow looknfeel of the QuickHelp window to act accordingly
	-- (at the time of writing this, it fades the FrameWindow out over 5 seconds)
	self.frameWindow:fireEvent("StartSlowHideTransition", CEGUI.WindowEventArgs:new_local(self.frameWindow))
end

function QuickHelp:frameStarted(timeSinceLastUpdate)
	if self.widget:isActive() then 
		self.timer:setText("")
		self:disableAlphaConnector()
		self.timeBlurbShown = 0
		
		-- ensure the widget is shown at this point no matter what
		if not self.widget:isVisible() then
			self:showWithTransition()
		end
		
	else
		self.timeBlurbShown = timeSinceLastUpdate + self.timeBlurbShown
		local timeLeft = math.floor(math.max(self.timeToShowBlurb-self.timeBlurbShown, 0))
		
		--only update the text when it has changed
		if self.timeBlurbLastUpdate ~= timeLeft then
			self.timer:setText("Hiding in " .. timeLeft .. " seconds.")
			self.timeBlurbLastUpdate = timeLeft
		end
		
		if self.timeBlurbShown > self.timeToShowBlurb then
			if not self.hideTransitionStarted then
				self:hideWithSlowTransition()
			end
		end
	end
end

function QuickHelp:toggleVisibility()
	self.hidden = false
	self.widget:showWithTransition()
end

function QuickHelp:updateText(helpMessage)
	local text = helpMessage:getHelp()
	if not self.hidden then
		--Adapt the time the help widget is shown to how many words there are in the text
		local words = 0
		for word in string.gfind(text, "[^%s]+") do words=words+1 end
		--a normal human reads about 250 words per minute, which is ~4 words per second. We'll go for two words per second
		--as the user might not concentrate on the help message at first. Show it no lesser than four seconds 
		self.timeToShowBlurb = math.max(words / 2.0, 4)
		self.timeBlurbLastUpdate = -1
		self.timeBlurbShown = 0
		
		if self.updateAlpha_connector then
			self.updateAlpha_connector:disconnect()
		end
		self.updateAlpha_connector = createConnector(self.widget.EventFrameStarted):connect(self.frameStarted, self)
		
		-- even if the widget is already visible at this point, this won't harm things
		self:showWithTransition()
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
	self:hideWithTransition()
	
	return true
end

function QuickHelp:CloseClicked(args)
	self:hideWithTransition()
	
	return true
end

function QuickHelp:updateButtons()
	self.widget:getWindow("Next"):setEnabled(self.helper:getCursorLocation() < self.helper:getSize())
	self.widget:getWindow("Previous"):setEnabled(self.helper:getCursorLocation() > 1)
end

QuickHelp.buildWidget()
