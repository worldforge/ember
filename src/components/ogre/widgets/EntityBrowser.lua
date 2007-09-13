--Allows the editing of entities

EntityBrowser = {connectors={}}
--EntityBrowser.connectors = {}
EntityBrowser.listbox = nil

EntityBrowser.sceneNodes = {}
EntityBrowser.sceneNodes.listbox = nil
EntityBrowser.sceneNodes.selectedSceneNode = nil

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

function EntityBrowser.SceneNodesList_SelectionChanged(args)
	local item = EntityBrowser.sceneNodes.listbox:getFirstSelectedItem()
	if item ~= nil then
		--we've stored the sceneNode in the user data (we should perhaps store the key instead, and then do a look up, in case the scene node has been removed in the interim)
		local sceneNode = item:getUserData()
		sceneNode = tolua.cast(sceneNode, "Ogre::Node")
		EntityBrowser.sceneNodes.selectedSceneNode = sceneNode
		EntityBrowser.updateSceneNodeInfo(sceneNode)
--		local positionInfo = "x: " .. sceneNode:getPosition().x .. " y: " .. sceneNode:getPosition().y .. " z: " .. sceneNode:getPosition().z
--		EntityBrowser.sceneNodes.nodeInfo:setText(positionInfo);
	end
end

function EntityBrowser.updateSceneNodeInfo(sceneNode)
	EntityBrowser.sceneNodes.positionAdapter:updateGui(sceneNode:getPosition())
	EntityBrowser.sceneNodes.rotationAdapter:updateGui(sceneNode:getOrientation())
end

function EntityBrowser.sceneNodes_positionAdapter_changed()
	EntityBrowser.sceneNodes.selectedSceneNode:setPosition(EntityBrowser.sceneNodes.positionAdapter:getValue())
end

function EntityBrowser.sceneNodes_rotationAdapter_changed()
	EntityBrowser.sceneNodes.selectedSceneNode:setOrientation(EntityBrowser.sceneNodes.rotationAdapter:getValue())
end

function EntityBrowser.refreshSceneNodes()
	EntityBrowser.sceneNodes.listholder:resetList()
	EntityBrowser.addSceneNode(emberOgre:getRootSceneNode(), 0)
end

function EntityBrowser.addSceneNode(sceneNode, level)
--	if entity ~= nil then
		local label = ""
		for i = 0, level  do
			label = label .. "-"
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
			label = label .. "-"
		end	
		label = label .. entity:getName() .. " (" .. entity:getType():getName() .. ")"
		
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
	EntityBrowser.listholder = EmberOgre.Gui.ListHolder:new_local(EntityBrowser.listbox, EntityBrowser.filter)
	EntityBrowser.widget:getWindow("Refresh"):subscribeEvent("MouseClick", "EntityBrowser.Refresh_MouseClick")

	
	--the ogre scene nodes part
	EntityBrowser.sceneNodes.listbox = CEGUI.toListbox(EntityBrowser.widget:getWindow("SceneNodesList"))
	EntityBrowser.sceneNodes.listbox:subscribeEvent("ItemSelectionChanged", "EntityBrowser.SceneNodesList_SelectionChanged")
	EntityBrowser.sceneNodes.nodeInfo = EntityBrowser.widget:getWindow("SceneNodeInfo")
	EntityBrowser.sceneNodes.filter = CEGUI.toEditbox(EntityBrowser.widget:getWindow("FilterSceneNodes"))
	EntityBrowser.sceneNodes.listholder = EmberOgre.Gui.ListHolder:new_local(EntityBrowser.sceneNodes.listbox, EntityBrowser.sceneNodes.filter)
	EntityBrowser.widget:getWindow("RefreshSceneNodes"):subscribeEvent("MouseClick", "EntityBrowser.RefreshSceneNodes_MouseClick")


	EntityBrowser.widget:registerConsoleVisibilityToggleCommand("entityBrowser")
	EntityBrowser.widget:enableCloseButton()
	EntityBrowser.widget:hide()

	local xW = EntityBrowser.widget:getWindow("SceneNodeInfo/Position/X")
	local yW = EntityBrowser.widget:getWindow("SceneNodeInfo/Position/Y")
	local zW = EntityBrowser.widget:getWindow("SceneNodeInfo/Position/Z")
	EntityBrowser.sceneNodes.positionAdapter = EmberOgre.Gui.Vector3Adapter:new_local(xW, yW ,zW)
	connect(EntityBrowser.connectors, EntityBrowser.sceneNodes.positionAdapter.EventValueChanged, "EntityBrowser.sceneNodes_positionAdapter_changed")
	
	local xW = EntityBrowser.widget:getWindow("SceneNodeInfo/Orientation/X")
	local yW = EntityBrowser.widget:getWindow("SceneNodeInfo/Orientation/Y")
	local zW = EntityBrowser.widget:getWindow("SceneNodeInfo/Orientation/Z")
	local degreeW = EntityBrowser.widget:getWindow("SceneNodeInfo/Orientation/Scalar")
	EntityBrowser.sceneNodes.rotationAdapter = EmberOgre.Gui.QuaternionAdapter:new_local(degreeW, xW, yW ,zW)
	connect(EntityBrowser.connectors, EntityBrowser.sceneNodes.rotationAdapter.EventValueChanged, "EntityBrowser.sceneNodes_rotationAdapter_changed")


end


EntityBrowser.buildWidget()
