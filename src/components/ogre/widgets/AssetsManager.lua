--Lists all of the graphical resources available

AssetsManager = {connectors={}, 
textures = {controls = {}, listbox = nil,selectedTexture = nil}, 
materials = {controls = {}, listbox = nil,selectedTexture = nil}, 
images = {controls = {}, listbox = nil,selectedTexture = nil}, 
windows = {controls = {}, listbox = nil, selectedWindow = nil},
meshes = {controls = {}, listbox = nil, selectedWindow = nil}
}

--Reloads a resource
function AssetsManager.reloadResource(manager, resourceName)
	local resourcePtr = manager:getByName(resourceName)
	if (resourcePtr:isNull() == false) then
		local resource = resourcePtr:get()
		resource:reload();
	end
end


--Reloads the currently selected resource in the supplied listbox
function AssetsManager.reloadResourceFromList(listbox, manager)
	local item = listbox:getFirstSelectedItem()
	if item ~= nil then
		local name = item:getText()
		AssetsManager.reloadResource(manager, name)
	end
end


function AssetsManager.TexturesReload_Clicked(args)
	AssetsManager.reloadResourceFromList(AssetsManager.textures.controls.listbox, Ogre.TextureManager:getSingleton())
end

function AssetsManager.TexturesRefresh_Clicked(args)
	AssetsManager.textures.refresh()
end

function AssetsManager.textures.refresh()
	AssetsManager.textures.listholder:resetList()
	
	local manager = Ogre.TextureManager:getSingleton()
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
		local texturePair = AssetsManager.helper:showTexture(textureName)
		if texturePair:hasData() then 
			AssetsManager.textures.controls.textureView:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
		end
	end
	
end

function AssetsManager.MaterialsRefresh_Clicked(args)
	AssetsManager.materials.refresh()
end

function AssetsManager.materials.refresh()
	AssetsManager.materials.listholder:resetList()
	
	local manager = Ogre.MaterialManager:getSingleton()
	local I = manager:getResourceIterator()
	local i = 0
	while I:hasMoreElements() do
		local definitionPtr = I:getNext()
		definitionPtr = tolua.cast(definitionPtr, "Ogre::MaterialPtr")
		local definition = definitionPtr:get()
		local name = definition:getName()
		local item = EmberOgre.Gui.ColouredListItem:new(name, i)
		AssetsManager.materials.listholder:addItem(item)
		i = i + 1
	end	
end

function AssetsManager.MaterialsList_ItemSelectionChanged(args)
	local item = AssetsManager.materials.controls.listbox:getFirstSelectedItem()
	if item ~= nil then
		local manager = Ogre.MaterialManager:getSingleton()
		local materialName = item:getText()
		local res = manager:getByName(materialName)
		res = tolua.cast(res, "Ogre::MaterialPtr")
		local text = AssetsManager.helper:materialAsText(res)
		AssetsManager.materials.controls.textWidget:setProperty("Text", text)
	end
	
end


function AssetsManager.RefreshImages_Clicked(args)
	AssetsManager.images.refresh()
end

function AssetsManager.images.refresh()
	AssetsManager.images.listholder:resetList()
	
	manager = CEGUI.ImagesetManager:getSingleton()
	local I = manager:getIterator()
	while I:isAtEnd() == false do
		local value = I:value()
		local J = value:getIterator()
		while J:isAtEnd() == false do
			local name = J:key();
			local item = EmberOgre.Gui.ColouredListItem:new(name, 0, J:value())
			AssetsManager.images.listholder:addItem(item)
			J:next();
		end
		
		I:next();
	end

end

function AssetsManager.ImagesList_ItemSelectionChanged(args)
	local item = AssetsManager.images.controls.listbox:getFirstSelectedItem()
	if item ~= nil then
		local image = item:getUserData()
		image = tolua.cast(image, "CEGUI::Image")
		AssetsManager.images.controls.textureView:setProperty("Image", CEGUI.PropertyHelper:imageToString(image))
	end

end



function AssetsManager.meshes.refresh()
	AssetsManager.meshes.listholder:resetList()

	local manager = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	local meshes = manager:getAllMeshes()
	
	for i = 0, meshes:size() - 1 do
		local name = meshes[i]
		local item = EmberOgre.Gui.ColouredListItem:new(name, i)
		AssetsManager.meshes.listholder:addItem(item)
	end	
end

function AssetsManager.MeshesRefresh_Clicked(args)
	AssetsManager.meshes.refresh()
end



function AssetsManager.RefreshWindows_Clicked(args)
	AssetsManager.windows.refresh()
end

function AssetsManager.WindowsList_ItemSelectionChanged(args)
	local item = AssetsManager.windows.controls.listbox:getFirstSelectedItem()
	if item ~= nil then
		local window = item:getUserData()
		AssetsManager.windows.selectedWindow = tolua.cast(window, "CEGUI::Window")
		AssetsManager.windows.controls.visibleCheckbox:setSelected(AssetsManager.windows.selectedWindow:isVisible())
		local info = ""
		info = "Position: " .. CEGUI.PropertyHelper:uvector2ToString(AssetsManager.windows.selectedWindow:getPosition()) .. "\n"
		info = info .."Size: " .. CEGUI.PropertyHelper:uvector2ToString(AssetsManager.windows.selectedWindow:getSize()) .. "\n"
		AssetsManager.windows.controls.infoText:setText(info)
	end
end

function AssetsManager.WindowsList_CheckStateChanged(args)
	if AssetsManager.windows.selectedWindow ~= nil then
		AssetsManager.windows.selectedWindow:setVisible(AssetsManager.windows.controls.visibleCheckbox:isSelected())
	end
end

function AssetsManager.windows.refresh()
	AssetsManager.windows.listholder:resetList()
	
	AssetsManager.windows.addWindow(CEGUI.System:getSingleton():getGUISheet(), 0)

end

function AssetsManager.windows.addWindow(window, depth)
	if window ~= nil then
		local label = ""
		for i = 0, depth  do
			label = label .. "-"
		end	
		label = label .. window:getName()
		local item = EmberOgre.Gui.ColouredListItem:new(label, window:getID(), window)
		AssetsManager.windows.listholder:addItem(item)
		if window:getChildCount() > 0 then
			for i = 0, window:getChildCount() - 1 do
				local childWindow = window:getChildAtIdx(i)
				AssetsManager.windows.addWindow(childWindow, depth + 1)
			end
		end
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
	AssetsManager.textures.listholder = EmberOgre.Gui.ListHolder:new_local(AssetsManager.textures.controls.listbox, AssetsManager.textures.controls.filter)
	AssetsManager.textures.controls.textureView = AssetsManager.widget:getWindow("TextureInfo/Image")
	
	--the materials part
	AssetsManager.materials.controls.listbox = CEGUI.toListbox(AssetsManager.widget:getWindow("MaterialsList"))
-- 	AssetsManager.sceneNodes.nodeInfo = AssetsManager.widget:getWindow("SceneNodeInfo")
	AssetsManager.materials.controls.filter = CEGUI.toEditbox(AssetsManager.widget:getWindow("FilterMaterials"))
	AssetsManager.materials.listholder = EmberOgre.Gui.ListHolder:new_local(AssetsManager.materials.controls.listbox, AssetsManager.materials.controls.filter)
	AssetsManager.materials.controls.textWidget = AssetsManager.widget:getWindow("MaterialInfo/Text")
	
	
	--the images part
	AssetsManager.images.controls.listbox = CEGUI.toListbox(AssetsManager.widget:getWindow("ImagesList"))
-- 	AssetsManager.sceneNodes.nodeInfo = AssetsManager.widget:getWindow("SceneNodeInfo")
	AssetsManager.images.controls.filter = CEGUI.toEditbox(AssetsManager.widget:getWindow("FilterImages"))
	AssetsManager.images.listholder = EmberOgre.Gui.ListHolder:new_local(AssetsManager.images.controls.listbox, AssetsManager.images.controls.filter)
	AssetsManager.images.controls.textureView = AssetsManager.widget:getWindow("ImagesInfo/Image")
	
	--the windows part
	AssetsManager.windows.controls.listbox = CEGUI.toListbox(AssetsManager.widget:getWindow("WindowsList"))
	AssetsManager.windows.controls.filter = CEGUI.toEditbox(AssetsManager.widget:getWindow("FilterWindows"))
	AssetsManager.windows.listholder = EmberOgre.Gui.ListHolder:new_local(AssetsManager.windows.controls.listbox, AssetsManager.windows.controls.filter)
	AssetsManager.windows.controls.visibleCheckbox = CEGUI.toCheckbox(AssetsManager.widget:getWindow("WindowInfo/Visible"))
	AssetsManager.windows.controls.infoText = AssetsManager.widget:getWindow("WindowInfo/Text")
	
	--the meshes part
	AssetsManager.meshes.controls.listbox = CEGUI.toListbox(AssetsManager.widget:getWindow("MeshesList"))
-- 	AssetsManager.sceneNodes.nodeInfo = AssetsManager.widget:getWindow("SceneNodeInfo")
	AssetsManager.meshes.controls.filter = CEGUI.toEditbox(AssetsManager.widget:getWindow("FilterMeshes"))
	AssetsManager.meshes.listholder = EmberOgre.Gui.ListHolder:new_local(AssetsManager.meshes.controls.listbox, AssetsManager.meshes.controls.filter)
	AssetsManager.meshes.controls.textureView = AssetsManager.widget:getWindow("MeshInfo/Preview")


	AssetsManager.helper = EmberOgre.Gui.AssetsManager:new_local()

	AssetsManager.widget:registerConsoleVisibilityToggleCommand("assetsManager")
	AssetsManager.widget:enableCloseButton()
	AssetsManager.widget:hide()


end


AssetsManager.buildWidget()
