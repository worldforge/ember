ActionBar = {}

function ActionBar:addSlot()
	local yPosition = math.floor(self.slotcounter / self.columns)
	local xPosition = self.slotcounter - math.floor(self.slotcounter%self.columns)*self.columns
	
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

function ActionBar:buildCEGUIWidget(args)
	self.widget = guiManager:createWidget()
	if args == nil then
		args = "ActionBar/"
	end
	self.widget:loadMainSheet("ActionBar.layout", args)
	self.iconContainer = self.widget:getWindow("IconContainer")
	self.widget:show()
--	self:addSlot()
--	self:addSlot()
--	self:addSlot()
--	self:addSlot()
end

function ActionBar:gotInput(args)
	debugObject(args)
end

function ActionBar:new()
	local actionbar = {   iconsize = 32,
				columns = 1,
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

function ActionBar:init(args)
	--TODO: When we implement the shutdown method, we need to delete this
	--self.inputHelper = EmberOgre.Gui.ActionBarInput:new()
	--self.entityIconManager = guiManager:getEntityIconManager()
	
	--connect(self.connectors, self.inputHelper.EventGotHotkeyInput, self.gotInput, self)
	
	self:buildCEGUIWidget(args)
end

function ActionBar:shutdown()
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

--connect(connectors, emberServices:getServerService().GotAvatar, ActionBar.buildWidget)
