Status = {connectors={}}

function Status.createStatusInstance(name)
	local wrapper = {}
	wrapper.widget = guiManager:createWidget()
	wrapper.widget:loadMainSheet("Status.layout", "Status_" .. name .. "/")
	
	wrapper.healthBar = CEGUI.toProgressBar(wrapper.widget:getWindow("HealthBar"))
	wrapper.staminaBar = CEGUI.toProgressBar(wrapper.widget:getWindow("StaminaBar"))
	wrapper.strengthBar = CEGUI.toProgressBar(wrapper.widget:getWindow("StrengthBar"))
	
	wrapper.renderImage = wrapper.widget:getWindow("RenderImage")
	wrapper.renderer = EmberOgre.ModelRenderer:new_local(wrapper.renderImage)
	wrapper.renderer:setActive(false)
	wrapper.renderer:setIsInputCatchingAllowed(false)
	
	wrapper.nameWindow = wrapper.widget:getWindow("EntityName")
	
	wrapper.entity = nil
	
	wrapper.updateStatus = function()
		if wrapper.entity:hasAttr("status") then
			local statusElement = wrapper.entity:valueOfAttr("status")
			local status = 0
			if statusElement:isFloat() then
				status = statusElement:asFloat()
			end
			wrapper.healthBar:setProgress(status)
			wrapper.healthBar:setTooltipText("Health: " .. status * 100 .. "/100")
		end
		if wrapper.entity:hasAttr("stamina") then
			local element = wrapper.entity:valueOfAttr("stamina")
			local value = 0
			if element:isFloat() then
				value = element:asFloat()
			end
			wrapper.staminaBar:setProgress(value)
			wrapper.staminaBar:setTooltipText("Stamina: " .. value * 100 .. "/100")
		end
		if wrapper.entity:hasAttr("strength") then
			local element = wrapper.entity:valueOfAttr("strength")
			local value = 0
			if element:isFloat() then
				value = element:asFloat()
			end
			wrapper.strengthBar:setProgress(value)
			wrapper.strengthBar:setTooltipText("Strength: " .. value * 100 .. "/100")
		end
	end
	
	wrapper.setEntity = function(entity)
		wrapper.entity = entity
		if entity == nil then
			wrapper.widget:hide()
		else
			if entity:getName() == "" then
				wrapper.nameWindow:setText(entity:getType():getName())
			else
				wrapper.nameWindow:setText(entity:getName())
			end
			wrapper.updateStatus()
			
			--don't cast the world
			--HACK: we need to refactor the entity classes so we don't have to do it like this
			if entity:getId() ~= 0 then
				local physEntity = tolua.cast(entity, "EmberOgre::EmberPhysicalEntity")
				wrapper.renderer:showModel(physEntity:getModel():getDefinition():get():getName())
				wrapper.renderer:setCameraDistance(0.75)
			else 
				wrapper.renderer:showModel("")
			end
			wrapper.renderer:updateRender()
-- 			wrapper.renderer:pitch(Ogre.Degree:new_local(-45))
			wrapper.widget:show()

		end
	end
	wrapper.widget:hide()
	return wrapper
end

function Status.buildWidget()

--[[	Status.widget = guiManager:createWidget()
	Status.widget:loadMainSheet("Status.layout", "Status/")
	
	Status.healthBar = CEGUI.toProgressBar(Status.widget:getWindow("HealthBar"))
	Status.staminaBar = CEGUI.toProgressBar(Status.widget:getWindow("StaminaBar"))
	Status.strengthBar = CEGUI.toProgressBar(Status.widget:getWindow("StrengthBar"))
	
-- 	Status.healthBar:setProgress(0.5)
-- 	Status.staminaBar:setProgress(0.5)

	Status.nameWindow = Status.widget:getWindow("EntityName")]]
	
	Status.avatarStatus = Status.createStatusInstance("avatar")
	
	Status.npcStatus = Status.createStatusInstance("npc")
	local uPosition = CEGUI.UVector2:new_local(CEGUI.UDim(0,200), CEGUI.UDim(0,0))
	Status.npcStatus.widget:getMainWindow():setPosition(uPosition )
	
	connect(Status.connectors, emberOgre.EventCreatedAvatarEntity, "Status.emberOgre_CreatedAvatarEntity")

--	Tasks.widget:hide()

end

function Status.emberOgre_CreatedAvatarEntity(avatarEntity)
--wire up the listening for status
	connect(Status.connectors, guiManager.EventEntityAction, "Status.handleAction")
	connect(Status.connectors, avatarEntity.Changed, "Status.entity_Changed")
	Status.avatarStatus.setEntity(avatarEntity)
end

function Status.handleAction(action, entity) 

	if action == "use" or action == "inspect" then
		Status.observerNpc(entity)
	end
end

function Status.observerNpc(entity)
	Status.npcStatus.setEntity(entity)
	if Status.connectors.npcChangedConnector ~= nil then
		Status.connectors.npcChangedConnector:disconnect()
	end
	if entity ~= nil then
		Status.connectors.npcChangedConnector = EmberOgre.LuaConnector:new_local(entity.Changed):connect("Status.npc_Changed")
		Status.connectors.npcConnector = EmberOgre.LuaConnector:new_local(entity.BeingDeleted):connect("Status.npc_BeingDeleted")
	end
end

function Status.entity_Changed(keys)
	Status.avatarStatus.updateStatus()
end

function Status.npc_Changed(keys)
	Status.npcStatus.updateStatus()
end

function Status.npc_BeingDeleted()
	Status.observerNpc(nil)
end
Status.buildWidget()