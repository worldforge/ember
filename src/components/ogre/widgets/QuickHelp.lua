-----------------------------------------


-----------------------------------------
QuickHelp = {}

function QuickHelp:Next_Click(args)
	EmberOgre.Gui.QuickHelp:getSingleton():nextMessage();
end

function QuickHelp:Back_Click(args)
	EmberOgre.Gui.QuickHelp:getSingleton():previousMessage();
end

function QuickHelp:buildWidget()
	
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("QuickHelp.layout", "QuickHelp/")
	
	self.textWindow = self.widget:getWindow("HelpTextBox")
	
	function self.updateText(text)
		self.textWindow:setText(text)
	end
	
	self.updateText_connector = EmberOgre.LuaConnector:new_local(EmberOgre.Gui.QuickHelp:getSingleton().EventUpdateText):connect(self.updateText)
	
	self.widget:getWindow("Next"):subscribeEvent("Clicked", "QuickHelp.Next_Click")
	self.widget:getWindow("Back"):subscribeEvent("Clicked", "QuickHelp.Back_Click")
	
	self.widget:enableCloseButton()
	self.widget:show()
end

function QuickHelp:shutdown()
end

connect(connectors, emberServices:getServerService().GotAvatar, QuickHelp.buildWidget)