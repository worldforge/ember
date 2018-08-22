-----------------------------------------
-- Widget which is shown when an entity is picked. Basically it's a menu which allows the user to perform actions on the entity.


--TODO: 
-- * perhaps implement it as a pie menu with some nice icons?
-----------------------------------------


EntityPicker = {}

function EntityPicker:buildWidget(world)

    self.world = world
    self.widget = guiManager:createWidget()
    self.selectorWidget = guiManager:createWidget()

    local entityPickListener = world:getEntityPickListener()
    connect(self.connectors, entityPickListener.EventPickedEntity, self.pickedEntity, self)
    --createConnector(mousePicker.EventPickedNothing):connect(self.pickedNothing, self)

    self.widget:loadMainSheet("EntityPicker.layout", "EntityPicker")

    self.selectorWidget:loadMainSheet("EntityPickerSelector.layout", "EntityPickerSelector")
    self.selectorWidget:getWindow("PreviousButton"):subscribeEvent("MouseEntersSurface", self.previousButton_MouseEnters, self)
    self.selectorWidget:getWindow("NextButton"):subscribeEvent("MouseEntersSurface", self.nextButton_MouseEnters, self)


    self.menuWindow = self.widget:getWindow("Menu")
    self.entityName = self.widget:getWindow("EntityName")


    self.buttons.moveto = self.widget:getWindow("MoveToButton")
    self.buttons.moveto:subscribeEvent("MouseButtonUp", self.buttonMoveto_Click, self)
    self.buttons.teleportto = self.widget:getWindow("TeleportToButton")
    self.buttons.teleportto:subscribeEvent("MouseButtonUp", self.buttonTeleportto_Click, self)
    self.buttons.talk = self.widget:getWindow("TalkButton")
    self.buttons.talk:subscribeEvent("MouseButtonUp", self.buttonTalk_Click, self)
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

    --Collect a list of all the entity types which the user can normally talk to.
    --Note that the user still can talk to other entities, it's just not as easy to address them.
    self.talkingEntityTypes = {}
    self.typeService = world:getView():getAvatar():getConnection():getTypeService()

    --get a couple of use buttons to allow for different use actions

    self:addButton("UseButton1")
    self:addButton("UseButton2")
    self:addButton("UseButton3")
    self:addButton("UseButton4")
    self:addButton("UseButton5")

    self.stackableContainer = Ember.OgreView.Gui.StackableContainer:new_local(self.menuWindow)
    self.stackableContainer:setInnerContainerWindow(self.menuWindow)
    connect(self.connectors, Ember.Input:getSingleton().EventMouseButtonReleased, self.input_MouseButtonReleased, self)


    --Check whether we should show the inspect button even for non admin types.
    local configService = emberServices:getConfigService()
    local evaluateShowInspect = function()
        if world:getAvatar():isAdmin() then
            self.buttons.inspect:setVisible(true)
        else
            local variable = configService:getValue("authoring", "showinspectforall")
            if variable and variable:is_bool() then
                local showInspectForAll = variable[".bool"](variable);
                self.buttons.inspect:setVisible(showInspectForAll)
            else
                self.buttons.inspect:setVisible(false)
            end
        end
    end

    evaluateShowInspect()

    local valueChangedCall = function(section, key)
        if section == "authoring" and key == "showinspectforall" then
            evaluateShowInspect()
        end
    end
    connect(self.connectors, configService.EventChangedConfigItem, valueChangedCall)
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

function EntityPicker:showMenu(position, entity)
    self.widget:show()

    --disable the edit and teleport buttons if we're not admin
    if self.world:getAvatar():isAdmin() then
        self.buttons.edit:setVisible(true)
        self.buttons.teleportto:setVisible(true)
    else
        self.buttons.edit:setVisible(false)
        self.buttons.teleportto:setVisible(false)
    end

    --only show "talk" button for entities which we can talk to
    local isTalkable = false
    for i, v in ipairs(self.talkingEntityTypes) do
        if entity:getType():isA(v) then
            isTalkable = true
        end
    end
    if isTalkable then
        self.buttons.talk:setVisible(true)
    else
        self.buttons.talk:setVisible(false)
    end



    self.stackableContainer:repositionWindows()

    local localPosition = CEGUI.Vector2f:new_local(position.x, position.y)

    localPosition.x = localPosition.x - self.widget:getMainWindow():getPixelSize().width * 0.5
    localPosition.y = localPosition.y - 10.0

    --Make sure the menu is fully contained within the main window
    if localPosition.x < 0 then
        localPosition.x = 0
    end
    if localPosition.y < 0 then
        localPosition.y = 0
    end
    local width = self.widget:getMainWindow():getPixelSize().width
    local height = self.stackableContainer:getAbsoluteHeight() + self.entityName:getPixelSize().height

    local mainWindowSize = root:getPixelSize()
    if localPosition.x + width > mainWindowSize.width then
        localPosition.x = mainWindowSize.width - width
    end
    if localPosition.y + height > mainWindowSize.height then
        localPosition.y = mainWindowSize.height - height
    end


    local uPosition = CEGUI.UVector2:new_local(CEGUI.UDim(0, localPosition.x), CEGUI.UDim(0, localPosition.y))
    self.widget:getMainWindow():setPosition(uPosition)
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

    if args.pickType == Ember.OgreView.MPT_PRESSED then
        self.pickedPoint = CEGUI.Vector2f:new_local(args.windowX, args.windowY)

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
            local point = CEGUI.Vector2f:new_local(args.windowX, args.windowY)
            self.selectorWidget:show()
            point.x = point.x - self.selectorWidget:getMainWindow():getPixelSize().width * 0.5
            point.y = point.y - 40.0
            local uPosition = CEGUI.UVector2:new_local(CEGUI.UDim(0, point.x), CEGUI.UDim(0, point.y))
            self.selectorWidget:getMainWindow():setPosition(uPosition)
            self:updateSelector()
        else
            self.selectorWidget:hide()
        end

        self:pickedOneEntity(self.pickedEntities[0])
    elseif args.pickType == Ember.OgreView.MPT_CLICK then
        if results:size() > 0 then
            local entity = results[0].entity
            if entity then
                emberServices:getServerService():touch(entity, Ember.OgreView.Convert:toWF_Point3(results[0].position))
                guiManager:EmitEntityAction("touch", entity)
                local name
                --if the entity has a name, use it, else use the type name
                --perhaps we should prefix the type name with an "a" or "an"?
                if entity:getName() ~= "" then
                    name = entity:getName()
                else
                    name = entity:getType():getName()
                end
                guiManager:appendAvatarImaginary("You touch " .. name .. ".")

                if entity:hasAttr("message") then
                    local messageElement = entity:valueOfAttr("message")
                    if messageElement:isString() and messageElement:asString() ~= "" then
                        guiManager:appendAvatarImaginary("Message: " .. messageElement:asString())
                    end
                end
            end
        end
    end
end

--called when an entity has been picked
function EntityPicker:pickedOneEntity(pickedResult)

    if pickedResult ~= nil then
        emberOgre:doWithEntity(pickedResult.entityId, function(entity)
            self.entityId = entity:getId()
            --we must make a copy, else the vector object will be deleted by C++ and we'll end up with garbage
            self.position = Ogre.Vector3:new_local(pickedResult.position)

            if (entity:getId() == '0') then
                self.buttons.move:setVisible(false)
                self.buttons.take:setVisible(false)
            else
                self.buttons.move:setVisible(true)
                self.buttons.take:setVisible(true)
            end

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
    for i, v in ipairs(self.useButtons) do
        v.button:setVisible(false)
    end

    local currentButtonIndex = 0

    --first fill up with actions defined for the entity being picked
    local actionList = entity:getUsages();
    if actionList:size() > 0 then
        for i = 0, actionList:size() - 1 do
            currentButtonIndex = currentButtonIndex + 1
            local action = actionList[i]
            local currentButton = self.useButtons[currentButtonIndex]
            self:addAction(currentButton, entity:getId(), action)
        end
    end

    --then fill up with operations that can be performed with the currently wielded entity
    local wieldedEntity = self.world:getAvatar():getEmberEntity():getAttachedEntity("attached_hand_primary")
    if wieldedEntity then
        currentButtonIndex = currentButtonIndex + 1
        local operatorList = wieldedEntity:getUsages();
        if operatorList:size() > 0 then
            for i = 0, operatorList:size() - 1 do
                currentButtonIndex = currentButtonIndex + 1
                local defaultOp = operatorList[i]
                local currentButton = self.useButtons[currentButtonIndex]
                self:addUse(currentButton, entity:getId(), wieldedEntity, defaultOp)
            end
        end
    else
        --else, if nothing is wielded, check if there are any usages on the avatar itself
        local operatorList = self.world:getAvatar():getEmberEntity():getUsagesProtected();
        if operatorList:size() > 0 then
            for i = 0, operatorList:size() - 1 do
                currentButtonIndex = currentButtonIndex + 1
                local defaultOp = operatorList[i]
                local currentButton = self.useButtons[currentButtonIndex]
                self:addUseSelf(currentButton, entity:getId(), self.world:getAvatar():getEmberEntity(), defaultOp)
            end
        end
    end
end

function EntityPicker:addUseSelf(buttonWrapper, entityId, wieldedEntity, operation)
    buttonWrapper.clickedHandler = function()
        local entity = self.world:getEmberEntity(entityId)
        if entity then
            self.world:getAvatar():useTool(wieldedEntity, operation, entity, Ember.OgreView.Convert:toWF_Point3(self.position))
            guiManager:EmitEntityAction("use", entity)
        end
        self:removeMenu()
    end

    local button = buttonWrapper.button
    button:setVisible(true)
    button:setText(operation)
end

function EntityPicker:addUse(buttonWrapper, entityId, wieldedEntity, operation)
    buttonWrapper.clickedHandler = function()
        local entity = self.world:getEmberEntity(entityId)
        if entity then
            self.world:getAvatar():useTool(wieldedEntity, operation, entity, Ember.OgreView.Convert:toWF_Point3(self.position))
            --			emberServices:getServerService():use(wieldedEntity, entity, Ember.OgreView.Convert:toWF_Point3(self.position), operation)
            guiManager:EmitEntityAction("use", entity)
        end
        self:removeMenu()
    end

    local button = buttonWrapper.button
    button:setVisible(true)
    button:setText(operation .. " with " .. wieldedEntity:getType():getName())
end

function EntityPicker:addAction(buttonWrapper, entityId, action)
    buttonWrapper.clickedHandler = function()
        local entity = self.world:getEmberEntity(entityId)
        if entity ~= nil then
            self.world:getAvatar():useTool(entity, action)
            guiManager:EmitEntityAction("use", entity)
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
    self.world:getMovementController():moveToPoint(self.position)
    self:removeMenu()
end

function EntityPicker:buttonTeleportto_Click(args)
    self:doWithPickedEntity(function(entity)
        self.world:getMovementController():teleportTo(self.position, entity)
    end)
    self:removeMenu()
end

function EntityPicker:buttonTouch_Click(args)
    self:doWithPickedEntity(function(entity)
        emberServices:getServerService():touch(entity, Ember.OgreView.Convert:toWF_Point3(self.position))
        guiManager:EmitEntityAction("touch", entity)
    end)
    self:removeMenu()
end

function EntityPicker:buttonTalk_Click(args)
    self:doWithPickedEntity(function(entity)
        guiManager:EmitEntityAction("talk", entity)
    end)
    self:removeMenu()
end

function EntityPicker:buttonTake_Click(args)
    self:doWithPickedEntity(function(entity)
        emberServices:getServerService():take(entity)
        guiManager:EmitEntityAction("take", entity)
    end)
    self:removeMenu()
end

function EntityPicker:buttonGive_Click(args)
    self:doWithPickedEntity(function(entity)
        guiManager:EmitEntityAction("give", entity)
    end)
    self:removeMenu()
end

function EntityPicker:buttonInspect_Click(args)
    self:doWithPickedEntity(function(entity)
        guiManager:EmitEntityAction("inspect", entity)
    end)
    self:removeMenu()
end

function EntityPicker:buttonMove_Click(args)
    self:doWithPickedEntity(function(entity)
        guiManager:EmitEntityAction("move", entity)
    end)
    self:removeMenu()
end

function EntityPicker:editButton_Click(args)
    self:doWithPickedEntity(function(entity)
        guiManager:EmitEntityAction("edit", entity)
    end)
    self:removeMenu()
end


function EntityPicker:removeMenu()
    self.widget:hide()
    self.selectorWidget:hide()
end

function EntityPicker:input_MouseButtonReleased(button, mode)
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
    createConnector(world.EventGotAvatar):connect(function()
        entityPicker = { connectors = {}, menuWindow = nil, entityName = nil, useButtons = {}, entityId = nil, position = nil, buttons = {}, currentPickedEntityIndex = 0 }
        setmetatable(entityPicker, { __index = EntityPicker })

        entityPicker:buildWidget(world)
        connect(entityPicker.connectors, emberOgre.EventWorldDestroyed, function()
            entityPicker:shutdown()
            entityPicker = nil
        end)
    end)
end)

