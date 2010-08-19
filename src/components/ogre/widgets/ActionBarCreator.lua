--[[
Responsible for creating and managing action bars. The use can create either horizontal or vertical action bars,
as well as deleting existing bars by selecting them in the list and clicking delete.
]]--
ActionBarCreator = {}

loadScript("DefaultEntityActions.lua")

--Build the action bar creator widget.
function ActionBarCreator:buildCEGUIWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("ActionBarCreator.layout", "ActionBarCreator/")
	self.widget:registerConsoleVisibilityToggleCommand("actionBarCreator")
	
	self.createHButton = CEGUI.toPushButton(self.widget:getWindow("Create_Horiz"))
	self.createHButton:subscribeEvent("Clicked", self.CreateHoriz_Click, self)
	
	self.createVButton = CEGUI.toPushButton(self.widget:getWindow("Create_Vert"))
	self.createVButton:subscribeEvent("Clicked", self.CreateVert_Click, self)
	
	self.deleteButton = CEGUI.toPushButton(self.widget:getWindow("Delete"))
	self.deleteButton:subscribeEvent("Clicked", self.Delete_Click, self)
	
	self.wieldComboBox = CEGUI.toCombobox(self.widget:getWindow("WieldFunction"))
	self.wieldComboBox:subscribeEvent("ListSelectionChanged", self.WieldComboBox_Change ,self)
	self:populateCombobox(self.wieldComboBox)
	self.wieldComboBox:setItemSelectState(0, true)
	self.wieldComboBox:setShowVertScrollbar(true)
	
	self.edibleComboBox = CEGUI.toCombobox(self.widget:getWindow("EdibleFunction"))
	self.edibleComboBox:subscribeEvent("ListSelectionChanged", self.EdibleComboBox_Change ,self)
	self:populateCombobox(self.edibleComboBox)
	self.edibleComboBox:setItemSelectState(1, true)
	self.edibleComboBox:setShowVertScrollbar(true)
		
	self.actionBarListbox = CEGUI.toListbox(self.widget:getWindow("ActionBarList"))
	--We only want the use to select one action bar at a time.
	self.actionBarListbox:setMultiselectEnabled(false)
	
	self.widget:hide()
end

--Create a new action bar.
--@param layout Either "Horiz" or "Vert", represents the rotation of the action bar.
function ActionBarCreator:createActionBar(layout)
	--Find the first available actionbar#.
	local i = 1
	while self.actionbars["ActionBar"..i] ~= nil do
		i = i+1
	end

	--Name that will be used as the key and presented to the user.
	local name = "ActionBar"..i
	
	--Create the new Actionbar.
	a1 = ActionBar.new(layout, self.defaultActionList)
	a1:init(name)
	
	--Insert into our dictionary.
	self.actionbars[name] = a1
	
	--Add to the Listbox presented in the ActionBarCreator widget.
	local actionBar = EmberOgre.Gui.ColouredListItem:new(name)
	self.actionBarListbox:addItem(actionBar)
	
	return a1
end

--Delete the selected action bar in the list.
function ActionBarCreator:deleteActionBar()
	--Get the selected Listbox item.
	local selectedItem = self.actionBarListbox:getFirstSelectedItem()
	--Check if the user has actually selected an action bar.
	if selectedItem ~= nil then
		local name = selectedItem:getText()
		--Delete and remove from the dictionary.
		self.actionbars[name]:shutdown()
		self.actionbars[name] = nil
		--Remove from the Listbox.
		self.actionBarListbox:removeItem(selectedItem)
	else
		--No action bar has been selected, we update the help widget and present it to the user.
		local message = EmberOgre.Gui.HelpMessage:new_local("ActionBarCreator", "Select an ActionBar to remove from the list first.", "action bar creator", "actionBarCreatorMessage")
		EmberOgre.Gui.QuickHelp:getSingleton():updateText(message)
	end
	
	self.actionbarCount = self.actionbarCount-1
end

function ActionBarCreator:populateCombobox(combobox)
	local item = EmberOgre.Gui.ColouredListItem:new("Wield", 0)
	combobox:addItem(item)
	item = EmberOgre.Gui.ColouredListItem:new("Eat", 1)
	combobox:addItem(item)
	combobox:setItemSelectState(0, true)
	combobox:setSingleClickEnabled(true)
end

--Capture user clicks of the delete button.
function ActionBarCreator:Delete_Click()
	self:deleteActionBar()
	return true
end

--Capture user clicks of the create vertical action bar button.
function ActionBarCreator:CreateVert_Click()
	local type = "Vert"
	self:createActionBar(type)
	return true
end

function ActionBarCreator:WieldComboBox_Change()
	local item = self.wieldComboBox:getSelectedItem()
	if item ~= nil then
		local selectId = item:getID()
		if selectId == 0 then
			self.defaultActionList:setDefaultWearableFunction(self.defaultActionList.wield)
		elseif selectId == 1 then
			self.defaultActionList:setDefaultWearableFunction(self.defaultActionList.eat)
		end
	end
	return true
end

function ActionBarCreator:EdibleComboBox_Change()
	local item = self.edibleComboBox:getSelectedItem()
	if item ~= nil then
		local selectId = item:getID()
		if selectId == 0 then
			self.defaultActionList:setDefaultBioMassFunction(self.defaultActionList.wield)
		elseif selectId == 1 then
			self.defaultActionList:setDefaultBioMassFunction(self.defaultActionList.eat)
		end
	end
	return true
end

--Capture user clicks of the create horizontal action bar button.
function ActionBarCreator:CreateHoriz_Click()
	local type = "Horiz"
	self:createActionBar(type)
	return true
end

--Cleanup the widget.
function ActionBarCreator:shutdown()
	--Delete all of the action bars
	for k,v in pairs(self.actionbars) do
		v:shutdown()
	end
	
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

--Creates a starting action bar for the user, and adds keyboard hooks.
function ActionBarCreator:init()
	--Create an inital actionbar for the user.
	self:createActionBar("Horiz"):defaultKeyMapping()
end

--The widget should load when the player has logged in the game.
ActionBarCreator.createdAvatarEntityConnector = createConnector(emberOgre.EventCreatedAvatarEntity):connect(function(avatarEntity)
		if emberOgre:getWorld():getAvatar():isAdmin() == false then
			actionbarCreator = {connectors={},
				actionbars = {},
				defaultActionList = DefaultEntityActions.new()}
			setmetatable(actionbarCreator, {__index = ActionBarCreator})
			actionbarCreator:buildCEGUIWidget()
			actionbarCreator:init()
			connect(actionbarCreator.connectors, emberServices:getServerService().DestroyedAvatar, function()
					actionbarCreator:shutdown()
					actionbarCreator = nil
				end
			)
		end
	end
)
