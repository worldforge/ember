-----------------------------------------


-----------------------------------------
QuickHelp = {}


function QuickHelp:buildWidget()
	
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("QuickHelp.layout", "QuickHelp/")
	
	self.textWindow = self.widget:getWindow("HelpTextBox")
	self.enabledCheckbox = CEGUI.toCheckbox(self.widget:getWindow("Hide"))
	
	self.helper = EmberOgre.Gui.QuickHelpCursor:new()
	
	function self.updateText(text)
		if not self.enabledCheckbox:isSelected() then
			self.widget:show()
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