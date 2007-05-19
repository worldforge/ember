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
	EntityEditor.infoWindow:setText('Id: ' .. entity:getId() .. ' Name: ' .. entity:getName())
	
	EntityEditor.outercontainer = windowManager:createWindow("DefaultGUISheet", EntityEditor.windowName("EntityEditor" .. EntityEditor.entity:getId()))
	EntityEditor.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(EntityEditor.outercontainer)
	local adapter = EntityEditor.factory:createMapAdapter(EntityEditor.outercontainer, EntityEditor.entity:getId(), EntityEditor.entity)
	EntityEditor.rootMapAdapter = adapter
	EntityEditor.helper = EmberOgre.Gui.EntityEditor:new(entity, EntityEditor.rootMapAdapter)
	EntityEditor.attributesContainer:addChildWindow(EntityEditor.outercontainer)
	--EntityEditor.outercontainer = EntityEditor.addUnNamedAdapterContainer(adapter, container, EntityEditor.attributesContainer)
	

	
	local attributeNames = EntityEditor.rootMapAdapter:getAttributeNames()
	for i = 0, attributeNames:size() - 1 do
		local name = attributeNames[i]
		local element = EntityEditor.rootMapAdapter:valueOfAttr(name)
		local adapterWrapper = EntityEditor.createAdapter(name, element)
		if adapterWrapper ~= nil then
			EntityEditor.rootMapAdapter:addAttributeAdapter(name, adapterWrapper.adapter, adapterWrapper.outercontainer)
			EntityEditor.addNamedAdapterContainer(name, adapterWrapper.adapter, adapterWrapper.container, EntityEditor.outercontainer)
		end
	end	
end

function EntityEditor.createAdapter(attributeName, element)
	if EntityEditor.hiddenAttributes[attributeName] == nil then
		if attributeName == 'bbox' then
			return EntityEditor.createSizeAdapter(element)
		elseif element:isString() then
			return EntityEditor.createStringAdapter(element)
		elseif element:isNum() then
			return EntityEditor.createNumberAdapter(element)
		elseif element:isMap() then
			return EntityEditor.createMapAdapter(element)
		elseif element:isList() then
			return EntityEditor.createListAdapter(element)
		end
	end
end

function EntityEditor.createMapAdapter(element)
	local wrapper = {}
	wrapper.container = windowManager:createWindow("DefaultGUISheet", EntityEditor.windowName("EntityEditor" .. EntityEditor.entity:getId()))
	wrapper.adapter = EntityEditor.factory:createMapAdapter(wrapper.container, EntityEditor.entity:getId(), element)
	--TODO: make sure that this is cleaned up at destruction
	wrapper.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(wrapper.container)
	local attributeNames = wrapper.adapter:getAttributeNames()
	for i = 0, attributeNames:size() - 1 do
		local name = attributeNames[i]
		local childElement = wrapper.adapter:valueOfAttr(name)
		local adapterWrapper = EntityEditor.createAdapter(name, childElement)
		if adapterWrapper ~= nil then
			if adapterWrapper.adapter ~= nil then
				EntityEditor.addNamedAdapterContainer(name, adapterWrapper.adapter, adapterWrapper.container, wrapper.container)
				wrapper.adapter:addAttributeAdapter(name, adapterWrapper.adapter, adapterWrapper.outercontainer)
			end
		end
	end	
	return wrapper	
end

function EntityEditor.createListAdapter(element)
	local wrapper = {}
	wrapper.container = windowManager:createWindow("DefaultGUISheet", EntityEditor.windowName("EntityEditor" .. EntityEditor.entity:getId()))
	wrapper.adapter = EntityEditor.factory:createListAdapter(wrapper.container, EntityEditor.entity:getId(), element)
	--TODO: make sure that this is cleaned up at destruction
	wrapper.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(wrapper.container)
	for i = 0, wrapper.adapter:getSize() - 1 do
		local childElement = wrapper.adapter:valueOfAttr(i)
		local adapterWrapper = EntityEditor.createAdapter("", childElement)
		if adapterWrapper ~= nil then
			EntityEditor.addUnNamedAdapterContainer(adapterWrapper.adapter, adapterWrapper.container, wrapper.container)
			wrapper.adapter:addAttributeAdapter(adapterWrapper.adapter, adapterWrapper.outercontainer)
		end
	end	
	return wrapper	
end

function EntityEditor.createStringAdapter(element)
	local wrapper = {}
	wrapper.container = windowManager:createWindow("DefaultGUISheet", EntityEditor.windowName("EntityEditor" .. EntityEditor.entity:getId()))
	wrapper.adapter = EntityEditor.factory:createStringAdapter(wrapper.container, EntityEditor.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createNumberAdapter(element)
	local wrapper = {}
	wrapper.container = windowManager:createWindow("DefaultGUISheet", EntityEditor.windowName("EntityEditor" .. EntityEditor.entity:getId()))
	wrapper.adapter = EntityEditor.factory:createNumberAdapter(wrapper.container, EntityEditor.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createSizeAdapter(element)
	local wrapper = {}
	wrapper.container = windowManager:createWindow("DefaultGUISheet", EntityEditor.windowName("EntityEditor" .. EntityEditor.entity:getId()))
	wrapper.adapter = EntityEditor.factory:createSizeAdapter(wrapper.container, EntityEditor.entity:getId(), element)
	return wrapper	
end

function EntityEditor.windowName(windowName)
	EntityEditor.windowCounter = EntityEditor.windowCounter + 1
	return windowName .. EntityEditor.windowCounter
end

function EntityEditor.addUnNamedAdapterContainer(adapter, container, parentContainer)
	local outercontainer = windowManager:createWindow("DefaultGUISheet", EntityEditor.windowName("EntityEditor" .. EntityEditor.entity:getId() .. "_" .. "outercontainer"))
	
	local width = container:getWidth()
	outercontainer:setWidth(width)
	
	outercontainer:setHeight(container:getHeight())
	
	outercontainer:addChildWindow(container)

	parentContainer:addChildWindow(outercontainer)
	return outercontainer
--	EntityEditor.attributesContainer:addChildWindow(outercontainer)
	--EntityEditor.helper:addAttributeAdapter(attributeName, adapter, outercontainer)
	--outercontainer:setYPosition(CEGUI.UDim(0, EntityEditor.accumulatedHeight))
	--EntityEditor.accumulatedHeight = EntityEditor.accumulatedHeight + outercontainer:getHeight():asAbsolute(0)
end


function EntityEditor.addNamedAdapterContainer(attributeName, adapter, container, parentContainer)
	local textWidth = 75
	local outercontainer = windowManager:createWindow("DefaultGUISheet", EntityEditor.windowName("EntityEditor" .. EntityEditor.entity:getId() .. "_" .. attributeName .. "outercontainer"))
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

	EntityEditor.factory = EmberOgre.Gui.Adapters.Atlas.AdapterFactory:new_local("EntityEditor")
	
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
