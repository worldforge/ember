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

function ActionBarCreator:createActionBar()
	--Find the first available actionbar#.
	local i = 1
	while self.actionbars["ActionBar"..i] ~= nil do
		i = i+1
	end

	--Name that will be used as the key and presented to the user.
	local name = "ActionBar"..i
	
	--Create the new Actionbar.
	a1 = ActionBar.new()
	a1:init(name)
	
	--Insert into our dictionary.
	self.actionbars[name] = a1
	
	--Add to the Listbox presented in the ActionBarCreator widget.
	local actionBar = EmberOgre.Gui.ColouredListItem:new(name)
	self.actionBarListbox:addItem(actionBar)
	
end

function ActionBarCreator:deleteActionBar()
	--Get the selected Listbox item.
	local selectedItem = self.actionBarListbox:getFirstSelectedItem()
	if selectedItem ~= nil then
		local name = selectedItem:getText()
		--Delete and remove from the dictionary.
		self.actionbars[name]:shutdown()
		self.actionbars[name] = nil
		--Remove from the Listbox.
		self.actionBarListbox:removeItem(selectedItem)
	else
		--Update the help widget.
		local message = EmberOgre.Gui.HelpMessage:new_local("ActionBarCreator", "Select an ActionBar to remove from the list first.", "action bar creator", "actionBarCreatorMessage")
		EmberOgre.Gui.QuickHelp:getSingleton():updateText(message)
	end
	
	self.actionbarCount = self.actionbarCount-1
end

function ActionBarCreator:Delete_Click()
	self:deleteActionBar()
end

function ActionBarCreator:Create_Click()
	self:createActionBar()
end

function ActionBarCreator.buildWidget()
	actionbarCreator = {
	actionbars = {},
	}
	
	setmetatable(actionbarCreator, {__index = ActionBarCreator})
	
	actionbarCreator:buildCEGUIWidget()
end

ActionBarCreator.buildWidget()