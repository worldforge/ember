-----------------------------------------
--Widget which is shown when an entity is picked. Basically it's a menu which allows the user to perform actions on the entity.


--TODO: 
-- * perhaps implement it as a pie menu with some nice icons?
-----------------------------------------


EntityPicker = {}

function EntityPicker:buildWidget(world)

	self.widget = guiManager:createWidget()
	self.selectorWidget = guiManager:createWidget()

	local entityPickListener = world:getEntityPickListener()
 	connect(self.connectors, entityPickListener.EventPickedEntity, self.pickedEntity, self)
 	--EmberOgre.LuaConnector:new(mousePicker.EventPickedNothing):connect(self.pickedNothing, self)
    
	self.widget:loadMainSheet("EntityPicker.layout", "EntityPicker/")
	
	self.selectorWidget:loadMainSheet("EntityPickerSelector.layout", "EntityPickerSelector/")
	self.selectorWidget:getWindow("PreviousButton"):subscribeEvent("MouseEnter", self.previousButton_MouseEnters, self)
	self.selectorWidget:getWindow("NextButton"):subscribeEvent("MouseEnter", self.nextButton_MouseEnters, self)

	
	self.menuWindow = self.widget:getWindow("Menu")
	self.entityName = self.widget:getWindow("EntityName")
	

	self.buttons.moveto = self.widget:getWindow("MoveToButton")
	self.buttons.moveto:subscribeEvent("MouseButtonUp", self.buttonMoveto_Click, self)
	self.buttons.teleportto = self.widget:getWindow("TeleportToButton")
	self.buttons.teleportto:subscribeEvent("MouseButtonUp", self.buttonTeleportto_Click, self)
	self.buttons.touch = self.widget:getWindow("TouchButton")
	self.buttons.touch:subscribeEvent("MouseButtonUp", self.buttonTouch_Click, self)
	self.buttons.take = self.widget:getWindow("TakeButton")
	self.buttons.take:subscribeEvent("MouseButtonUp", self.buttonTake_Click, self)
	self.buttons.give = self.widget:getWindow("GiveButton")
	self.buttons.give:subscribeEvent("MouseButtonUp", self.buttonGive_Click, self)
	self.buttons.inspect = self.widget:getWindow("InspectButton")
	self.buttons.inspect:subscribeEvent("MouseButtonUp", self.buttonInspect_Click, self)
	self.buttons.move = self.widget:getWindow("MoveButton")
	self.buttons.move:subscribeEvent("MouseButtonUp", self.buttonMove_Click, self)
	self.buttons.edit = self.widget:getWindow("EditButton")
	self.buttons.edit:subscribeEvent("MouseButtonUp", self.editButton_Click, self)
	self.buttons.attack = self.widget:getWindow("AttackButton")
	self.buttons.attack:subscribeEvent("MouseButtonUp", self.attackButton_Click, self)
	--we can't eat stuff that aren't in our inventory (yet)
--[[	self.buttons.eat = self.widget:getWindow("EatButton")
	self.buttons.eat:subscribeEvent("MouseButtonUp", self.eatButton_Click, self)]]
	
	
	
	--get a couple of use buttons to allow for different use actions

	self:addButton("UseButton1")
	self:addButton("UseButton2")
	self:addButton("UseButton3")
	self:addButton("UseButton4")
	self:addButton("UseButton5")
		
	self.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(self.menuWindow)
	self.stackableContainer:setInnerContainerWindow(self.menuWindow)
	connect(self.connectors, Ember.Input:getSingleton().EventMouseButtonReleased, self.input_MouseButtonReleased, self)

end

function EntityPicker:addButton(buttonName)
	local buttonWrapper = {}
	buttonWrapper.button = self.widget:getWindow(buttonName)
	buttonWrapper.clicked = function(args)
		buttonWrapper.clickedHandler()
	end
	buttonWrapper.button:subscribeEvent("MouseButtonUp", buttonWrapper.clicked)
	local i = table.getn(self.useButtons)
	self.useButtons[i + 1] = buttonWrapper
	
end

function EntityPicker:showMenu(position)
	self.widget:show()
	
	--disble the edit and teleport buttons if we're not admin
	if emberOgre:getWorld():getAvatar():isAdmin() then
		self.buttons.edit:setVisible(true)
		self.buttons.teleportto:setVisible(true)
	else
		self.buttons.edit:setVisible(false)
		self.buttons.teleportto:setVisible(false)
	end
	
	local localPosition = CEGUI.Vector2:new_local(position.x, position.y)
	
	localPosition.x = localPosition.x - self.widget:getMainWindow():getWidth():asAbsolute(0) * 0.5
	localPosition.y = localPosition.y - 10.0
	local uPosition = CEGUI.UVector2:new_local(CEGUI.UDim(0,localPosition.x), CEGUI.UDim(0,localPosition.y))
	self.widget:getMainWindow():setPosition(uPosition )
	self.stackableContainer:repositionWindows()
end

function EntityPicker:previousButton_MouseEnters(args)
	self.currentPickedEntityIndex = self.currentPickedEntityIndex - 1
	self:updateSelector()
	self:pickedOneEntity(self.pickedEntities[self.currentPickedEntityIndex])
	return true
end

function EntityPicker:nextButton_MouseEnters(args)
	self.currentPickedEntityIndex = self.currentPickedEntityIndex + 1
	self:updateSelector()
	self:pickedOneEntity(self.pickedEntities[self.currentPickedEntityIndex])
	return true
end

function EntityPicker:updateSelector()
	local numberingWidget = self.selectorWidget:getWindow("Numbering")
	numberingWidget:setText((self.currentPickedEntityIndex + 1) .. "/" .. (#self.pickedEntities + 1))
	local previousWidget = self.selectorWidget:getWindow("PreviousButton")
	local nextWidget = self.selectorWidget:getWindow("NextButton")
	
	if self.currentPickedEntityIndex == #self.pickedEntities then
		nextWidget:setVisible(false)
	else
		nextWidget:setVisible(true)
	end
	
	if self.currentPickedEntityIndex == 0 then
		previousWidget:setVisible(false)
	else
		previousWidget:setVisible(true)
	end
end

--called when an entity has been picked
function EntityPicker:pickedEntity(results, args)

	if args.pickType == EmberOgre.MPT_CLICK then
		self.pickedPoint = CEGUI.Vector2:new_local(args.windowX, args.windowY)
	
		self.pickedEntities = {}
		self.currentPickedEntityIndex = 0
		for i = 0, results:size() - 1 do
			local resultCopy = {}
			local entity = results[i].entity
			resultCopy.entityId = entity:getId()
			--we must make a copy, else the vector object will be deleted by C++ and we'll end up with garbage
			resultCopy.position = Ogre.Vector3:new_local(results[i].position)
			self.pickedEntities[i] = resultCopy
		end	
		
		if results:size() > 1 then
			local point = CEGUI.Vector2:new_local(args.windowX, args.windowY)
			self.selectorWidget:show()
			point.x = point.x - self.selectorWidget:getMainWindow():getWidth():asAbsolute(0) * 0.5
			point.y = point.y - 40.0
			local uPosition = CEGUI.UVector2:new_local(CEGUI.UDim(0,point.x), CEGUI.UDim(0,point.y))
			self.selectorWidget:getMainWindow():setPosition(uPosition)
			self:updateSelector()
		else
			self.selectorWidget:hide()
		end
	
		self:pickedOneEntity(self.pickedEntities[0])
	end
end

--called when an entity has been picked
function EntityPicker:pickedOneEntity(pickedResult)
	
	if pickedResult ~= nil then
		emberOgre:doWithEntity(pickedResult.entityId, function (entity) 
			self.entityId = entity:getId()
			--we must make a copy, else the vector object will be deleted by C++ and we'll end up with garbage
			self.position = Ogre.Vector3:new_local(pickedResult.position)
			
			if (entity:getId() == '0') then
				self.buttons.move:setVisible(false)
				self.buttons.take:setVisible(false)
				self.buttons.attack:setVisible(false)
			else 
				self.buttons.move:setVisible(true)
				self.buttons.take:setVisible(true)
				self.buttons.attack:setVisible(true)
			end
				
				--only show the eat button if the entity has biomass (and thus is edible)
		-- 		if result.entity:hasAttr("biomass") then
		-- 			self.buttons.eat:setVisible(true)
		-- 		else
		-- 			self.buttons.eat:setVisible(false)
		-- 		end
					
			self:checkUse(entity)
			self:showMenu(self.pickedPoint, entity)
			local name
			--if the entity has a name, use it, else use the type name
			--perhaps we should prefix the type name with an "a" or "an"?
			if entity:getName() ~= "" then
				name = entity:getName()
			else
				name = entity:getType():getName()
			end	
			self.entityName:setText(name)
		end)
	end
end

function EntityPicker:checkUse(entity)
	--try to find the default operation for the wielded entity
	for i,v in ipairs(self.useButtons) do
		v.button:setVisible(false)
	end
	
	local currentButtonIndex = 0
	
	--first fill up with actions defined for the entity being picked
	local actionList = entity:getActions();
	if actionList:size() > 0 then 
		for i = 0, actionList:size() - 1 do
			currentButtonIndex = currentButtonIndex + 1
			local action = actionList[i]
			local currentButton = self.useButtons[currentButtonIndex]
			self:addAction(currentButton, entity:getId(), action)
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
				local currentButton = self.useButtons[currentButtonIndex]
				self:addUse(currentButton, entity:getId(), wieldedEntity, defaultOp)
			end
		end
	end
end

function EntityPicker:addUse(buttonWrapper, entityId, wieldedEntity, operation)
	buttonWrapper.clickedHandler = function()
		local entity = emberOgre:getWorld():getEmberEntity(entityId)
		if entity ~= nil then
			emberServices:getServerService():use(entity, EmberOgre.Convert:toWF_Point3(self.position), operation)
			guiManager:EmitEntityAction("use", entity)
		end
		self:removeMenu()
	end	
	
	local button = buttonWrapper.button
	button:setVisible(true)
	if operation == "" then
		button:setText("Use with " .. wieldedEntity:getType():getName())
	else
		button:setText(operation .. " with " .. wieldedEntity:getType():getName())
	end
end

function EntityPicker:addAction(buttonWrapper, entityId, action)
	buttonWrapper.clickedHandler = function()
		local entity = emberOgre:getWorld():getEmberEntity(entityId)
		if entity ~= nil then
			emberServices:getServerService():actuate(entity, action)
			guiManager:EmitEntityAction("actuate", entity)
		end
		self:removeMenu()
	end	
	
	local button = buttonWrapper.button
	button:setVisible(true)
	button:setText(action)
end
--function EntityPicker:pickedNothing(args)
--	if self.widget:getMainWindow():isVisible() then
--		self:removeMenu()
--	end
--end

--Tries to find the selected entity (it might have disappeared from the world in the span of clicking on it and selecting an action) and if it can be found it will call the supplied function with the entity as the first argument.
--This allows you to easily specify functions to call when there is a selected entity. If no entity can be found nothing will happen.
function EntityPicker:doWithPickedEntity(aFunction)
	emberOgre:doWithEntity(self.entityId, aFunction)
end

function EntityPicker:buttonMoveto_Click(args)
	emberOgre:getWorld():getMovementController():moveToPoint(self.position)
	self:removeMenu()
end

function EntityPicker:buttonTeleportto_Click(args)
	self.doWithPickedEntity(function (entity)
		emberOgre:getWorld():getMovementController():teleportTo(self.position, entity)
	end)
	self:removeMenu()
end

function EntityPicker:buttonTouch_Click(args)
	self.doWithPickedEntity(function (entity)
		emberServices:getServerService():touch(entity)
		guiManager:EmitEntityAction("touch", entity)
	end)
	--print("Type: ", tolua.type(self.position))
	self:removeMenu()
end

function EntityPicker:buttonTake_Click(args)
	self.doWithPickedEntity(function (entity)
		emberServices:getServerService():take(entity)
		guiManager:EmitEntityAction("take", entity)
	end)
	self:removeMenu()
end

function EntityPicker:buttonGive_Click(args)
	self.doWithPickedEntity(function (entity)
		guiManager:EmitEntityAction("give", entity)
	end)
	self:removeMenu()
end

function EntityPicker:buttonInspect_Click(args)
	self.doWithPickedEntity(function (entity)
		guiManager:EmitEntityAction("inspect", entity)
	end)
	self:removeMenu()
end

function EntityPicker:buttonMove_Click(args)
	self.doWithPickedEntity(function (entity)
		guiManager:EmitEntityAction("move", entity)
	end)
	self:removeMenu()
end

function EntityPicker:buttonUse_Click(args)
	self.doWithPickedEntity(function (entity)
	--	emberServices:getServerService():use(entity, EmberOgre.Convert:toWF_Point3(tolua.cast(self.position, "Ogre::Vector<3>")))
		emberServices:getServerService():use(entity, EmberOgre.Convert:toWF_Point3(self.position))
		guiManager:EmitEntityAction("use", entity)
	end)
	self:removeMenu()
end

function EntityPicker:editButton_Click(args)
	self.doWithPickedEntity(function (entity)
		guiManager:EmitEntityAction("edit", entity)
	end)
	self:removeMenu()
end

function EntityPicker:attackButton_Click(args)
	self.doWithPickedEntity(function (entity)
		emberServices:getServerService():attack(entity)
		guiManager:EmitEntityAction("attack", entity)
	end)
	self:removeMenu()
end

function EntityPicker:removeMenu()
	self.widget:hide()
	self.selectorWidget:hide()
end

function EntityPicker:input_MouseButtonReleased(button,  mode)
	--only show the menu while the left mouse button is pressed
	if button == Ember.Input.MouseButtonLeft then
		self:removeMenu()
	end
end

function EntityPicker:shutdown()
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
	guiManager:destroyWidget(self.selectorWidget)
end

connect(connectors, emberOgre.EventWorldCreated, function(world)
	entityPicker = {connectors={}, menuWindow = nil, entityName = nil, useButtons = {}, entityId = nil, position = nil, buttons = {}, currentPickedEntityIndex = 0}
	setmetatable(entityPicker, {__index = EntityPicker})
	
	entityPicker:buildWidget(world)
	connect(entityPicker.connectors, emberOgre.EventWorldDestroyed, function()
			entityPicker:shutdown()
			entityPicker = nil
		end
	)
	
end
)

