MerchantTradeConfirmationDialog = {}
MerchantTradeConfirmationDialog.__index = MerchantTradeConfirmationDialog

function MerchantTradeConfirmationDialog.create(itemName, itemPrice, merchantEntity, uniqueIndex, shutdownCallback)
	local ret = {connectors = {} }
	setmetatable(ret, MerchantTradeConfirmationDialog)
	
	ret.shutdownCallback = shutdownCallback
	ret.itemPrice = tonumber(itemPrice)
	ret.merchantEntity = merchantEntity
	
	ret.widget = guiManager:createWidget()
	ret.widget:loadMainSheet("IngameMerchantTradeConfirmationDialog.layout", "IngameMerchantWidget/TradeConfirmationDialog/" .. uniqueIndex .. "/")
	ret.uniqueIndex = uniqueIndex
	
	ret.window = ret.widget:getWindow("MainWindow")
	ret.label = ret.widget:getWindow("MainWindow/Label")
	ret.label:setText("Are you sure you want to buy '" .. itemName .. "' from '" .. merchantEntity:getName() .. "' for " .. itemPrice .. " coins?")
	
	ret.confirmButton = ret.widget:getWindow("MainWindow/ConfirmButton")
	ret.confirmButton:subscribeEvent("Clicked", MerchantTradeConfirmationDialog.handleConfirmClicked, ret)
	ret.cancelButton = ret.widget:getWindow("MainWindow/CancelButton")
	ret.cancelButton:subscribeEvent("Clicked", MerchantTradeConfirmationDialog.handleCancelClicked, ret)
	
	ret.window:subscribeEvent("CloseClicked", MerchantTradeConfirmationDialog.handleCloseClicked, ret)
	
	ret.widget:show()
	
	return ret
end

function MerchantTradeConfirmationDialog:closeDialog()
	guiManager:destroyWidget(self.widget)
	self.widget = nil
	self.shutdownCallback()
end

--This method retrieves amount of coins current player has
function MerchantTradeConfirmationDialog:getAmountOfCoins()
	local avatarEntity = emberOgre:getWorld():getAvatar():getEmberEntity()
	local money = 0
	
	local numContained = avatarEntity:numContained()
	if numContained ~= 0 then
		for i = 0, numContained - 1 do
			local containedEntity = avatarEntity:getContained(i)

			if containedEntity:getType():getName() == "coin" then
				money = money + 1
			end
		end
	end
	
	return money
end

--Returns true if current player can afford the item this confirmation dialog was spawned for
function MerchantTradeConfirmationDialog:checkCoins()
	return self:getAmountOfCoins() >= self.itemPrice
end

--Gives the needed amount of coins to the merchant, merchant should give the player the desired item
--(The one this confirmation dialog was spawned for)
function MerchantTradeConfirmationDialog:giveCoins()
	if not self:checkCoins() then
		return false
	end
	
	local avatarEntity = emberOgre:getWorld():getAvatar():getEmberEntity()
	local given = 0
	
	local numContained = avatarEntity:numContained()
	if numContained ~= 0 then
		for i = 0, numContained - 1 do
			local containedEntity = avatarEntity:getContained(i)
			
			if containedEntity:getType():getName() == "coin" then
				emberOgre:doWithEntity(self.merchantEntity:getId(), function (targetMerchantEntity) 
					emberServices:getServerService():place(containedEntity, targetMerchantEntity)
				end)
			
				given = given + 1
			end
			
			if given >= self.itemPrice then
				break
			end
		end
	end
	
	if given >= self.itemPrice then
		return true
	else
		return false
	end
end

function MerchantTradeConfirmationDialog:handleConfirmClicked(args)
	-- if false is returned from giveCoins it can only mean that user doesn't have enough coins
	if self:giveCoins() then
		self:closeDialog()
	else
		self.label:setText("You don't have enough coins to proceed.")
	end
	
	return true
end

function MerchantTradeConfirmationDialog:handleCancelClicked(args)
	self:closeDialog()
	return true
end

function MerchantTradeConfirmationDialog:handleCloseClicked(args)
	return self:handleCancelClicked(args)
end

MerchantWindow = {}
MerchantWindow.__index = MerchantWindow

function MerchantWindow.create(entity, uniqueIndex, shutdownCallback, ingameMerchantWidget)
	local ret = {connectors = {}, merchantTradeConfirmationDialogs={} }
	setmetatable(ret, MerchantWindow)
	
	ret.ingameMerchantWidget = ingameMerchantWidget
	ret.shutdownCallback = shutdownCallback
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
	
	local message = Ember.OgreView.Gui.HelpMessage:new_local("Merchant Window", "You have opened a merchant window, browse the wares and double click to buy items of your choice.", "merchant window", "merchantWindow")
	Ember.OgreView.Gui.QuickHelp:getSingleton():updateText(message)
	
	return ret
end

function MerchantWindow:shutdown()
	for key,value in pairs(self.merchantTradeConfirmationDialogs) do
		if value ~= nil then
			value:closeDialog()
		end
	end
	self.merchantTradeConfirmationDialogs = {}
	
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
	
	self.shutdownCallback()
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
	if quantityAvailable < 0 then
		quantityAvailable = "inf"
	end
	
	local existing = false
	local rowCount = self.goods:getRowCount()
	--we assume we will have to create a new row, in other case we set this to the row
	--index of the preexisting row with our item
	local rowNumber = rowCount
	
	for i=0,rowCount-1 do
		if self.goods:getItemAtGridReference(CEGUI.MCLGridRef:new_local(i, 0)):getText() == itemName then
			existing = true
			rowNumber = i
			
			break
		end
	end
	
	if not existing then
		self.goods:addRow()
		
		local item
		
		item = Ember.OgreView.Gui.ColouredListItem:new(itemName)
		self.goods:setItem(item, 0, rowNumber);	

		item = Ember.OgreView.Gui.ColouredListItem:new(price)
		self.goods:setItem(item, 1, rowNumber)
	
		item = Ember.OgreView.Gui.ColouredListItem:new(quantityAvailable)
		self.goods:setItem(item, 2, rowNumber)
	else
		self.goods:getItemAtGridReference(CEGUI.MCLGridRef:new_local(rowNumber, 1)):setText(price)
		self.goods:getItemAtGridReference(CEGUI.MCLGridRef:new_local(rowNumber, 2)):setText(quantityAvailable)
	end
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
	
			local windowIndex = self.ingameMerchantWidget.confirmationDialogIndex
			local shutdownCallback = function()
				-- setting the table entry to nil effectively removes it from the table
				self.merchantTradeConfirmationDialogs[windowIndex] = nil
			end

			-- the confirmation dialog itself will give the coins and finish the transaction
			self.merchantTradeConfirmationDialogs[windowIndex] = MerchantTradeConfirmationDialog.create(itemName, itemPrice, self.merchantEntity, windowIndex, shutdownCallback)
			
			self.ingameMerchantWidget.confirmationDialogIndex = self.ingameMerchantWidget.confirmationDialogIndex + 1
		end
	end
end

function MerchantWindow:handleCloseClicked(args)
	self:shutdown()
end

IngameMerchantWidget = {}
IngameMerchantWidget.__index = IngameMerchantWidget

function IngameMerchantWidget.create()
	local ret = {connectors={}}
	-- this is a table of references to "alive" merchant windows
	-- we keep this around to prevent Lua from garbage collecting the windows when they are active
	ret.merchantWindows = {}
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
	for key,value in pairs(self.merchantWindows) do
		value:shutdown()
	end
	self.merchantWindows = {}
	disconnectAll(self.connectors)
end

function IngameMerchantWidget:handleEntityAction(action, entity)
	if (action == "merchant") then
		local windowIndex = self.tradeWindowIndex
		local shutdownCallback = function()
			-- setting the table entry to nil effectively removes it from the table
			self.merchantWindows[windowIndex] = nil
		end
		self.merchantWindows[windowIndex] = MerchantWindow.create(entity, windowIndex, shutdownCallback, self)
		
		self.tradeWindowIndex = self.tradeWindowIndex + 1
	end
end

IngameMerchantWidget.startConnector = createConnector(emberOgre.EventCreatedAvatarEntity):connect(function()
		local ingameMerchantWidget = IngameMerchantWidget.create()
		
		connect(ingameMerchantWidget.connectors, emberOgre.EventWorldDestroyed, function()
			ingameMerchantWidget:shutdown()
			ingameMerchantWidget = nil
		end
		)
	end
)

