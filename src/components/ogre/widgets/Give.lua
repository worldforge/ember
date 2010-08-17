-----------------------------------------

--A widget for giving something from the inventory to someone. Right now just shows a list of stuff that can be given. This will need to be extended to something much nicer. Perhaps even something which interacts with other inventory widgets.

-----------------------------------------
Give = {}

function Give:buildWidget(avatar)

	self.widget = guiManager:createWidget()
	
	self.widget:loadMainSheet("Give.layout", "Give/")
	
	self.widget:hide()
	
	connect(self.connectors, avatar.EventAddedEntityToInventory, self.addedEntity, self)
	connect(self.connectors, avatar.EventRemovedEntityFromInventory, self.removedEntity, self)
	
	giveButton = self.widget:getWindow("Give")
	giveButton:subscribeEvent("Clicked", self.Give_Click, self)
	
	cancelButton = self.widget:getWindow("Cancel")
	cancelButton:subscribeEvent("Clicked", self.Cancel_Click, self)

	connect(self.connectors, guiManager.EventEntityAction, self.handleAction, self)
	
	local widget = self.widget:getWindow("ListBox")
	self.listbox = CEGUI.toListbox(widget)
end

function Give:addedEntity(entity)
	local name = entity:getType():getName() .. " (" .. entity:getId() .. " : " .. entity:getName() .. ")"
	local item = EmberOgre.Gui.ColouredListItem:new(name, entity:getId(), entity)
	self.listboxMap[entity] = item
	--we need to cast it down
	self.listbox:addItem(item)
end

function Give:removedEntity(entity) 
	local item = self.listboxMap[entity];
	if item ~= nil then
		self.listbox:removeItem(tolua.cast(item, "CEGUI::ListboxItem"))
		self.listboxMap[entity] = nil
	end
end

function Give:Give_Click(args)
	local item = self.listbox:getFirstSelectedItem()
	while (item ~= nil) do
		local entityId = item:getID()
		local entity = emberOgre:getWorld():getEmberEntity(entityId);
		if (entity ~= nil) then
			emberOgre:doWithEntity(self.targetEntityId, function (targetEntity) 
				emberServices:getServerService():place(entity, targetEntity)
			end)
		end
		item = self.listbox:getNextSelected(item)
	end
end

function Give:Cancel_Click(args)
	self.widget:hide()
end

function Give:handleAction(action, entity) 

	if action == "give" then
		self:show(entity)
	end
end

function Give:show(entity)
	self.targetEntityId = entity:getId()
	self.widget:show()
	local textWidget = self.widget:getWindow("Text")
	local text = "Give to " .. entity:getName() .. " ( a " .. entity:getType():getName() .. ")"
	textWidget:setText(text)

end

function Give:shutdown()
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

Give.startConnector = createConnector(emberOgre.EventCreatedAvatarEntity):connect(function()
		local give = {connectors={}, listbox = nil, targetEntityId = nil, listboxMap = {}}
		setmetatable(give, {__index = Give})
		give:buildWidget(emberOgre:getWorld():getAvatar())
		
		connect(give.connectors, emberOgre.EventWorldDestroyed, function()
			give:shutdown()
			give = nil
		end
		)
	end
)