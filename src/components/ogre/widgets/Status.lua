Status = {connectors={}}

function Status.buildWidget()

	Status.widget = guiManager:createWidget()
	Status.widget:loadMainSheet("Status.layout", "Status/")
	
	Status.healthBar = CEGUI.toProgressBar(Status.widget:getWindow("HealthBar"))
	Status.staminaBar = CEGUI.toProgressBar(Status.widget:getWindow("StaminaBar"))
	Status.strengthBar = CEGUI.toProgressBar(Status.widget:getWindow("StrengthBar"))
	
-- 	Status.healthBar:setProgress(0.5)
-- 	Status.staminaBar:setProgress(0.5)

	Status.nameWindow = Status.widget:getWindow("EntityName")
	
	connect(Status.connectors, emberOgre.EventCreatedAvatarEntity, "Status.emberOgre_CreatedAvatarEntity")

--	Tasks.widget:hide()

end

function Status.emberOgre_CreatedAvatarEntity(avatarEntity)
--wire up the listening for status
	connect(Status.connectors, avatarEntity.Changed, "Status.entity_Changed")
	Status.entity = avatarEntity
	Status.nameWindow:setText(avatarEntity:getName())
	Status.widget:show()
end

function Status.entity_Changed(keys)
	if Status.entity:hasAttr("status") then
		local statusElement = Status.entity:valueOfAttr("status")
		local status = 0
		if statusElement:isFloat() then
			status = statusElement:asFloat()
		end
		Status.healthBar:setProgress(status)
		Status.healthBar:setTooltipText("Health: " .. status)
	end
	if Status.entity:hasAttr("stamina") then
		local element = Status.entity:valueOfAttr("stamina")
		local value = 0
		if element:isFloat() then
			value = element:asFloat()
		end
		Status.staminaBar:setProgress(value)
		Status.staminaBar:setTooltipText("Stamina: " .. value)
	end
	if Status.entity:hasAttr("strength") then
		local element = Status.entity:valueOfAttr("strength")
		local value = 0
		if element:isFloat() then
			value = element:asFloat()
		end
		Status.strengthBar:setProgress(value)
		Status.strengthBar:setTooltipText("Strength: " .. value)
	end
end


Status.buildWidget()