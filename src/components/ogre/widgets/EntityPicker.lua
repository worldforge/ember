-----------------------------------------
--Widget which is shown when an entity is picked. Basically it's a menu which allows the user to perform actions on the entity.


--TODO: 
-- * perhaps implement it as a pie menu with some nice icons?
-----------------------------------------


EntityPicker = {connectors={}, menuWindow = nil, entityName = nil, useButtons = {}, entityId = nil, position = nil, buttons = {}, currentPickedEntityIndex = 0}

EntityPicker.widget = guiManager:createWidget()
EntityPicker.selectorWidget = guiManager:createWidget()

function EntityPicker.buildWidget()

	local entityPickListener = emberOgre:getWorld():getEntityPickListener()
 	connect(EntityPicker.connectors, entityPickListener.EventPickedEntity, "EntityPicker.pickedEntity")
 	--EmberOgre.LuaConnector:new(mousePicker.EventPickedNothing):connect("EntityPicker.pickedNothing")
    
	EntityPicker.widget:loadMainSheet("EntityPicker.layout", "EntityPicker/")
	
	EntityPicker.selectorWidget:loadMainSheet("EntityPickerSelector.layout", "EntityPickerSelector/")
	EntityPicker.selectorWidget:getWindow("PreviousButton"):subscribeEvent("MouseEnter", "EntityPicker.previousButton_MouseEnters")
	EntityPicker.selectorWidget:getWindow("NextButton"):subscribeEvent("MouseEnter", "EntityPicker.nextButton_MouseEnters")

	
	EntityPicker.menuWindow = EntityPicker.widget:getWindow("Menu")
	EntityPicker.entityName = EntityPicker.widget:getWindow("EntityName")
	

	EntityPicker.buttons.moveto = EntityPicker.widget:getWindow("MoveToButton")
	EntityPicker.buttons.moveto:subscribeEvent("MouseButtonUp", "EntityPicker.buttonMoveto_Click")
	EntityPicker.buttons.teleportto = EntityPicker.widget:getWindow("TeleportToButton")
	EntityPicker.buttons.teleportto:subscribeEvent("MouseButtonUp", "EntityPicker.buttonTeleportto_Click")
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
	EntityPicker.buttons.attack = EntityPicker.widget:getWindow("AttackButton")
	EntityPicker.buttons.attack:subscribeEvent("MouseButtonUp", "EntityPicker.attackButton_Click")
	--we can't eat stuff that aren't in our inventory (yet)
--[[	EntityPicker.buttons.eat = EntityPicker.widget:getWindow("EatButton")
	EntityPicker.buttons.eat:subscribeEvent("MouseButtonUp", "EntityPicker.eatButton_Click")]]
	
	
	
	--get a couple of use buttons to allow for different use actions

	EntityPicker.addButton("UseButton1")
	EntityPicker.addButton("UseButton2")
	EntityPicker.addButton("UseButton3")
	EntityPicker.addButton("UseButton4")
	EntityPicker.addButton("UseButton5")
		
	EntityPicker.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(EntityPicker.menuWindow)
	EntityPicker.stackableContainer:setInnerContainerWindow(EntityPicker.menuWindow)
	connect(EntityPicker.connectors, Ember.Input:getSingleton().EventMouseButtonReleased, "EntityPicker.input_MouseButtonReleased")

end

function EntityPicker.addButton(buttonName)
	local buttonWrapper = {}
	buttonWrapper.button = EntityPicker.widget:getWindow(buttonName)
	buttonWrapper.clicked = function(args)
		buttonWrapper.clickedHandler()
	end
	buttonWrapper.button:subscribeEvent("MouseButtonUp", buttonWrapper.clicked)
	local i = table.getn(EntityPicker.useButtons)
	EntityPicker.useButtons[i + 1] = buttonWrapper
	
end

function EntityPicker.showMenu(position)
	EntityPicker.widget:show()
	
	--disble the edit and teleport buttons if we're not admin
	if emberOgre:getWorld():getAvatar():isAdmin() then
		EntityPicker.buttons.edit:setVisible(true)
		EntityPicker.buttons.teleportto:setVisible(true)
	else
		EntityPicker.buttons.edit:setVisible(false)
		EntityPicker.buttons.teleportto:setVisible(false)
	end
	
	local localPosition = CEGUI.Vector2:new_local(position.x, position.y)
	
	localPosition.x = localPosition.x - EntityPicker.widget:getMainWindow():getWidth():asAbsolute(0) * 0.5
	localPosition.y = localPosition.y - 10.0
	local uPosition = CEGUI.UVector2:new_local(CEGUI.UDim(0,localPosition.x), CEGUI.UDim(0,localPosition.y))
	EntityPicker.widget:getMainWindow():setPosition(uPosition )
	EntityPicker.stackableContainer:repositionWindows()
end

function EntityPicker.previousButton_MouseEnters(args)
	EntityPicker.currentPickedEntityIndex = EntityPicker.currentPickedEntityIndex - 1
	EntityPicker.updateSelector()
	EntityPicker.pickedOneEntity(EntityPicker.pickedEntities[EntityPicker.currentPickedEntityIndex])
	return true
end

function EntityPicker.nextButton_MouseEnters(args)
	EntityPicker.currentPickedEntityIndex = EntityPicker.currentPickedEntityIndex + 1
	EntityPicker.updateSelector()
	EntityPicker.pickedOneEntity(EntityPicker.pickedEntities[EntityPicker.currentPickedEntityIndex])
	return true
end

function EntityPicker.updateSelector()
	local numberingWidget = EntityPicker.selectorWidget:getWindow("Numbering")
	numberingWidget:setText((EntityPicker.currentPickedEntityIndex + 1) .. "/" .. (#EntityPicker.pickedEntities + 1))
	local previousWidget = EntityPicker.selectorWidget:getWindow("PreviousButton")
	local nextWidget = EntityPicker.selectorWidget:getWindow("NextButton")
	
	if EntityPicker.currentPickedEntityIndex == #EntityPicker.pickedEntities then
		nextWidget:setVisible(false)
	else
		nextWidget:setVisible(true)
	end
	
	if EntityPicker.currentPickedEntityIndex == 0 then
		previousWidget:setVisible(false)
	else
		previousWidget:setVisible(true)
	end
end

--called when an entity has been picked
function EntityPicker.pickedEntity(results, args)

	if args.pickType == EmberOgre.MPT_CLICK then
		EntityPicker.pickedPoint = CEGUI.Vector2:new_local(args.windowX, args.windowY)
	
		EntityPicker.pickedEntities = {}
		EntityPicker.currentPickedEntityIndex = 0
		for i = 0, results:size() - 1 do
			local resultCopy = {}
			local entity = results[i].entity
			resultCopy.entityId = entity:getId()
			--we must make a copy, else the vector object will be deleted by C++ and we'll end up with garbage
			resultCopy.position = Ogre.Vector3:new_local(results[i].position)
			EntityPicker.pickedEntities[i] = resultCopy
		end	
		
		if results:size() > 1 then
			local point = CEGUI.Vector2:new_local(args.windowX, args.windowY)
			EntityPicker.selectorWidget:show()
			point.x = point.x - EntityPicker.selectorWidget:getMainWindow():getWidth():asAbsolute(0) * 0.5
			point.y = point.y - 40.0
			local uPosition = CEGUI.UVector2:new_local(CEGUI.UDim(0,point.x), CEGUI.UDim(0,point.y))
			EntityPicker.selectorWidget:getMainWindow():setPosition(uPosition)
			EntityPicker.updateSelector()
		else
			EntityPicker.selectorWidget:hide()
		end
	
		EntityPicker.pickedOneEntity(EntityPicker.pickedEntities[0])
	end
end

--called when an entity has been picked
function EntityPicker.pickedOneEntity(pickedResult)
	
	if pickedResult ~= nil then
		emberOgre:doWithEntity(pickedResult.entityId, function (entity) 
			EntityPicker.entityId = entity:getId()
			--we must make a copy, else the vector object will be deleted by C++ and we'll end up with garbage
			EntityPicker.position = Ogre.Vector3:new_local(pickedResult.position)
			
			if (entity:getId() == '0') then
				EntityPicker.buttons.move:setVisible(false)
				EntityPicker.buttons.take:setVisible(false)
				EntityPicker.buttons.attack:setVisible(false)
			else 
				EntityPicker.buttons.move:setVisible(true)
				EntityPicker.buttons.take:setVisible(true)
				EntityPicker.buttons.attack:setVisible(true)
			end
				
				--only show the eat button if the entity has biomass (and thus is edible)
		-- 		if result.entity:hasAttr("biomass") then
		-- 			EntityPicker.buttons.eat:setVisible(true)
		-- 		else
		-- 			EntityPicker.buttons.eat:setVisible(false)
		-- 		end
					
			EntityPicker.checkUse(entity)
			EntityPicker.showMenu(EntityPicker.pickedPoint, entity)
			local name
			--if the entity has a name, use it, else use the type name
			--perhaps we should prefix the type name with an "a" or "an"?
			if entity:getName() ~= "" then
				name = entity:getName()
			else
				name = entity:getType():getName()
			end	
			EntityPicker.entityName:setText(name)
		end)
	end
end

function EntityPicker.checkUse(entity)
	--try to find the default operation for the wielded entity
	for i,v in ipairs(EntityPicker.useButtons) do
		v.button:setVisible(false)
	end
	
	local currentButtonIndex = 0
	
	--first fill up with actions defined for the entity being picked
	local actionList = entity:getActions();
	if actionList:size() > 0 then 
		for i = 0, actionList:size() - 1 do
			currentButtonIndex = currentButtonIndex + 1
			local action = actionList[i]
			local currentButton = EntityPicker.useButtons[currentButtonIndex]
			EntityPicker.addAction(currentButton, entity:getId(), action)
		end
	end	
	
	--then fill up with operations that can be performed with the currently wielded entity
	local wieldedEntity = emberOgre:getWorld():getAvatar():getEmberEntity():getAttachedEntity("right_hand_wield")
	if wieldedEntity then
		local operatorList = wieldedEntity:getDefaultUseOperators();
		if operatorList:size() > 0 then 
			for i = 0, operatorList:size() - 1 do
				currentButtonIndex = currentButtonIndex + 1
				local defaultOp = operatorList[i]
				local currentButton = EntityPicker.useButtons[currentButtonIndex]
				EntityPicker.addUse(currentButton, entity:getId(), wieldedEntity, defaultOp)
			end
		end
	end
end

function EntityPicker.addUse(buttonWrapper, entityId, wieldedEntity, operation)
	buttonWrapper.clickedHandler = function()
		local entity = emberOgre:getWorld():getEmberEntity(entityId)
		if entity ~= nil then
			emberServices:getServerService():use(entity, EmberOgre.Convert:toWF_Point3(EntityPicker.position), operation)
			guiManager:EmitEntityAction("use", entity)
		end
		EntityPickerWidget_removeMenu()
	end	
	
	local button = buttonWrapper.button
	button:setVisible(true)
	if operation == "" then
		button:setText("Use with " .. wieldedEntity:getType():getName())
	else
		button:setText(operation .. " with " .. wieldedEntity:getType():getName())
	end
end

function EntityPicker.addAction(buttonWrapper, entityId, action)
	buttonWrapper.clickedHandler = function()
		local entity = emberOgre:getWorld():getEmberEntity(entityId)
		if entity ~= nil then
			emberServices:getServerService():actuate(entity, action)
			guiManager:EmitEntityAction("actuate", entity)
		end
		EntityPickerWidget_removeMenu()
	end	
	
	local button = buttonWrapper.button
	button:setVisible(true)
	button:setText(action)
end
--function EntityPicker.pickedNothing(args)
--	if EntityPicker.widget:getMainWindow():isVisible() then
--		EntityPickerWidget_removeMenu()
--	end
--end

--Tries to find the selected entity (it might have disappeared from the world in the span of clicking on it and selecting an action) and if it can be found it will call the supplied function with the entity as the first argument.
--This allows you to easily specify functions to call when there is a selected entity. If no entity can be found nothing will happen.
function EntityPicker.doWithPickedEntity(aFunction)
	emberOgre:doWithEntity(EntityPicker.entityId, aFunction)
end

function EntityPicker.buttonMoveto_Click(args)
	emberOgre:getWorld():getMovementController():moveToPoint(EntityPicker.position)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.buttonTeleportto_Click(args)
	EntityPicker.doWithPickedEntity(function (entity)
		emberOgre:getWorld():getMovementController():teleportTo(EntityPicker.position, entity)
	end)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.buttonTouch_Click(args)
	EntityPicker.doWithPickedEntity(function (entity)
		emberServices:getServerService():touch(entity)
		guiManager:EmitEntityAction("touch", entity)
	end)
	--print("Type: ", tolua.type(EntityPicker.position))
	EntityPickerWidget_removeMenu()
end

function EntityPicker.buttonTake_Click(args)
	EntityPicker.doWithPickedEntity(function (entity)
		emberServices:getServerService():take(entity)
		guiManager:EmitEntityAction("take", entity)
	end)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.buttonGive_Click(args)
	EntityPicker.doWithPickedEntity(function (entity)
		guiManager:EmitEntityAction("give", entity)
	end)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.buttonInspect_Click(args)
	EntityPicker.doWithPickedEntity(function (entity)
		guiManager:EmitEntityAction("inspect", entity)
	end)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.buttonMove_Click(args)
	EntityPicker.doWithPickedEntity(function (entity)
		guiManager:EmitEntityAction("move", entity)
	end)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.buttonUse_Click(args)
	EntityPicker.doWithPickedEntity(function (entity)
	--	emberServices:getServerService():use(entity, EmberOgre.Convert:toWF_Point3(tolua.cast(EntityPicker.position, "Ogre::Vector<3>")))
		emberServices:getServerService():use(entity, EmberOgre.Convert:toWF_Point3(EntityPicker.position))
		guiManager:EmitEntityAction("use", entity)
	end)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.editButton_Click(args)
	EntityPicker.doWithPickedEntity(function (entity)
		guiManager:EmitEntityAction("edit", entity)
	end)
	EntityPickerWidget_removeMenu()
end

function EntityPicker.attackButton_Click(args)
	EntityPicker.doWithPickedEntity(function (entity)
		emberServices:getServerService():attack(entity)
		guiManager:EmitEntityAction("attack", entity)
	end)
	EntityPickerWidget_removeMenu()
end

function EntityPickerWidget_removeMenu()
	EntityPicker.widget:hide()
	EntityPicker.selectorWidget:hide()
end

function EntityPicker.input_MouseButtonReleased(button,  mode)
	--only show the menu while the left mouse button is pressed
	if button == Ember.Input.MouseButtonLeft then
		EntityPickerWidget_removeMenu()
	end
end

connect(connectors, emberOgre.EventWorldCreated, EntityPicker.buildWidget)

