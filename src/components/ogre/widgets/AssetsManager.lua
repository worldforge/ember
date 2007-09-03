--Lists all of the graphical resources available

AssetsManager = {connectors={}, }

AssetsManager.textures = {controls = {}}
AssetsManager.textures.listbox = nil
AssetsManager.textures.selectedTexture = nil

function AssetsManager.TexturesRefresh_Clicked(args)
	AssetsManager.textures.refresh()
end

function AssetsManager.textures.refresh()
	AssetsManager.textures.listholder:resetList()
	
	manager = Ogre.TextureManager:getSingleton()
	local I = manager:getResourceIterator()
	local i = 0
	while I:hasMoreElements() do
		local definitionPtr = I:getNext()
		definitionPtr = tolua.cast(definitionPtr, "Ogre::TexturePtr")
		local definition = definitionPtr:get()
		local name = definition:getName()
		local item = EmberOgre.Gui.ColouredListItem:new(name, i)
		AssetsManager.textures.listholder:addItem(item)
		i = i + 1
	end	
end


function AssetsManager.TexturesList_ItemSelectionChanged(args)
	local item = AssetsManager.textures.controls.listbox:getFirstSelectedItem()
	if item ~= nil then
		local textureName = item:getText()
		AssetsManager.helper:showTexture(textureName)
		AssetsManager.textures.controls.textureView:setProperty("Image", CEGUI.PropertyHelper:imageToString(AssetsManager.helper:getCEGUIImage()))
	end
	
end

function AssetsManager.SceneNodesList_SelectionChanged(args)
	local item = AssetsManager.sceneNodes.listbox:getFirstSelectedItem()
	if item ~= nil then
		--we've stored the sceneNode in the user data (we should perhaps store the key instead, and then do a look up, in case the scene node has been removed in the interim)
		local sceneNode = item:getUserData()
		sceneNode = tolua.cast(sceneNode, "Ogre::Node")
		AssetsManager.sceneNodes.selectedSceneNode = sceneNode
		AssetsManager.updateSceneNodeInfo(sceneNode)
--		local positionInfo = "x: " .. sceneNode:getPosition().x .. " y: " .. sceneNode:getPosition().y .. " z: " .. sceneNode:getPosition().z
--		AssetsManager.sceneNodes.nodeInfo:setText(positionInfo);
	end
end

function AssetsManager.updateSceneNodeInfo(sceneNode)
	AssetsManager.sceneNodes.positionAdapter:updateGui(sceneNode:getPosition())
	AssetsManager.sceneNodes.rotationAdapter:updateGui(sceneNode:getOrientation())
end

function AssetsManager.sceneNodes_positionAdapter_changed()
	AssetsManager.sceneNodes.selectedSceneNode:setPosition(AssetsManager.sceneNodes.positionAdapter:getValue())
end

function AssetsManager.sceneNodes_rotationAdapter_changed()
	AssetsManager.sceneNodes.selectedSceneNode:setOrientation(AssetsManager.sceneNodes.rotationAdapter:getValue())
end



function AssetsManager.addSceneNode(sceneNode, level)
--	if entity ~= nil then

--	end
end


function AssetsManager.refresh()
	AssetsManager.listholder:resetList()
	AssetsManager.addEntity(emberOgre:getEntityFactory():getWorld(), 0)
end

function AssetsManager.addEntity(entity, level)
--	if entity ~= nil then
		local label = ""
		for i = 0, level  do
			label = label .. "-"
		end	
		label = label .. entity:getName() .. " (" .. entity:getType():getName() .. ")"
		
		local item = EmberOgre.Gui.ColouredListItem:new(label, entity:getId(), entity)
		AssetsManager.listholder:addItem(item)
	
	
		local numContained = entity:numContained()
		for i = 0, numContained - 1 do
			local childEntity = entity:getContained(i)
			AssetsManager.addEntity(childEntity, level + 1)
		end 
--	end
end

function AssetsManager.buildWidget()

	AssetsManager.widget = guiManager:createWidget()
	AssetsManager.widget:loadMainSheet("AssetsManager.layout", "AssetsManager/")
	

	
	--the texture part
	AssetsManager.textures.controls.listbox = CEGUI.toListbox(AssetsManager.widget:getWindow("TexturesList"))
-- 	AssetsManager.sceneNodes.nodeInfo = AssetsManager.widget:getWindow("SceneNodeInfo")
	AssetsManager.textures.controls.filter = CEGUI.toEditbox(AssetsManager.widget:getWindow("FilterTextures"))
	AssetsManager.textures.listholder = EmberOgre.ListHolder:new_local(AssetsManager.textures.controls.listbox, AssetsManager.textures.controls.filter)
	AssetsManager.textures.controls.textureView = AssetsManager.widget:getWindow("TextureInfo/Image")

	AssetsManager.helper = EmberOgre.Gui.AssetsManager:new_local()

	AssetsManager.widget:registerConsoleVisibilityToggleCommand("assetsManager")
	AssetsManager.widget:enableCloseButton()
	AssetsManager.widget:hide()


end


AssetsManager.buildWidget()
