ActionBar = {}

loadScript("ActionBarDefaultAction.lua")

function ActionBar:removeExistingIcon(slot)
	--If an icon exists in that slot, then delete and replace.
	if slot:getActionBarIcon() ~= nil then
		debugObject("Attempting to remove an existing icon.")
		slot:getActionBarIcon():setSlot(nil)
		slot:notifyIconDraggedOff(slot:getActionBarIcon())
		self.actionBarIconManager:destroyIcon(actionBarIcon)
	end
end
	
function ActionBar:addSlot()
	local yPosition = 0
	local xPosition = 0
	if self.layout == "Horiz" then
		yPosition = math.floor(self.slotcounter / self.maxSlots)
		xPosition = self.slotcounter - math.floor(self.slotcounter/self.maxSlots)*self.maxSlots 
	else
		yPosition = self.slotcounter - math.floor(self.slotcounter/self.maxSlots)*self.maxSlots 
		xPosition = math.floor(self.slotcounter / self.maxSlots)
	end	
	
	self.slotcounter = self.slotcounter + 1
	local slot = self.actionBarIconManager:createSlot(self.iconSize)
	slot:getWindow():setPosition(CEGUI.UVector2(CEGUI.UDim(0, self.iconSize * xPosition), CEGUI.UDim(0, self.iconSize * yPosition)))
	slot:getWindow():setAlpha(0.6)
	slot:getWindow():setProperty("FrameEnabled", "true")
	slot:getWindow():setProperty("BackgroundEnabled", "true")
	self.iconContainer:addChildWindow(slot:getWindow())
	
	local slotWrapper = {slot = slot}
	
	table.insert(self.slots, slotWrapper)
	

	
	slotWrapper.entityIconDropped = function(entityIcon)
		debugObject("Got an entity icon drop.")
		self:removeExistingIcon(slotWrapper.slot)

		--Create a new ActionBarIcon from the existing entityIcon.
		local newIconWrapper = self:createActionBarIconFromEntity(entityIcon:getEntity())
		slotWrapper.slot:addActionBarIcon(newIconWrapper.actionBarIcon)
	end
	
	slotWrapper.actionBarIconDropped = function(actionBarIcon)		
		local oldSlot = actionBarIcon:getSlot()
		slotWrapper.slot:addActionBarIcon(actionBarIcon)
		if oldSlot ~= nil then
			self:removeExistingIcon(oldSlot)
		end
	end
	
	slotWrapper.entityIconDropped_connector = EmberOgre.LuaConnector:new_local(slot.EventActionBarIconDropped):connect(slotWrapper.actionBarIconDropped)
	slotWrapper.entityIconDropped_connector = EmberOgre.LuaConnector:new_local(slot.EventEntityIconDropped):connect(slotWrapper.entityIconDropped)
	return slotWrapper
end

function ActionBar:createActionBarIconFromEntity(entity)
	local icon = guiManager:getIconManager():getIcon(self.iconSize, entity)
	if icon ~= nil then
		local name = entity:getType():getName() .. " (" .. entity:getId() .. " : " .. entity:getName() .. ")"
		local actionBarIconWrapper = {defaultAction = nil}
		self:createActionBarIcon(actionBarIconWrapper, icon)
		actionBarIconWrapper.actionBarIcon:setTooltipText(name)
		actionBarIconWrapper.defaultAction:initFromEntityIcon(entity)
		return actionBarIconWrapper	
	else 
		return nil
	end
end

function ActionBar:createActionBarIcon(actionBarIconWrapper, icon)
	actionBarIconWrapper.defaultAction = ActionBarDefaultAction:new()
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
	
	--Default action for icon click.
	actionBarIconWrapper.windowClick = function(args)
		actionBarIconWrapper.defaultAction:executeAction()
		return true
	end
	actionBarIconWrapper.actionBarIcon:getDragContainer():subscribeEvent("MouseClick", actionBarIconWrapper.windowClick)
		
	actionBarIconWrapper.actionBarIcon:getDragContainer():subscribeEvent("MouseEnter", actionBarIconWrapper.mouseEnters)
	actionBarIconWrapper.actionBarIcon:getDragContainer():subscribeEvent("MouseLeave", actionBarIconWrapper.mouseLeaves)
end

function ActionBar:buildCEGUIWidget(widgetName)
	self.widget = guiManager:createWidget()
	if widgetName == nil then
		widgetName = "ActionBar/"
	end
	self.widget:loadMainSheet("ActionBar.layout", widgetName)
	self.iconContainer = self.widget:getWindow("IconContainer")
	
	local slotSize = (self.maxSlots*self.iconSize)+(self.maxSlots*2)
	self.dragBar = self.widget:getWindow("TitleBar")
	--Need to disable double clicks as the TitleBar treats the parent window as a FrameWindow.
	self.dragBar:setWantsMultiClickEvents(false)
	
	if self.layout == "Horiz" then
		self.widget:getMainWindow():setSize(CEGUI.UVector2(CEGUI.UDim(0.0,slotSize),CEGUI.UDim(0.0,self.iconSize))) 
	else
		self.widget:getMainWindow():setSize(CEGUI.UVector2(CEGUI.UDim(0.0,self.iconSize),CEGUI.UDim(0.0,slotSize+20)))
		--Drag bar needs to be resized to the top for vertical action bars.
		self.dragBar:setSize(CEGUI.UVector2(CEGUI.UDim(1.0,0.0),CEGUI.UDim(0.0,12.0)))
		--Need to shift the icon container down so that our drag bar doesn't overlap.
		self.iconContainer:setPosition(CEGUI.UVector2(CEGUI.UDim(0.0,0.0),CEGUI.UDim(0.0,12.0)))
	end
	self.widget:show()
	
	self.worldDragDrop = EmberOgre.Gui.ActionBarIconDragDropTarget(root)
	
	self.worldDragDrop_Dropped = function(actionBarIcon)
		if actionBarIcon ~= nil and actionBarIcon:getSlot() ~= nil then
			self:removeExistingIcon(actionBarIcon:getSlot())
		end
	end
	self.worldDragDrop_Dropped_connector = EmberOgre.LuaConnector:new_local(self.worldDragDrop.EventActionBarIconDropped):connect(self.worldDragDrop_Dropped)
	
	--Make 10 slots in the ActionBar.
	for i = 1,self.maxSlots do
		self:addSlot()
	end
end

function ActionBar:gotInput(args)
	debugObject(args)
end

function ActionBar.new(rotation)
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
				defaultAction = nil,
				slots = {}};
				
	setmetatable(actionbar,{__index=ActionBar})
	return actionbar
end

function ActionBar:init(widgetName)
	--TODO: When we implement the shutdown method, we need to delete this
	self.input1 = EmberOgre.Gui.ActionBarInput:new("1")
	
	self.actionBarIconManager = guiManager:getActionBarIconManager()
	
	connect(self.connectors, self.input1.EventGotHotkeyInput, self.gotInput, self)
	
	self:buildCEGUIWidget(widgetName)
end

function ActionBar:shutdown()
	for k,v in pairs(self.slots) do
		self.actionBarIconManager:destroySlot(v.slot)
	end
	
	for k,v in pairs(self.icons) do
		self.actionBarIconManager:destroyIcon(v.actionBarIcon)
	end
	
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)

end
