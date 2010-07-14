ActionBar = {}

function ActionBar:buildWidget()
	
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("ActionBar.layout", "ActionBar/")
	self.widget:show()
end

--connect(connectors, emberServices:getServerService().GotAvatar, ActionBar.buildWidget)
ActionBar:buildWidget()