-----------------------------------------

--A widget for giving something from the inventory to someone. Right now just shows a list of stuff that can be given. This will need to be extended to something much nicer. Perhaps even something which interacts with other inventory widgets.

-----------------------------------------
Give = {connectors={}}
Give.listbox = nil
Give.targetEntityId = nil
Give.listboxMap = {}
Give.widget = guiManager:createWidget()

function Give.buildWidget()
	
	Give.widget:loadMainSheet("Give.layout", "Give/")
	
	Give.widget:hide()
	
	connect(Give.connectors, emberOgre.EventCreatedAvatarEntity, "Give.createdAvatarEmberEntity")
	
	giveButton = Give.widget:getWindow("Give")
	giveButton:subscribeEvent("Clicked", "Give.Give_Click")
	
	cancelButton = Give.widget:getWindow("Cancel")
	cancelButton:subscribeEvent("Clicked", "Give.Cancel_Click")

	connect(Give.connectors, guiManager.EventEntityAction, "Give.handleAction")
	
	local widget = Give.widget:getWindow("ListBox")
	Give.listbox = CEGUI.toListbox(widget)
end

function Give.createdAvatarEmberEntity(avatarEntity)
	connect(Give.connectors, emberOgre:getWorld():getAvatar().EventAddedEntityToInventory, "Give.addedEntity")
	connect(Give.connectors, emberOgre:getWorld():getAvatar().EventRemovedEntityFromInventory, "Give.removedEntity")
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
		local entity = emberOgre:getWorld():getEmberEntity(entityId);
		if (entity ~= nil) then
			emberOgre:doWithEntity(Give.targetEntityId, function (targetEntity) 
				emberServices:getServerService():place(entity, targetEntity)
			end)
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
	Give.targetEntityId = entity:getId()
	Give.widget:show()
	local textWidget = Give.widget:getWindow("Text")
	local text = "Give to " .. entity:getName() .. " ( a " .. entity:getType():getName() .. ")"
	textWidget:setText(text)

end

Give.buildWidget()