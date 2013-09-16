--[[
Responsible for the functionality of an ActionBar. To add support for different types of icons, first the ActionBarIconDragDropTarget class needs to be updated.
You can then catch the event of the icon being dropped, and deal with it accordingly. Currently supported are: EntityIcons and ActionBarIcons.
]]--
ActionBar = {}

--ActionBarDefaultAction is required for handling the default action taken when an action bar icon is executed, either by clicking or hotkeys.
loadScript("ActionBarDefaultAction.lua")

--If the slot passed contains an action bar icon, then we remove it.
--@param slot The slot we want to remove any icons from.
function ActionBar:removeExistingIcon(slot)
	--If an icon exists in that slot, then delete and replace.
	if slot:getActionBarIcon() ~= nil then
		slot:getActionBarIcon():setSlot(nil)
		slot:notifyIconDraggedOff(slot:getActionBarIcon())
		self.actionBarIconManager:destroyIcon(actionBarIcon)
	end
end
	
--Add a slot to the action bar. Handles adding slots to both horizontal and vertical action bars.
--We also handle the drop events received by the slot here.	
function ActionBar:addSlot()
	local yPosition = 0
	local xPosition = 0
	--If we have a horizontal action bar, we add a slot to the right of the drag bar.
	if self.layout == "Horiz" then
		yPosition = math.floor(self.slotcounter / self.maxSlots)
		xPosition = self.slotcounter - math.floor(self.slotcounter/self.maxSlots)*self.maxSlots 
	else
	--If we have a vertical action bar, we add a slot below the drag bar.
		yPosition = self.slotcounter - math.floor(self.slotcounter/self.maxSlots)*self.maxSlots 
		xPosition = math.floor(self.slotcounter / self.maxSlots)
	end	
	
	self.slotcounter = self.slotcounter + 1
	local slot = self.actionBarIconManager:createSlot(self.iconSize)
	slot:getWindow():setPosition(CEGUI.UVector2(CEGUI.UDim(0, self.iconSize * xPosition), CEGUI.UDim(0, self.iconSize * yPosition)))
	slot:getWindow():setAlpha(0.6)
	--We want to present an outline of the slot to the user.
	slot:getWindow():setProperty("FrameEnabled", "true")
	slot:getWindow():setProperty("BackgroundEnabled", "true")
	slot:getWindow():setTooltipText("Drag and drop an entity from your inventory to attach it to this action bar slot.")
	self.iconContainer:addChild(slot:getWindow())
	
	local slotWrapper = {slot = slot}
	
	table.insert(self.slots, slotWrapper)
	
	--Handle an entity icon dropped on the action bar slot
	--We create a new action bar icon based on the entity icon
	slotWrapper.entityIconDropped = function(entityIcon)
		--If we have an existing icon in the slot, we remove it.
		self:removeExistingIcon(slotWrapper.slot)

		--Create a new ActionBarIcon from the existing entityIcon.
		local newIconWrapper = self:createActionBarIconFromEntity(entityIcon:getEntity())
		slotWrapper.slot:addActionBarIcon(newIconWrapper.actionBarIcon)
	end
	
	--Handle an action bar icon dropped on the action bar slot.
	slotWrapper.actionBarIconDropped = function(actionBarIcon)		
		local oldSlot = actionBarIcon:getSlot()
		if oldSlot ~= slotWrapper.slot then
			slotWrapper.slot:addActionBarIcon(actionBarIcon)
			if oldSlot ~= nil then
				self:removeExistingIcon(oldSlot)
			end
		end
	end
	
	slotWrapper.entityIconDropped_connector = createConnector(slot.EventActionBarIconDropped):connect(slotWrapper.actionBarIconDropped)
	slotWrapper.entityIconDropped_connector = createConnector(slot.EventEntityIconDropped):connect(slotWrapper.entityIconDropped)
	return slotWrapper
end

--Create a new action bar icon based on an entity.
function ActionBar:createActionBarIconFromEntity(entity)
	local icon = guiManager:getIconManager():getIcon(self.iconSize, entity)
	if icon ~= nil then
		--We use the name to set the tooltip text
		local name = entity:getType():getName() .. " (" .. entity:getId() .. " : " .. entity:getName() .. ")"
		local actionBarIconWrapper = {actionBarIcon = nil,
										defaultAction = nil,
										type = "entity",
										entityid = nil}
		self:createActionBarIcon(actionBarIconWrapper, icon)
		actionBarIconWrapper.actionBarIcon:setTooltipText(name)
		--Our command object for the default action is the entity.
		actionBarIconWrapper.defaultAction:initFromEntityIcon(entity:getId())
		actionBarIconWrapper.entityid = entity:getId()
		
		
		self.icons[entity:getId()] = actionBarIconWrapper
		connect(self.connectors, emberOgre:getWorld():getAvatar().EventRemovedEntityFromInventory, self.RemovedEntityFromInventory, self)
		return actionBarIconWrapper	
	else 
		return nil
	end
end

function ActionBar:RemovedEntityFromInventory(entity)
	local actionBarIconWrapper = self.icons[entity:getId()]
	if actionBarIconWrapper ~= nil then
		actionBarIconWrapper.actionBarIcon:setSlot(nil)
		self.actionBarIconManager:destroyIcon(actionBarIconWrapper.actionBarIcon)
	end
	self.icons[entity:getId()] = nil
end

--Generic code all action bar icons require.
--@param actionBarIconWrapper Contains
--@param icon Generic icon to use for the layout.
function ActionBar:createActionBarIcon(actionBarIconWrapper, icon)
	--Create and attach an empty default action for the icon.
	actionBarIconWrapper.defaultAction = ActionBarDefaultAction:new(self.defaultActionList)
	--Based on the generic icon passed, we create an action bar icon.
	actionBarIconWrapper.actionBarIcon = self.actionBarIconManager:createIcon(icon, self.iconSize)
	actionBarIconWrapper.actionBarIcon:getImage():setProperty("InheritsAlpha", "false")
	actionBarIconWrapper.actionBarIcon:getImage():setAlpha(1.0)
	
	actionBarIconWrapper.mouseEnters = function(args)
		actionBarIconWrapper.actionBarIcon:getImage():setProperty("FrameEnabled", "true")
		return true
	end
	actionBarIconWrapper.mouseLeaves = function(args)
		actionBarIconWrapper.actionBarIcon:getImage():setProperty("FrameEnabled", "false")
		return true
	end
	
	--Execute the default action of the icon on a mouse button click.
	actionBarIconWrapper.windowClick = function(args)
		actionBarIconWrapper.defaultAction:executeAction()
		return true
	end
	actionBarIconWrapper.actionBarIcon:getDragContainer():subscribeEvent("MouseClick", actionBarIconWrapper.windowClick)
		
	actionBarIconWrapper.actionBarIcon:getDragContainer():subscribeEvent("MouseEntersSurface", actionBarIconWrapper.mouseEnters)
	actionBarIconWrapper.actionBarIcon:getDragContainer():subscribeEvent("MouseLeavesSurface", actionBarIconWrapper.mouseLeaves)
end

--Saves action bar properties such as position and icons.
function ActionBar:saveAttr()
	self.actionBarIconManager:saveValue(self.avatarId, self.name, self.name)
	
	self.actionBarIconManager:saveValue(self.avatarId, self.name .. "position", CEGUI.PropertyHelper:uvector2ToString(self.widget:getMainWindow():getPosition()))
	
	for k,v in pairs(self.slots) do
		local aBarIcon = v.slot:getActionBarIcon()
		if aBarIcon ~= nil then
			for k2,v2 in pairs(self.icons) do
				if aBarIcon == v2.actionBarIcon then
					self.actionBarIconManager:saveValue(self.avatarId, self.name..k, v2.type)
					self.actionBarIconManager:saveValue(self.avatarId, self.name..k.."id", v2.entityid)
				end
			end
		else
			self.actionBarIconManager:eraseValue(self.avatarId, self.name..k)
			self.actionBarIconManager:eraseValue(self.avatarId, self.name..k.."id")
		end
	end
end

--Loads saved properties for the action bar. 
--We call this at the initialization of the action bar.
function ActionBar:loadSavedAttributes()
	if self.actionBarIconManager:getSavedValue(self.avatarId, self.name) ~= "null" then
		local position = "".. self.actionBarIconManager:getSavedValue(self.avatarId, self.name .. "position") ..""
		
		if position ~= "" then
			self.widget:getMainWindow():setPosition(CEGUI.PropertyHelper:stringToUVector2(position))
		end
	end
	
	for k,v in pairs(self.slots) do
		local type = self.actionBarIconManager:getSavedValue(self.avatarId, self.name..k)
		local id = self.actionBarIconManager:getSavedValue(self.avatarId, self.name..k.."id")
		if type == "entity" then
			self.entityCandidates[id] = v.slot
			--Keep track of the size, because lua doesn't have a function for getting the size of dictionaries
			--without looping through all elements.
			self.entityCandidates.size = self.entityCandidates.size + 1
		end
	end
	
	self.entityCandidates.AddedEntityToInventory = function(entity)
		local id = entity:getId()
		if self.entityCandidates[id] ~= nil then
			local newIconWrapper = self:createActionBarIconFromEntity(entity)
			self.entityCandidates[id]:addActionBarIcon(newIconWrapper.actionBarIcon)
			self.entityCandidates[id] = nil
			self.entityCandidates.size = self.entityCandidates.size - 1
		end
		if self.entityCandidates.size == 0 then
			self.entityCandidates.AddedEntityToInventory_connector:disconnect()
		end
	end
	
	self.timer = Eris.Timeout:new_local(10000)
	self.timer.disconnect = function()
		self.entityCandidates.AddedEntityToInventory_connector:disconnect()
	end
	
	self.timer.disconnect_connector = createConnector(self.timer.Expired):connect(self.timer.disconnect)
	self.entityCandidates.AddedEntityToInventory_connector = createConnector(emberOgre:getWorld():getAvatar().EventAddedEntityToInventory):connect(self.entityCandidates.AddedEntityToInventory)
	
end

--Build the action bar widget.
--@param widgetName Unique widget name passed by AcionBarCreator.
function ActionBar:buildCEGUIWidget(widgetName)
	self.widget = guiManager:createWidget()
	if widgetName == nil then
		widgetName = "ActionBar"
	end
	self.name = widgetName
	self.widget:loadMainSheet("ActionBar.layout", widgetName)
	self.iconContainer = self.widget:getWindow("IconContainer")
	
	local slotSize = (self.maxSlots*self.iconSize)+(self.maxSlots*2)
	self.dragBar = self.widget:getWindow("TitleBar")
	--Need to disable double clicks as the TitleBar treats the parent window as a FrameWindow.
	self.dragBar:setWantsMultiClickEvents(false)
	
	if self.layout == "Horiz" then
		self.widget:getMainWindow():setSize(CEGUI.USize(CEGUI.UDim(0.0,slotSize),CEGUI.UDim(0.0,self.iconSize)))
		--Place it at the bottom of the screen 
		self.widget:getMainWindow():setYPosition(CEGUI.UDim(1.0, -self.iconSize))
		self.widget:getMainWindow():setXPosition(CEGUI.UDim(0.5, -(slotSize / 2.0)))
	else
		self.widget:getMainWindow():setSize(CEGUI.USize(CEGUI.UDim(0.0,self.iconSize),CEGUI.UDim(0.0,slotSize+20)))
		--Drag bar needs to be resized to the top for vertical action bars.
		self.dragBar:setSize(CEGUI.USize(CEGUI.UDim(1.0,0.0),CEGUI.UDim(0.0,12.0)))
		--Need to shift the icon container down so that our drag bar doesn't overlap.
		self.iconContainer:setPosition(CEGUI.UVector2(CEGUI.UDim(0.0,0.0),CEGUI.UDim(0.0,12.0)))
	end
	self.widget:show()
	
	self.worldDragDrop = Ember.OgreView.Gui.ActionBarIconDragDropTarget(root)
	
	--Dragging an icon from the action bar to the world, and releasing it, should cause that icon to be destroyed.
	self.worldDragDrop_Dropped = function(actionBarIcon)
		if actionBarIcon ~= nil and actionBarIcon:getSlot() ~= nil then
			self:removeExistingIcon(actionBarIcon:getSlot())
		end
	end
	self.worldDragDrop_Dropped_connector = createConnector(self.worldDragDrop.EventActionBarIconDropped):connect(self.worldDragDrop_Dropped)
	
	--Create our slots in the ActionBar.
	for i = 1,self.maxSlots do
		self:addSlot()
	end
end

--Input caught by ActionBarInput is handled here.
--@param key The hotkey pressed.
function ActionBar:gotInput(key)
	local slotNum = self.hotkeys[key].slotNum
	local actionBarIcon = self.slots[slotNum].slot:getActionBarIcon()
	for k,v in pairs(self.icons) do
		if actionBarIcon == v.actionBarIcon and v.defaultAction ~= nil then
			v.defaultAction:executeAction()
		end
	end
end

--Create a new action bar.
--@param rotation The rotation of the action bar, either horizontal or veritcal.
function ActionBar.new(rotation, defActionList, erisAvatar)
	local actionbar = {   iconSize = 50,
				maxSlots = 5,
				iconcounter = 0,
				slotcounter = 0,
				inputHelper = nil,
				iconContainer = nil,
				icons = {},
				connectors={},
				widget = nil,
				layout = rotation, --Vertical or horizontal action bar
				defaultActionList = defActionList,
				hotkeys = {},
				name = "",
				entityCandidates = {size = 0},
				timer = nil,
				slots = {}};
	serverInfo = Eris.ServerInfo()
	erisAvatar:getConnection():getServerInfo(serverInfo)
	actionbar.avatarId = Ember.OgreView.Gui.ActionBarIconManager.AvatarIdType(serverInfo, erisAvatar:getId())
				
	setmetatable(actionbar,{__index=ActionBar})
	return actionbar
end

--The default hotkey mapping of an actionbar. Only the first action bar created should respond to these.
function ActionBar:defaultKeyMapping()
	self:keyMapping("1",1)
	self:keyMapping("2",2)
	self:keyMapping("3",3)
	self:keyMapping("4",4)
	self:keyMapping("5",5)
end

--Given a key and the slot number in the action bar, we create a hotkey mapping.
--@param key The key which we wish to use as a hotkey.
--@param slotNum The slot number executed by the key press.
function ActionBar:keyMapping(key, slotNum)
	local input = {}
 	input.key = Ember.OgreView.Gui.ActionBarInput:new(key)
 	input.slotNum = slotNum
	self.hotkeys[key] = input
	
	local slotWrapper = self.slots[slotNum]
	if slotWrapper then
		local numWindow = self.widget:createWindow("EmberLook/StaticText")
		numWindow:setProperty("FrameEnabled", "false");
	 	numWindow:setProperty("BackgroundEnabled", "false");
		numWindow:setProperty("VertFormatting", "TopAligned");
		numWindow:setText(key)
		numWindow:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 1), CEGUI.UDim(0, 1)))
		numWindow:setHeight(CEGUI.UDim(0, 20))
		numWindow:setMousePassThroughEnabled(true)
		slotWrapper.slot:getWindow():addChild(numWindow)
	end
	
	connect(self.connectors, input.key.EventGotHotkeyInput, self.gotInput, self)
end

--Initialization of the action bar, we build the widget here.
function ActionBar:init(widgetName)	
	self.actionBarIconManager = guiManager:getActionBarIconManager()
		
	self:buildCEGUIWidget(widgetName)
	self:loadSavedAttributes()
end

function ActionBar:shutdown()

	--Save action bar properties
	self:saveAttr()

	--Delete all of the action bar slots.
	for k,v in pairs(self.slots) do
		self.actionBarIconManager:destroySlot(v.slot)
	end
	
	--Delete all of the action bar icons.
	for k,v in pairs(self.icons) do
		self.actionBarIconManager:destroyIcon(v.actionBarIcon)
	end
	
	--Delete all of the action bar hotkeys.
	for k,v in pairs(self.hotkeys) do
		v.key:delete()
	end 
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)

end
