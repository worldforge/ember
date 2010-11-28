--Allows the editing of entities

EntityBrowser = {}

function EntityBrowser:Refresh_Clicked(args)
	self:refresh()
end

function EntityBrowser:RefreshSceneNodes_Clicked(args)
	self:refreshSceneNodes();
end

function EntityBrowser:EntityList_SelectionChanged(args)
	local item = self.listbox:getFirstSelectedItem()
	if item ~= nil then
		local entityId = item:getID()
		local entity = emberOgre:getWorld():getEmberEntity(entityId);
		if (entity ~= nil) then
			--if we're admin, edit, else just inspect
			if emberOgre:getWorld():getAvatar():isAdmin() then
				guiManager:EmitEntityAction("edit", entity)
			else
				guiManager:EmitEntityAction("inspect", entity)
			end
		end
	end
	
end

function EntityBrowser:SceneNodesList_SelectionChanged(args)
	local item = self.sceneNodes.listbox:getFirstSelectedItem()
	if item ~= nil then
		--we've stored the sceneNode in the user data (we should perhaps store the key instead, and then do a look up, in case the scene node has been removed in the interim)
		local sceneNode = item:getUserData()
		sceneNode = tolua.cast(sceneNode, "Ogre::Node")
		self.sceneNodes.selectedSceneNode = sceneNode
		self:updateSceneNodeInfo(sceneNode)
--		local positionInfo = "x: " .. sceneNode:getPosition().x .. " y: " .. sceneNode:getPosition().y .. " z: " .. sceneNode:getPosition().z
--		self.sceneNodes.nodeInfo:setText(positionInfo);
	end
end

function EntityBrowser:updateSceneNodeInfo(sceneNode)
	self.sceneNodes.positionAdapter:updateGui(sceneNode:getPosition())
	self.sceneNodes.rotationAdapter:updateGui(sceneNode:getOrientation())
	self.sceneNodes.scaleAdapter:updateGui(sceneNode:getScale())
end

function EntityBrowser:sceneNodes_positionAdapter_changed()
	self.sceneNodes.selectedSceneNode:setPosition(self.sceneNodes.positionAdapter:getValue())
end

function EntityBrowser:sceneNodes_rotationAdapter_changed()
	self.sceneNodes.selectedSceneNode:setOrientation(self.sceneNodes.rotationAdapter:getValue())
end

function EntityBrowser:sceneNodes_scaleAdapter_changed()
	self.sceneNodes.selectedSceneNode:setScale(self.sceneNodes.scaleAdapter:getValue())
end

function EntityBrowser:refreshSceneNodes()
	local world = emberOgre:getWorld()
	
	if world then
		local rootNode = world:getSceneManager():getRootSceneNode()
		self.sceneNodes.listholder:resetList()
		self:addSceneNode(rootNode, 0)
	end
end

function EntityBrowser:addSceneNode(sceneNode, level)
--	if entity ~= nil then
		local label = ""
		for i = 0, level  do
			label = label .. "-"
		end	
		label = label .. sceneNode:getName()
		
		local item = Ember.OgreView.Gui.ColouredListItem:new(label, 0, sceneNode)
		self.sceneNodes.listholder:addItem(item)
	
	
		local numContained = sceneNode:numChildren()
		for i = 0, numContained - 1 do
			local chilNode = sceneNode:getChildByIndex(i)
			self:addSceneNode(chilNode, level + 1)
		end 
--	end
end


function EntityBrowser:refresh()
	self.listholder:resetList()
	local world = emberOgre:getWorld()
	if world ~= nil then
		self:addEntity(world:getEntityFactory():getWorld(), 0)
	end
end

function EntityBrowser:addEntity(entity, level)
	if entity ~= nil then
		local label = ""
		for i = 0, level  do
			label = label .. "-"
		end	
		label = label .. entity:getName() .. " (" .. entity:getType():getName() .. ")"
		
		local item = Ember.OgreView.Gui.ColouredListItem:new(label, entity:getId(), entity)
		self.listholder:addItem(item)
	
	
		local numContained = entity:numContained()
		for i = 0, numContained - 1 do
			local childEntity = entity:getContained(i)
			self:addEntity(childEntity, level + 1)
		end 
	end
end

function EntityBrowser:buildWidget()

	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("EntityBrowser.layout", "EntityBrowser/")
	
	--the eris part
	self.listbox = CEGUI.toListbox(self.widget:getWindow("EntityList"))
	self.listbox:subscribeEvent("ItemSelectionChanged", self.EntityList_SelectionChanged, self)
	
	self.filter = CEGUI.toEditbox(self.widget:getWindow("FilterEntities"))
	self.listholder = Ember.OgreView.Gui.ListHolder:new(self.listbox, self.filter)
	self.widget:getWindow("Refresh"):subscribeEvent("Clicked", self.Refresh_Clicked, self)

	
	--the ogre scene nodes part
	self.sceneNodes.listbox = CEGUI.toListbox(self.widget:getWindow("SceneNodesList"))
	self.sceneNodes.listbox:subscribeEvent("ItemSelectionChanged", self.SceneNodesList_SelectionChanged, self)
	self.sceneNodes.nodeInfo = self.widget:getWindow("SceneNodeInfo")
	self.sceneNodes.filter = CEGUI.toEditbox(self.widget:getWindow("FilterSceneNodes"))
	self.sceneNodes.listholder = Ember.OgreView.Gui.ListHolder:new(self.sceneNodes.listbox, self.sceneNodes.filter)
	self.widget:getWindow("RefreshSceneNodes"):subscribeEvent("Clicked", self.RefreshSceneNodes_Clicked, self)


	self.widget:registerConsoleVisibilityToggleCommand("entityBrowser")
	self.widget:enableCloseButton()

	local xW = self.widget:getWindow("SceneNodeInfo/Position/X")
	local yW = self.widget:getWindow("SceneNodeInfo/Position/Y")
	local zW = self.widget:getWindow("SceneNodeInfo/Position/Z")
	self.sceneNodes.positionAdapter = Ember.OgreView.Gui.Vector3Adapter:new(xW, yW ,zW)
	connect(self.connectors, self.sceneNodes.positionAdapter.EventValueChanged, self.sceneNodes_positionAdapter_changed, self)
	
	local xW = self.widget:getWindow("SceneNodeInfo/Orientation/X")
	local yW = self.widget:getWindow("SceneNodeInfo/Orientation/Y")
	local zW = self.widget:getWindow("SceneNodeInfo/Orientation/Z")
	local degreeW = self.widget:getWindow("SceneNodeInfo/Orientation/Scalar")
	self.sceneNodes.rotationAdapter = Ember.OgreView.Gui.QuaternionAdapter:new(degreeW, xW, yW ,zW)
	connect(self.connectors, self.sceneNodes.rotationAdapter.EventValueChanged, self.sceneNodes_rotationAdapter_changed, self)

	local xW = self.widget:getWindow("SceneNodeInfo/Scale/X")
	local yW = self.widget:getWindow("SceneNodeInfo/Scale/Y")
	local zW = self.widget:getWindow("SceneNodeInfo/Scale/Z")
	self.sceneNodes.scaleAdapter = Ember.OgreView.Gui.Vector3Adapter:new(xW, yW ,zW)
	connect(self.connectors, self.sceneNodes.scaleAdapter.EventValueChanged, self.sceneNodes_scaleAdapter_changed, self)
	
	local sizeW = self.widget:getWindow("SceneNodeInfo/Scale/SizeText")
	
	--force a refresh the first time the window is shown
	connect(self.connectors, self.widget.EventFirstTimeShown, self.refresh, self)


end

function EntityBrowser:shutdown()
	deleteSafe(self.sceneNodes.scaleAdapter)
	deleteSafe(self.sceneNodes.rotationAdapter)
	deleteSafe(self.sceneNodes.positionAdapter)
	deleteSafe(self.sceneNodes.listholder)
	deleteSafe(self.listholder)

	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end


connect(connectors, emberOgre.EventGUIManagerInitialized, function(guiManager)
	local entityBrowser = {connectors = {}, listbox = nil, sceneNodes = {listbox = nil, selectedSceneNode = nil} }
	setmetatable(entityBrowser, {__index = EntityBrowser})
	
	entityBrowser:buildWidget()

	connect(entityBrowser.connectors, emberOgre.EventGUIManagerBeingDestroyed, function()
		entityBrowser:shutdown()
		entityBrowser = nil
	end)
end)


