MerchantWindow = {}
MerchantWindow.__index = MerchantWindow

function MerchantWindow.create(entity)
	local ret = {connectors = {} }
	setmetatable(ret, MerchantWindow)
	
	ret.widget = guiManager:createWidget()
	ret.widget:loadMainSheet("IngameMerchantWidget.layout", "IngameMerchantWidget/MerchantWindow/")
	
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
	if (entity ~= nil) then
		self.window:setText("Trading dialog with '" .. entity:getName() .. "'.")
		self.widget:show()
		connect(self.connectors, entity.Say, MerchantWindow.handleEntitySay, self)
		--console:runCommand("/say " .. entity:getName() .. ": list me price")
		console:runCommand("/say list me price")
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
		
		if selectedRowIndex ~= -1 then
			local selectedItemName = self.goods:getItemAtGridReference(CEGUI.MCLGridRef:new_local(selectedRowIndex, 0))
			if selectedItemName ~= nil then
				itemName = selectedItemName:getText()
			end
		end
		
		if itemName ~= "" then
			console:runCommand("/say I would like to buy " .. itemName)
		end
	end
end

function MerchantWindow:handleCloseClicked(args)
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
