ActionBarCreator = {}


function ActionBarCreator:buildCEGUIWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("ActionBarCreator.layout", "ActionBarCreator/")
	self.widget:show()
end

function ActionBarCreator.buildWidget()
	actionbarCreator = {}
	
	setmetatable(actionbarCreator, {__index = ActionBarCreator})
	
	actionbarCreator:buildCEGUIWidget()
end

ActionBarCreator.buildWidget()