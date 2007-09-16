--Lists all of the graphical resources available

AssetsManager = {connectors={}, textures = {controls = {}, listbox = nil,selectedTexture = nil}, windows = {controls = {}, listbox = nil, selectedWindow = nil} }



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
	
	--the windows part
	AssetsManager.windows.controls.listbox = CEGUI.toListbox(AssetsManager.widget:getWindow("WindowsList"))
	AssetsManager.windows.controls.filter = CEGUI.toEditbox(AssetsManager.widget:getWindow("FilterWindows"))
	AssetsManager.windows.listholder = EmberOgre.Gui.ListHolder:new_local(AssetsManager.windows.controls.listbox, AssetsManager.windows.controls.filter)
	AssetsManager.windows.controls.visibleCheckbox = CEGUI.toCheckbox(AssetsManager.widget:getWindow("WindowInfo/Visible"))
	AssetsManager.windows.controls.infoText = AssetsManager.widget:getWindow("WindowInfo/Text")
	

	AssetsManager.helper = EmberOgre.Gui.AssetsManager:new_local()

	AssetsManager.widget:registerConsoleVisibilityToggleCommand("assetsManager")
	AssetsManager.widget:enableCloseButton()
	AssetsManager.widget:hide()


end


AssetsManager.buildWidget()
