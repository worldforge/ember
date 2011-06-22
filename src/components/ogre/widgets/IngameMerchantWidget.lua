MerchantWindow = {}
MerchantWindow.__index = MerchantWindow

function MerchantWindow.create(entity)
	local ret = {}
	setmetatable(ret, MerchantWindow)
	
	ret.widget = guiManager:createWidget()
	ret.widget:loadMainSheet("IngameMerchantWidget.layout", "IngameMerchantWidget/MerchantWindow/")
	
	ret.widget:hide()
	
	ret.window = ret.widget:getWindow("MainWindow")
	ret.label = ret.widget:getWindow("MainWindow/Label")
	
	ret:setTargetEntity(entity)

	ret.window:subscribeEvent("CloseClicked", "MerchantWindow.EventCloseClicked", ret)
	return ret
end

function MerchantWindow:shutdown()
	windowManager:destroyWindow(self.window)
end

function MerchantWindow:setTargetEntity(entity)
	if (entity ~= nil) then
		self.window:setText("Trading dialog with '" .. entity:getName() .. "'.")
		self.widget:show()
	else
		self.widget:hide()
	end
end

function MerchantWindow:EventCloseClicked(args)
	guiManager:destroyWidget(self.widget)
end

IngameMerchantWidget = {}
IngameMerchantWidget.__index = IngameMerchantWidget

function IngameMerchantWidget.create()
	local ret = {}
	setmetatable(ret, IngameMerchantWidget)
	
	ret:buildWidget()
	return ret
end

function IngameMerchantWidget:buildWidget()
	connect(self.connectors, guiManager.EventEntityAction, IngameMerchantWidget.handleEntityAction, self)
end

function IngameMerchantWidget:shutdown()
	disconnectAll(self.connectors)
end

function IngameMerchantWidget:handleEntityAction(action, entity)
	if (action == "Merchant") then
		local wnd = MerchantWindow.create(entity)
	end
end

IngameMerchantWidget.create()
