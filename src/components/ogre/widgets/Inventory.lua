-----------------------------------------


-----------------------------------------
Inventory = {connectors={}, iconsize = 32, columns = 4, iconcounter = 0, slotcounter = 0, icons = {}, slots = {}, menu = {menuShown = false, activeEntityWrapper = nil}}

function Inventory.CreatedAvatarEntity(avatarEntity)
	
	connect(Inventory.connectors, emberOgre:getAvatar().EventAddedEntityToInventory, "Inventory.AddedEntityToInventory")
	connect(Inventory.connectors, emberOgre:getAvatar().EventRemovedEntityFromInventory, "Inventory.RemovedEntityFromInventory")
	
	Inventory.widget:registerConsoleVisibilityToggleCommand("inventory")
	if avatarEntity:getAvatar():isAdmin() == false then
		Inventory.setupDoll(avatarEntity)
		Inventory.widget:show()
	end
end

function Inventory.AddedEntityToInventory(entity)
	local entityIconWrapper = Inventory.createIcon(entity)
	if entityIconWrapper ~= nil then
		local slotWrapper = Inventory.getFreeSlot()
		local slot = slotWrapper.slot
		slot:addEntityIcon(entityIconWrapper.entityIcon)
		local entityIconBucket = {}
		if Inventory.icons[entity:getId()] == nil then
			Inventory.icons[entity:getId()] = entityIconBucket
		else
			entityIconBucket = Inventory.icons[entity:getId()]
		end
		table.insert(entityIconBucket, entityIconWrapper)
	end
end

function Inventory.RemovedEntityFromInventory(entity)
	local entityIconBucket = Inventory.icons[entity:getId()]
	if entityIconBucket ~= nil then
		for k,v in pairs(entityIconBucket) do 
			local entityIconWrapper = v
			entityIconWrapper.entityIcon:setSlot(nil)
			--guiManager:getIconManager():destroyIcon(entityIconWrapper.entityIcon:getIcon())
			Inventory.entityIconManager:destroyIcon(entityIconWrapper.entityIcon)
		end
	end
	Inventory.icons[entity:getId()] = nil
end
	
function Inventory.getFreeSlot()
	--see if there's any free slots
	for k,v in pairs(Inventory.slots) do 
		if v.slot:getEntityIcon() == nil then
			return v
		end
	end
	--if we couldn't find a free one, add one
	return Inventory.addSlot()
end

function Inventory.addSlot()
	local yPosition = math.floor(Inventory.slotcounter / Inventory.columns)
	local xPosition = Inventory.slotcounter % Inventory.columns
	
	Inventory.slotcounter = Inventory.slotcounter + 1
	
	local slot = Inventory.entityIconManager:createSlot(Inventory.iconsize)
	slot:getWindow():setPosition(CEGUI.UVector2(CEGUI.UDim(0, Inventory.iconsize * xPosition), CEGUI.UDim(0, Inventory.iconsize * yPosition)))
	Inventory.iconContainer:addChildWindow(slot:getWindow())
	local slotWrapper = {slot = slot}
	table.insert(Inventory.slots, slotWrapper)
	slotWrapper.entityIconDropped = function(entityIcon)
		slotWrapper.slot:addEntityIcon(entityIcon)
	end
	slotWrapper.entityIconDropped_connector = EmberOgre.LuaConnector:new_local(slot.EventIconDropped):connect(slotWrapper.entityIconDropped)
	
	return slotWrapper
end

function Inventory.showMenu(args, entityIconWrapper)
	Inventory.menu.activeEntityWrapper = entityIconWrapper
	entityIconWrapper.entityIcon:getDragContainer():addChildWindow(Inventory.menu.container)
	Inventory.menu.menuShown = true
	Inventory.menu.container:setXPosition(CEGUI.UDim(0.5, -Inventory.menu.container:getWidth():asAbsolute(0) * 0.5))
	Inventory.menu.innercontainer:setYPosition(CEGUI.UDim(1, -(Inventory.iconsize + Inventory.menu.innercontainer:getHeight():asAbsolute(0))))
	Inventory.menu.container:setHeight(CEGUI.UDim(0, Inventory.iconsize + Inventory.menu.innercontainer:getHeight():asAbsolute(0) + 10))
	Inventory.menu.container:setYPosition(CEGUI.UDim(1, -Inventory.menu.container:getHeight():asAbsolute(0)))

	--Inventory.menu.container:setPosition()
end

-- function Inventory.input_MouseButtonReleased()
-- 	if Inventory.menu.menuShown then
-- 		Inventory.menu.container:setVisible(false)
-- 		Inventory.menu.menuShown = false
-- 	end
-- end

function Inventory.createIcon(entity)
	local icon = guiManager:getIconManager():getIcon(Inventory.iconsize, entity:getType())
	
	if icon ~= nil then
		local name = entity:getType():getName() .. " (" .. entity:getId() .. " : " .. entity:getName() .. ")"
		local entityIconWrapper = {}
		entityIconWrapper.entityIcon = Inventory.entityIconManager:createIcon(icon, entity, Inventory.iconsize)
		entityIconWrapper.entityIcon:setTooltipText(name)
		entityIconWrapper.entity = entity
		entityIconWrapper.mouseEnters = function(args)
			entityIconWrapper.entityIcon:getImage():setProperty("FrameEnabled", "true")
		end
		entityIconWrapper.mouseLeaves = function(args)
			entityIconWrapper.entityIcon:getImage():setProperty("FrameEnabled", "false")
		end
		entityIconWrapper.mouseClick = function(args)
			Inventory.showMenu(args, entityIconWrapper)
		end
		entityIconWrapper.entityIcon:getDragContainer():subscribeEvent("MouseClick", entityIconWrapper.mouseClick)
		entityIconWrapper.entityIcon:getDragContainer():subscribeEvent("MouseEnter", entityIconWrapper.mouseEnters)
		entityIconWrapper.entityIcon:getDragContainer():subscribeEvent("MouseLeave", entityIconWrapper.mouseLeaves)
		return entityIconWrapper
	else 
		return nil
	end
end

function Inventory.buildWidget()
	
	Inventory.widget = guiManager:createWidget()
	Inventory.widget:loadMainSheet("Inventory.layout", "Inventory/")
	
	Inventory.entityIconManager = guiManager:getEntityIconManager()
	
	Inventory.iconContainer = Inventory.widget:getWindow("IconContainer");
	
	Inventory.widget:enableCloseButton()
	Inventory.widget:hide()
	
	connect(Inventory.connectors, emberOgre.EventCreatedAvatarEntity, "Inventory.CreatedAvatarEntity")
	
	Inventory.menu.container = guiManager:createWindow("DefaultGUISheet")
	Inventory.menu.container:setSize(CEGUI.UVector2(CEGUI.UDim(0, 100), CEGUI.UDim(0, 200)))
	Inventory.menu.container:setClippedByParent(false)
	
	Inventory.menu.innercontainer = guiManager:createWindow("DefaultGUISheet")
	Inventory.menu.innercontainer:setSize(CEGUI.UVector2(CEGUI.UDim(0, 80), CEGUI.UDim(0, 200)))
	Inventory.menu.innercontainer:setClippedByParent(false)
	Inventory.menu.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(Inventory.menu.innercontainer)
	Inventory.menu.stackableContainer:setInnerContainerWindow(Inventory.menu.innercontainer)
	Inventory.menu.container:addChildWindow(Inventory.menu.innercontainer)
	Inventory.menu.innercontainer:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 10), CEGUI.UDim(1, -Inventory.iconsize)))
	
	Inventory.menu.hide = function()
		Inventory.menu.container:getParent():removeChildWindow(Inventory.menu.container)
		Inventory.menu.menuShown = false
	end
	
	Inventory.menu.mouseLeaves = function(args)
		if Inventory.menu.menuShown then
			local window = CEGUI.System:getSingleton():getWindowContainingMouse()
			if window:isAncestor(Inventory.menu.container) == false then
				Inventory.menu.hide()
			end
		end
	end
	
	Inventory.menu.container:subscribeEvent("MouseLeave", Inventory.menu.mouseLeaves)
	
	
	--add default buttons
	
	Inventory.menu.dropButton = guiManager:createWindow("EmberLook/Button")
	Inventory.menu.dropButton:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(0, 25)))
	Inventory.menu.dropButton:setText("drop")
	Inventory.menu.dropButton_MouseClick = function(args)
		if Inventory.menu.activeEntityWrapper ~= nil then
			if Inventory.menu.activeEntityWrapper.entity ~= nil then
				emberServices:getServerService():drop(Inventory.menu.activeEntityWrapper.entity)
			end
		end
		Inventory.menu.hide()
	end
	Inventory.menu.dropButton:subscribeEvent("MouseClick", Inventory.menu.dropButton_MouseClick)
	Inventory.menu.innercontainer:addChildWindow(Inventory.menu.dropButton)
		
		
	Inventory.menu.wieldButton = guiManager:createWindow("EmberLook/Button")
	Inventory.menu.wieldButton:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(0, 25)))
	Inventory.menu.wieldButton:setText("wield")
	Inventory.menu.wieldButton_MouseClick = function(args)
		if Inventory.menu.activeEntityWrapper ~= nil then
			if Inventory.menu.activeEntityWrapper.entity ~= nil then
				emberServices:getServerService():wield(Inventory.menu.activeEntityWrapper.entity)
			end
		end
		Inventory.menu.hide()
	end
	Inventory.menu.wieldButton:subscribeEvent("MouseClick", Inventory.menu.wieldButton_MouseClick)
	Inventory.menu.innercontainer:addChildWindow(Inventory.menu.wieldButton)
	
	Inventory.menu.useButton = guiManager:createWindow("EmberLook/Button")
	Inventory.menu.useButton:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0), CEGUI.UDim(0, 25)))
	Inventory.menu.useButton:setText("use")
	Inventory.menu.useButton_MouseClick = function(args)
		if Inventory.menu.activeEntityWrapper ~= nil then
			if Inventory.menu.activeEntityWrapper.entity ~= nil then
				emberServices:getServerService():use(Inventory.menu.activeEntityWrapper.entity)
			end
		end
		Inventory.menu.hide()
	end
	Inventory.menu.useButton:subscribeEvent("MouseClick", Inventory.menu.useButton_MouseClick)
	Inventory.menu.innercontainer:addChildWindow(Inventory.menu.useButton)
	

		
	
	Inventory.menu.container:setVisible(true)
-- 	connect(Inventory.connectors, guiManager:getInput().EventMouseButtonReleased, "Inventory.input_MouseButtonReleased")
--	guiManager:getMainSheet():addChildWindow(Inventory.menu.container)

	
end

function Inventory.setupDoll(avatarEntity)
	Inventory.doll = {}
	
	Inventory.doll.image = Inventory.widget:getWindow("DollImage")
	Inventory.doll.renderer = EmberOgre.Gui.ModelRenderer:new_local(Inventory.doll.image)
	Inventory.doll.renderer:setActive(false)
	Inventory.doll.renderer:setIsInputCatchingAllowed(false)
	
	Inventory.doll.renderer:showModel(avatarEntity:getModel():getDefinition():get():getName())
	Inventory.doll.renderer:setCameraDistance(0.75)
	Inventory.doll.renderer:updateRender()
	
	
	Inventory.doll.righHand = {}
	Inventory.doll.righHand.slot = Inventory.entityIconManager:createSlot(Inventory.iconsize)
	Inventory.doll.righHand.container = Inventory.widget:getWindow("Doll/RightHand")
	Inventory.doll.righHand.container:addChildWindow(Inventory.doll.righHand.slot:getWindow())
	Inventory.doll.righHand.entityIconDropped = function(entityIcon)
		emberServices:getServerService():wield(entityIcon:getEntity())
		Inventory.doll.righHand.slot:addEntityIcon(entityIcon)
	end
	Inventory.doll.righHand.entityIconDropped_connector = EmberOgre.LuaConnector:new_local(Inventory.doll.righHand.slot.EventIconDropped):connect(Inventory.doll.righHand.entityIconDropped)
	
	Inventory.doll.torso = Inventory.createDollSlot("body", Inventory.widget:getWindow("Doll/Torso"), "Drop an entity here to attach it to the torso.")
	
	Inventory.doll.torso.entityIconDropped = function(entityIcon)
		if Inventory.doll.torso.isValidDrop(entityIcon) then
			emberServices:getServerService():wield(entityIcon:getEntity())
			Inventory.doll.torso.slot:addEntityIcon(entityIcon)
		end
	end
	Inventory.doll.torso.entityIconDropped_connector = EmberOgre.LuaConnector:new_local(Inventory.doll.torso.slot.EventIconDropped):connect(Inventory.doll.torso.entityIconDropped)

	Inventory.doll.legs = Inventory.createDollSlot("legs", Inventory.widget:getWindow("Doll/Legs"), "Drop an entity here to attach it to the legs.")
	Inventory.doll.legs.entityIconDropped = function(entityIcon)
		if Inventory.doll.legs.isValidDrop(entityIcon) then
			emberServices:getServerService():wield(entityIcon:getEntity())
			Inventory.doll.legs.slot:addEntityIcon(entityIcon)
		end
	end
	Inventory.doll.legs.entityIconDropped_connector = EmberOgre.LuaConnector:new_local(Inventory.doll.legs.slot.EventIconDropped):connect(Inventory.doll.legs.entityIconDropped)
end

function Inventory.createDollSlot(outfitPlacement, containerWindow, tooltipText)
	local dollSlot = {}
	dollSlot.slot = Inventory.entityIconManager:createSlot(Inventory.iconsize)
	dollSlot.container = containerWindow
	dollSlot.container:addChildWindow(dollSlot.slot:getWindow())
	dollSlot.container:setTooltipText(tooltipText)
	dollSlot.outfitPlacement = outfitPlacement
	
	dollSlot.isValidDrop = function(entityIcon)
		if dollSlot.outfitPlacement == "" then
			return true
		end
		if entityIcon:getEntity():hasAttr("worn") then
			local wornElement = entityIcon:getEntity():valueOfAttr("worn")
			if wornElement:isString() then
				local worn = wornElement:asString()
				if worn == dollSlot.outfitPlacement then
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
	
	dollSlot.entityIconDragStart_connector = EmberOgre.LuaConnector:new_local(Inventory.entityIconManager.EventIconDragStart):connect(dollSlot.entityIconDragStart)
	dollSlot.entityIconDragStop_connector = EmberOgre.LuaConnector:new_local(Inventory.entityIconManager.EventIconDragStop):connect(dollSlot.entityIconDragStop)

	return dollSlot
end


Inventory.buildWidget()