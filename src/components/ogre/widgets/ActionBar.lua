ActionBar = {}

function ActionBar:addSlot()
	local yPosition = math.floor(self.slotcounter / self.maxSlots)
	local xPosition = self.slotcounter - math.floor(self.slotcounter/self.maxSlots)*self.maxSlots 
	
	self.slotcounter = self.slotcounter + 1
	
	local slot = self.entityIconManager:createSlot(self.iconsize)
	slot:getWindow():setPosition(CEGUI.UVector2(CEGUI.UDim(0, self.iconsize * xPosition), CEGUI.UDim(0, self.iconsize * yPosition)))
	slot:getWindow():setAlpha(1.0)
	slot:getWindow():setProperty("FrameEnabled", "true")
	slot:getWindow():setProperty("BackgroundEnabled", "true")
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

function ActionBar:buildCEGUIWidget(widgetName)
	self.widget = guiManager:createWidget()
	if widgetName == nil then
		widgetName = "ActionBar/"
	end
	self.widget:loadMainSheet("ActionBar.layout", widgetName)
	self.iconContainer = self.widget:getWindow("IconContainer")
	self.widget:show()
	
	--Make 10 slots in the ActionBar.
	for i = 1,self.maxSlots do
		self:addSlot()
	end
	
end

function ActionBar:gotInput(args)
	debugObject(args)
end

function ActionBar.new()
	local actionbar = {   iconsize = 50,
				maxSlots = 5,
				iconcounter = 0,
				slotcounter = 0,
				inputHelper = nil,
				iconContainer = nil,
				icons = {},
				connectors={},
				widget = nil,
				slots = {}};
				
	setmetatable(actionbar,{__index=ActionBar})
	return actionbar
end

function ActionBar:init(widgetName)
	--TODO: When we implement the shutdown method, we need to delete this
	self.inputHelper = EmberOgre.Gui.ActionBarInput:new()
	self.entityIconManager = guiManager:getEntityIconManager()
	
	connect(self.connectors, self.inputHelper.EventGotHotkeyInput, self.gotInput, self)
	
	self:buildCEGUIWidget(widgetName)
end

function ActionBar:shutdown()
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end
