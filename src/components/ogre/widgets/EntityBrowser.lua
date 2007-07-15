--Allows the editing of entities

EntityBrowser = {}
EntityBrowser.connectors = {}
EntityBrowser.listbox = nil

EntityBrowser.sceneNodes = {}
EntityBrowser.sceneNodes.listbox = nil

function EntityBrowser.Refresh_MouseClick(args)
	EntityBrowser.refresh()
end

function EntityBrowser.RefreshSceneNodes_MouseClick(args)
	EntityBrowser.refreshSceneNodes();
end

function EntityBrowser.EntityList_SelectionChanged(args)
	local item = EntityBrowser.listbox:getFirstSelectedItem()
	if item ~= nil then
		local entityId = item:getID()
		local entity = emberOgre:getEntity(entityId);
		if (entity ~= nil) then
			guiManager:EmitEntityAction("inspect", entity)
		end
	end
	
end

function EntityBrowser.refreshSceneNodes()
	EntityBrowser.sceneNodes.listholder:resetList()
	EntityBrowser.addSceneNode(emberOgre:getRootSceneNode(), 0)
end

function EntityBrowser.addSceneNode(sceneNode, level)
--	if entity ~= nil then
		local label = ""
		for i = 0, level  do
			label = label .. " "
		end	
		label = label .. sceneNode:getName()
		
		local item = EmberOgre.Gui.ColouredListItem:new(label, 0, sceneNode)
		EntityBrowser.sceneNodes.listholder:addItem(item)
	
	
		local numContained = sceneNode:numChildren()
		for i = 0, numContained - 1 do
			local chilNode = sceneNode:getChildByIndex(i)
			EntityBrowser.addSceneNode(chilNode, level + 1)
		end 
--	end
end


function EntityBrowser.refresh()
	EntityBrowser.listholder:resetList()
	EntityBrowser.addEntity(emberOgre:getEntityFactory():getWorld(), 0)
end

function EntityBrowser.addEntity(entity, level)
--	if entity ~= nil then
		local label = ""
		for i = 0, level  do
			label = label .. " "
		end	
		label = label .. entity:getName()
		
		local item = EmberOgre.Gui.ColouredListItem:new(label, entity:getId(), entity)
		EntityBrowser.listholder:addItem(item)
	
	
		local numContained = entity:numContained()
		for i = 0, numContained - 1 do
			local childEntity = entity:getContained(i)
			EntityBrowser.addEntity(childEntity, level + 1)
		end 
--	end
end

function EntityBrowser.buildWidget()

	EntityBrowser.widget = guiManager:createWidget()
	EntityBrowser.widget:loadMainSheet("EntityBrowser.layout", "EntityBrowser/")
	
	--the eris part
	EntityBrowser.listbox = CEGUI.toListbox(EntityBrowser.widget:getWindow("EntityList"))
	EntityBrowser.listbox:subscribeEvent("ItemSelectionChanged", "EntityBrowser.EntityList_SelectionChanged")
	
	EntityBrowser.filter = CEGUI.toEditbox(EntityBrowser.widget:getWindow("FilterEntities"))
	EntityBrowser.listholder = EmberOgre.ListHolder:new_local(EntityBrowser.listbox, EntityBrowser.filter)
	EntityBrowser.widget:getWindow("Refresh"):subscribeEvent("MouseClick", "EntityBrowser.Refresh_MouseClick")

	
	--the ogre scene nodes part
	EntityBrowser.sceneNodes.listbox = CEGUI.toListbox(EntityBrowser.widget:getWindow("SceneNodesList"))
	EntityBrowser.sceneNodes.listbox:subscribeEvent("ItemSelectionChanged", "EntityBrowser.SceneNodesList_SelectionChanged")
	EntityBrowser.sceneNodes.filter = CEGUI.toEditbox(EntityBrowser.widget:getWindow("FilterSceneNodes"))
	EntityBrowser.sceneNodes.listholder = EmberOgre.ListHolder:new_local(EntityBrowser.sceneNodes.listbox, EntityBrowser.sceneNodes.filter)
	EntityBrowser.widget:getWindow("RefreshSceneNodes"):subscribeEvent("MouseClick", "EntityBrowser.RefreshSceneNodes_MouseClick")


	EntityBrowser.widget:registerConsoleVisibilityToggleCommand("entityBrowser")
	EntityBrowser.widget:enableCloseButton()
	EntityBrowser.widget:hide()

end


EntityBrowser.buildWidget()
