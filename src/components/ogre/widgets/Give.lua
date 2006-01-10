-----------------------------------------
-- Start of handler functions
-----------------------------------------

-----------------------------------------
-- Script Entry Point
-----------------------------------------
Give = {}
Give.listbox = nil
Give.targetEntity = nil
Give.listboxMap = {}
Give.widget = guiManager:createWidget()

function GiveWidget_buildWidget()
	
	Give.widget:loadMainSheet("Give.layout", "Give/")
	
	Give.widget:getMainWindow():setVisible(false)
	
	EmberOgre.LuaConnector:new(emberOgre.EventCreatedAvatarEntity):connect("Give_createdAvatarEmberEntity")
	
	giveButton = Give.widget:getWindow("Give")
	giveButton:subscribeEvent("MouseClick", "Give_Give_Click")
	
	cancelButton = Give.widget:getWindow("Cancel")
	cancelButton:subscribeEvent("MouseClick", "Give_Cancel_Click")

	EmberOgre.LuaConnector:new(guiManager.EventEntityAction):connect("Give_handleAction")
	
	local widget = Give.widget:getWindow("ListBox")
	Give.listbox = CEGUI.toListbox(widget)
end

function Give_createdAvatarEmberEntity(avatarEntity)
--	console:pushMessage("Created avatar.")
--	console:pushMessage("Avatar type: " .. tolua.type(avatarEntity:getAvatar()))
--	console:pushMessage("Avatar event type: " .. tolua.type(avatarEntity:getAvatar().EventAddedEntityToInventory))
--	console:pushMessage("Avatar event type: " .. tolua.type(avatarEntity:getAvatar().EventRemovedEntityFromInventory))
	EmberOgre.LuaConnector:new(avatarEntity:getAvatar().EventAddedEntityToInventory):connect("Give_addedEntity")
	EmberOgre.LuaConnector:new(avatarEntity:getAvatar().EventRemovedEntityFromInventory):connect("Give_removedEntity")
end

function Give_addedEntity(entity)
	console:pushMessage("Added entity.")
	local name = entity:getType():getName() .. " (" .. entity:getId() .. " : " .. entity:getName() .. ")"
	local item = EmberOgre.ColoredListItem:new(name, entity:getId(), entity)
	Give.listboxMap[entity] = item
	--we need to cast it down
	Give.listbox:addItem(tolua.cast(item, "CEGUI::ListboxItem"))
end

function Give_removedEntity(entity) 
	local item = Give.listboxMap[entity];
	if item ~= nil then
		Give.listbox:removeItem(tolua.cast(item, "CEGUI::ListboxItem"))
		Give.listboxMap[entity] = nil
	end
end

function Give_Give_Click(args)
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

function Give_Cancel_Click(args)
	Give.widget:hide()
end

function Give_handleAction(action, entity) 

	if action == "give" then
		Give_show(entity)
	end
end

function Give_show(entity)
	Give.targetEntity = entity
	Give.widget:show()
	local textWidget = Give.widget:getWindow("Text")
	local text = "Give to " .. entity:getName() .. " ( a " .. entity:getType():getName() .. ")"
	textWidget:setText(text)

end

GiveWidget_buildWidget()