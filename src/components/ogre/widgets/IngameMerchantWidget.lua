MerchantTradeConfirmationDialog = {}
MerchantTradeConfirmationDialog.__index = MerchantTradeConfirmationDialog

merchantTradeConfirmationDialogs = {}

function MerchantTradeConfirmationDialog.create(itemName, itemPrice, merchantEntity, uniqueIndex)
	local ret = {connectors = {} }
	setmetatable(ret, MerchantTradeConfirmationDialog)
	
	ret.widget = guiManager:createWidget()
	ret.widget:loadMainSheet("IngameMerchantTradeConfirmationDialog.layout", "IngameMerchantWidget/TradeConfirmationDialog/" .. uniqueIndex .. "/")
	ret.uniqueIndex = uniqueIndex
	
	ret.window = ret.widget:getWindow("MainWindow")
	ret.label = ret.widget:getWindow("MainWindow/Label")
	ret.label:setText("Are you sure you want to buy '" .. itemName .. "' from '" .. merchantEntity:getName() .. "' for " .. itemPrice .. " coins?")
	
	ret.window:subscribeEvent("CloseClicked", MerchantTradeConfirmationDialog.handleCloseClicked, ret)
	return ret
end

function MerchantTradeConfirmationDialog:handleCloseClicked(args)
	return true
end

MerchantWindow = {}
MerchantWindow.__index = MerchantWindow
merchantWindows = {}

function MerchantWindow.create(entity, uniqueIndex)
	local ret = {connectors = {} }
	setmetatable(ret, MerchantWindow)
	
	ret.widget = guiManager:createWidget()
	ret.uniqueIndex = uniqueIndex
	ret.widget:loadMainSheet("IngameMerchantWidget.layout", "IngameMerchantWidget/MerchantWindow/" .. uniqueIndex .. "/")
	
	ret.widget:hide()
	
	ret.window = ret.widget:getWindow("MainWindow")
	ret.label = ret.widget:getWindow("MainWindow/Label")
	ret.goods = CEGUI.toMultiColumnList(ret.widget:getWindow("MainWindow/AvailableGoods"))
	ret.goods:subscribeEvent("MouseDoubleClick", MerchantWindow.handleGoodsDoubleClicked, ret)
	
	ret:setTargetEntity(entity)

	ret.window:subscribeEvent("CloseClicked", MerchantWindow.handleCloseClicked, ret)
	return ret
end

function MerchantWindow:shutdown()
	disconnectAll(self.connectors)
	windowManager:destroyWindow(self.window)
end

function MerchantWindow:setTargetEntity(entity)
	disconnectAll(self.connectors)
	
	if (entity ~= nil) then
		self.window:setText("Trading dialog with '" .. entity:getName() .. "'.")
		self.widget:show()
		connect(self.connectors, entity.Say, MerchantWindow.handleEntitySay, self)
		--console:runCommand("/say " .. entity:getName() .. ": list me price")
		console:runCommand("/say list me price")
		self.merchantEntity = entity
	else
		self.widget:hide()
	end
end

function MerchantWindow:addItemForSale(itemName, price, quantityAvailable)
	local rowNumber = self.goods:getRowCount()
	self.goods:addRow()

	local item = Ember.OgreView.Gui.ColouredListItem:new(itemName)
	self.goods:setItem(item, 0, rowNumber);	

	local item = Ember.OgreView.Gui.ColouredListItem:new(price)
	self.goods:setItem(item, 1, rowNumber)

	if quantityAvailable < 0 then
		quantityAvailable = "inf"
	end
	
	local item = Ember.OgreView.Gui.ColouredListItem:new(quantityAvailable)
	self.goods:setItem(item, 2, rowNumber)
end

function MerchantWindow:handleEntitySay(root)
	--we are only listening to our target entity so whatever gets here is indeed the target entity speaking
	local rootObject = root:get()
	
	if not rootObject:hasAttr("say") then
		return
	end
	
	--message now contains what our target entity said
	local message = rootObject:getAttr("say"):asString()
	
	if string.find(message, "The price of [%a]+ is [%d]+") ~= nil then
		local item = ""
		local price = 0
		local quantityAvailable = -1
		
		_, _, item, price = string.find(message, "The price of ([%a]+) is ([%d]+)")
		
		self:addItemForSale(item, price, quantityAvailable)
	end
end

function MerchantWindow:handleGoodsDoubleClicked(args)
	local selection = self.goods:getFirstSelectedItem()
	
	if selection ~= nil then
		local selectedRowIndex = self.goods:getItemRowIndex(selection)
		local itemName = ""
		local itemPrice = 0
		
		if selectedRowIndex ~= -1 then
			local selectedItemName = self.goods:getItemAtGridReference(CEGUI.MCLGridRef:new_local(selectedRowIndex, 0))
			if selectedItemName ~= nil then
				itemName = selectedItemName:getText()
			end
			
			local selectedItemPrice = self.goods:getItemAtGridReference(CEGUI.MCLGridRef:new_local(selectedRowIndex, 1))
			if selectedItemPrice ~= nil then
				itemPrice = selectedItemPrice:getText()
			end
		end
		
		if itemName ~= "" and itemPrice ~= 0 then
			console:runCommand("/say I would like to buy " .. itemName)
			
			merchantTradeConfirmationDialogs[ingameMerchantWidget.confirmationDialogIndex] = MerchantTradeConfirmationDialog.create(itemName, itemPrice, self.merchantEntity, ingameMerchantWidget.confirmationDialogIndex)
			
			ingameMerchantWidget.confirmationDialogIndex = ingameMerchantWidget.confirmationDialogIndex + 1
		end
	end
end

function MerchantWindow:handleCloseClicked(args)
	guiManager:destroyWidget(self.widget)
	
	merchantWindows[self.uniqueIndex] = nil
end

IngameMerchantWidget = {}
IngameMerchantWidget.__index = IngameMerchantWidget

function IngameMerchantWidget.create()
	local ret = {}
	setmetatable(ret, IngameMerchantWidget)
	
	ret:buildWidget()
	
	ret.tradeWindowIndex = 0
	ret.confirmationDialogIndex = 0
	
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
		merchantWindows[self.tradeWindowIndex] = MerchantWindow.create(entity, self.tradeWindowIndex)
		
		self.tradeWindowIndex = self.tradeWindowIndex + 1
	end
end

ingameMerchantWidget = IngameMerchantWidget.create()
