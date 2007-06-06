-----------------------------------------

--A widget for giving something from the inventory to someone. Right now just shows a list of stuff that can be given. This will need to be extended to something much nicer. Perhaps even something which interacts with other inventory widgets.

-----------------------------------------
Give = {}
Give.listbox = nil
Give.targetEntity = nil
Give.listboxMap = {}
Give.widget = guiManager:createWidget()

function Give.buildWidget()
	
	Give.widget:loadMainSheet("Give.layout", "Give/")
	
	Give.widget:getMainWindow():setVisible(false)
	
	EmberOgre.LuaConnector:new(emberOgre.EventCreatedAvatarEntity):connect("Give.createdAvatarEmberEntity")
	
	giveButton = Give.widget:getWindow("Give")
	giveButton:subscribeEvent("MouseClick", "Give.Give_Click")
	
	cancelButton = Give.widget:getWindow("Cancel")
	cancelButton:subscribeEvent("MouseClick", "Give.Cancel_Click")

	EmberOgre.LuaConnector:new(guiManager.EventEntityAction):connect("Give.handleAction")
	
	local widget = Give.widget:getWindow("ListBox")
	Give.listbox = CEGUI.toListbox(widget)
end

function Give.createdAvatarEmberEntity(avatarEntity)
	EmberOgre.LuaConnector:new(avatarEntity:getAvatar().EventAddedEntityToInventory):connect("Give.addedEntity")
	EmberOgre.LuaConnector:new(avatarEntity:getAvatar().EventRemovedEntityFromInventory):connect("Give.removedEntity")
end

function Give.addedEntity(entity)
	local name = entity:getType():getName() .. " (" .. entity:getId() .. " : " .. entity:getName() .. ")"
	local item = EmberOgre.Gui.ColouredListItem:new(name, entity:getId(), entity)
	Give.listboxMap[entity] = item
	--we need to cast it down
	Give.listbox:addItem(item)
end

function Give.removedEntity(entity) 
	local item = Give.listboxMap[entity];
	if item ~= nil then
		Give.listbox:removeItem(tolua.cast(item, "CEGUI::ListboxItem"))
		Give.listboxMap[entity] = nil
	end
end

function Give.Give_Click(args)
	local item = Give.listbox:getFirstSelectedItem()
	while (item ~= nil) do
		local entityId = item:getID()
		local entity = emberOgre:getEntity(entityId);
		if (entity ~= nil) then
			emberServices:getServerService():place(entity, Give.targetEntity)
		end
		item = Give.listbox:getNextSelected(item)
	end
end

function Give.Cancel_Click(args)
	Give.widget:hide()
end

function Give.handleAction(action, entity) 

	if action == "give" then
		Give.show(entity)
	end
end

function Give.show(entity)
	Give.targetEntity = entity
	Give.widget:show()
	local textWidget = Give.widget:getWindow("Text")
	local text = "Give to " .. entity:getName() .. " ( a " .. entity:getType():getName() .. ")"
	textWidget:setText(text)

end

Give.buildWidget()