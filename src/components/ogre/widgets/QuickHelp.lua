-----------------------------------------


-----------------------------------------
QuickHelp = {}


function QuickHelp:buildWidget()
	
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("QuickHelp.layout", "QuickHelp/")
	
	self.textWindow = self.widget:getWindow("HelpTextBox")
	self.enabledCheckbox = CEGUI.toCheckbox(self.widget:getWindow("Hide"))
	
	self.helper = EmberOgre.Gui.QuickHelpCursor:new()
	
	
	self.timeToShowBlurb = 4
	self.timeToFade = 5
	self.timeBlurbShown = 0
	
	function self.frameStarted(timeSinceLastUpdate)
		self.timeBlurbShown = timeSinceLastUpdate + self.timeBlurbShown
		if self.timeBlurbShown > self.timeToShowBlurb then
		
			self.widget:getMainWindow():setAlpha(1.0 - ((self.timeBlurbShown-self.timeToShowBlurb) / self.timeToFade ))
			
			if self.timeBlurbShown > (self.timeToShowBlurb+self.timeToFade) then
				self.timeBlurbShown = 0
				self.widget:hide()
				self.updateAlpha_connector:disconnect()
				self.updateAlpha_connector:delete()
			end
		end
	end
	
	function self.updateText(text)
		if not self.enabledCheckbox:isSelected() then
			if not self.widget:isVisible() then
				self.updateAlpha_connector = EmberOgre.LuaConnector:new(self.widget.EventFrameStarted):connect(self.frameStarted)
			end
			self.widget:show()
			self.widget:getMainWindow():activate()
		end
		self.textWindow:setText(text)
	end
	
	
	
	self.updateText_connector = EmberOgre.LuaConnector:new_local(self.helper.EventUpdateText):connect(self.updateText)
		
	self.widget:getWindow("Next"):subscribeEvent("Clicked", "QuickHelp.Next_Click", self)
	self.widget:getWindow("Back"):subscribeEvent("Clicked", "QuickHelp.Back_Click", self)
	
	self.widget:enableCloseButton()
	self.widget:show()
end

function QuickHelp:Next_Click(args)
	self.helper:nextMessage()
end

function QuickHelp:Back_Click(args)
	self.helper:previousMessage()
end

function QuickHelp:shutdown()
end

connect(connectors, emberServices:getServerService().GotAvatar, QuickHelp.buildWidget)