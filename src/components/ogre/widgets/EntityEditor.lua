--Allows the editing of entities

EntityEditor = {connectors={}}
EntityEditor.instance = {}
EntityEditor.instance.stackableContainers = {}
EntityEditor.instance.entity = nil
EntityEditor.instance.rootMapAdapter = nil
EntityEditor.instance.helper = nil
EntityEditor.instance.newElements = {}
EntityEditor.instance.deleteListener = nil
EntityEditor.factory = nil
EntityEditor.attributesContainer = nil
EntityEditor.prototypes = 
{
	external = {
		readonly = true
	},
	parents = {
		nodelete = true,
		readonly = true
	},
	objtype = {
		type = "static",
		nodelete = true
	},
	velocity = {
		hidden = true
	},
	stamp = {
		hidden = true,
		nodelete = true
	},
	name = {
		nodelete = true
	},
	bbox = {
		type = "size",
		nodelete = true
	},
	pos = {
		type = "position",
		nodelete = true
	},
	orientation = {
		type = "orientation",
		nodelete = true
	},
	area = {
		type = "area",
	},
	points = {
		type = "points"
	},
	style = {
		suggestions = {
			"gnarly",
			"knotted",
			"weathered"
		}
	}
}

EntityEditor.modelTab = {}


function editEntity(id)
	local entity = emberOgre:getEmberEntity(id)
	if entity ~= nil then
		EntityEditor.editEntity(entity)
	end
end

function EntityEditor.createStackableContainer(container)
	local stackableContainer = EmberOgre.Gui.StackableContainer:new_local(container)
	stackableContainer:setInnerContainerWindow(container)
	EntityEditor.instance.stackableContainers[container:getName()] = stackableContainer
	return stackableContainer
end

function EntityEditor.clearEditing()
	if EntityEditor.instance ~= nil then
		
		if EntityEditor.instance.entity ~= nil then
			--as we're not editing anymore, hide the bounding boxes
			EntityEditor.instance.entity:showOgreBoundingBox(false)
			EntityEditor.instance.entity:showErisBoundingBox(false)
		end
		
		--we want to disconnect all stackable containers before we start
		for index,value in ipairs(EntityEditor.instance.stackableContainers) do
			value:disconnect()
		end
	
		if EntityEditor.instance.outercontainer ~= nil then
			windowManager:destroyWindow(EntityEditor.instance.outercontainer)
		end
		if EntityEditor.instance.deleteListener ~= nil then
			EntityEditor.instance.deleteListener:disconnect()
		end
		if EntityEditor.instance.entityChangeConnection ~= nil then
			EntityEditor.instance.entityChangeConnection:disconnect()
		end
		EntityEditor.instance = nil
	end
	EntityEditor.instance = {}
	EntityEditor.instance.stackableContainers = {}
	EntityEditor.instance.newElements = {}
	EntityEditor.instance.addNewElement = function(element) 
		table.insert(EntityEditor.instance.newElements,element)
	end
end

function EntityEditor.editEntity(entity)
	EntityEditor.widget:show()

	EntityEditor.clearEditing()
	
	EntityEditor.instance.entity = entity
	
	--show the bounding boxes by default when editing
	EntityEditor.instance.entity:showOgreBoundingBox(false)
	EntityEditor.instance.entity:showErisBoundingBox(true)
	
	EntityEditor.instance.deleteListener = EmberOgre.LuaConnector:new_local(entity.BeingDeleted):connect("EntityEditor.Entity_BeingDeleted")
	
	
	EntityEditor.refreshChildren(entity)
	EntityEditor.refreshModelInfo(entity)
	
	
	EntityEditor.instance.entityChangeConnection = EmberOgre.LuaConnector:new_local(entity.Changed):connect("EntityEditor.Entity_Changed")
	EntityEditor.instance.outercontainer = guiManager:createWindow("DefaultGUISheet")
	local adapter = EntityEditor.factory:createMapAdapter(EntityEditor.instance.outercontainer, EntityEditor.instance.entity:getId(), EntityEditor.instance.entity)
	EntityEditor.instance.rootMapAdapter = adapter
	EntityEditor.instance.helper = EmberOgre.Gui.EntityEditor:new_local(entity, EntityEditor.instance.rootMapAdapter)
	EntityEditor.attributesContainer:addChildWindow(EntityEditor.instance.outercontainer)
	
	local attributeNames = EntityEditor.instance.rootMapAdapter:getAttributeNames()
	for i = 0, attributeNames:size() - 1 do
		local name = attributeNames[i]
		local element = EntityEditor.instance.rootMapAdapter:valueOfAttr(name)
		local prototype = EntityEditor.getPrototype(name, element)
		--there's currently no way to delete from the root attributes, so we'll just disallow that
		prototype.nodelete = true
		local adapterWrapper = EntityEditor.createAdapterFromPrototype(element, prototype)
		if adapterWrapper ~= nil then
			EntityEditor.instance.rootMapAdapter:addAttributeAdapter(name, adapterWrapper.adapter, adapterWrapper.outercontainer)
			EntityEditor.addNamedAdapterContainer(name, adapterWrapper.adapter, adapterWrapper.container, EntityEditor.instance.outercontainer, adapterWrapper.prototype)
		end
	end
	local newElementWrapper = EntityEditor.createNewMapElementWidget(adapter, EntityEditor.instance.outercontainer)
	EntityEditor.instance.outercontainer:addChildWindow(newElementWrapper.container)
	EntityEditor.createStackableContainer(EntityEditor.instance.outercontainer):repositionWindows()

	EntityEditor.infoWindow:setText('Id: ' .. entity:getId() .. ' Name: ' .. entity:getName())
	
end

function EntityEditor.createNewListElementWidget(listAdapter, outercontainer, prototype)
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
			adapterWrapper = EntityEditor.createStringAdapter(element, EntityEditor.getPrototype("", element))
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 1 then
			element = EntityEditor.instance.helper:createIntElement()
			adapterWrapper = EntityEditor.createNumberAdapter(element, EntityEditor.getPrototype("", element))
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 2 then
			element = EntityEditor.instance.helper:createFloatElement()
			adapterWrapper = EntityEditor.createNumberAdapter(element, EntityEditor.getPrototype("", element))
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 3 then
			element = EntityEditor.instance.helper:createMapElement()
			adapterWrapper = EntityEditor.createMapAdapter(element, EntityEditor.getPrototype("", element))
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 4 then
			element = EntityEditor.instance.helper:createListElement()
			adapterWrapper = EntityEditor.createListAdapter(element, EntityEditor.getPrototype("", element))
		end
		
		--store a reference to the element so it isn't garbage collected
		EntityEditor.instance.addNewElement(element)
		
		if adapterWrapper ~= nil then
			wrapper.adapter:addAttributeAdapter(adapterWrapper.adapter, adapterWrapper.outercontainer)
			local newPrototype = {}
			EntityEditor.addUnNamedAdapterContainer(adapterWrapper.adapter, adapterWrapper.container, wrapper.outercontainer, newPrototype)
			--by adding the window again we make sure that it's at the bottom of the child window list
			wrapper.outercontainer:addChildWindow(wrapper.container)
		end
	end
	wrapper.button:subscribeEvent("Clicked", wrapper.buttonPressed)

	return wrapper
end

function EntityEditor.createNewPointsElementWidget(listAdapter, outercontainer)
	local wrapper = {}
	wrapper.adapter = listAdapter
	wrapper.outercontainer = outercontainer
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	EntityEditor.factory:loadLayoutIntoContainer(wrapper.container, "newUnnamedElement", "adapters/atlas/ListAdapterNewElement.layout")
	wrapper.container:setHeight(CEGUI.UDim(0, 25))
	wrapper.typeCombobox = CEGUI.toCombobox(windowManager:getWindow(EntityEditor.factory:getCurrentPrefix().. "ElementType"))
	
	local item = EmberOgre.Gui.ColouredListItem:new("Point", 0)
	wrapper.typeCombobox:addItem(item)
	wrapper.typeCombobox:setHeight(CEGUI.UDim(0, 100))
	--combobox:setProperty("ReadOnly", "true")
	
	wrapper.button = CEGUI.toPushButton(windowManager:getWindow(EntityEditor.factory:getCurrentPrefix().. "NewElementButton"))
	wrapper.buttonPressed = function(args)
		local adapterWrapper = nil
		local element = nil
		if wrapper.typeCombobox:getSelectedItem():getID() == 0 then
			element = EntityEditor.instance.helper:createPosition2dElement()
			adapterWrapper = EntityEditor.createPosition2DAdapter(element, EntityEditor.getPrototype("", element))
		end
		
		EntityEditor.instance.addNewElement(element)
		
		if adapterWrapper ~= nil then
			local newPrototype = {}
			wrapper.adapter:addAttributeAdapter(adapterWrapper.adapter, adapterWrapper.outercontainer)
			EntityEditor.addUnNamedAdapterContainer(adapterWrapper.adapter, adapterWrapper.container, wrapper.outercontainer, newPrototype)
			--by adding the window again we make sure that it's at the bottom of the child window list
			wrapper.outercontainer:addChildWindow(wrapper.container)
		end
	end
	wrapper.button:subscribeEvent("Clicked", wrapper.buttonPressed)

	return wrapper
end

function EntityEditor.createNewMapElementWidget(mapAdapter, outercontainer, prototype)
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
		local name = wrapper.nameEditbox:getText()
		if wrapper.typeCombobox:getSelectedItem():getID() == 0 then
			element = EntityEditor.instance.helper:createStringElement()
			adapterWrapper = EntityEditor.createStringAdapter(element, EntityEditor.getPrototype(name, element))
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 1 then
			element = EntityEditor.instance.helper:createIntElement()
			adapterWrapper = EntityEditor.createNumberAdapter(element, EntityEditor.getPrototype(name, element))
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 2 then
			element = EntityEditor.instance.helper:createFloatElement()
			adapterWrapper = EntityEditor.createNumberAdapter(element, EntityEditor.getPrototype(name, element))
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 3 then
			element = EntityEditor.instance.helper:createMapElement()
			adapterWrapper = EntityEditor.createMapAdapter(element, EntityEditor.getPrototype(name, element))
		elseif wrapper.typeCombobox:getSelectedItem():getID() == 4 then
			element = EntityEditor.instance.helper:createListElement()
			adapterWrapper = EntityEditor.createListAdapter(element, EntityEditor.getPrototype(name, element))
		end
		
		EntityEditor.instance.addNewElement(element)
		
		if adapterWrapper ~= nil then
			local newPrototype = {}
			wrapper.adapter:addAttributeAdapter(name, adapterWrapper.adapter, adapterWrapper.outercontainer)
			EntityEditor.addNamedAdapterContainer(name, adapterWrapper.adapter, adapterWrapper.container, wrapper.outercontainer, newPrototype)
			--by adding the window again we make sure that it's at the bottom of the child window list
			wrapper.outercontainer:addChildWindow(wrapper.container)
		end
	end
	wrapper.button:subscribeEvent("Clicked", wrapper.buttonPressed)

	return wrapper
end

function EntityEditor.createAdapter(attributeName, element)
	local prototype = EntityEditor.getPrototype(attributeName, element)
	return EntityEditor.createAdapterFromPrototype(element, prototype)
end

function EntityEditor.createAdapterFromPrototype(element, prototype)
	local adapterWrapper = nil
	if prototype.hidden == nil then
		if prototype.type == "map" then
			adapterWrapper = EntityEditor.createMapAdapter(element, prototype)
		elseif prototype.type == "list" then
			adapterWrapper = EntityEditor.createListAdapter(element, prototype)
		elseif prototype.type == "static" or prototype.readonly ~= nil then
			adapterWrapper = EntityEditor.createStaticAdapter(element, prototype)
		elseif prototype.type == 'size' then
			adapterWrapper = EntityEditor.createSizeAdapter(element, prototype)
		elseif prototype.type == 'pos' then
			adapterWrapper = EntityEditor.createPositionAdapter(element, prototype)
		elseif prototype.type == 'pos2d' then
			adapterWrapper = EntityEditor.createPosition2DAdapter(element, prototype)
		elseif prototype.type == 'orientation' then
			adapterWrapper = EntityEditor.createOrientationAdapter(element, prototype)
		elseif prototype.type == 'points' then
			adapterWrapper = EntityEditor.createPointsAdapter(element, prototype)
		elseif prototype.type == "string" then
			adapterWrapper = EntityEditor.createStringAdapter(element, prototype)
		elseif prototype.type == "number" then
			adapterWrapper = EntityEditor.createNumberAdapter(element, prototype)
		elseif prototype.type == "area" then
			adapterWrapper = EntityEditor.createAreaAdapter(element, prototype)
		end
		if adapterWrapper ~= nil then
			if prototype.suggestions ~= nil then
				for index,value in ipairs(prototype.suggestions) do
					adapterWrapper.adapter:addSuggestion(value)
				end
			end
			adapterWrapper.prototype = prototype
		end
	end
	return adapterWrapper
end

function EntityEditor.getPrototype(attributeName, element)
	local prototype = {}
	if EntityEditor.prototypes[attributeName] ~= nil then
		 prototype = EntityEditor.prototypes[attributeName]
	end
	if prototype.type == nil then
		if element:isString() then
			prototype.type = "string"
		elseif element:isNum() then
			prototype.type = "number"
		elseif element:isMap() then
			prototype.type = "map"
		elseif element:isList() then
			prototype.type = "list"
		end
	end
	return prototype
end

function EntityEditor.createMapAdapter(element, prototype)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createMapAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	if wrapper.adapter == nil then
		return nil
	end
	
	local attributeNames = wrapper.adapter:getAttributeNames()
	for i = 0, attributeNames:size() - 1 do
		local name = attributeNames[i]
		local childElement = wrapper.adapter:valueOfAttr(name)
		local adapterWrapper = EntityEditor.createAdapter(name, childElement)
		if adapterWrapper ~= nil then
			if adapterWrapper.adapter ~= nil then
				EntityEditor.addNamedAdapterContainer(name, adapterWrapper.adapter, adapterWrapper.container, wrapper.container, adapterWrapper.prototype)
				wrapper.adapter:addAttributeAdapter(name, adapterWrapper.adapter, adapterWrapper.outercontainer)
			end
		end
	end
	
	if prototype.readonly == nil then
		local newElementWrapper = EntityEditor.createNewMapElementWidget(wrapper.adapter, wrapper.container)
		wrapper.container:addChildWindow(newElementWrapper.container)
	end
	EntityEditor.createStackableContainer(wrapper.container):repositionWindows()
	return wrapper	
end

function EntityEditor.createListAdapter(element, prototype)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createListAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	if wrapper.adapter == nil then
		return nil
	end
	for i = 0, wrapper.adapter:getSize() - 1 do
		local childElement = wrapper.adapter:valueOfAttr(i)
		local childPrototype = EntityEditor.getPrototype("", childElement)
		--if the prototype for the list have it marked as nodelete, mark the childelements too
		if prototype.readonly ~= nil then
			childPrototype.readonly = true
			childPrototype.nodelete = true
		end
		local adapterWrapper = EntityEditor.createAdapterFromPrototype(childElement, childPrototype)
		if adapterWrapper ~= nil then
			EntityEditor.addUnNamedAdapterContainer(adapterWrapper.adapter, adapterWrapper.container, wrapper.container, adapterWrapper.prototype)
			wrapper.adapter:addAttributeAdapter(adapterWrapper.adapter, adapterWrapper.outercontainer)
		end
	end	
	
	if prototype.readonly == nil then
		local newElementWrapper = EntityEditor.createNewListElementWidget(wrapper.adapter, wrapper.container)
		wrapper.container:addChildWindow(newElementWrapper.container)
	end
	EntityEditor.createStackableContainer(wrapper.container):repositionWindows()
	
	return wrapper	
end

function EntityEditor.createPointsAdapter(element, prototype)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createListAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	if wrapper.adapter == nil then
		return nil
	end
	for i = 0, wrapper.adapter:getSize() - 1 do
		local childElement = wrapper.adapter:valueOfAttr(i)
		local childPrototype = {type = "pos2d"}
		local adapterWrapper = EntityEditor.createAdapterFromPrototype(childElement, childPrototype)
		if adapterWrapper ~= nil then
			EntityEditor.addUnNamedAdapterContainer(adapterWrapper.adapter, adapterWrapper.container, wrapper.container, adapterWrapper.prototype)
			wrapper.adapter:addAttributeAdapter(adapterWrapper.adapter, adapterWrapper.outercontainer)
		end
	end	
	
	local newElementWrapper = EntityEditor.createNewPointsElementWidget(wrapper.adapter, wrapper.container)
	wrapper.container:addChildWindow(newElementWrapper.container)
	EntityEditor.createStackableContainer(wrapper.container):repositionWindows()
	
	return wrapper	
end


function EntityEditor.createStaticAdapter(element, prototype)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createStaticAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createStringAdapter(element, prototype)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createStringAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
-- 	wrapper.adapter:addSuggestion("test")
	return wrapper	
end

function EntityEditor.createNumberAdapter(element, prototype)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createNumberAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createSizeAdapter(element, prototype)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createSizeAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createPositionAdapter(element, prototype)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createPositionAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createPosition2DAdapter(element, prototype)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createPosition2DAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createOrientationAdapter(element, prototype)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createOrientationAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element)
	return wrapper	
end

function EntityEditor.createAreaAdapter(element, prototype)
	local wrapper = {}
	wrapper.container = guiManager:createWindow("DefaultGUISheet")
	wrapper.adapter = EntityEditor.factory:createAreaAdapter(wrapper.container, EntityEditor.instance.entity:getId(), element, EntityEditor.instance.entity)
	return wrapper	
end

function EntityEditor.addUnNamedAdapterContainer(adapter, container, parentContainer, prototype)
	local outercontainer = guiManager:createWindow("DefaultGUISheet")
	
	local deleteButton = nil
	local deleteButtonWidth = 0
	if prototype.nodelete == nil then
		deleteButton = EntityEditor.createDeleteButton("list")
		deleteButton:setProperty("UnifiedPosition", "{{0,0},{0,2}}")
		deleteButton:setProperty("Tooltip", "Delete list item");
		deleteButtonWidth = 16
		
		function removeAdapter(args)
			adapter:remove()
			outercontainer:setAlpha(0.2)
		end
		deleteButton:subscribeEvent("Clicked", removeAdapter)
	end
		
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
	
	if deleteButton ~= nil then
		outercontainer:addChildWindow(deleteButton)
	end
	outercontainer:addChildWindow(container)

	parentContainer:addChildWindow(outercontainer)
	return outercontainer
end

function EntityEditor.addNamedAdapterContainer(attributeName, adapter, container, parentContainer, prototype)
	local textWidth = 75
	local outercontainer = guiManager:createWindow("DefaultGUISheet")
	--outercontainer:setRiseOnClickEnabled(false)
	local label = guiManager:createWindow("EmberLook/StaticText")
	

	
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
	
	if prototype.nodelete == nil then
	
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
		deleteButton:subscribeEvent("Clicked", removeAdapter)
		
		label:addChildWindow(deleteButton)
	end
	
	outercontainer:addChildWindow(label)
	outercontainer:addChildWindow(container)

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
	--combobox:getDropList():setProperty("ClippedByParent", "false")
end



function EntityEditor.Submit_Clicked(args)
	EntityEditor.instance.helper:submitChanges()
	--we want to update the next time a change comes from the server
	EntityEditor.listenForChanges = true
	--EntityEditor.editEntity(EntityEditor.instance.entity)
	return true
end

function EntityEditor.DeleteButton_Clicked(args)
	if EntityEditor.instance ~= nil then
		local entity = EntityEditor.instance.entity
		if entity ~= nil then
		-- 	EntityEditor.clearEditing()
			emberServices:getServerService():deleteEntity(entity)
		end
	end
	return true
end

function EntityEditor.ExportButton_Clicked(args)
	emberOgre:getEntityFactory():dumpAttributesOfEntity(EntityEditor.instance.entity:getId())
	return true
end

function EntityEditor.RefreshButton_Clicked(args)
	if EntityEditor.instance.entity ~= nil then
		EntityEditor.editEntity(EntityEditor.instance.entity)
	end
	return true
end

function EntityEditor.ShowOgreBbox_CheckStateChanged(args)
	if EntityEditor.instance.entity ~= nil then
		EntityEditor.instance.entity:showOgreBoundingBox(EntityEditor.modelTab.showOgreBbox:isSelected())
	end
	return true
end

function EntityEditor.ShowErisBbox_CheckStateChanged(args)
	if EntityEditor.instance.entity ~= nil then
		EntityEditor.instance.entity:showErisBoundingBox(EntityEditor.modelTab.showErisBbox:isSelected())
	end
	return true
end

function EntityEditor.ChildList_MouseDoubleClick(args)
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
	if numContained ~= 0 then
		for i = 0, numContained - 1 do
			local childEntity = entity:getContained(i)
			local label = childEntity:getName()
			
			local item = EmberOgre.Gui.ColouredListItem:new(label, childEntity:getId(), childEntity)
			EntityEditor.childListholder:addItem(item)
		end
	end
end

function EntityEditor.refreshModelInfo(entity)
	EntityEditor.modelTab.showOgreBbox:setSelected(entity:getShowOgreBoundingBox())
	EntityEditor.modelTab.showErisBbox:setSelected(entity:getShowErisBoundingBox())
	
--[[	local physEntity = entity
	tolua.cast(physEntity, "const EmberOgre::EmberPhysicalEntity")
	
	local modelDef = physEntity:getModel():getDefinition()
	local modelInfoText = "Model name: " .. physEntity:getModel():getName()
	--modelInfoText = modelInfoText .. "\nModel type: " .. modelDef:getName()
	--modelInfoText = modelInfoText .. "\nOgre node: " .. physEntity:getModel():getName()
	EntityEditor.modelTab.modelInfo:setText(modelInfoText)]]
end

function EntityEditor.Entity_Changed(attributes)
	--only update if we're actively listening (for example right after an update)
	if EntityEditor.listenForChanges then
		EntityEditor.listenForChanges = false
		EntityEditor.editEntity(EntityEditor.instance.entity)
	end
end

--we need to clean up when the entity is deleted, so we don't cause segfaults when trying to access a null ref
function EntityEditor.Entity_BeingDeleted()
	EntityEditor.clearEditing()
end


function EntityEditor.buildWidget()

	EntityEditor.factory = EmberOgre.Gui.Adapters.Atlas.AdapterFactory:new_local("EntityEditor")
	
	EntityEditor.widget = guiManager:createWidget()
	EntityEditor.widget:loadMainSheet("EntityEditor.layout", "EntityEditor/")
	
	EntityEditor.attributesContainer = EntityEditor.widget:getWindow("AttributesContainer")
	EntityEditor.infoWindow = EntityEditor.widget:getWindow("EntityInfo")
	
	EntityEditor.childlistbox = CEGUI.toListbox(EntityEditor.widget:getWindow("ChildList"))
	--EntityBrowser.childlistbox:subscribeEvent("ItemSelectionChanged", "EntityBrowser.EntityList_SelectionChanged")
	
	EntityEditor.childlistFilter = CEGUI.toEditbox(EntityEditor.widget:getWindow("FilterChildren"))
	EntityEditor.childListholder = EmberOgre.Gui.ListHolder:new_local(EntityEditor.childlistbox, EntityEditor.childlistFilter)
	
--[[	EntityEditor.modelTab.stackableWindow = EntityEditor.widget:getWindow("ModelPanelStackable")
	EntityEditor.modelTab.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(EntityEditor.modelTab.stackableWindow)
	EntityEditor.modelTab.stackableContainer:setInnerContainerWindow(EntityEditor.modelTab.stackableWindow)]]
	EntityEditor.modelTab.showOgreBbox = CEGUI.toCheckbox(EntityEditor.widget:getWindow("ShowOgreBbox"))
	EntityEditor.modelTab.showErisBbox = CEGUI.toCheckbox(EntityEditor.widget:getWindow("ShowErisBbox"))
	EntityEditor.modelTab.modelInfo = EntityEditor.widget:getWindow("ModelInfo")
	
	
	connect(EntityEditor.connectors, guiManager.EventEntityAction, "EntityEditor.handleAction")
	
	
	--EntityEditor.attributeStackableContainer = EmberOgre.Gui.StackableContainer:new_local(EntityEditor.attributesContainer)
	EntityEditor.widget:registerConsoleVisibilityToggleCommand("entityEditor")
	EntityEditor.widget:enableCloseButton()
	EntityEditor.widget:hide()

end


EntityEditor.buildWidget()
