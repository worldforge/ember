--Allows the editing of entities

EntityEditor = {}
EntityEditor.instance = {}
EntityEditor.instance.entity = nil
EntityEditor.instance.rootMapAdapter = nil
EntityEditor.instance.helper = nil
EntityEditor.factory = nil
EntityEditor.attributesContainer = nil
EntityEditor.hiddenAttributes = {objtype = 1, stamp = 1, area = 1}


function editEntity(id)
	local entity = emberOgre:getEntity(id)
	if entity ~= nil then
		EntityEditor.editEntity(entity)
	end
end

function EntityEditor.createStackableContainer(container)
	local stackableContainer = EmberOgre.Gui.StackableContainer:new(container)
	EntityEditor.instance.stackableContainers[container:getName()] = stackableContainer
	return stackableContainer
end

function EntityEditor.editEntity(entity)
	EntityEditor.widget:show()

	if EntityEditor.instance ~= nil then
		if EntityEditor.instance.outercontainer ~= nil then
			windowManager:destroyWindow(EntityEditor.instance.outercontainer)
		end
		if EntityEditor.instance.entityChangeConnection ~= nil then
			EntityEditor.instance.entityChangeConnection:disconnect()
		end
		if EntityEditor.instance.helper ~= nil then
			EntityEditor.instance.helper:delete()
		end
		EntityEditor.instance = nil
	end
	EntityEditor.instance = {}
	EntityEditor.instance.stackableContainers = {}
	EntityEditor.instance.entity = entity
	
	EntityEditor.instance.entityChangeConnection = EmberOgre.LuaConnector:new_local(entity.Changed):connect("EntityEditor.Entity_Changed")
	EntityEditor.instance.outercontainer = guiManager:createWindow("DefaultGUISheet")
	EntityEditor.createStackableContainer(EntityEditor.instance.outercontainer)
	local adapter = EntityEditor.factory:createMapAdapter(EntityEditor.instance.outercontainer, EntityEditor.instance.entity:getId(), EntityEditor.instance.entity)
	EntityEditor.instance.rootMapAdapter = adapter
	EntityEditor.instance.helper = EmberOgre.Gui.EntityEditor:new(entity, EntityEditor.instance.rootMapAdapter)
	EntityEditor.attributesContainer:addChildWindow(EntityEditor.instance.outercontainer)
	
	local attributeNames = EntityEditor.instance.rootMapAdapter:getAttributeNames()
	for i = 0, attributeNames:size() - 1 do
		local name = attributeNames[i]
		local element = EntityEditor.instance.rootMapAdapter:valueOfAttr(name)
		local adapterWrapper = EntityEditor.createAdapter(name, element)
		if adapterWrapper ~= nil then
			EntityEditor.instance.rootMapAdapter:addAttributeAdapter(name, adapterWrapper.adapter, adapterWrapper.outercontainer)
			EntityEditor.addNamedAdapterContainer(name, adapterWrapper.adapter, adapterWrapper.container, EntityEditor.instance.outercontainer)
		end
	end
	local newElementWrapper = EntityEditor.createNewMapElementWidget(adapter, EntityEditor.instance.outercontainer)
	EntityEditor.instance.outercontainer:addChildWindow(newElementWrapper.container)

	EntityEditor.infoWindow:setText('Id: ' .. entity:getId() .. ' Name: ' .. entity:getName())
	
	EntityEditor.refreshChildren(entity)

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
			element = EntityEditor.instance.helper:createStringElement()
			adapterWrapper = EntityEditor.createStringAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 1 then
			element = EntityEditor.instance.helper:createIntElement()
			adapterWrapper = EntityEditor.createNumberAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 2 then
			element = EntityEditor.instance.helper:createFloatElement()
			adapterWrapper = EntityEditor.createNumberAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 3 then
			element = EntityEditor.instance.helper:createMapElement()
			adapterWrapper = EntityEditor.createMapAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 4 then
			element = EntityEditor.instance.helper:createListElement()
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
			element = EntityEditor.instance.helper:createStringElement()
			adapterWrapper = EntityEditor.createStringAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 1 then
			element = EntityEditor.instance.helper:createIntElement()
			adapterWrapper = EntityEditor.createNumberAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 2 then
			element = EntityEditor.instance.helper:createFloatElement()
			adapterWrapper = EntityEditor.createNumberAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 3 then
			element = EntityEditor.instance.helper:createMapElement()
			adapterWrapper = EntityEditor.createMapAdapter(element)
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 4 then
			element = EntityEditor.instance.helper:createListElement()
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
		elseif attributeName == 'orientation' then
			return EntityEditor.createOrientationAdapter(element)
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
	wrapper.adapter = EntityEditor.factory:createMapAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	--TODO: make sure that this is cleaned up at destruction
	EntityEditor.createStackableContainer(wrapper.container)
	
	
	
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
	wrapper.adapter = EntityEditor.factory:createListAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	--TODO: make sure that this is cleaned up at destruction
	EntityEditor.createStackableContainer(wrapper.container)
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
	wrapper.adapter = EntityEditor.factory:createStringAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	wrapper.adapter:addSuggestion("test")
	return wrapper	
end

function EntityEditor.createNumberAdapter(element)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createNumberAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createSizeAdapter(element)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createSizeAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createPositionAdapter(element)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createPositionAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createOrientationAdapter(element)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createOrientationAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
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
	local SizedConnection = container:subscribeEvent("Sized", syncWindowHeights)
	
	outercontainer:addChildWindow(deleteButton)
	outercontainer:addChildWindow(container)

	parentContainer:addChildWindow(outercontainer)
	return outercontainer
end

function EntityEditor.addNamedAdapterContainer(attributeName, adapter, container, parentContainer)
	local textWidth = 75
	local outercontainer = guiManager:createWindow("DefaultGUISheet")
	--outercontainer:setRiseOnClickEnabled(false)
	local label = guiManager:createWindow("EmberLook/StaticText")
	
	local deleteButton = EntityEditor.createDeleteButton(attributeName)
	deleteButton:setProperty("UnifiedPosition", "{{1,-16},{0,2}}")
	deleteButton:setProperty("Tooltip", "Delete '" .. attributeName .. "'");
	
-- 	function showDeleteButton(args)
-- 		console:pushMessage("wee")
-- 		deleteButton:setVisible(true)
-- 	end
-- 	function hideDeleteButton(args)
-- 		console:pushMessage("waa")
-- 		deleteButton:setVisible(false)
-- 	end
-- 	outercontainer:subscribeEvent("MouseEnter", showDeleteButton)
-- 	outercontainer:subscribeEvent("MouseLeave", hideDeleteButton)
	
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
	local SizedConnection = container:subscribeEvent("Sized", syncWindowHeights)
	
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
	deleteButton:setAlpha(0.5)
	return deleteButton
end

function EntityEditor.fillNewElementCombobox(combobox)
	local item = nil
	item = EmberOgre.Gui.ColouredListItem:new("String", 0)
	combobox:addItem(item)
	item = EmberOgre.Gui.ColouredListItem:new("Integer", 1)
	combobox:addItem(item)
	item = EmberOgre.Gui.ColouredListItem:new("Float", 2)
	combobox:addItem(item)
	item = EmberOgre.Gui.ColouredListItem:new("Map", 3)
	combobox:addItem(item)
	item = EmberOgre.Gui.ColouredListItem:new("List", 4)
	combobox:addItem(item)
	combobox:setHeight(CEGUI.UDim(0, 100))
	combobox:setProperty("ReadOnly", "true")
	combobox:getDropList():setProperty("ClippedByParent", "false")
end



function EntityEditor.Submit_MouseClick(args)
	EntityEditor.instance.helper:submitChanges()
	--we want to update the next time a change comes from the server
	EntityEditor.listenForChanges = true
	--EntityEditor.editEntity(EntityEditor.instance.entity)
	return true
end

function EntityEditor.DeleteButton_MouseClick(args)
	emberServices:getServerService():deleteEntity(EntityEditor.instance.entity)
	return true
end

function EntityEditor.ExportButton_MouseClick(args)
	emberOgre:getEntityFactory():dumpAttributesOfEntity(EntityEditor.instance.entity:getId())
	return true
end

function EntityEditor.ShowOgreBoundingBox_MouseClick(args)
	EntityEditor.instance.entity:showOgreBoundingBox(not EntityEditor.instance.entity:getShowOgreBoundingBox())
	return true
end

function EntityEditor.ShowErisBoundingBox_MouseClick(args)
	EntityEditor.instance.entity:showErisBoundingBox(not EntityEditor.instance.entity:getShowErisBoundingBox())
	return true
end

function EntityEditor.Submit_MouseDoubleClick(args)
	local entityId = EntityEditor.childlistbox:getFirstSelectedItem():getID()
	editEntity(entityId)
	return true
end

function EntityEditor.handleAction(action, entity) 

	if action == "edit" then
		EntityEditor.editEntity(entity)
	end
end

function EntityEditor.refreshChildren(entity)
	EntityEditor.childListholder:resetList()
	local numContained = entity:numContained()
	for i = 0, numContained - 1 do
		local childEntity = entity:getContained(i)
		local label = childEntity:getName()
		
		local item = EmberOgre.Gui.ColouredListItem:new(label, childEntity:getId(), childEntity)
		EntityEditor.childListholder:addItem(item)
	end 
	
	EntityBrowser.addEntity(emberOgre:getEntityFactory():getWorld(), 0)
end

function EntityEditor.Entity_Changed(attributes)
	--only update if we're actively listening (for example right after an update)
	if EntityEditor.listenForChanges then
		EntityEditor.listenForChanges = false
		EntityEditor.editEntity(EntityEditor.instance.entity)
	end
end


function EntityEditor.buildWidget()

	EntityEditor.factory = EmberOgre.Gui.Adapters.Atlas.AdapterFactory:new_local("EntityEditor")
	
	EntityEditor.widget = guiManager:createWidget()
	EntityEditor.widget:loadMainSheet("EntityEditor.layout", "EntityEditor/")
	
	EntityEditor.attributesContainer = EntityEditor.widget:getWindow("AttributesContainer")
	--EntityEditor.attributesContainer = CEGUI.toScrollablePane(EntityEditor.attributesContainer):getContentPane()
	EntityEditor.infoWindow = EntityEditor.widget:getWindow("EntityInfo")
	
	EntityEditor.childlistbox = CEGUI.toListbox(EntityEditor.widget:getWindow("ChildList"))
	--EntityBrowser.childlistbox:subscribeEvent("ItemSelectionChanged", "EntityBrowser.EntityList_SelectionChanged")
	
	EntityEditor.childlistFilter = CEGUI.toEditbox(EntityEditor.widget:getWindow("FilterChildren"))
	EntityEditor.childListholder = EmberOgre.ListHolder:new_local(EntityEditor.childlistbox, EntityEditor.childlistFilter)
	
	
	
--[[	EntityEditor.widget:getWindow("Submit"):subscribeEvent("MouseClick", "EntityEditor.Submit_MouseClick")
	EntityEditor.widget:getWindow("DeleteButton"):subscribeEvent("MouseClick", "EntityEditor.DeleteButton_MouseClick")
	EntityEditor.widget:getWindow("ExportButton"):subscribeEvent("MouseClick", "EntityEditor.ExportButton_MouseClick")]]
	
	
	EmberOgre.LuaConnector:new(guiManager.EventEntityAction):connect("EntityEditor.handleAction")
	
	
	--EntityEditor.attributeStackableContainer = EmberOgre.Gui.StackableContainer:new_local(EntityEditor.attributesContainer)
	EntityEditor.widget:registerConsoleVisibilityToggleCommand("entityEditor")
	EntityEditor.widget:enableCloseButton()
	EntityEditor.widget:hide()

end


EntityEditor.buildWidget()
