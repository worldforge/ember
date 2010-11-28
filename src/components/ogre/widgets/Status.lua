Status = {connectors={}}
StatusInstance = {}

function StatusInstance:updateStatus()
	if self.entity:hasAttr("status") then
		self:status_Changed(self.entity:valueOfAttr("status"))
	end
	if self.entity:hasAttr("stamina") then
		self:stamina_Changed(self.entity:valueOfAttr("stamina"))
	end
	if self.entity:hasAttr("strength") then
		self:strength_Changed(self.entity:valueOfAttr("strength"))
	end
end

function StatusInstance:status_Changed(element)
	local status = 0
	if element:isFloat() then
		status = element:asFloat()
	end
	self.healthBar:setProgress(status)
	self.healthBar:setTooltipText("Health: " .. status * 100 .. "/100")
end

function StatusInstance:stamina_Changed(element)
	local value = 0
	if element:isFloat() then
		value = element:asFloat()
	end
	self.staminaBar:setProgress(value)
	self.staminaBar:setTooltipText("Stamina: " .. value * 100 .. "/100")
end

function StatusInstance:strength_Changed(element)
	local value = 0
	if element:isFloat() then
		value = element:asFloat()
	end
	self.strengthBar:setProgress(value)
	self.strengthBar:setTooltipText("Strength: " .. value * 100 .. "/100")
end

function StatusInstance:setEntity(entity)
	self.entity = entity

	if self.connectors ~= nil then
		if self.connectors.changedConnector ~= nil then
			self.connectors.changedConnector:disconnect()
		end
		if self.connectors.deleteConnector ~= nil then
			self.connectors.deleteConnector:disconnect()
		end
	end

	if entity == nil then
		self.widget:hide()
	else
		self.observers = {}
		self.observers.status = Ember.AttributeObserver(entity, "status")
		connect(self.connectors, self.observers.status.EventChanged, self.status_Changed, self)
		
		self.observers.stamina = Ember.AttributeObserver(entity, "stamina")
		connect(self.connectors, self.observers.stamina.EventChanged, self.stamina_Changed, self)

		self.observers.strength = Ember.AttributeObserver(entity, "strength")
		connect(self.connectors, self.observers.strength.EventChanged, self.strength_Changed, self)

		self.connectors.deleteConnector = createConnector(entity.BeingDeleted):connect(self.entity_BeingDeleted, self)

		if entity:getName() == "" then
			self.nameWindow:setText(entity:getType():getName())
		else
			self.nameWindow:setText(entity:getName())
		end
		self:updateStatus()
		
		local model = Ember.OgreView.Model.ModelRepresentationManager:getSingleton():getModelForEntity(entity)
		if model ~= nil then
			self.renderer:showModel(model:getDefinition():get():getName())
			self.renderer:setCameraDistance(0.75)
		else 
			self.renderer:showModel("")
		end
		self.renderer:updateRender()
		self.widget:show()

	end
end

function StatusInstance:entity_BeingDeleted()
	self:setEntity(nil)
end

function StatusInstance:shutdown()
	disconnectAll(self.connectors)
	
	deleteSafe(self.renderer)
	if self.widget ~= nil then
		guiManager:destroyWidget(self.widget)
	end
end

function Status:createStatusInstance(name)
	local statusInstance = {connectors={}}
	setmetatable(statusInstance, {__index = StatusInstance})
	statusInstance.widget = guiManager:createWidget()
	statusInstance.widget:loadMainSheet("Status.layout", "Status_" .. name .. "/")
	statusInstance.widget:setIsActiveWindowOpaque(false)
	
	statusInstance.healthBar = CEGUI.toProgressBar(statusInstance.widget:getWindow("HealthBar"))
	statusInstance.staminaBar = CEGUI.toProgressBar(statusInstance.widget:getWindow("StaminaBar"))
	statusInstance.strengthBar = CEGUI.toProgressBar(statusInstance.widget:getWindow("StrengthBar"))
	
	statusInstance.renderImage = statusInstance.widget:getWindow("RenderImage")
	statusInstance.renderer = Ember.OgreView.Gui.ModelRenderer:new(statusInstance.renderImage)
	statusInstance.renderer:setActive(false)
	statusInstance.renderer:setIsInputCatchingAllowed(false)
	
	statusInstance.nameWindow = statusInstance.widget:getWindow("EntityName")
	
	statusInstance.entity = nil

	statusInstance.widget:hide()
	return statusInstance
end

function Status:buildWidget(avatarEntity)
	
	self.avatarStatus = self:createStatusInstance("avatar")
	
	self.npcStatus = self:createStatusInstance("npc")
	local uPosition = CEGUI.UVector2:new_local(CEGUI.UDim(0,150), CEGUI.UDim(0,0))
	self.npcStatus.widget:getMainWindow():setPosition(uPosition )
	
	connect(self.connectors, guiManager.EventEntityAction, Status.handleAction, self)
	self.avatarStatus:setEntity(avatarEntity)

end

function Status:handleAction(action, entity) 
	if action == "use" or action == "inspect" or action == "attack" then
		self.npcStatus:setEntity(entity)
	end
end

function Status:shutdown()
	disconnectAll(self.connectors)
	self.npcStatus:shutdown()
	self.avatarStatus:shutdown()
end

Status.createdAvatarEntityConnector = createConnector(emberOgre.EventCreatedAvatarEntity):connect(function(avatarEntity)
		status = {connectors={} }
		setmetatable(status, {__index = Status})
		status:buildWidget(avatarEntity)
		connect(status.connectors, avatarEntity.BeingDeleted, function()
				status:shutdown()
				status = nil
			end
		)		
	end
)