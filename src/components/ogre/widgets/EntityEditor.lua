--Allows the editing of entities

EntityEditor = {}
EntityEditor.entity = nil
EntityEditor.rootMapAdapter = nil
EntityEditor.helper = nil
EntityEditor.factory = nil
EntityEditor.attributesContainer = nil
EntityEditor.accumulatedHeight = 0
EntityEditor.stackableContainer = 0
EntityEditor.hiddenAttributes = {objtype = 1, stamp = 1}
EntityEditor.windowCounter = 0

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
	if EntityEditor.outercontainer ~= nil then
		windowManager:destroyWindow(EntityEditor.outercontainer)
	end
	EntityEditor.factory = EmberOgre.Gui.Adapters.Atlas.AdapterFactory:new_local(entity:getId())
	EntityEditor.infoWindow:setText('Id: ' .. entity:getId() .. ' Name: ' .. entity:getName())
	
	local container = windowManager:createWindow("DefaultGUISheet", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. EntityEditor.windowCounter)
	EntityEditor.windowCounter = EntityEditor.windowCounter + 1
	local adapter = EntityEditor.factory:createMapAdapter(container, EntityEditor.entity:getId(), EntityEditor.entity)
	EntityEditor.rootMapAdapter = adapter
	EntityEditor.helper = EmberOgre.Gui.EntityEditor:new(entity, EntityEditor.rootMapAdapter)
	EntityEditor.outercontainer = EntityEditor.addAdapterContainer("", adapter, container, EntityEditor.attributesContainer)
	

	
	local attributeNames = EntityEditor.rootMapAdapter:getAttributeNames()
	for i = 0, attributeNames:size() - 1 do
		local name = attributeNames[i]
		local element = EntityEditor.rootMapAdapter:valueOfAttr(name)
		local adapterWrapper = EntityEditor.createAdapter(name, element, EntityEditor.attributesContainer)
		if adapterWrapper ~= nil then
			EntityEditor.rootMapAdapter:addAttributeAdapter(name, adapterWrapper.adapter, adapterWrapper.outercontainer)
		end
	end	
end

function EntityEditor.createAdapter(attributeName, element, parentContainer)
	if EntityEditor.hiddenAttributes[attributeName] == nil then
		if attributeName == 'bbox' then
			return EntityEditor.createSizeAdapter(attributeName, element, parentContainer)
		elseif element:isString() then
			return EntityEditor.createStringAdapter(attributeName, element, parentContainer)
		elseif element:isNum() then
			return EntityEditor.createNumberAdapter(attributeName, element, parentContainer)
		elseif element:isMap() then
			return EntityEditor.createMapAdapter(attributeName, element, parentContainer)
		elseif element:isList() then
			return EntityEditor.createListAdapter(attributeName, element, parentContainer)
		end
	end
end

function EntityEditor.createMapAdapter(attributeName, element, parentContainer)
	local wrapper = {}
	local container = windowManager:createWindow("DefaultGUISheet", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName .. EntityEditor.windowCounter)
	EntityEditor.windowCounter = EntityEditor.windowCounter + 1
	wrapper.adapter = EntityEditor.factory:createMapAdapter(container, EntityEditor.entity:getId(), element)
	--TODO: make sure that this is cleaned up at destruction
	wrapper.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(container)
	local attributeNames = wrapper.adapter:getAttributeNames()
	for i = 0, attributeNames:size() - 1 do
		local name = attributeNames[i]
		local childElement = wrapper.adapter:valueOfAttr(name)
		local adapterWrapper = EntityEditor.createAdapter(name, childElement, container)
		if adapterWrapper ~= nil then
			wrapper.adapter:addAttributeAdapter(name, adapterWrapper.adapter, adapterWrapper.outercontainer)
		end
	end	
	wrapper.outercontainer = EntityEditor.addAdapterContainer(attributeName, adapter, container, parentContainer)
	return wrapper	
end

function EntityEditor.createListAdapter(attributeName, element, parentContainer)
	local wrapper = {}
	local container = windowManager:createWindow("DefaultGUISheet", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName .. EntityEditor.windowCounter)
	EntityEditor.windowCounter = EntityEditor.windowCounter + 1
	wrapper.adapter = EntityEditor.factory:createListAdapter(container, EntityEditor.entity:getId(), element)
	--TODO: make sure that this is cleaned up at destruction
	wrapper.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(container)
	for i = 0, wrapper.adapter:getSize() - 1 do
		local childElement = wrapper.adapter:valueOfAttr(i)
		local adapterWrapper = EntityEditor.createAdapter(i, childElement, container)
		if adapterWrapper ~= nil then
			wrapper.adapter:addAttributeAdapter(adapterWrapper.adapter, adapterWrapper.outercontainer)
		end
	end	
	wrapper.outercontainer = EntityEditor.addAdapterContainer(attributeName, adapter, container, parentContainer)
	return wrapper	
end

function EntityEditor.createStringAdapter(attributeName, element, parentContainer)
	local wrapper = {}
	local container = windowManager:createWindow("DefaultGUISheet", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName .. EntityEditor.windowCounter)
	EntityEditor.windowCounter = EntityEditor.windowCounter + 1
	wrapper.adapter = EntityEditor.factory:createStringAdapter(container, EntityEditor.entity:getId(), element)
	--EntityEditor.adapters[attributeName] = adapter
	wrapper.outercontainer = EntityEditor.addAdapterContainer(attributeName, adapter, container, parentContainer)
	return wrapper	
end

function EntityEditor.createNumberAdapter(attributeName, element, parentContainer)
	local wrapper = {}
	local container = windowManager:createWindow("DefaultGUISheet", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName .. EntityEditor.windowCounter)
	EntityEditor.windowCounter = EntityEditor.windowCounter + 1
	wrapper.adapter = EntityEditor.factory:createNumberAdapter(container, EntityEditor.entity:getId(), element)
	--EntityEditor.adapters[attributeName] = adapter
	wrapper.outercontainer = EntityEditor.addAdapterContainer(attributeName, adapter, container, parentContainer)
	return wrapper	
end

function EntityEditor.createSizeAdapter(attributeName, element, parentContainer)
	local wrapper = {}
	local container = windowManager:createWindow("DefaultGUISheet", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName .. EntityEditor.windowCounter)
	EntityEditor.windowCounter = EntityEditor.windowCounter + 1
	wrapper.adapter = EntityEditor.factory:createSizeAdapter(container, EntityEditor.entity:getId(), element)
	--EntityEditor.adapters[attributeName] = adapter
	wrapper.outercontainer = EntityEditor.addAdapterContainer(attributeName, wrapper.adapter, container, parentContainer)
	return wrapper	
end



function EntityEditor.addAdapterContainer(attributeName, adapter, container, parentContainer)
	local textWidth = 75
	local outercontainer = windowManager:createWindow("DefaultGUISheet", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName .. "outercontainer" .. EntityEditor.windowCounter)
	EntityEditor.windowCounter = EntityEditor.windowCounter + 1
	local label = windowManager:createWindow("EmberLook/StaticText", "EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName .. "outercontainerLabel" .. EntityEditor.windowCounter)
	EntityEditor.windowCounter = EntityEditor.windowCounter + 1
	label:setText(attributeName)
	label:setWidth(CEGUI.UDim(0, textWidth))
	label:setProperty("FrameEnabled", "false");
 	label:setProperty("BackgroundEnabled", "false");
	label:setProperty("VertFormatting", "TopAligned");
	
	local width = container:getWidth()
	width = width + CEGUI.UDim(0, textWidth)
	outercontainer:setWidth(width)
	container:setXPosition(CEGUI.UDim(0, textWidth))
	
	outercontainer:setHeight(container:getHeight())
	
	outercontainer:addChildWindow(container)
	outercontainer:addChildWindow(label)

	parentContainer:addChildWindow(outercontainer)
	return outercontainer
--	EntityEditor.attributesContainer:addChildWindow(outercontainer)
	--EntityEditor.helper:addAttributeAdapter(attributeName, adapter, outercontainer)
	--outercontainer:setYPosition(CEGUI.UDim(0, EntityEditor.accumulatedHeight))
	--EntityEditor.accumulatedHeight = EntityEditor.accumulatedHeight + outercontainer:getHeight():asAbsolute(0)
end

function EntityEditor.Submit_MouseClick(args)
	EntityEditor.helper:submitChanges()
end

function EntityEditor.DeleteButton_MouseClick(args)
	emberServices:getServerService():deleteEntity(EntityEditor.entity)
end

function EntityEditor.ExportButton_MouseClick(args)
	emberOgre:getEntityFactory():dumpAttributesOfEntity(EntityEditor.entity:getId())
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
	EntityEditor.widget:getWindow("ExportButton"):subscribeEvent("MouseClick", "EntityEditor.ExportButton_MouseClick")
	
	
	EmberOgre.LuaConnector:new(guiManager.EventEntityAction):connect("EntityEditor.handleAction")
	
	
	EntityEditor.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(EntityEditor.attributesContainer)
	EntityEditor.widget:registerConsoleVisibilityToggleCommand("entityEditor")
	EntityEditor.widget:enableCloseButton()
	EntityEditor.widget:hide()

end


EntityEditor.buildWidget()
