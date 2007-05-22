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
	
	EntityEditor.outercontainer = guiManager:createWindow("DefaultGUISheet")
	EntityEditor.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(EntityEditor.outercontainer)
	local adapter = EntityEditor.factory:createMapAdapter(EntityEditor.outercontainer, EntityEditor.entity:getId(), EntityEditor.entity)
	EntityEditor.rootMapAdapter = adapter
	EntityEditor.helper = EmberOgre.Gui.EntityEditor:new(entity, EntityEditor.rootMapAdapter)
	EntityEditor.attributesContainer:addChildWindow(EntityEditor.outercontainer)
	
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
	local newElementWrapper = EntityEditor.createNewMapElementWidget(adapter, EntityEditor.outercontainer)
	EntityEditor.outercontainer:addChildWindow(newElementWrapper.container)

end

function EntityEditor.createNewListElementWidget(listAdapter, outercontainer)
	local wrapper = {}
	wrapper.adapter = listAdapter
	wrapper.outercontainer = outercontainer
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	EntityEditor.factory:loadLayoutIntoContainer(wrapper.container, "newUnnamedElement", "adapters/atlas/ListAdapterNewElement.layout")
	wrapper.container:setHeight(CEGUI.UDim(0, 25))
	wrapper.typeCombobox = CEGUI.toCombobox(windowManager:getWindow(EntityEditor.factory:getCurrentPrefix().. "ElementType"))
	EntityEditor.fillNewElementCombobox(wrapper.typeCombobox)
	wrapper.button = CEGUI.toPushButton(windowManager:getWindow(EntityEditor.factory:getCurrentPrefix().. "NewElementButton"))
	wrapper.buttonPressed = function(args)
		local adapterWrapper = nil
		local element = nil
		if wrapper.typeCombobox:getSelectedItem():getID() == 0 then
			element = EntityEditor.helper:createStringElement()
			adapterWrapper = EntityEditor.createStringAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 1 then
			element = EntityEditor.helper:createIntElement()
			adapterWrapper = EntityEditor.createNumberAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 2 then
			element = EntityEditor.helper:createFloatElement()
			adapterWrapper = EntityEditor.createNumberAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 3 then
			element = EntityEditor.helper:createMapElement()
			adapterWrapper = EntityEditor.createMapAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 4 then
			element = EntityEditor.helper:createListElement()
			adapterWrapper = EntityEditor.createListAdapter(element)
		end
		
		if adapterWrapper ~= nil then
			wrapper.adapter:addAttributeAdapter(adapterWrapper.adapter, adapterWrapper.outercontainer)
			EntityEditor.addUnNamedAdapterContainer(adapterWrapper.adapter, adapterWrapper.container, wrapper.outercontainer)
			--by adding the window again we make sure that it's at the bottom of the child window list
			wrapper.outercontainer:addChildWindow(wrapper.container)
		end
	end
	wrapper.button:subscribeEvent("MouseClick", wrapper.buttonPressed)

	return wrapper
end

function EntityEditor.createNewMapElementWidget(mapAdapter, outercontainer)
	local wrapper = {}
	wrapper.adapter = mapAdapter
	wrapper.outercontainer = outercontainer
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	EntityEditor.factory:loadLayoutIntoContainer(wrapper.container, "newNamedElement", "adapters/atlas/MapAdapterNewElement.layout")
	wrapper.container:setHeight(CEGUI.UDim(0, 25))
	wrapper.typeCombobox = CEGUI.toCombobox(windowManager:getWindow(EntityEditor.factory:getCurrentPrefix().. "ElementType"))
	EntityEditor.fillNewElementCombobox(wrapper.typeCombobox)
	wrapper.nameEditbox = CEGUI.toEditbox(windowManager:getWindow(EntityEditor.factory:getCurrentPrefix().. "ElementName"))
	wrapper.button = CEGUI.toPushButton(windowManager:getWindow(EntityEditor.factory:getCurrentPrefix().. "NewElementButton"))
	wrapper.buttonPressed = function(args)
		local adapterWrapper = nil
		local element = nil
		if wrapper.typeCombobox:getSelectedItem():getID() == 0 then
			element = EntityEditor.helper:createStringElement()
			adapterWrapper = EntityEditor.createStringAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 1 then
			element = EntityEditor.helper:createIntElement()
			adapterWrapper = EntityEditor.createNumberAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 2 then
			element = EntityEditor.helper:createFloatElement()
			adapterWrapper = EntityEditor.createNumberAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 3 then
			element = EntityEditor.helper:createMapElement()
			adapterWrapper = EntityEditor.createMapAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 4 then
			element = EntityEditor.helper:createListElement()
			adapterWrapper = EntityEditor.createListAdapter(element)
		end
		
		if adapterWrapper ~= nil then
			local name = wrapper.nameEditbox:getText()
			wrapper.adapter:addAttributeAdapter(name, adapterWrapper.adapter, adapterWrapper.outercontainer)
			EntityEditor.addNamedAdapterContainer(name, adapterWrapper.adapter, adapterWrapper.container, wrapper.outercontainer)
			--by adding the window again we make sure that it's at the bottom of the child window list
			wrapper.outercontainer:addChildWindow(wrapper.container)
		end
	end
	wrapper.button:subscribeEvent("MouseClick", wrapper.buttonPressed)

	return wrapper
end

function EntityEditor.createAdapter(attributeName, element)
	if EntityEditor.hiddenAttributes[attributeName] == nil then
		if attributeName == 'bbox' then
			return EntityEditor.createSizeAdapter(element)
		elseif attributeName == 'pos' then
			return EntityEditor.createPositionAdapter(element)
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
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
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
	
	local newElementWrapper = EntityEditor.createNewMapElementWidget(wrapper.adapter, wrapper.container)
	wrapper.container:addChildWindow(newElementWrapper.container)
	return wrapper	
end

function EntityEditor.createListAdapter(element)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
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
	
	local newElementWrapper = EntityEditor.createNewListElementWidget(wrapper.adapter, wrapper.container)
	wrapper.container:addChildWindow(newElementWrapper.container)
	
	return wrapper	
end

function EntityEditor.createStringAdapter(element)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createStringAdapter(wrapper.container, EntityEditor.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createNumberAdapter(element)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createNumberAdapter(wrapper.container, EntityEditor.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createSizeAdapter(element)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createSizeAdapter(wrapper.container, EntityEditor.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createPositionAdapter(element)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createPositionAdapter(wrapper.container, EntityEditor.entity:getId(), element)
	return wrapper	
end


function EntityEditor.addUnNamedAdapterContainer(adapter, container, parentContainer)
	local outercontainer = guiManager:createWindow("DefaultGUISheet")
	
	local deleteButton = EntityEditor.createDeleteButton("list")
	deleteButton:setProperty("UnifiedPosition", "{{0,0},{0,2}}")
	deleteButton:setProperty("Tooltip", "Delete list item");
	local deleteButtonWidth = 16
	
	function removeAdapter(args)
		adapter:remove()
		outercontainer:setAlpha(0.2)
	end
	deleteButton:subscribeEvent("MouseClick", removeAdapter)
	
	local width = container:getWidth()
	--increase with delete button width
	width = width + CEGUI.UDim(0, deleteButtonWidth)
	outercontainer:setWidth(width)
	
	outercontainer:setHeight(container:getHeight())
	
	container:setXPosition(CEGUI.UDim(0, deleteButtonWidth))
	
	--make sure that the outer container has the same height as the inner container (so that when we add new child adapters it's updated)
	function syncWindowHeights(args)
		outercontainer:setHeight(container:getHeight())
	end
	container:subscribeEvent("Sized", syncWindowHeights)
	
	outercontainer:addChildWindow(deleteButton)
	outercontainer:addChildWindow(container)

	parentContainer:addChildWindow(outercontainer)
	return outercontainer
end

function EntityEditor.addNamedAdapterContainer(attributeName, adapter, container, parentContainer)
	local textWidth = 75
	local outercontainer = guiManager:createWindow("DefaultGUISheet")
	local label = guiManager:createWindow("EmberLook/StaticText")
	
	local deleteButton = EntityEditor.createDeleteButton(attributeName)
	deleteButton:setProperty("UnifiedPosition", "{{1,-16},{0,2}}")
	deleteButton:setProperty("Tooltip", "Delete '" .. attributeName .. "'");
	
	function removeAdapter(args)
		adapter:remove()
		outercontainer:setAlpha(0.2)
	end
	deleteButton:subscribeEvent("MouseClick", removeAdapter)
	
	
	label:setText(attributeName)
	label:setWidth(CEGUI.UDim(0, textWidth))
	label:setProperty("FrameEnabled", "false");
 	label:setProperty("BackgroundEnabled", "false");
	label:setProperty("VertFormatting", "TopAligned");
	label:setProperty("Tooltip", attributeName);
	
	local width = container:getWidth()
	width = width + CEGUI.UDim(0, textWidth)
	outercontainer:setWidth(width)
	container:setXPosition(CEGUI.UDim(0, textWidth))
	container:setProperty("Tooltip", attributeName);
	
	outercontainer:setHeight(container:getHeight())
	
	--make sure that the outer container has the same height as the inner container (so that when we add new child adapters it's updated)
	function syncWindowHeights(args)
		outercontainer:setHeight(container:getHeight())
	end
	container:subscribeEvent("Sized", syncWindowHeights)
	
	
	label:addChildWindow(deleteButton)
	outercontainer:addChildWindow(container)
	outercontainer:addChildWindow(label)

	parentContainer:addChildWindow(outercontainer)
	return outercontainer
end

function EntityEditor.createDeleteButton(attributeName)
	local deleteButton = guiManager:createWindow("EmberLook/SystemButton")
	deleteButton:setProperty("NormalImage", "set:EmberLook image:CloseButtonNormal")
	deleteButton:setProperty("HoverImage", "set:EmberLook image:CloseButtonHover")
	deleteButton:setProperty("PushedImage", "set:EmberLook image:CloseButtonPushed")
	deleteButton:setProperty("UnifiedSize", "{{0,16},{0,16}}")
	return deleteButton
end

function EntityEditor.fillNewElementCombobox(combobox)
	local item = nil
	item = EmberOgre.ColoredListItem:new("String", 0)
	combobox:addItem(item)
	item = EmberOgre.ColoredListItem:new("Integer", 1)
	combobox:addItem(item)
	item = EmberOgre.ColoredListItem:new("Float", 2)
	combobox:addItem(item)
	item = EmberOgre.ColoredListItem:new("Map", 3)
	combobox:addItem(item)
	item = EmberOgre.ColoredListItem:new("List", 4)
	combobox:addItem(item)
	combobox:setHeight(CEGUI.UDim(0, 100))
	combobox:setProperty("ReadOnly", "true")
	combobox:getDropList():setProperty("ClippedByParent", "false")
end



function EntityEditor.Submit_MouseClick(args)
	EntityEditor.helper:submitChanges()
	EntityEditor.editEntity(EntityEditor.entity)
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
