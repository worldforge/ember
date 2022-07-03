--EntityBrowser: Allows the editing of entities

EntityBrowser = {}

function EntityBrowser:updateSceneNodeInfo(sceneNode)
	self.sceneNodes.positionAdapter:updateGui(sceneNode:getPosition())
	self.sceneNodes.rotationAdapter:updateGui(sceneNode:getOrientation())
	self.sceneNodes.scaleAdapter:updateGui(sceneNode:getScale())
	self.sceneNodes.showBoundingBox:setSelected(sceneNode:getShowBoundingBox())
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
	local label = ""
	for _ = 0, level do
		label = label .. "-"
	end
	label = label .. sceneNode:getName()

	self.sceneNodes.listholder:addItem(Ember.OgreView.Gui.ColouredListItem.new_ptr(label, 0))

	local numContained = sceneNode:numChildren()
	for i = 0, numContained - 1 do
		local chilNode = sceneNode:getChildByIndex(i)
		self:addSceneNode(chilNode, level + 1)
	end
end

function EntityBrowser:refresh()
	self.listholder:resetList()
	local world = emberOgre:getWorld()
	if world then
		self:addEntity(world:getView():getTopLevel(), 0)
	end
end

function EntityBrowser:addEntity(entity, level)
	if entity then
		local label = ""
		for _ = 0, level do
			label = label .. "-"
		end
		label = label .. entity:getName() .. " (" .. entity:getType():getName() .. ")"

		self.listholder:addItem(Ember.OgreView.Gui.ColouredListItem.new_ptr(label, tonumber(entity:getId())))

		local numContained = entity:numContained()
		for i = 0, numContained - 1 do
			local childEntity = entity:getContained(i)
			self:addEntity(childEntity, level + 1)
		end
	end
end

function EntityBrowser:buildWidget()

	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("EntityBrowser.layout", "EntityBrowser")

	--the eris part
	self.listbox = CEGUI.toListbox(self.widget:getWindow("EntityList"))
	subscribe(self.connectors, self.listbox, "SelectionChanged", function()
		local item = self.listbox:getFirstSelectedItem()
		if item then
			local entityId = item:getID()
			local entity = emberOgre:getWorld():getEmberEntity(entityId);
			if entity then
				--if we're admin, edit, else just inspect
				if emberOgre:getWorld():getAvatar():isAdmin() then
					guiManager:EmitEntityAction("edit", entity)
				else
					guiManager:EmitEntityAction("inspect", entity)
				end
			end
		end

	end)

	self.filter = CEGUI.toEditbox(self.widget:getWindow("FilterEntities"))
	self.listholder = Ember.OgreView.Gui.ListHolder.new(self.listbox, self.filter)
	subscribe(self.connectors, self.widget:getWindow("Refresh"), "Clicked", function()
		self:refresh()
		return true
	end)


	--the ogre scene nodes part
	self.sceneNodes.listbox = CEGUI.toListbox(self.widget:getWindow("SceneNodesList"))
	subscribe(self.connectors, self.sceneNodes.listbox, "SelectionChanged", function()
		local item = self.sceneNodes.listbox:getFirstSelectedItem()
		if item then
			--TODO: make this work again without storing stuff in getUserData
			----we've stored the sceneNode in the user data (we should perhaps store the key instead, and then do a look up, in case the scene node has been removed in the interim)
			--local sceneNode = item:getUserData()
			--sceneNode = tolua.cast(sceneNode, "Ogre::SceneNode")
			--self.sceneNodes.selectedSceneNode = sceneNode
			--self:updateSceneNodeInfo(sceneNode)
			----        local positionInfo = "x: " .. sceneNode:getPosition().x .. " y: " .. sceneNode:getPosition().y .. " z: " .. sceneNode:getPosition().z
			----        self.sceneNodes.nodeInfo:setText(positionInfo);
		end
	end)
	self.sceneNodes.nodeInfo = self.widget:getWindow("SceneNodeInfo")
	self.sceneNodes.filter = CEGUI.toEditbox(self.widget:getWindow("FilterSceneNodes"))
	self.sceneNodes.listholder = Ember.OgreView.Gui.ListHolder.new(self.sceneNodes.listbox, self.sceneNodes.filter)
	self.sceneNodes.showBoundingBox = CEGUI.toToggleButton(self.widget:getWindow("ShowBoundingBox"))
	subscribe(self.connectors, self.sceneNodes.showBoundingBox, "SelectStateChanged", function()
		self.sceneNodes.selectedSceneNode:showBoundingBox(self.sceneNodes.showBoundingBox:isSelected())
		return true
	end)
	subscribe(self.connectors, self.widget:getWindow("RefreshSceneNodes"), "Clicked", function()
		self:refreshSceneNodes();
		return true
	end)

	self.widget:registerConsoleVisibilityToggleCommand("entityBrowser")
	self.widget:enableCloseButton()

	local ogreWindow = self.widget:getWindow("Ogre")

	local xW = ogreWindow:getChild("SceneNodeInfo/Position/X")
	local yW = ogreWindow:getChild("SceneNodeInfo/Position/Y")
	local zW = ogreWindow:getChild("SceneNodeInfo/Position/Z")
	self.sceneNodes.positionAdapter = Ember.OgreView.Gui.Vector3Adapter.new(xW, yW, zW)
	connect(self.connectors, self.sceneNodes.positionAdapter.EventValueChanged, function()
		self.sceneNodes.selectedSceneNode:setPosition(self.sceneNodes.positionAdapter:getValue())
	end
	)

	local xW = ogreWindow:getChild("SceneNodeInfo/Orientation/X")
	local yW = ogreWindow:getChild("SceneNodeInfo/Orientation/Y")
	local zW = ogreWindow:getChild("SceneNodeInfo/Orientation/Z")
	local degreeW = ogreWindow:getChild("SceneNodeInfo/Orientation/Scalar")
	self.sceneNodes.rotationAdapter = Ember.OgreView.Gui.QuaternionAdapter.new(degreeW, xW, yW, zW)
	connect(self.connectors, self.sceneNodes.rotationAdapter.EventValueChanged, function()
		self.sceneNodes.selectedSceneNode:setOrientation(self.sceneNodes.rotationAdapter:getValue())
	end
	)

	local xW = ogreWindow:getChild("SceneNodeInfo/Scale/X")
	local yW = ogreWindow:getChild("SceneNodeInfo/Scale/Y")
	local zW = ogreWindow:getChild("SceneNodeInfo/Scale/Z")
	self.sceneNodes.scaleAdapter = Ember.OgreView.Gui.Vector3Adapter.new(xW, yW, zW)
	connect(self.connectors, self.sceneNodes.scaleAdapter.EventValueChanged, function()
		self.sceneNodes.selectedSceneNode:setScale(self.sceneNodes.scaleAdapter:getValue())
	end
	)

	--local sizeW = ogreWindow:getChild("SceneNodeInfo/Scale/SizeText")

	--force a refresh the first time the window is shown
	connect(self.connectors, self.widget.EventFirstTimeShown, self.refresh, self)


end

function EntityBrowser:shutdown()
	deleteSafe(self.sceneNodes.scaleAdapter)
	deleteSafe(self.sceneNodes.rotationAdapter)
	deleteSafe(self.sceneNodes.positionAdapter)
	deleteSafe(self.sceneNodes.listholder)
	deleteSafe(self.listholder)

	self.sceneNodes.listholder:resetList()
	self.sceneNodes.listholder = nil
	self.listholder:resetList()
	self.listholder = nil

	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

connect(connectors, emberOgre.EventWorldCreated, function()
	local entityBrowser = { connectors = {}, listbox = nil, sceneNodes = { listbox = nil, selectedSceneNode = nil } }
	setmetatable(entityBrowser, { __index = EntityBrowser })

	entityBrowser:buildWidget()

	connect(entityBrowser.connectors, emberOgre.EventWorldBeingDestroyed, function()
		entityBrowser:shutdown()
		entityBrowser = nil
		collectgarbage()
	end)
end)

