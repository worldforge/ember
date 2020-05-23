-----------------------------------------
-- Widget which is shown when an entity is picked. Basically it's a menu which allows the user to perform actions on the entity.


--TODO: 
-- * perhaps implement it as a pie menu with some nice icons?
-----------------------------------------


EntityPicker = {}

function EntityPicker:buildWidget(world)

    self.world = world
    self.widget = guiManager:createWidget()

    local entityPickListener = world:getEntityPickListener()
    connect(self.connectors, entityPickListener.EventPickedEntity, self.pickedEntity, self)
    --createConnector(mousePicker.EventPickedNothing):connect(self.pickedNothing, self)

    self.widget:loadMainSheet("EntityPicker.layout", "EntityPicker")
    self.selectorWidget = self.widget:getWindow("Selector")

    self.widget:getWindow("PreviousButton"):subscribeEvent("MouseEntersSurface", self.previousButton_MouseEnters, self)
    self.widget:getWindow("NextButton"):subscribeEvent("MouseEntersSurface", self.nextButton_MouseEnters, self)

    self.menuWindow = CEGUI.toLayoutContainer(self.widget:getWindow("Menu"))
    self.entityName = self.widget:getWindow("EntityName")

    --Check whether we should show the inspect button even for non admin types.
    local configService = emberServices:getConfigService()
    local evaluateShowInspect = function()
        if world:getAvatar():isAdmin() then
            self.showInspect = true
        else
            local variable = configService:getValue("authoring", "showinspectforall")
            if variable and variable:is_bool() then
                self.showInspect = variable[".bool"](variable);
            else
                self.showInspect = false
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

    --Remove menu when mouse leaves it
    self.widget:getMainWindow():subscribeEvent("MouseLeavesArea", function()
        self:removeMenu()
        return true
    end)

    connect(self.connectors, guiManager.EventEntityAction, self.handleAction, self)

    -- Start with 20 buttons
    for _ = 1, 20 do
        self:createButton()
    end

end

function EntityPicker:createButton()
    local button = guiManager:createWindow("EmberLook/Button")
    button:setWidth(CEGUI.UDim(1, 0))
    local wrapper = { button = button, clickedHandler = nil }
    button:subscribeEvent("Clicked", function()
        if wrapper.clickedHandler then
            wrapper.clickedHandler()
        end
    end, self)
    self.menuWindow:addChild(wrapper.button)

    table.insert(self.buttons, wrapper)
end

function EntityPicker:showMenu(position)
    self.widget:show()

    self.menuWindow:layout()
    local menuSize = self.menuWindow:getPixelSize()

    local newHeight = CEGUI.UDim(0.0, menuSize.height + self.entityName:getPixelSize().height + self.selectorWidget:getPixelSize().height)
    self.widget:getMainWindow():setHeight(newHeight)

    local localPosition = CEGUI.Vector2f:new_local(position.x, position.y)

    localPosition.x = localPosition.x - self.widget:getMainWindow():getPixelSize().width * 0.5
    localPosition.y = localPosition.y - self.selectorWidget:getPixelSize().height - (self.entityName:getPixelSize().height * 0.5)

    --Make sure the menu is fully contained within the main window
    if localPosition.x < 0 then
        localPosition.x = 0
    end
    if localPosition.y < 0 then
        localPosition.y = 0
    end
    local width = self.widget:getMainWindow():getPixelSize().width
    local height = self.widget:getMainWindow():getPixelSize().height

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

function EntityPicker:previousButton_MouseEnters()
    self.currentPickedEntityIndex = self.currentPickedEntityIndex - 1
    self:updateSelector()
    self:pickedOneEntity(self.pickedEntities[self.currentPickedEntityIndex])
    return true
end

function EntityPicker:nextButton_MouseEnters()
    self.currentPickedEntityIndex = self.currentPickedEntityIndex + 1
    self:updateSelector()
    self:pickedOneEntity(self.pickedEntities[self.currentPickedEntityIndex])
    return true
end

function EntityPicker:updateSelector()
    local numberingWidget = self.widget:getWindow("Numbering")
    numberingWidget:setText((self.currentPickedEntityIndex + 1) .. "/" .. (#self.pickedEntities + 1))
    local previousWidget = self.widget:getWindow("PreviousButton")
    local nextWidget = self.widget:getWindow("NextButton")

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

function EntityPicker:handleAction(action, entity)

    -- Some other script has picked an entity (like the script which handles inventory).
    if action == "pick" then
        local mousePosition = CEGUI.System:getSingleton():getDefaultGUIContext():getMouseCursor():getPosition()

        self.pickedPoint = mousePosition

        self.pickedEntities = {}
        self.currentPickedEntityIndex = 0

        -- set an arbitrary position
        local result = { entityId = entity:getId(), position = Ogre.Vector3:new_local(0, 0, 0) }
        self:pickedOneEntity(result)

    end
end

--called when an entity has been picked
function EntityPicker:pickedEntity(results, args)

    if args.pickType == Ember.OgreView.MPT_PRESSED then
        -- initiate moving of entity
        if results:size() > 0 then
            guiManager:EmitEntityAction("move", results[0].entity)
        end
    elseif args.pickType == Ember.OgreView.MPT_CLICK then
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
            self.selectorWidget:show()
            self:updateSelector()
        else
            self.selectorWidget:hide()
        end

        self:pickedOneEntity(self.pickedEntities[0])
    end
end

function isEntityContainedByOther(entity, other)

    local parent = entity:getLocation()
    while parent do
        if parent == other then
            return true
        end
        parent = parent:getLocation()
    end

    return false
end

--called when an entity has been picked
function EntityPicker:pickedOneEntity(pickedResult)

    if pickedResult ~= nil then
        emberOgre:doWithEntity(pickedResult.entityId, function(entity)
            self.entityId = entity:getId()
            --we must make a copy, else the vector object will be deleted by C++ and we'll end up with garbage
            self.position = Ogre.Vector3:new_local(pickedResult.position)

            --first hide all buttons
            for _, v in pairs(self.activeButtons) do
                v:setHeight(CEGUI.UDim(0, 0))
            end

            --As they are hidden they are all now "inactive"
            self.activeButtons = {}


            --Entities that themselves have physical domains shouldn't be moved.
            local isPhysicalDomain = false
            if entity:hasProperty("domain") then
                local domainElement = entity:valueOfProperty("domain")
                if domainElement:isString() and domainElement:asString() == "physical" then
                    isPhysicalDomain = true
                end
            end

            local selfEntity = self.world:getAvatar():getEmberEntity()
            local isInInventory = isEntityContainedByOther(entity, selfEntity)

            --We only show certain buttons for entities that aren't part of the avatar's inventory.
            if not isInInventory then
                if entity ~= selfEntity then
                    self:showButton("Move to", "Move to this point.", function()
                        self.world:getMovementController():moveToPoint(self.position)
                        self:removeMenu()
                    end)
                    if self.world:getAvatar():isAdmin() then
                        self:showButton("Teleport to", "Teleport to this point.", function()
                            self:doWithPickedEntity(function(pickedEntity)
                                self.world:getMovementController():teleportTo(self.position, pickedEntity)
                            end)
                            self:removeMenu()
                        end)
                    end
                    if (not isPhysicalDomain) then
                        self:showButton("Pick up", "Pick up the entity.", function()
                            self:doWithPickedEntity(function(pickedEntity)
                                self.world:getAvatar():getErisAvatar():place(pickedEntity, self.world:getAvatar():getErisAvatar():getEntity())
                                guiManager:EmitEntityAction("take", pickedEntity)
                            end)
                            self:removeMenu()
                        end)
                        self:showButton("Move", "Move the entity.", function()
                            self:doWithPickedEntity(function(pickedEntity)
                                guiManager:EmitEntityAction("move", pickedEntity)
                            end)
                            self:removeMenu()
                        end)
                        -- We'll check if the entity can be addressed. Currently we do this by checking for a "mind" property; we might look into if this is enough
                        if entity:hasProperty("mind") then
                            self:showButton("Talk to", "Talk with the entity.", function()
                                self:doWithPickedEntity(function(pickedEntity)
                                    guiManager:EmitEntityAction("talk", pickedEntity)
                                end)
                                self:removeMenu()
                            end)
                        end
                    end
                end
            end

            if self.showInspect then
                self:showButton("Inspect", "Inspect the entity.", function()
                    self:doWithPickedEntity(function(pickedEntity)
                        guiManager:EmitEntityAction("inspect", pickedEntity)
                    end)
                    self:removeMenu()
                end)
            end

            if self.world:getAvatar():isAdmin() then
                self:showButton("Edit", "Edit the entity.", function()
                    self:doWithPickedEntity(function(pickedEntity)
                        guiManager:EmitEntityAction("edit", pickedEntity)
                    end)
                    self:removeMenu()
                end)
            end

            self:checkUse(entity)
            self:showMenu(self.pickedPoint)
            self.menuWindow:invalidate(true)
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

function EntityPicker:showButton(text, tooltip, clickFn)
    local index = table.getn(self.activeButtons)
    if index == table.getn(self.buttons) then
        --We can't show too many buttons, that would just look strange.
        log.warning("Too many buttons to show for entity.")
    else
        local buttonWrapper = self.buttons[index + 1]
        buttonWrapper.clickedHandler = clickFn
        buttonWrapper.button:setText(text)
        buttonWrapper.button:setTooltipText(tooltip)
        local verticalExtent = Ember.Cegui.Helper:calculateRenderedCentredStringVerticalExtent(buttonWrapper.button)
        buttonWrapper.button:setHeight(CEGUI.UDim(0, verticalExtent + 8))
        table.insert(self.activeButtons, buttonWrapper.button)
    end
end

function EntityPicker:checkUse(entity)
    --try to find the default operation for the wielded entity

    --first fill up with actions defined for the entity being picked
    local actionList = entity:getUsages();
    if actionList:size() > 0 then
        for i = 0, actionList:size() - 1 do
            local action = actionList[i]
            local usage = entity:getUsage(action)
            self:addAction(entity:getId(), action, usage)
        end
    end

    --then fill up with operations that can be performed with the currently wielded entity
    local wieldedEntity = self.world:getAvatar():getEmberEntity():getAttachedEntity("attached_hand_primary")
    if wieldedEntity then
        local operatorList = wieldedEntity:getUsages();
        if operatorList:size() > 0 then
            for i = 0, operatorList:size() - 1 do
                local defaultOp = operatorList[i]
                local usage = wieldedEntity:getUsage(defaultOp)
                self:addUse(entity:getId(), wieldedEntity, defaultOp, usage)
            end
        end
    else
        --else, if nothing is wielded, check if there are any usages on the avatar itself
        local operatorList = self.world:getAvatar():getEmberEntity():getUsagesProtected();
        if operatorList:size() > 0 then
            for i = 0, operatorList:size() - 1 do
                local defaultOp = operatorList[i]
                local usage = self.world:getAvatar():getEmberEntity():getUsageProtected(defaultOp)
                self:addUseSelf(entity:getId(), self.world:getAvatar():getEmberEntity(), defaultOp, usage)
            end
        end
    end
end

function EntityPicker:addUseSelf(entityId, wieldedEntity, operation, usage)
    local clickedHandler = function()
        local entity = self.world:getEmberEntity(entityId)
        if entity then
            self.world:getAvatar():useTool(wieldedEntity, operation, entity, Ember.OgreView.Convert:toWF_Point3(self.position))
            guiManager:EmitEntityAction("use:"..operation, entity)
        end
        self:removeMenu()
    end

    self:showButton(usage.name, usage.description, clickedHandler)
end

function EntityPicker:addUse(entityId, wieldedEntity, operation, usage)
    local clickedHandler = function()
        local entity = self.world:getEmberEntity(entityId)
        if entity then
            self.world:getAvatar():useTool(wieldedEntity, operation, entity, Ember.OgreView.Convert:toWF_Point3(self.position))
            --			emberServices:getServerService():use(wieldedEntity, entity, Ember.OgreView.Convert:toWF_Point3(self.position), operation)
            guiManager:EmitEntityAction("use:"..operation, entity)
        end
        self:removeMenu()
    end

    self:showButton(usage.name .. " with " .. entityNameOrType(wieldedEntity), usage.description, clickedHandler)
end

function entityNameOrType(entity)
    if entity:getName() ~= "" then
        return entity:getName()
    end
    return entity:getType():getName()
end

function EntityPicker:addAction(entityId, action, usage)
    local clickedHandler = function()
        local entity = self.world:getEmberEntity(entityId)
        if entity ~= nil then
            self.world:getAvatar():useTool(entity, action)
            guiManager:EmitEntityAction("use:"..action, entity)
        end
        self:removeMenu()
    end

    self:showButton(usage.name, usage.description, clickedHandler)

end

--Tries to find the selected entity (it might have disappeared from the world in the span of clicking on it and selecting an action) and if it can be found it will call the supplied function with the entity as the first argument.
--This allows you to easily specify functions to call when there is a selected entity. If no entity can be found nothing will happen.
function EntityPicker:doWithPickedEntity(aFunction)
    emberOgre:doWithEntity(self.entityId, aFunction)
end

function EntityPicker:removeMenu()
    self.widget:hide()
    self.selectorWidget:hide()
end

function EntityPicker:shutdown()
    disconnectAll(self.connectors)
    guiManager:destroyWidget(self.widget)
end

connect(connectors, emberOgre.EventWorldCreated, function(world)
    createConnector(world.EventGotAvatar):connect(function()
        entityPicker = { connectors = {},
                         buttons = {},
                         activeButtons = {},
                         menuWindow = nil,
                         entityName = nil,
                         world = world,
                         entityId = nil,
                         position = nil,
                         currentPickedEntityIndex = 0 }
        setmetatable(entityPicker, { __index = EntityPicker })

        entityPicker:buildWidget(world)
        connect(entityPicker.connectors, emberOgre.EventWorldDestroyed, function()
            entityPicker:shutdown()
            entityPicker = nil
        end)
    end)
end)

