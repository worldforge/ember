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
        emberOgre:getWorld():getAvatar():getErisAvatar():take(entityIcon:getEntity())
    end)

    self.iconView:showEntityContents(avatarEntity)

    self.widget:enableCloseButton()

    self.helper = Ember.OgreView.Gui.EntityIconDragDropPreview:new(emberOgre:getWorld())
    --User has dragged an entityIcon from the inventory to the world
    self.DragDrop = Ember.OgreView.Gui.EntityIconDragDropTarget:new(root)
    local dragDrop_DraggedOver = function(entityIcon)
        if entityIcon ~= nil then
            if entityIcon:getImage() ~= nil then
                --alpha is already low when dragging, so 0.7 of an already reduced alpha
                entityIcon:getImage():setAlpha(0.7)
                self.helper:createPreview(entityIcon)
            end
        end
    end
    connect(self.connectors, self.DragDrop.EventIconEntered, dragDrop_DraggedOver)

    --User has dragged an entityIcon over the world, and onto another window
    local dragDrop_DragLeaves = function(entityIcon)
        if entityIcon ~= nil then
            if entityIcon:getEntity() ~= nil then
                entityIcon:getImage():setAlpha(1.0)
                self.helper:cleanupCreation()
            end
        end
    end
    connect(self.connectors, self.DragDrop.EventIconLeaves, dragDrop_DragLeaves)

    --Responds when preview model has been released on the world
    local dragDrop_Finalize = function(emberEntity)
        if emberEntity then
            local offset = self.helper:getDropOffset()
            local orientation = self.helper:getDropOrientation()
            emberOgre:getWorld():getAvatar():getErisAvatar():drop(emberEntity, offset, orientation)
        end
    end
    connect(self.connectors, self.helper.EventEntityFinalized, dragDrop_Finalize)

    self.widget:registerConsoleVisibilityToggleCommand("inventory")
    self.avatarEntity = avatarEntity
    self:setupDoll(avatarEntity)
    self.widget:show()

end

function Inventory:createAttachmentSlot(avatarEntity, dollSlot, attachment)
    -- 	self.doll.torso = self:createDollSlot("body", self.doll.image:getChild("Torso"), "Drop an entity here to attach it to the torso.")
    dollSlot.droppedHandler = function(entityIcon)
        if dollSlot.isValidDrop(entityIcon) then
            emberOgre:getWorld():getAvatar():getErisAvatar():wield(entityIcon:getEntity(), attachment)
            local icon = dollSlot.slot:getEntityIcon()
            if icon then
                self.iconView:addEntityIcon(icon)
            end
            dollSlot.slot:addEntityIcon(entityIcon)
        end
    end
    dollSlot.entityIconDropped_connector = createConnector(dollSlot.slot.EventIconDropped):connect(dollSlot.droppedHandler)
    dollSlot.observer = Ember.AttributeObserver:new_local(avatarEntity, dollSlot.attributePath, ".")
    dollSlot.attributeChanged = function(element)
        local result, entityId = Eris.Entity:extractEntityId(element, entityId)
        if result then
            local slotUpdateFunc = function(icon)
                --check that we've not already have added the icon to this slot
                if dollSlot.slot:getEntityIcon() ~= icon then
                    local oldIcon = dollSlot.slot:removeEntityIcon()
                    dollSlot.slot:addEntityIcon(icon)
                    if oldIcon then
                        self.iconView:addEntityIcon(oldIcon)
                    end
                end
            end

            ----Either we have created an icon for the entity yet, or we have to wait a little until it's available
            local icon = self.iconView:getEntityIcon(entityId)
            if icon then
                slotUpdateFunc(icon)
            else
                local delayedUpdater = function(entityIcon)
                    if entityIcon:getEntity():getId() == entityId then
                        slotUpdateFunc(entityIcon)
                        self.newEntityListeners[dollSlot.attributePath] = nil
                    end
                end
                self.newEntityListeners[dollSlot.attributePath] = delayedUpdater
            end
        end
    end
    dollSlot.attributeChanged_connector = createConnector(dollSlot.observer.EventChanged):connect(dollSlot.attributeChanged)

    dollSlot.iconDraggedOff = function()
        --do unwield stuff
        emberOgre:getWorld():getAvatar():getErisAvatar():wield(nil, attachment)
    end
    dollSlot.iconDraggedOff_connector = createConnector(dollSlot.slot.EventIconDraggedOff):connect(dollSlot.iconDraggedOff)

    dollSlot.observer:forceEvaluation()

end

function Inventory:setupDoll(avatarEntity)
    self.doll = {}
    self.doll.image = self.widget:getWindow("DollImage")
    self.doll.renderer = Ember.OgreView.Gui.ModelRenderer:new(self.doll.image, "doll")
    self.doll.renderer:setActive(false)

    self.doll.handPrimary = self:createDollSlot("attached_hand_primary", self.doll.image:getChild("RightHand"), "Drop an entity here to attach it to the primary hand.", "")
    self.doll.handPrimaryAttachmentSlot = self:createAttachmentSlot(avatarEntity, self.doll.handPrimary, "hand_primary")

    self.doll.handSecondary = self:createDollSlot("attached_hand_secondary", self.doll.image:getChild("LeftHand"), "Drop an entity here to attach it to the secondary hand.", "hand_secondary")
    self.doll.handSecondaryAttachmentSlot = self:createAttachmentSlot(avatarEntity, self.doll.handSecondary, "hand_secondary")

    self.doll.torso = self:createDollSlot("attached_torso", self.doll.image:getChild("Torso"), "Drop an entity here to attach it to the torso.", "torso")
    self.doll.torsoAttachmentSlot = self:createAttachmentSlot(avatarEntity, self.doll.torso, "torso")

    self.doll.back = self:createDollSlot("attached_back", self.doll.image:getChild("Back"), "Drop an entity here to attach it to the back.", "back")
    self.doll.backAttachmentSlot = self:createAttachmentSlot(avatarEntity, self.doll.back, "back")

    self.doll.head = self:createDollSlot("attached_head", self.doll.image:getChild("Head"), "Drop an entity here to attach it to the head.", "head")
    self.doll.headAttachmentSlot = self:createAttachmentSlot(avatarEntity, self.doll.head, "head")

    self.doll.legs = self:createDollSlot("attached_legs", self.doll.image:getChild("Legs"), "Drop an entity here to attach it to the legs.", "legs")
    self.doll.legsAttachmentSlot = self:createAttachmentSlot(avatarEntity, self.doll.legs, "legs")

    self.doll.feet = self:createDollSlot("attached_feet", self.doll.image:getChild("Feet"), "Drop an entity here to attach it to the feet.", "feet")
    self.doll.feetAttachmentSlot = self:createAttachmentSlot(avatarEntity, self.doll.feet, "feet")

    local representationUpdate = function()
        local model = Ember.OgreView.Model.ModelRepresentation:getModelForEntity(avatarEntity)
        if model ~= nil then
            self.doll.renderer:showModel(model:getDefinition())
            self.doll.renderer:setCameraDistance(0.75)
            self.doll.renderer:updateRender()
        end
    end

    representationUpdate()

    table.insert(self.connectors, createConnector(avatarEntity.EventChangedGraphicalRepresentation):connect(representationUpdate))

end

function Inventory:updateDoll()
end

function Inventory:createDollSlot(attributePath, containerWindow, tooltipText, wearRestriction)
    local dollSlot = {}
    dollSlot.slot = self.entityIconManager:createSlot(self.iconsize)
    dollSlot.container = containerWindow
    dollSlot.container:addChild(dollSlot.slot:getWindow())
    dollSlot.slot:getWindow():setInheritsTooltipText(true)
    dollSlot.container:setTooltipText(tooltipText)
    dollSlot.wearRestriction = wearRestriction
    dollSlot.attributePath = attributePath

    dollSlot.isValidDrop = function(entityIcon)
        if wearRestriction == "" then
            return true
        end
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

    dollSlot.entityIconDragStop = function(entityIcon)
        dollSlot.container:setProperty("FrameEnabled", "true")
    end

    dollSlot.entityIconDragStart_connector = createConnector(self.entityIconManager.EventIconDragStart):connect(dollSlot.entityIconDragStart)
    dollSlot.entityIconDragStop_connector = createConnector(self.entityIconManager.EventIconDragStop):connect(dollSlot.entityIconDragStop)

    dollSlot.shutdown = function()
        self.entityIconManager:destroySlot(dollSlot.slot)
        dollSlot.entityIconDragStart_connector:disconnect()
        dollSlot.entityIconDragStop_connector:disconnect()
    end

    return dollSlot
end

function Inventory:shutdown()
    disconnectAll(self.connectors)
    deleteSafe(self.helper)
    deleteSafe(self.DragDrop)
    deleteSafe(self.iconView)
    if self.doll then
        if deleteSafe(self.doll.renderer) then
            self.doll.handPrimary.shutdown()
            self.doll.torso.shutdown()
            self.doll.back.shutdown()
            self.doll.head.shutdown()
            self.doll.legs.shutdown()
            self.doll.feet.shutdown()
        end
    end

    guiManager:destroyWidget(self.widget)
end

Inventory.createdAvatarEntityConnector = createConnector(emberOgre.EventCreatedAvatarEntity):connect(function(avatarEntity)
    if emberOgre:getWorld():getAvatar():isAdmin() == false then
        inventory = { connectors = {},
                      useButtons = {},
                      iconsize = 32,
                      columns = 4,
                      iconcounter = 0,
                      slotcounter = 0,
                      icons = {},
                      slots = {},
                      menu = { menuShown = false, activeEntityWrapper = nil },
                      newEntityListeners = {} }
        setmetatable(inventory, { __index = Inventory })
        inventory:buildWidget(avatarEntity)
        connect(inventory.connectors, avatarEntity.BeingDeleted, function()
            inventory:shutdown()
            inventory = nil
        end
        )
    end
end
)
