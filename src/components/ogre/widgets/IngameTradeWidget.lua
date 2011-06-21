TradeWindow = {}
TradeWindow.__index = TradeWindow

function TradeWindow.create(entity)
	local ret = {}
	setmetatable(ret, TradeWindow)
	
	ret.widget = guiManager:createWidget()
	ret.widget:loadMainSheet("IngameTradeWidget.layout", "IngameTradeWidget/TradeWindow")
	
	ret.widget:hide()
	
	ret.window = ret.widget:getWindow("MainWindow")
	
	ret:setTargetEntity(entity)

	ret.window:subscribeEvent("CloseClicked", "TradeWindow.EventCloseClicked", ret)
	return ret
end

function TradeWindow:shutdown()
	windowManager:destroyWindow(self.window)
end

function TradeWindow:setTargetEntity(entity)
	if (entity ~= nil) then
		self.widget:show()
	else
		self.widget:hide()
	end
end

function TradeWindow:EventCloseClicked(args)
	self.widget:hide()
	guiManager:destroyWidget(self.widget)
end

IngameTradeWidget = {}
IngameTradeWidget.__index = IngameTradeWidget

function IngameTradeWidget.create()
	local ret = {}
	setmetatable(ret, IngameTradeWidget)
	
	ret:buildWidget()
	return ret
end

function IngameTradeWidget:buildWidget()
	connect(self.connectors, guiManager.EventEntityAction, self.GUIManager_EventEntityAction, self)
end

function IngameTradeWidget:shutdown()
	disconnectAll(self.connectors)
end

function IngameTradeWidget:GUIManager_EventEntityAction(action, entity)
	if (action == "Trade") then
		local wnd = TradeWindow:create(entity)
	end
end

IngameTradeWidget.create()
