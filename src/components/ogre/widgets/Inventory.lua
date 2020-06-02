-----------------------------------------


-----------------------------------------
Inventory = {}

function Inventory:buildWidget(avatarEntity)

    self.widget = guiManager:createWidget()

    --inventory has transitions, if we don't set this to false, every time it gets activated a
    --visible "alpha pop" would happen and then the transition would continue
    self.widget:setIsActiveWindowOpaque(false)

    self.widget:loadMainSheet("Inventory.layout", "Inventory")

    self.entityIconManager = guiManager:getEntityIconManager()

    self.iconContainer = self.widget:getWindow("IconContainer");
    self.iconView = Ember.OgreView.Gui.ContainerView:new(guiManager:getEntityIconManager(), guiManager:getIconManager(), guiManager:getEntityTooltip():getTooltipWindow(), self.iconContainer)

    connect(self.connectors, self.iconView.EventEntityPicked, function(entity)
        guiManager:EmitEntityAction("pick", entity)
    end)
    connect(self.connectors, self.iconView.EventIconAdded, function(entityIcon)
        for _, v in pairs(self.newEntityListeners) do
            v(entityIcon)
        end
    end)
    connect(self.connectors, self.iconView.EventIconDropped, function(entityIcon, entityIconSlot)
        local entity = entityIcon:getEntity()

        --If the parent isn't ourselves we should try to take the entity
        if entity:getLocation():getId() ~= avatarEntity:getId() then
            if Ember.Input:getSingleton():isKeyDown(224) then
                local amount = 1
                local amountProp = entity:valueOfProperty("amount")
                if amountProp:isInt() then
                    amount = amountProp:asInt()
                end
                emberOgre:getWorld():getAvatar():getErisAvatar():take(entity, amount)
            else
                emberOgre:getWorld():getAvatar():getErisAvatar():take(entity)
            end
        else
            --Check if the entity is attached, and if so detach it.
            local iconTag = entityIcon:getTag()
            if string.sub(iconTag, 1, 4) == "doll" then
                local attachment = string.sub(iconTag, 6)
                emberOgre:getWorld():getAvatar():getErisAvatar():wield(null, attachment)
            end
        end
    end)

    self.iconView:showEntityContents(avatarEntity)

    self.widget:enableCloseButton()

    self.helper = Ember.OgreView.Gui.EntityIconDragDropPreview:new(emberOgre:getWorld())
    --User has dragged an entityIcon from the inventory to the world
    self.DragDrop = Ember.OgreView.Gui.EntityIconDragDropTarget:new(root)
    local dragDrop_DraggedOver = function(entityIcon)
        if entityIcon then
            if entityIcon:getImage() then
                --alpha is already low when dragging, so 0.7 of an already reduced alpha
                entityIcon:getImage():setAlpha(0.7)
                self.helper:createPreview(entityIcon)
            end
        end
    end
    connect(self.connectors, self.DragDrop.EventIconEntered, dragDrop_DraggedOver)

    --User has dragged an entityIcon over the world, and onto another window
    local dragDrop_DragLeaves = function(entityIcon)
        if entityIcon then
            if entityIcon:getEntity() then
                entityIcon:getImage():setAlpha(1.0)
                self.helper:cleanupCreation()
            end
        end
    end
    connect(self.connectors, self.DragDrop.EventIconLeaves, dragDrop_DragLeaves)

    --Responds when preview model has been released on the world
    local dragDrop_Finalize = function(emberEntity)
        if emberEntity then
            local position = self.helper:getDropPosition()
            local orientation = self.helper:getDropOrientation()
            emberOgre:getWorld():getAvatar():getErisAvatar():place(emberEntity, emberOgre:getWorld():getAvatar():getErisAvatar():getEntity():getLocation(), position, orientation)
        end
    end
    connect(self.connectors, self.helper.EventEntityFinalized, dragDrop_Finalize)

    self.widget:registerConsoleVisibilityToggleCommand("inventory")
    self.avatarEntity = avatarEntity
    self:setupDoll(avatarEntity)
    self.widget:show()

end

function Inventory:setupDoll(avatarEntity)
    local doll = { slots = {} }
    doll.image = self.widget:getWindow("DollImage")
    doll.renderer = Ember.OgreView.Gui.ModelRenderer:new(doll.image, "doll")
    doll.renderer:setActive(false)

    function createDollSlot(attributePath, containerWindow, tooltipText, wearRestriction)
        local dollSlot = {}
        dollSlot.slot = self.entityIconManager:createSlot(self.iconsize)
        dollSlot.container = containerWindow
        dollSlot.container:addChild(dollSlot.slot:getWindow())
        dollSlot.slot:getWindow():setInheritsTooltipText(true)
        dollSlot.container:setTooltipText(tooltipText)
        dollSlot.wearRestriction = wearRestriction
        dollSlot.attributePath = attributePath

        dollSlot.isValidDrop = function(entityIcon)
            --TODO: look at the constraint and do client resolving to check if it's allowed
            if wearRestriction == "" then
                return true
            end
            --TODO: this should be replaced with checking the constraint at the client instead
            if entityIcon:getEntity():hasProperty("worn") then
                local wornElement = entityIcon:getEntity():valueOfProperty("worn")
                if wornElement:isString() then
                    local worn = wornElement:asString()
                    if worn == wearRestriction then
                        return true
                    end
                end
            end
            return false
        end

        dollSlot.entityIconDragStart = function(entityIcon)
            if dollSlot.isValidDrop(entityIcon) then
            else
                dollSlot.container:setProperty("FrameEnabled", "false")
            end
        end

        dollSlot.entityIconDragStop = function()
            dollSlot.container:setProperty("FrameEnabled", "true")
        end

        connect(self.connectors, self.entityIconManager.EventIconDragStart, dollSlot.entityIconDragStart)
        connect(self.connectors, self.entityIconManager.EventIconDragStop, dollSlot.entityIconDragStop)

        dollSlot.shutdown = function()
            self.entityIconManager:destroySlot(dollSlot.slot)
            if dollSlot.entityIcon then
                guiManager:getEntityIconManager():destroyIcon(dollSlot.entityIcon)
            end
            if dollSlot.connection_ChildAdded then
                dollSlot.connection_ChildAdded:disconnect()
            end
        end

        return dollSlot
    end

    function createAttachmentSlot(dollSlot, attachment)
        dollSlot.droppedHandler = function(entityIcon)
            if dollSlot.isValidDrop(entityIcon) then
                emberOgre:getWorld():getAvatar():getErisAvatar():wield(entityIcon:getEntity(), attachment)
            end
        end
        dollSlot.entityIconDropped_connector = createConnector(dollSlot.slot.EventIconDropped):connect(dollSlot.droppedHandler)
        dollSlot.observer = Ember.AttributeObserver:new_local(avatarEntity, dollSlot.attributePath, ".")
        local showAttachedIcon = function(entity)
            local icon = guiManager:getIconManager():getIcon(self.iconsize, entity)
            if dollSlot.entityIcon then
                dollSlot.slot:removeEntityIcon()
                guiManager:getEntityIconManager():destroyIcon(dollSlot.entityIcon)
            end
            dollSlot.entityIcon = guiManager:getEntityIconManager():createIcon(icon, entity)
            --Mark the entity icon so we know it's on the doll, later useful when dropping on inventory (for unwield).
            dollSlot.entityIcon:setTag("doll:" .. attachment)
            dollSlot.slot:addEntityIcon(dollSlot.entityIcon)
        end

        dollSlot.attributeChanged = function(element)
            local result, entityId = Eris.Entity:extractEntityId(element, entityId)
            if result and entityId then
                local attachedEntity = self.avatarEntity:getAttachedEntity("attached_" .. attachment)
                if attachedEntity then
                    showAttachedIcon(attachedEntity)
                else
                    dollSlot.connection_ChildAdded = createConnector(emberOgre:getWorld():getAvatar().EventAddedEntityToInventory):connect(function(entity)
                        if entity:getId() == entityId then
                            showAttachedIcon(entity)
                        end
                    end)
                end
            else
                local entityIcon = dollSlot.slot:removeEntityIcon()
                if entityIcon then
                    guiManager:getEntityIconManager():destroyIcon(entityIcon)
                    dollSlot.entityIcon = null
                end
            end
        end
        connect(self.connectors, dollSlot.observer.EventChanged, dollSlot.attributeChanged)

        dollSlot.observer:forceEvaluation()

    end

    function createSlot(attachment, windowName, tooltipText, wearRestriction)
        local slot = createDollSlot("attached_" .. attachment, doll.image:getChild(windowName), tooltipText, wearRestriction)
        table.insert(doll.slots, slot)
        createAttachmentSlot(slot, attachment)
    end

    createSlot("hand_primary", "RightHand", "Drop an entity here to attach it to the primary hand.", "")
    createSlot("hand_secondary", "LeftHand", "Drop an entity here to attach it to the secondary hand.", "hand_secondary")
    createSlot("torso", "Torso", "Drop an entity here to attach it to the torso.", "torso")
    createSlot("back", "Back", "Drop an entity here to attach it to the back.", "back")
    createSlot("head", "Head", "Drop an entity here to attach it to the head.", "head")
    createSlot("legs", "Legs", "Drop an entity here to attach it to the legs.", "legs")
    createSlot("feet", "Feet", "Drop an entity here to attach it to the feet.", "feet")

    local representationUpdate = function()
        local model = Ember.OgreView.Model.ModelRepresentation:getModelForEntity(avatarEntity)
        if model ~= nil then
            doll.renderer:showModel(model:getDefinition())
            doll.renderer:setCameraDistance(0.75)
            doll.renderer:updateRender()
        end
    end

    representationUpdate()

    connect(self.connectors, avatarEntity.EventChangedGraphicalRepresentation, representationUpdate)

    self.doll = doll
end

function Inventory:shutdown()
    disconnectAll(self.connectors)
    deleteSafe(self.helper)
    deleteSafe(self.DragDrop)
    deleteSafe(self.iconView)
    for _, value in pairs(self.doll.slots) do
        value:shutdown()
    end
    deleteSafe(self.doll.renderer)

    guiManager:destroyWidget(self.widget)
end

Inventory.createdAvatarEntityConnector = createConnector(emberOgre.EventCreatedAvatarEntity):connect(function(avatarEntity)
    if emberOgre:getWorld():getAvatar():isAdmin() == false then
        Inventory.instance = { connectors = {},
                                iconsize = 32,
                                icons = {},
                                slots = {},
                                newEntityListeners = {} }
        setmetatable(Inventory.instance, { __index = Inventory })
        Inventory.instance:buildWidget(avatarEntity)
    end
end
)

Inventory.destroyedConnector = createConnector(emberServices:getServerService().DestroyedAvatar):connect(function()
    if Inventory.instance then
        Inventory.instance:shutdown()
        Inventory.instance = nil
    end
end
)