RuleManager = {}

function RuleManager:buildWidget()

	self.widget = guiManager:createWidget()

	local setup = function()

		self.ruleTree = CEGUI.toTree(self.widget:getWindow("RuleList"))
		self.ruleTree:subscribeEvent("SelectionChanged", self.RuleList_SelectionChanged, self)

		self.codecTypeCombobox = CEGUI.toCombobox(self.widget:getWindow("CodecType"))

		local item = Ember.OgreView.Gui.ColouredListItem.new("XML", 0)
		self.codecTypeCombobox:addItem(item)
		item = Ember.OgreView.Gui.ColouredListItem.new("Bach", 1)
		self.codecTypeCombobox:addItem(item)
		item = Ember.OgreView.Gui.ColouredListItem.new("Packed", 2)
		self.codecTypeCombobox:addItem(item)
		self.codecTypeCombobox:setItemSelectState(0, true)
		self.codecTypeCombobox:setSingleClickEnabled(true)
		self.codecTypeCombobox:subscribeEvent("ListSelectionChanged", function()
			local item = self.codecTypeCombobox:getSelectedItem()
			if item then
				local selectId = item:getID()
				if selectId == 0 then
					self.codec = "xml"
				elseif selectId == 1 then
					self.codec = "bach"
				else
					self.codec = "packed"
				end
				self:printRule()
			end
		end)

		self.ruleInfoText = CEGUI.toMultiLineEditbox(self.widget:getWindow("RuleInfoText"))

		self.ruleAdapter = Ember.OgreView.Gui.Adapters.Eris.RuleTreeAdapter.new(self.avatar:getConnection(), self.avatar:getId(), self.ruleTree)
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

		local ruleUpdateOverlay = self.widget:getWindow("RuleUpdateOverlay")
		ruleUpdateOverlay:subscribeEvent("MouseEntersSurface", function()
			ruleUpdateOverlay:fireEvent("StartHideTransition", CEGUI.WindowEventArgs.new(ruleUpdateOverlay))
			return true
		end)

		self.editor = Ember.OgreView.Authoring.RuleEditor.new(self.avatar)

		connect(self.connectors, self.editor.EventRuleCreated, function(refno)
			ruleUpdateOverlay:setVisible(true)
			ruleUpdateOverlay:setText("New rule created on server.")
		end
		)
		connect(self.connectors, self.editor.EventRuleUpdated, function(refno)
			ruleUpdateOverlay:setVisible(true)
			ruleUpdateOverlay:setText("Existing rule updated on server.")
		end
		)
		connect(self.connectors, self.editor.EventRuleEditError, function(refno)
			ruleUpdateOverlay:setVisible(true)
			ruleUpdateOverlay:setText("Error when updating or creating rule.")
		end
		)

		self.widget:enableCloseButton()
	end

	connect(self.connectors, self.widget.EventFirstTimeShown, setup)
	self.widget:loadMainSheet("RuleManager.layout", "RuleManager")
	self.widget:registerConsoleVisibilityToggleCommand("ruleManager")

end

function RuleManager:sendRuleToServer()

	local ruleUpdateOverlay = self.widget:getWindow("RuleUpdateOverlay")

	local parsedObject = Ember.OgreView.Gui.AtlasHelper.deserialize(self.ruleInfoText:getText(), self.codec)

	if parsedObject:isValid() then
		ruleUpdateOverlay:setVisible(true)
		ruleUpdateOverlay:setText("Sending rule to server.")

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

		local text = Ember.OgreView.Gui.AtlasHelper.serialize(rawRuleData, self.codec)
		self.ruleInfoText:setText(text)
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

RuleManager.gotAvatarConnector = emberServices:getServerService().GotAvatar:connect(function(avatar)
	ruleManager = { connectors = {}, codec = "xml", avatar = avatar }
	setmetatable(ruleManager, { __index = RuleManager })

	ruleManager:buildWidget()
	connect(ruleManager.connectors, emberServices:getServerService().DestroyedAvatar, function()
		ruleManager:shutdown()
		ruleManager = nil
		collectgarbage()
	end
	)
end
)


