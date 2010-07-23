-----------------------------------------


-----------------------------------------
Inventory = {}

function Inventory:AddedEntityToInventory(entity)
	local entityIconWrapper = self:createIcon(entity)
	if entityIconWrapper ~= nil then
		local slotWrapper = self:getFreeSlot()
		local slot = slotWrapper.slot
		slot:addEntityIcon(entityIconWrapper.entityIcon)
		local entityIconBucket = {}
		if self.icons[entity:getId()] == nil then
			self.icons[entity:getId()] = entityIconBucket
		else
			entityIconBucket = self.icons[entity:getId()]
		end
		table.insert(entityIconBucket, entityIconWrapper)
		for k,v in pairs(self.newEntityListeners) do
			v(entity)
		end
	end
end

function Inventory:RemovedEntityFromInventory(entity)
	local entityIconBucket = self.icons[entity:getId()]
	if entityIconBucket ~= nil then
		for k,v in pairs(entityIconBucket) do 
			local entityIconWrapper = v
			entityIconWrapper.entityIcon:setSlot(nil)
			--guiManager:getIconManager():destroyIcon(entityIconWrapper.entityIcon:getIcon())
			self.entityIconManager:destroyIcon(entityIconWrapper.entityIcon)
		end
	end
	self.icons[entity:getId()] = nil
end
	
function Inventory:getFreeSlot()
	--see if there's any free slots
	for k,v in pairs(self.slots) do 
		if v.slot:getEntityIcon() == nil then
			return v
		end
	end
	--if we couldn't find a free one, add one
	return self:addSlot()
end

function Inventory:addSlot()
	local yPosition = math.floor(self.slotcounter / self.columns)
	local xPosition = self.slotcounter - math.floor(self.slotcounter/self.columns)*self.columns  --lua 5.0 can't do modulus, in 5.1 we would have done: self.slotcounter % self.columns
	
	
	
	self.slotcounter = self.slotcounter + 1
	
	local slot = self.entityIconManager:createSlot(self.iconsize)
	slot:getWindow():setPosition(CEGUI.UVector2(CEGUI.UDim(0, self.iconsize * xPosition), CEGUI.UDim(0, self.iconsize * yPosition)))
	self.iconContainer:addChildWindow(slot:getWindow())
	local slotWrapper = {slot = slot}
	table.insert(self.slots, slotWrapper)
	slotWrapper.entityIconDropped = function(entityIcon)
		local oldSlot = entityIcon:getSlot()
		slotWrapper.slot:addEntityIcon(entityIcon)
		if oldSlot ~= nil then
			oldSlot:notifyIconDraggedOff(entityIcon)
		end
	end
	slotWrapper.entityIconDropped_connector = EmberOgre.LuaConnector:new_local(slot.EventIconDropped):connect(slotWrapper.entityIconDropped)
	
	return slotWrapper
end

function Inventory:showMenu(args, entityIconWrapper)
	self.menu.activeEntityWrapper = entityIconWrapper
	guiManager:getMainSheet():addChildWindow(self.menu.container)
	self.menu.menuShown = true
	self.menu.innercontainer:setYPosition(CEGUI.UDim(1, -(self.iconsize + self.menu.innercontainer:getHeight():asAbsolute(0))))
	self.menu.container:setHeight(CEGUI.UDim(0, self.iconsize + self.menu.innercontainer:getHeight():asAbsolute(0) + 10))
	local mousePos = CEGUI.MouseCursor:getSingleton():getPosition()
	local menuPos = CEGUI.UVector2(CEGUI.UDim(0, mousePos.x - (self.menu.container:getWidth():asAbsolute(0) * 0.5)), CEGUI.UDim(0, mousePos.y - self.menu.innercontainer:getHeight():asAbsolute(0)))
	self.menu.container:setPosition(menuPos)
	
	--only show the eat button if the entity has biomass (and thus is edible)
	if entityIconWrapper.entity:hasAttr("biomass") then
		self.menu.eatButton:setVisible(true)
	else
		self.menu.eatButton:setVisible(false)
	end
	

	--self.menu.container:setPosition()
end

-- function Inventory:input_MouseButtonReleased()
-- 	if self.menu.menuShown then
-- 		self.menu.container:setVisible(false)
-- 		self.menu.menuShown = false
-- 	end
-- end

function Inventory:createIcon(entity)
	local icon = guiManager:getIconManager():getIcon(self.iconsize, entity)
	
	if icon ~= nil then
		local name = entity:getType():getName() .. " (" .. entity:getId() .. " : " .. entity:getName() .. ")"
		local entityIconWrapper = {}
		entityIconWrapper.entityIcon = self.entityIconManager:createIcon(icon, entity, self.iconsize)
		entityIconWrapper.entityIcon:setTooltipText(name)
		entityIconWrapper.entity = entity
		entityIconWrapper.mouseEnters = function(args)
			entityIconWrapper.entityIcon:getImage():setProperty("FrameEnabled", "true")
			return true
		end
		entityIconWrapper.mouseLeaves = function(args)
			entityIconWrapper.entityIcon:getImage():setProperty("FrameEnabled", "false")
			return true
		end
		entityIconWrapper.mouseClick = function(args)
			self:showMenu(args, entityIconWrapper)
			return true
		end
		entityIconWrapper.entityIcon:getDragContainer():subscribeEvent("MouseClick", entityIconWrapper.mouseClick)
		entityIconWrapper.entityIcon:getDragContainer():subscribeEvent("MouseEnter", entityIconWrapper.mouseEnters)
		entityIconWrapper.entityIcon:getDragContainer():subscribeEvent("MouseLeave", entityIconWrapper.mouseLeaves)
		return entityIconWrapper
	else 
		return nil
	end
end

function Inventory:buildWidget(avatarEntity)
	
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("Inventory.layout", "Inventory/")
	
	self.entityIconManager = guiManager:getEntityIconManager()
	
	self.iconContainer = self.widget:getWindow("IconContainer");
	
	self.widget:enableCloseButton()
	
	self.menu.container = guiManager:createWindow("DefaultGUISheet")
	self.menu.container:setSize(CEGUI.UVector2(CEGUI.UDim(0, 50), CEGUI.UDim(0, 200)))
	self.menu.container:setClippedByParent(false)
	
	self.menu.innercontainer = guiManager:createWindow("DefaultGUISheet")
	self.menu.innercontainer:setSize(CEGUI.UVector2(CEGUI.UDim(0, 50), CEGUI.UDim(0, 200)))
	self.menu.innercontainer:setClippedByParent(false)
	self.menu.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(self.menu.innercontainer)
	self.menu.stackableContainer:setInnerContainerWindow(self.menu.innercontainer)
	self.menu.container:addChildWindow(self.menu.innercontainer)
	self.menu.innercontainer:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 10), CEGUI.UDim(1, -self.iconsize)))
	
	self.menu.hide = function()
		self.menu.container:getParent():removeChildWindow(self.menu.container)
		self.menu.menuShown = false
	end
	
	self.menu.mouseLeaves = function(args)
		if self.menu.menuShown then
			local window = CEGUI.System:getSingleton():getWindowContainingMouse()
			if window:isAncestor(self.menu.container) == false then
				self.menu.hide()
			end
		end
		return true
	end
	
	self.menu.container:subscribeEvent("MouseLeave", self.menu.mouseLeaves)
	
	
	--add default buttons
	
	self.menu.eatButton = guiManager:createWindow("EmberLook/Button")
	self.menu.eatButton:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(0, 25)))
	self.menu.eatButton:setText("eat")
	self.menu.eatButton_MouseClick = function(args)
		if self.menu.activeEntityWrapper ~= nil then
			if self.menu.activeEntityWrapper.entity ~= nil then
				emberServices:getServerService():eat(self.menu.activeEntityWrapper.entity)
			end
		end
		self.menu.hide()
		return true
	end
	self.menu.eatButton:subscribeEvent("Clicked", self.menu.eatButton_MouseClick)
	self.menu.innercontainer:addChildWindow(self.menu.eatButton)
	
	
	self.menu.dropButton = guiManager:createWindow("EmberLook/Button")
	self.menu.dropButton:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(0, 25)))
	self.menu.dropButton:setText("drop")
	self.menu.dropButton_MouseClick = function(args)
		if self.menu.activeEntityWrapper ~= nil then
			if self.menu.activeEntityWrapper.entity ~= nil then
				emberServices:getServerService():drop(self.menu.activeEntityWrapper.entity)
			end
		end
		self.menu.hide()
		return true
	end
	self.menu.dropButton:subscribeEvent("Clicked", self.menu.dropButton_MouseClick)
	self.menu.innercontainer:addChildWindow(self.menu.dropButton)
		
		
	self.menu.wieldButton = guiManager:createWindow("EmberLook/Button")
	self.menu.wieldButton:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(0, 25)))
	self.menu.wieldButton:setText("wield")
	self.menu.wieldButton_MouseClick = function(args)
		if self.menu.activeEntityWrapper ~= nil then
			if self.menu.activeEntityWrapper.entity ~= nil then
				emberServices:getServerService():wield(self.menu.activeEntityWrapper.entity)
			end
		end
		self.menu.hide()
		return true
	end
	self.menu.wieldButton:subscribeEvent("Clicked", self.menu.wieldButton_MouseClick)
	self.menu.innercontainer:addChildWindow(self.menu.wieldButton)
	
	self.menu.useButton = guiManager:createWindow("EmberLook/Button")
	self.menu.useButton:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(0, 25)))
	self.menu.useButton:setText("use")
	self.menu.useButton_MouseClick = function(args)
		if self.menu.activeEntityWrapper ~= nil then
			if self.menu.activeEntityWrapper.entity ~= nil then
				emberServices:getServerService():use(self.menu.activeEntityWrapper.entity)
			end
		end
		self.menu.hide()
		return true
	end
	self.menu.useButton:subscribeEvent("Clicked", self.menu.useButton_MouseClick)
	self.menu.innercontainer:addChildWindow(self.menu.useButton)
	

		
	
	self.menu.container:setVisible(true)
	
	
	connect(self.connectors, emberOgre:getAvatar().EventAddedEntityToInventory, self.AddedEntityToInventory, self)
	connect(self.connectors, emberOgre:getAvatar().EventRemovedEntityFromInventory, self.RemovedEntityFromInventory, self)
	
	self.widget:registerConsoleVisibilityToggleCommand("inventory")
	self.avatarEntity = avatarEntity
	self:setupDoll(avatarEntity)
	connect(self.connectors, avatarEntity.Changed, function(self, keys)
			self:updateDoll()
		end
	, self)
	self.widget:show()
		
-- 	connect(self.connectors, Ember.Input:getSingleton().EventMouseButtonReleased, self.input_MouseButtonReleased, self)
--	guiManager:getMainSheet():addChildWindow(self.menu.container)

	
end

function Inventory:createOutfitSlot(avatarEntity, dollSlot, outfitPartName)
-- 	self.doll.torso = self:createDollSlot("body", self.widget:getWindow("Doll/Torso"), "Drop an entity here to attach it to the torso.")
	dollSlot.droppedHandler = function(entityIcon)
		if dollSlot.isValidDrop(entityIcon) then
			emberServices:getServerService():wield(entityIcon:getEntity(), dollSlot.wearRestriction)
			local icon = dollSlot.slot:getEntityIcon()
			if icon ~= null then
				local slot = self:getFreeSlot()
				slot:addEntityIcon(icon)
			end
			dollSlot.slot:addEntityIcon(entityIcon)
		end
	end
	dollSlot.entityIconDropped_connector = EmberOgre.LuaConnector:new_local(dollSlot.slot.EventIconDropped):connect(dollSlot.droppedHandler)
	dollSlot.observer = Ember.AttributeObserver:new_local(avatarEntity, dollSlot.attributePath, ".")
	dollSlot.attributeChanged = function(element)
		if element:isString() then
			local entityId = element:asString()
			local slotUpdateFunc = function()
				local entityBucket = self.icons[entityId]

				if entityBucket ~= nil then
					local icon = entityBucket[1].entityIcon
					if icon ~= nil then
						--check that we've not already have added the icon to this slot
						if dollSlot.slot:getEntityIcon() ~= icon then
							local oldIcon = dollSlot.slot:removeEntityIcon()
							dollSlot.slot:addEntityIcon(icon)
							if oldIcon ~= nil then
								local slotWrapper = self:getFreeSlot()
								local slot = slotWrapper.slot
								slot:addEntityIcon(oldIcon)
							end
						end
					end
				end
			end

			--Either we have created an icon for the entity yet, or we have to wait a little until it's available			
			local entityBucket = self.icons[entityId]
			if entityBucket ~= nil then
				slotUpdateFunc()
			else
				local delayedUpdater = function(newEntity)
					if newEntity:getId() == entityId then
						slotUpdateFunc()
						self.newEntityListeners[dollSlot.attributePath] = nil
					end
				end
				self.newEntityListeners[dollSlot.attributePath] = delayedUpdater
			end
		end
	end
	dollSlot.attributeChanged_connector = EmberOgre.LuaConnector:new_local(dollSlot.observer.EventChanged):connect(dollSlot.attributeChanged)
	
	dollSlot.iconDraggedOff = function(entityIcon)
		--do unwield stuff
	end
	dollSlot.iconDraggedOff_connector = EmberOgre.LuaConnector:new_local(dollSlot.slot.EventIconDraggedOff):connect(dollSlot.iconDraggedOff)
	
	dollSlot.observer:forceEvaluation()
	
--	dollSlot.newEntityCreated = function(newEntity)
--		if avatarEntity:hasAttr("outfit") then
--			dollSlot.attributeChanged(avatarEntity:valueOfAttr("outfit"))
--		end
--	end
	
--	table.insert(self.newEntityListeners, dollSlot.newEntityCreated)
	
-- 	dollSlot.attributeChanged(avatarEntity:valueOfAttr("outfit"))
end

function Inventory:setupDoll(avatarEntity)
	self.doll = {}
	local model = EmberOgre.Model.ModelRepresentationManager:getSingleton():getModelForEntity(avatarEntity)
	if model ~= nil then
		self.doll.image = self.widget:getWindow("DollImage")
		self.doll.renderer = EmberOgre.Gui.ModelRenderer:new(self.doll.image)
		self.doll.renderer:setActive(false)
		self.doll.renderer:setIsInputCatchingAllowed(false)
		
		self.doll.renderer:showModel(model:getDefinition():get():getName())
		self.doll.renderer:setCameraDistance(0.75)
		self.doll.renderer:updateRender()
				
		self.doll.rightHand = self:createDollSlot("right_hand_wield", self.widget:getWindow("Doll/RightHand"), "Drop an entity here to attach it to the right hand.", "")
		self.doll.rightHandOutfitSlot = self:createOutfitSlot(avatarEntity, self.doll.rightHand, "")
		
		self.doll.torso = self:createDollSlot("outfit.body", self.widget:getWindow("Doll/Torso"), "Drop an entity here to attach it to the torso.", "body")
		self.doll.torsoOutfitSlot = self:createOutfitSlot(avatarEntity, self.doll.torso, "body")
		
		self.doll.back = self:createDollSlot("outfit.back", self.widget:getWindow("Doll/Back"), "Drop an entity here to attach it to the back.", "back")
		self.doll.backOutfitSlot = self:createOutfitSlot(avatarEntity, self.doll.back, "back")

		self.doll.head = self:createDollSlot("outfit.head", self.widget:getWindow("Doll/Head"), "Drop an entity here to attach it to the head.", "head")
		self.doll.headOutfitSlot = self:createOutfitSlot(avatarEntity, self.doll.head, "head")
	
		self.doll.legs = self:createDollSlot("outfit.legs", self.widget:getWindow("Doll/Legs"), "Drop an entity here to attach it to the legs.", "legs")
		self.doll.legsOutfitSlot = self:createOutfitSlot(avatarEntity, self.doll.legs, "legs")
		
		self.doll.feet = self:createDollSlot("outfit.feet", self.widget:getWindow("Doll/Feet"), "Drop an entity here to attach it to the feet.", "feet")
		self.doll.feetOutfitSlot = self:createOutfitSlot(avatarEntity, self.doll.feet, "feet")
	end
end

function Inventory:updateDoll()
	if self.avatarEntity:hasAttr("right_hand_wield") then
		
	end
end


function Inventory:createDollSlot(attributePath, containerWindow, tooltipText, wearRestriction)
	local dollSlot = {}
	dollSlot.slot = self.entityIconManager:createSlot(self.iconsize)
	dollSlot.container = containerWindow
	dollSlot.container:addChildWindow(dollSlot.slot:getWindow())
	dollSlot.slot:getWindow():setInheritsTooltipText(true)
	dollSlot.container:setTooltipText(tooltipText)
	dollSlot.wearRestriction = wearRestriction
	dollSlot.attributePath = attributePath
	dollSlot.allowAny = allowAny
	
	dollSlot.isValidDrop = function(entityIcon)
		if dollSlot.wearRestriction == "" or dollSlot.allowAny then
			return true
		end
		if entityIcon:getEntity():hasAttr("worn") then
			local wornElement = entityIcon:getEntity():valueOfAttr("worn")
			if wornElement:isString() then
				local worn = wornElement:asString()
				if worn == dollSlot.wearRestriction then
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
	
	dollSlot.entityIconDragStart_connector = EmberOgre.LuaConnector:new_local(self.entityIconManager.EventIconDragStart):connect(dollSlot.entityIconDragStart)
	dollSlot.entityIconDragStop_connector = EmberOgre.LuaConnector:new_local(self.entityIconManager.EventIconDragStop):connect(dollSlot.entityIconDragStop)

	dollSlot.shutdown = function()
		self.entityIconManager:destroySlot(dollSlot.slot)
		dollSlot.entityIconDragStart_connector:disconnect()
		dollSlot.entityIconDragStop_connector:disconnect()
	end

	return dollSlot
end

function Inventory:shutdown()
	disconnectAll(self.connectors)
	if self.doll ~= nil then
		if deleteSafe(self.doll.renderer) then
			self.doll.rightHand.shutdown()
			self.doll.torso.shutdown()
			self.doll.back.shutdown()
			self.doll.head.shutdown()
			self.doll.legs.shutdown()
			self.doll.feet.shutdown()
		end
	end
	for k,v in pairs(self.slots) do
		self.entityIconManager:destroySlot(v.slot)
	end
	for k,v in pairs(self.icons) do
		local bucket = v
		for k,v in pairs(bucket) do
			self.entityIconManager:destroyIcon(v.entityIcon)
		end
	end
	
	windowManager:destroyWindow(self.menu.container)
	guiManager:destroyWidget(self.widget)
end

Inventory.createdAvatarEntityConnector = EmberOgre.LuaConnector:new_local(emberOgre.EventCreatedAvatarEntity):connect(function(avatarEntity)
		if emberOgre:getAvatar():isAdmin() == false then
			inventory = {connectors={},
				iconsize = 32,
				columns = 4,
				iconcounter = 0,
				slotcounter = 0,
				icons = {},
				slots = {},
				menu = {menuShown = false, activeEntityWrapper = nil},
				newEntityListeners = {}}
			setmetatable(inventory, {__index = Inventory})
			inventory:buildWidget(avatarEntity)
			connect(inventory.connectors, avatarEntity.BeingDeleted, function()
					inventory:shutdown()
					inventory = nil
				end
			)
		end
	end
)
