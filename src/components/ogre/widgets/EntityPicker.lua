local EntityPicker = {}

--local EntityPicker.mainWindow
EntityPicker.menuWindow = nil
EntityPicker.entityName = nil
EntityPicker.useButton = nil
EntityPicker.entity = nil
EntityPicker.position = nil

EntityPicker.widget = guiManager:createWidget()

function EntityPicker_buildWidget()

	local mousePicker = guiManager:getMousePicker()
    EmberOgre.LuaConnector:new(mousePicker.EventPickedEntity):connect("EntityPicker_pickedEntity")
    EmberOgre.LuaConnector:new(mousePicker.EventPickedNothing):connect("EntityPicker_pickedNothing")
    
    EntityPicker.widget:loadMainSheet("EntityPicker.layout", "EntityPicker/")

	
	EntityPicker.menuWindow = EntityPicker.widget:getWindow("Menu")
	EntityPicker.entityName = EntityPicker.widget:getWindow("EntityName")
	

	EntityPicker.widget:getWindow("TouchButton"):subscribeEvent("MouseButtonUp", "EntityPicker_buttonTouch_Click")
	EntityPicker.widget:getWindow("TakeButton"):subscribeEvent("MouseButtonUp", "EntityPicker_buttonTake_Click")
	EntityPicker.widget:getWindow("GiveButton"):subscribeEvent("MouseButtonUp", "EntityPicker_buttonGive_Click")
	EntityPicker.widget:getWindow("InspectButton"):subscribeEvent("MouseButtonUp", "EntityPicker_buttonInspect_Click")
	EntityPicker.useButton = EntityPicker.widget:getWindow("UseButton")
	EntityPicker.useButton:subscribeEvent("MouseButtonUp", "EntityPicker_buttonUse_Click")
		
    EmberOgre.LuaConnector:new(guiManager:getInput().EventMouseButtonReleased):connect("EntityPicker_input_MouseButtonReleased")

end

function EntityPicker_showMenu(position)
	EntityPicker.widget:show()
	position.x = position.x - 10.0
	position.y = position.y - 10.0
	EntityPicker.widget:getMainWindow():setPosition(CEGUI.Absolute, position )

end

function EntityPicker_pickedEntity(result, args)

	EntityPicker.entity = result.entity
	EntityPicker.position = result.position
	local point = CEGUI.Point:new_local(args.windowX, args.windowY)
	EntityPicker_showMenu(point)
	local name
	--if the entity has a name, use it, else use the type name
	--perhaps we should prefix the type name with an "a" or "an"?
	if EntityPicker.entity:getName() ~= "" then
		name = EntityPicker.entity:getName()
	else
		name = EntityPicker.entity:getType():getName()
	end	
	EntityPicker.entityName:setText(name)
	EntityPicker_checkUse()
end

function EntityPicker_checkUse()
	--try to find the default operation for the wielded entity
	local defaultOp = ""
	local wieldedEntity = emberOgre:getAvatar():getAvatarEmberEntity():getEntityAttachedToPoint("right_hand_wield")
	if wieldedEntity then
		EntityPicker.useButton:setVisible(true)
		local operatorList = wieldedEntity:getDefaultUseOperators();
		if operatorList:size() > 0 then 
			local defaultOp = operatorList[0]
			if defaultOp == "" then
				EntityPicker.useButton:setText("Use with " .. wieldedEntity:getType():getName())
			else
				EntityPicker.useButton:setText(defaultOp .. " with " .. wieldedEntity:getType():getName())
			end
		end
	else 
		EntityPicker.useButton:setVisible(false)
	end
end

function EntityPicker_pickedNothing(args)
	if EntityPicker.widget:getMainWindow():isVisible() then
		EntityPickerWidget_removeMenu()
	end
end

function EntityPicker_buttonTouch_Click(args)
	--print("Type: ", tolua.type(EntityPicker.position))
	emberServices:getServerService():touch(EntityPicker.entity)
	guiManager:EmitEntityAction("touch", EntityPicker.entity)
	EntityPickerWidget_removeMenu()
end

function EntityPicker_buttonTake_Click(args)
	emberServices:getServerService():take(EntityPicker.entity)
	guiManager:EmitEntityAction("take", EntityPicker.entity)
	EntityPickerWidget_removeMenu()
end

function EntityPicker_buttonGive_Click(args)
	guiManager:EmitEntityAction("give", EntityPicker.entity)
	EntityPickerWidget_removeMenu()
end

function EntityPicker_buttonInspect_Click(args)
	guiManager:EmitEntityAction("inspect", EntityPicker.entity)
	EntityPickerWidget_removeMenu()
end

function EntityPicker_buttonUse_Click(args)
--	emberServices:getServerService():use(EntityPicker.entity, EmberOgre.Ogre2Atlas(tolua.cast(EntityPicker.position, "Ogre::Vector<3>")))
	emberServices:getServerService():use(EntityPicker.entity, EmberOgre.Ogre2Atlas(EntityPicker.position))
	guiManager:EmitEntityAction("use", EntityPicker.entity)
	EntityPickerWidget_removeMenu()
end



function EntityPickerWidget_removeMenu()
	
	EntityPicker.widget:hide()
end

function EntityPicker_input_MouseButtonReleased(button,  mode)
	--only show the menu while the left mouse button is pressed
	if button == EmberOgre.Input.MouseButtonLeft then
		EntityPickerWidget_removeMenu()
	end
end

EntityPicker_buildWidget()
