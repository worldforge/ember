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
        end

        return dollSlot
    end

    function createAttachmentSlot(dollSlot, attachment)
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
        inventory = { connectors = {},
                      iconsize = 32,
                      icons = {},
                      slots = {},
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
