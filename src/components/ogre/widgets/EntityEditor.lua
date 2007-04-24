--Allows the editing of entities

EntityEditor = {}
EntityEditor.entity = nil
EntityEditor.adapters = {}
EntityEditor.helper = nil
EntityEditor.factory = nil
EntityEditor.attributesContainer = nil
EntityEditor.accumulatedHeight = 0
EntityEditor.stackableContainer = 0
EntityEditor.hiddenAttributes = {objtype = 1, stamp = 1}

function editEntity(id)
	local entity = emberOgre:getEntity(id)
	EntityEditor.editEntity(entity)
end

function EntityEditor.editEntity(entity)
	EntityEditor.widget:show()
	EntityEditor.entity = entity
	if EntityEditor.helper ~= nil then
		EntityEditor.helper:delete()
	end
	EntityEditor.helper = EmberOgre.Gui.EntityEditor:new(entity)
	EntityEditor.factory = EmberOgre.Gui.Adapters.Atlas.AdapterFactory:new_local(entity:getId())
	EntityEditor.infoWindow:setText('Id: ' .. entity:getId() .. ' Name: ' .. entity:getName())
	local attributeNames = EntityEditor.helper:getAttributeNames()
	for i = 0, attributeNames:size() - 1 do
		local name = attributeNames[i]
		local element = entity:valueOfAttr(name)
		EntityEditor.createAdapter(name, element, EntityEditor.attributesContainer)
	end	
end

function EntityEditor.createAdapter(attributeName, element, parentContainer)
	if EntityEditor.hiddenAttributes[attributeName] == nil then
		if attributeName == 'bbox' then
			local adapter = EntityEditor.createSizeAdapter(attributeName, element, parentContainer)
		elseif element:isString() then
			local adapter = EntityEditor.createStringAdapter(attributeName, element, parentContainer)
		elseif element:isNum() then
			local adapter = EntityEditor.createNumberAdapter(attributeName, element, parentContainer)
		end
	end
end

function EntityEditor.createStringAdapter(attributeName, element, parentContainer)
	local container = windowManager:createWindow("DefaultGUISheet", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName)
	local adapter = EntityEditor.factory:createStringAdapter(container, EntityEditor.entity:getId(), element)
	EntityEditor.adapters[attributeName] = adapter
	EntityEditor.addAdapterContainer(attributeName, adapter, container, parentContainer)
	return adapter	
end

function EntityEditor.createNumberAdapter(attributeName, element, parentContainer)
	local container = windowManager:createWindow("DefaultGUISheet", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName)
	local adapter = EntityEditor.factory:createNumberAdapter(container, EntityEditor.entity:getId(), element)
	EntityEditor.adapters[attributeName] = adapter
	EntityEditor.addAdapterContainer(attributeName, adapter, container, parentContainer)
	return adapter	
end

function 	EntityEditor.createSizeAdapter(attributeName, element, parentContainer)
	local container = windowManager:createWindow("DefaultGUISheet", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName)
	local adapter = EntityEditor.factory:createSizeAdapter(container, EntityEditor.entity:getId(), element)
	EntityEditor.adapters[attributeName] = adapter
	EntityEditor.addAdapterContainer(attributeName, adapter, container, parentContainer)
	return adapter	
end



function EntityEditor.addAdapterContainer(attributeName, adapter, container, parentContainer)
	local outercontainer = windowManager:createWindow("DefaultGUISheet", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName .. "outercontainer")
	local label = windowManager:createWindow("EmberLook/StaticText", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName .. "outercontainerLabel")
	label:setText(attributeName)
	label:setWidth(CEGUI.UDim(0, 100))
	label:setProperty("FrameEnabled", "false");
 	label:setProperty("BackgroundEnabled", "false");
	label:setProperty("VertFormatting", "TopAligned");
	
	local width = container:getWidth()
	width = width + CEGUI.UDim(0, 100)
	outercontainer:setWidth(width)
	container:setXPosition(CEGUI.UDim(0, 100))
	
	outercontainer:setHeight(container:getHeight())
	
	outercontainer:addChildWindow(container)
	outercontainer:addChildWindow(label)

	parentContainer:addChildWindow(outercontainer)
--	EntityEditor.attributesContainer:addChildWindow(outercontainer)
	EntityEditor.helper:addAttributeAdapter(attributeName, adapter, outercontainer)
	--outercontainer:setYPosition(CEGUI.UDim(0, EntityEditor.accumulatedHeight))
	--EntityEditor.accumulatedHeight = EntityEditor.accumulatedHeight + outercontainer:getHeight():asAbsolute(0)
end

function EntityEditor.Submit_MouseClick(args)
	EntityEditor.helper:submitChanges()
end

function EntityEditor.DeleteButton_MouseClick(args)
	emberServices:getServerService():deleteEntity(EntityEditor.entity)
end

function EntityEditor.handleAction(action, entity) 

	if action == "edit" then
		EntityEditor.editEntity(entity)
	end
end



function EntityEditor.buildWidget()

	EntityEditor.widget = guiManager:createWidget()
	EntityEditor.widget:loadMainSheet("EntityEditor.layout", "EntityEditor/")
	
	EntityEditor.attributesContainer = EntityEditor.widget:getWindow("AttributesContainer")
	--EntityEditor.attributesContainer = CEGUI.toScrollablePane(EntityEditor.attributesContainer):getContentPane()
	EntityEditor.infoWindow = EntityEditor.widget:getWindow("EntityInfo")
	
	
	EntityEditor.widget:getWindow("Submit"):subscribeEvent("MouseClick", "EntityEditor.Submit_MouseClick")
	EntityEditor.widget:getWindow("DeleteButton"):subscribeEvent("MouseClick", "EntityEditor.DeleteButton_MouseClick")
	
	
	EmberOgre.LuaConnector:new(guiManager.EventEntityAction):connect("EntityEditor.handleAction")
	
	
	EntityEditor.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(EntityEditor.attributesContainer)
	EntityEditor.widget:registerConsoleVisibilityToggleCommand("entityEditor")
	EntityEditor.widget:enableCloseButton()
	EntityEditor.widget:hide()

end


EntityEditor.buildWidget()
