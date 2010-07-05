ActionBarCreator = {}


function ActionBarCreator:buildCEGUIWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("ActionBarCreator.layout", "ActionBarCreator/")
	
	self.createButton = CEGUI.toPushButton(self.widget:getWindow("Create"))
	self.createButton:subscribeEvent("Clicked", self.Create_Click, self)
	
	self.deleteButton = CEGUI.toPushButton(self.widget:getWindow("Delete"))
	self.deleteButton:subscribeEvent("Clicked", self.Delete_Click, self)
	
	self.actionBarListbox = CEGUI.toListbox(self.widget:getWindow("ActionBarList"))
	self.actionBarListbox:setMultiselectEnabled(false)
	
	
	self.widget:show()
end

function ActionBarCreator:Create_ActionBar()
	local i = 1
	while self.actionbars["ActionBar"..i] ~= nil do
		i = i+1
	end

	local name = "ActionBar"..i
	
	a1 = ActionBar:new()
	a1:init(name)
	
	self.actionbars[name] = a1
	
	local actionBar = EmberOgre.Gui.ColouredListItem:new(name)
	self.actionBarListbox:addItem(actionBar)
	
end

function ActionBarCreator:Delete_ActionBar()
	debugObject("Deleting")
	--get the selected item
	local selectedItem = self.actionBarListbox:getFirstSelectedItem()
	if selectedItem ~= nil then
		local name = selectedItem:getText()
		self.actionbars[name]:shutdown()
		self.actionbars[name] = nil
		self.actionBarListbox:removeItem(selectedItem)
	else
		local message = EmberOgre.Gui.HelpMessage:new_local("ActionBarCreator", "Select an ActionBar to remove from the list first.", "action bar creator", "actionBarCreatorMessage")
		EmberOgre.Gui.QuickHelp:getSingleton():updateText(message)
	end
	
	self.actionbarCount = self.actionbarCount-1
	--call destroy then remove it from actionbars
end

function ActionBarCreator:Delete_Click()
	self:Delete_ActionBar()
end

function ActionBarCreator:Create_Click()
	self:Create_ActionBar()
end

function ActionBarCreator.buildWidget()
	actionbarCreator = {
	actionbars = {},
	}
	
	setmetatable(actionbarCreator, {__index = ActionBarCreator})
	
	actionbarCreator:buildCEGUIWidget()
end

ActionBarCreator.buildWidget()