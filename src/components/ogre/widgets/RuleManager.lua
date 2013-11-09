RuleManager = {}

function RuleManager:buildWidget()

	self.widget = guiManager:createWidget()

	local setup = function()
	
		self.editor = Ember.OgreView.Authoring.RuleEditor:new_local(self.account)
		
		self.ruleTree = tolua.cast(self.widget:getWindow("RuleList"), "CEGUI::Tree")
		self.ruleTree:subscribeEvent("SelectionChanged", self.RuleList_SelectionChanged, self)
	
		self.codecTypeCombobox = CEGUI.toCombobox(self.widget:getWindow("CodecType"))
		
		local item = Ember.OgreView.Gui.ColouredListItem:new("XML", 0)
		self.codecTypeCombobox:addItem(item)
		item = Ember.OgreView.Gui.ColouredListItem:new("Bach", 1)
		self.codecTypeCombobox:addItem(item)
		item = Ember.OgreView.Gui.ColouredListItem:new("Packed", 2)
		self.codecTypeCombobox:addItem(item)
		self.codecTypeCombobox:setItemSelectState(0, true)
		self.codecTypeCombobox:setSingleClickEnabled(true)
		self.codecTypeCombobox:subscribeEvent("ListSelectionChanged", self.CodecType_ListSelectionChanged, self)
	
		
		self.ruleInfoText = CEGUI.toMultiLineEditbox(self.widget:getWindow("RuleInfoText"))
	
		self.ruleAdapter = Ember.OgreView.Gui.Adapters.Eris.RuleTreeAdapter:new_local(self.account:getConnection(), self.ruleTree)
		local loadingOverlay = self.widget:getWindow("LoadingOverlay")
		local refreshButton = self.widget:getWindow("Refresh")

		local refresh = function()
			refreshButton:setEnabled(false)
			self.ruleAdapter:refresh("root")
			
			loadingOverlay:setVisible(true)
			loadingOverlay:setText("Getting rules from server.")
			
			connect(self.connectors, self.ruleAdapter.EventNewRuleReceived, function(numberOfRules)
				loadingOverlay:setText("Getting rules from server.\n" .. numberOfRules .. " rules received.")
				end
			)
			connect(self.connectors, self.ruleAdapter.EventAllRulesReceived, function()
					loadingOverlay:setVisible(false)
					refreshButton:setEnabled(true)
				end
			)
		end
		
		refresh()

		self.widget:getWindow("Refresh"):subscribeEvent("Clicked", refresh)
		
		
		self.widget:getWindow("SendToServerButton"):subscribeEvent("Clicked", self.SendToServerButton_Clicked, self)
			
		self.widget:enableCloseButton()
	end

	connect(self.connectors, self.widget.EventFirstTimeShown, setup)
	self.widget:loadMainSheet("RuleManager.layout", "RuleManager")
	self.widget:registerConsoleVisibilityToggleCommand("ruleManager")

end

function RuleManager:CodecType_ListSelectionChanged()
	local item = self.codecTypeCombobox:getSelectedItem()
	if item ~= nil then
		local selectId = item:getID()
		if selectId == 0 then
			self.codecClass = Atlas.Codecs.XML
		elseif selectId == 1 then
			self.codecClass = Atlas.Codecs.Bach
		else
			self.codecClass = Atlas.Codecs.Packed
		end
		self:printRule()
	end
end

function RuleManager:sendRuleToServer()
	local outstream = std.stringstream:new_local(self.ruleInfoText:getText())
	local decoder = Ember.AtlasObjectDecoder:new_local()

	local codec = self.codecClass:new_local(outstream, tolua.cast(decoder, "Atlas::Bridge"))
	codec:poll(true)
	
	local parsedObject = decoder:getLastObject()
	
	if parsedObject:isValid() then
		self.editor:updateOrCreateRule(parsedObject)
	end
end

function RuleManager:SendToServerButton_Clicked(args)

	self:sendRuleToServer()
	return true
end

function RuleManager:printRule()
	local rawRuleData = self.ruleAdapter:getSelectedRule()
	
	if rawRuleData:isValid() then
	
		local outstream = std.stringstream:new_local()
		local decoder = Atlas.Message.QueuedDecoder:new_local()
	
		local codec = self.codecClass:new_local(outstream, tolua.cast(decoder, "Atlas::Bridge"))
		local formatter = Atlas.Formatter:new_local(outstream, tolua.cast(codec, "Atlas::Bridge"))
		local encoder = Atlas.Message.Encoder:new_local(tolua.cast(formatter, "Atlas::Bridge"))
		local message = Atlas.Message.MapType:new_local()
		rawRuleData:get():addToMessage(message)
		formatter:streamBegin();
		encoder:streamMessageElement(message);
	
		formatter:streamEnd();
	
		self.ruleInfoText:setText(outstream:str())
	end
end

function RuleManager:RuleList_SelectionChanged(args)

	self:printRule()
	return true
end

function RuleManager:shutdown()
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

RuleManager.gotAccountConnector = createConnector(emberServices:getServerService().GotAccount):connect(function(account)
		ruleManager = {connectors={}, codecClass=Atlas.Codecs.XML, account=account}
		setmetatable(ruleManager, {__index = RuleManager})
		
		ruleManager:buildWidget()
		connect(ruleManager.connectors, emberServices:getServerService().DestroyedAccount, function()
				ruleManager:shutdown()
				ruleManager = nil
			end
		)
	end
)


