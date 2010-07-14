-----------------------------------------


-----------------------------------------
QuickHelp = {}

function QuickHelp:buildWidget()
	
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("QuickHelp.layout", "QuickHelp/")
	
	self.textWindow = self.widget:getWindow("HelpTextBox")
	
	function self.updateText(text)
		self.textWindow:setText(text)
	end
	
	self.updateText_connector = EmberOgre.LuaConnector:new_local(EmberOgre.Gui.QuickHelp:getSingleton().EventUpdateText):connect(self.updateText)
	
	self.widget:enableCloseButton()
	self.widget:show()
end

function QuickHelp:shutdown()
end


QuickHelp:buildWidget()

