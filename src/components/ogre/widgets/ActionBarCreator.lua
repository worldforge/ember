ActionBarCreator = {}


function ActionBarCreator:buildCEGUIWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("ActionBarCreator.layout", "ActionBarCreator/")
	
	self.createHButton = CEGUI.toPushButton(self.widget:getWindow("Create_Horiz"))
	self.createHButton:subscribeEvent("Clicked", self.CreateHoriz_Click, self)
	
	self.createVButton = CEGUI.toPushButton(self.widget:getWindow("Create_Vert"))
	self.createVButton:subscribeEvent("Clicked", self.CreateVert_Click, self)
	
	self.deleteButton = CEGUI.toPushButton(self.widget:getWindow("Delete"))
	self.deleteButton:subscribeEvent("Clicked", self.Delete_Click, self)
	
	self.actionBarListbox = CEGUI.toListbox(self.widget:getWindow("ActionBarList"))
	self.actionBarListbox:setMultiselectEnabled(false)
	
	self.widget:show()
end

function ActionBarCreator:createActionBar(layout)
	--Find the first available actionbar#.
	local i = 1
	while self.actionbars["ActionBar"..i] ~= nil do
		i = i+1
	end

	--Name that will be used as the key and presented to the user.
	local name = "ActionBar"..i
	
	--Create the new Actionbar.
	a1 = ActionBar.new(layout)
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

function ActionBarCreator:CreateVert_Click()
	local type = "Vert"
	self:createActionBar(type)
end

function ActionBarCreator:CreateHoriz_Click()
	local type = "Horiz"
	self:createActionBar(type)
end

function ActionBarCreator:shutdown()
	for k,v in pairs(self.actionbars) do
		v:shutdown()
	end
	
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end


ActionBarCreator.createdAvatarEntityConnector = EmberOgre.LuaConnector:new_local(emberOgre.EventCreatedAvatarEntity):connect(function(avatarEntity)
		if emberOgre:getWorld():getAvatar():isAdmin() == false then
			actionbarCreator = {connectors={},
				actionbars = {}}
			setmetatable(actionbarCreator, {__index = ActionBarCreator})
			actionbarCreator:buildCEGUIWidget()
			connect(actionbarCreator.connectors, avatarEntity.BeingDeleted, function()
					actionbarCreator:shutdown()
					actionbarCreator = nil
				end
			)
		end
	end
)
