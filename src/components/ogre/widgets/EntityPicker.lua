-----------------------------------------
--Widget which is shown when an entity is picked. Basically it's a menu which allows the user to perform actions on the entity.


--TODO: 
-- * allow for movement
-- * perhaps implement it as a pie menu with some nice icons?
-- * allow for more than the default action to be shown
-----------------------------------------


EntityPicker = {}

--local EntityPicker.mainWindow
EntityPicker.menuWindow = nil
EntityPicker.entityName = nil
EntityPicker.useButtons = {}
EntityPicker.entity = nil
EntityPicker.position = nil
EntityPicker.buttons = {}

EntityPicker.widget = guiManager:createWidget()

function EntityPicker.buildWidget()

	local entityPickListener = guiManager:getEntityPickListener()
    EmberOgre.LuaConnector:new(entityPickListener.EventPickedEntity):connect("EntityPicker.pickedEntity")
    --EmberOgre.LuaConnector:new(mousePicker.EventPickedNothing):connect("EntityPicker.pickedNothing")
    
    EntityPicker.widget:loadMainSheet("EntityPicker.layout", "EntityPicker/")

	
	EntityPicker.menuWindow = EntityPicker.widget:getWindow("Menu")
	EntityPicker.entityName = EntityPicker.widget:getWindow("EntityName")
	

	EntityPicker.buttons.touch = EntityPicker.widget:getWindow("TouchButton")
	EntityPicker.buttons.touch:subscribeEvent("MouseButtonUp", "EntityPicker.buttonTouch_Click")
	EntityPicker.buttons.take = EntityPicker.widget:getWindow("TakeButton")
	EntityPicker.buttons.take:subscribeEvent("MouseButtonUp", "EntityPicker.buttonTake_Click")
	EntityPicker.buttons.give = EntityPicker.widget:getWindow("GiveButton")
	EntityPicker.buttons.give:subscribeEvent("MouseButtonUp", "EntityPicker.buttonGive_Click")
	EntityPicker.buttons.inspect = EntityPicker.widget:getWindow("InspectButton")
	EntityPicker.buttons.inspect:subscribeEvent("MouseButtonUp", "EntityPicker.buttonInspect_Click")
	EntityPicker.buttons.move = EntityPicker.widget:getWindow("MoveButton")
	EntityPicker.buttons.move:subscribeEvent("MouseButtonUp", "EntityPicker.buttonMove_Click")
	EntityPicker.buttons.edit = EntityPicker.widget:getWindow("EditButton")
	EntityPicker.buttons.edit:subscribeEvent("MouseButtonUp", "EntityPicker.editButton_Click")
	
	--disble the edit button if we're not admin
	if emberOgre:getAvatar():isAdmin() then
		EntityPicker.buttons.edit:setVisible(true)
	else
		EntityPicker.buttons.edit:setVisible(false)
	end
	
	--get a couple of use buttons to allow for different use actions
	EntityPicker.useButtons[1] = EntityPicker.widget:getWindow("UseButton1")
	EntityPicker.useButtons[1]:subscribeEvent("MouseButtonUp", "EntityPicker.buttonUse_Click")
	EntityPicker.useButtons[2] = EntityPicker.widget:getWindow("UseButton2")
	EntityPicker.useButtons[2]:subscribeEvent("MouseButtonUp", "EntityPicker.buttonUse_Click")
	EntityPicker.useButtons[3] = EntityPicker.widget:getWindow("UseButton3")
	EntityPicker.useButtons[3]:subscribeEvent("MouseButtonUp", "EntityPicker.buttonUse_Click")
	EntityPicker.useButtons[4] = EntityPicker.widget:getWindow("UseButton4")
	EntityPicker.useButtons[4]:subscribeEvent("MouseButtonUp", "EntityPicker.buttonUse_Click")
	EntityPicker.useButtons[5] = EntityPicker.widget:getWindow("UseButton5")
	EntityPicker.useButtons[5]:subscribeEvent("MouseButtonUp", "EntityPicker.buttonUse_Click")
		
	EntityPicker.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(EntityPicker.menuWindow)
	EntityPicker.stackableContainer:setInnerContainerWindow(EntityPicker.menuWindow)
    EmberOgre.LuaConnector:new(guiManager:getInput().EventMouseButtonReleased):connect("EntityPicker.input_MouseButtonReleased")

end

function EntityPicker.showMenu(position)
	EntityPicker.widget:show()
	
	position.x = position.x - EntityPicker.widget:getMainWindow():getWidth():asAbsolute(0) * 0.5
	position.y = position.y - 10.0
	local uPosition = CEGUI.UVector2:new_local(CEGUI.UDim(0,position.x), CEGUI.UDim(0,position.y))
	EntityPicker.widget:getMainWindow():setPosition(uPosition )
	EntityPicker.stackableContainer:repositionWindows()
end

--called when an entity has been picked
function EntityPicker.pickedEntity(result, args)
	
	EntityPicker.entity = result.entity
	--we must make a copy, else the vector object will be deleted by C++ and we'll end up with garbage
	EntityPicker.position = Ogre.Vector3:new_local(result.position)
	local point = CEGUI.Vector2:new_local(args.windowX, args.windowY)
	
	if (EntityPicker.entity:getId() == '0') then
		EntityPicker.buttons.move:setVisible(false)
		EntityPicker.buttons.take:setVisible(false)
	else 
		EntityPicker.buttons.move:setVisible(true)
		EntityPicker.buttons.take:setVisible(true)
	end
	
	EntityPicker.checkUse()
	EntityPicker.showMenu(point)
	local name
	--if the entity has a name, use it, else use the type name
	--perhaps we should prefix the type name with an "a" or "an"?
	if EntityPicker.entity:getName() ~= "" then
		name = EntityPicker.entity:getName()
	else
		name = EntityPicker.entity:getType():getName()
	end	
	EntityPicker.entityName:setText(name)
end

function EntityPicker.checkUse()
	--try to find the default operation for the wielded entity
	for i,v in ipairs(EntityPicker.useButtons) do
		v:setVisible(false)
	end
	
	
	local wieldedEntity = emberOgre:getAvatar():getAvatarEmberEntity():getEntityAttachedToPoint("right_hand_wield")
	if wieldedEntity then
		local operatorList = wieldedEntity:getDefaultUseOperators();
		if operatorList:size() > 0 then 
			for i = 0, operatorList:size() - 1 do
				local defaultOp = operatorList[i]
				EntityPicker.useButtons[i+1]:setVisible(true)
				if defaultOp == "" then
					EntityPicker.useButtons[i+1]:setText("Use with " .. wieldedEntity:getType():getName())
				else
					EntityPicker.useButtons[i+1]:setText(defaultOp .. " with " .. wieldedEntity:getType():getName())
				end
			end
		end
	end
end

--function EntityPicker.pickedNothing(args)
--	if EntityPicker.widget:getMainWindow():isVisible() then
--		EntityPickerWidget_removeMenu()
--	end
--end

function EntityPicker.buttonTouch_Click(args)
	--print("Type: ", tolua.type(EntityPicker.position))
	emberServices:getServerService():touch(EntityPicker.entity)
	guiManager:EmitEntityAction("touch", EntityPicker.entity)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.buttonTake_Click(args)
	emberServices:getServerService():take(EntityPicker.entity)
	guiManager:EmitEntityAction("take", EntityPicker.entity)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.buttonGive_Click(args)
	guiManager:EmitEntityAction("give", EntityPicker.entity)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.buttonInspect_Click(args)
	guiManager:EmitEntityAction("inspect", EntityPicker.entity)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.buttonMove_Click(args)
	guiManager:EmitEntityAction("move", EntityPicker.entity)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.buttonUse_Click(args)
--	emberServices:getServerService():use(EntityPicker.entity, EmberOgre.Ogre2Atlas(tolua.cast(EntityPicker.position, "Ogre::Vector<3>")))
	emberServices:getServerService():use(EntityPicker.entity, EmberOgre.Ogre2Atlas(EntityPicker.position))
	guiManager:EmitEntityAction("use", EntityPicker.entity)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.editButton_Click(args)
	guiManager:EmitEntityAction("edit", EntityPicker.entity)
	EntityPickerWidget_removeMenu()
end


function EntityPickerWidget_removeMenu()
	
	EntityPicker.widget:hide()
end

function EntityPicker.input_MouseButtonReleased(button,  mode)
	--only show the menu while the left mouse button is pressed
	if button == EmberOgre.Input.MouseButtonLeft then
		EntityPickerWidget_removeMenu()
	end
end

EntityPicker.buildWidget()
