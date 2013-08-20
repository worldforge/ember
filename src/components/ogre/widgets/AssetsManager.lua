--Lists all of the graphical resources available

AssetsManager = {}

--Reloads a resource
function AssetsManager:reloadResource(manager, resourceName)
	local resourcePtr = manager:getByName(resourceName)
	if resourcePtr:isNull() == false then
		local resource = resourcePtr:get()
		resource:reload()
	end
end


--Reloads the currently selected resource in the supplied listbox
function AssetsManager:reloadResourceFromList(listbox, manager)
	local item 
	repeat
		item = listbox:getNextSelected(item)
		if item ~= nil then
			local name = item:getText()
			self:reloadResource(manager, name)
		end
	until item == nil
end


function AssetsManager:TexturesReload_Clicked(args)
	self:reloadResourceFromList(self.textures.controls.listbox, Ogre.TextureManager:getSingleton())
end

function AssetsManager:TexturesRefresh_Clicked(args)
	self.textures.refresh(self)
end

function AssetsManager:TexturesList_ItemSelectionChanged(args)
	local item = self.textures.controls.listbox:getFirstSelectedItem()
	if item ~= nil then
		local textureName = item:getText()
		local texturePair = self.helper:showTexture(textureName)
		if texturePair:hasData() then 
			self.textures.controls.textureView:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
			if texturePair:getOgreTexture():get():isReloadable() == true then
				self.widget:getWindow("TexturesReload"):enable()
			else
				self.widget:getWindow("TexturesReload"):disable()
			end
		end
	end
	
end

function AssetsManager:MaterialsRefresh_Clicked(args)
	self.materials.refresh(self)
end


function AssetsManager:MaterialsList_ItemSelectionChanged(args)
	local item = self.materials.controls.listbox:getFirstSelectedItem()
	if item ~= nil then
		local manager = Ogre.MaterialManager:getSingleton()
		local materialName = item:getText()
		local res = manager:getByName(materialName)
		res = tolua.cast(res, "Ogre::MaterialPtr")
		local text = self.helper:materialAsText(res)
		self.materials.controls.textWidget:setProperty("Text", text)
	end
	
end

function AssetsManager:MaterialsReload_Clicked(args)
	self:reloadResourceFromList(self.materials.controls.listbox, Ogre.MaterialManager:getSingleton())
	return true
end

function AssetsManager:RefreshShaders_Clicked(args)
	self.shaders.refresh(self)
end

function AssetsManager:ShadersList_ItemSelectionChanged(args)
	local item = self.shaders.controls.listbox:getFirstSelectedItem()
	if item ~= nil then
		local manager = Ogre.HighLevelGpuProgramManager:getSingleton()
		local materialName = item:getText()
		local res = manager:getByName(materialName)
		if res ~= nil then
			res = tolua.cast(res, "Ogre::GpuProgramPtr")
			resPtr = res:get()
			if resPtr ~= nil then
				resPtr:load()
				self.shaders.controls.textWidget:setProperty("Text", resPtr:getSource())
			end
		end
	end
	
end

function AssetsManager:ShadersReload_Clicked(args)
	self:reloadResourceFromList(self.shaders.controls.listbox, Ogre.HighLevelGpuProgramManager:getSingleton())
	self:ShadersList_ItemSelectionChanged(args)
end

function AssetsManager:RefreshImages_Clicked(args)
	self.images.refresh(self)
end

function AssetsManager:ImagesList_ItemSelectionChanged(args)
	local item = self.images.controls.listbox:getFirstSelectedItem()
	if item ~= nil then
		local image = item:getUserData()
		image = tolua.cast(image, "CEGUI::Image")
		self.images.controls.textureView:setProperty("Image", CEGUI.PropertyHelper:imageToString(image))
	end

end




function AssetsManager:MeshesRefresh_Clicked(args)
	self.meshes.refresh(self)
	return true
end

function AssetsManager:MeshesList_ItemSelectionChanged(args)
	local item = self.meshes.controls.listbox:getFirstSelectedItem()
	local meshInfo = self.widget:getWindow("MeshInfo")
	meshInfo:setEnabled(item ~= nil)
	if item then
		self:showMesh(item:getText())
	end
	return true
end

function AssetsManager:UserMeshList_ItemSelectionChanged(args)
	local item = self.meshes.controls.userlistbox:getFirstSelectedItem()
	if item then
		self.meshes.current.userlistboxSelected = item:getText()
		self.meshes.controls.userlistbox:ensureItemIsVisible(item)
	end
	return true
end

function AssetsManager:SubMeshesList_ItemSelectionChanged(args)
	local item = self.meshes.controls.submeshesListbox:getFirstSelectedItem()
	if item ~= nil then
		local submeshIndex = item:getID()
		self.meshes.current.submeshIndex = submeshIndex
		local mesh = self.meshes.current.meshPtr.get()
		self.meshes.current.submesh = mesh:getSubMesh(submeshIndex)
		
		self:selectMaterial(self.meshes.current.submesh)
		local info = self.meshes.rendererStats:getInfo(submeshIndex)
		self:updateMeshInfo()
	end
	return true
end

function AssetsManager:SubMeshMaterialsList_ItemSelectionChanged(args)
	if self.meshes.current ~= nil and self.meshes.current.submesh ~= nil then
		local list = self.meshes.controls.materialListbox
		local item = list:getFirstSelectedItem()
		if item ~= nil then
			self.meshes.current.submesh:setMaterialName(item:getText())
			self.meshes.renderer:showEntity(self.meshes.current.meshPtr.get():getName())
		end
	end
	return true
end

function AssetsManager:MeshInfoSaveMeshButton_Clicked(args)
	local mesh = self.meshes.current.meshPtr.get()
	mesh:setSkeletonName(self.meshes.controls.skeletonPath:getText())
	local manager = Ogre.MeshManager:getSingleton()
	local meshPtr = manager:getByName(mesh:getName())
	self.meshes.renderer:unloadEntity()
	mesh:removeLodLevels()
	self.helper:exportMesh(meshPtr, self.helper:resolveFilePathForMesh(meshPtr))
	self:LODRegenerateLods()
	return true
end

function AssetsManager:showMesh(meshName)
	self.meshes.renderer:showEntity(meshName)
	
	--we need to get hold of a mesh instance
	local manager = Ogre.MeshManager:getSingleton()
	local meshPtr = manager:getByName(meshName)
	self.meshes.current = {}
	self.meshes.current.userlistboxSelected = ""
	self.meshes.current.meshPtr = {}
	self.meshes.current.meshPtr.get = function()
		return manager:getByName(meshName):get()
	end
	
	self:fillSubMeshList(meshPtr)
	local mesh = meshPtr:get()
	self.meshes.controls.skeletonPath:setText(mesh:getSkeletonName())

	-- Fill distances list.
	local lodDefManager = Ember.OgreView.Lod.LodDefinitionManager:getSingleton()
	self.meshes.current.lodDefPtr = {}
	self.meshes.current.lodDefPtr.get = function()
		return lodDefManager:getByName(self:getLodDefName(meshName)):get()
	end
	local lodlevelsList = self.widget:getWindow("LODDistances")
	lodlevelsList = CEGUI.toListbox(lodlevelsList)
	lodlevelsList:resetList()
	local loddef = self.meshes.current.lodDefPtr.get()
	
	local distances = loddef:createListOfDistances()
	local size = distances:size()
	for i = 0, size - 1 do
		local value = self:round(distances[i], 6)
		local item = Ember.OgreView.Gui.ColouredListItem:new(tostring(value), value) 
		lodlevelsList:addItem(item)
	end
	self:LODSortDistances()
	local window = self.widget:getWindow("SubmeshInfoText")
	window:setText("")
	-- TODO: Save changes before switching to other mesh
	-- We need to clear the data after changing selection.
	self.meshes.current.selectedDistance = nil
	self.meshes.current.submesh = nil
	self.meshes.current.submeshIndex = nil
	self:LODUpdate()
	self:updatePreviewInfo()
	self:updateMeshInfo()
end

-- Strips directories and extension from meshName, then adds .loddef extension.
-- For example "../acorn.mesh" will be "acorn.loddef".
function AssetsManager:getLodDefName(meshName)
	return string.match(string.match(meshName, "[^\\/]*$"), "^[^%.]*") .. ".loddef"
end

-- Rounds a number to given decimal characters.
function AssetsManager:round(num, decimals)
	if num == 0 then return 0 end
	decimals = decimals or 0
	decimals = decimals - math.ceil(math.log10(num))
	decimals = 10 ^ decimals
	return math.floor(num * decimals + 0.5) / decimals
end
function AssetsManager:getSubMeshName(mesh, index)
	local submeshname = Ember.OgreView.OgreUtils:getSubMeshName(mesh, index)
	if submeshname == "" then
		submeshname = "(index: " .. index .. ")"
	end
	return submeshname
end

function AssetsManager:fillSubMeshList(meshPtr)
	local mesh = meshPtr:get()
	local list = self.meshes.controls.submeshesListbox
	list:resetList()
	
	--for now, since we don't have any good method for getting the submodel names yet we'll just use the index numbers
	local numberOfSubmeshes = mesh:getNumSubMeshes()
	local i = 0
	while i < numberOfSubmeshes do
		local submeshname = self:getSubMeshName(mesh, i)
		local item = Ember.OgreView.Gui.ColouredListItem:new(submeshname, i)
		list:addItem(item)
		i = i + 1
	end	
end

function AssetsManager:selectMaterial(submesh)

	if self.meshes.materialListadapter == nil then
		self.meshes.materialListadapter = Ember.OgreView.Gui.Adapters.Ogre.ResourceListAdapter:new_local(self.meshes.materialListholder, Ogre.MaterialManager:getSingleton())
		self.meshes.materialListadapter:update()
	end

	local list = self.meshes.controls.materialListbox
	if submesh == nil then
		list:clearAllSelections()
	else
		local materialName = submesh:getMaterialName()
		local item

		if materialName ~= "" then
			item = list:findItemWithText(materialName, list:getListboxItemFromIndex(0))
		end
		
		if item ~= nil then
			list:setItemSelectState(item, true)
			list:ensureItemIsVisible(item)
		else
			list:clearAllSelections()
		end
	end
end

function AssetsManager:RefreshWindows_Clicked(args)
	self.windows.refresh(self)
end

function AssetsManager:WindowsList_ItemSelectionChanged(args)
	local item = self.windows.controls.listbox:getFirstSelectedItem()
	if item ~= nil then
		local window_ = item:getUserData()
		local window = tolua.cast(window_, "CEGUI::Window")
		self.windows.selectedWindow = nil
		self.windows.controls.visibleCheckbox:setSelected(window:isVisible())
		
		self.windows.controls.widthRel:setText(tostring(window:getWidth().scale))
		self.windows.controls.widthFixed:setText(tostring(window:getWidth().offset))
		self.windows.controls.heightRel:setText(tostring(window:getHeight().scale))
		self.windows.controls.heightFixed:setText(tostring(window:getHeight().offset))

		self.windows.selectedWindow = window
		
		local info = ""
		info = "Position: " .. CEGUI.PropertyHelper:uvector2ToString(self.windows.selectedWindow:getPosition()) .. "\n"
		info = info .."Size: " .. CEGUI.PropertyHelper:uvector2ToString(self.windows.selectedWindow:getSize()) .. "\n"
		self.windows.controls.infoText:setText(info)
	end
end

function AssetsManager:WindowsList_CheckStateChanged(args)
	if self.windows.selectedWindow ~= nil then
		self.windows.selectedWindow:setVisible(self.windows.controls.visibleCheckbox:isSelected())
	end
end

function AssetsManager:SceneNodesList_SelectionChanged(args)
	local item = self.sceneNodes.listbox:getFirstSelectedItem()
	if item ~= nil then
		--we've stored the sceneNode in the user data (we should perhaps store the key instead, and then do a look up, in case the scene node has been removed in the interim)
		local sceneNode = item:getUserData()
		sceneNode = tolua.cast(sceneNode, "Ogre::Node")
		self.sceneNodes.selectedSceneNode = sceneNode
		self:updateSceneNodeInfo(sceneNode)
--		local positionInfo = "x: " .. sceneNode:getPosition().x .. " y: " .. sceneNode:getPosition().y .. " z: " .. sceneNode:getPosition().z
--		self.sceneNodes.nodeInfo:setText(positionInfo)
	end
end

function AssetsManager:updateSceneNodeInfo(sceneNode)
	self.sceneNodes.positionAdapter:updateGui(sceneNode:getPosition())
	self.sceneNodes.rotationAdapter:updateGui(sceneNode:getOrientation())
end

function AssetsManager:sceneNodes_positionAdapter_changed()
	self.sceneNodes.selectedSceneNode:setPosition(self.sceneNodes.positionAdapter:getValue())
end

function AssetsManager:sceneNodes_rotationAdapter_changed()
	self.sceneNodes.selectedSceneNode:setOrientation(self.sceneNodes.rotationAdapter:getValue())
end



function AssetsManager:addSceneNode(sceneNode, level)
--	if entity ~= nil then

--	end
end


function AssetsManager:refresh()
	self.listholder:resetList()
	local world = emberOgre:getWorld()
	if world then
		self:addEntity(world:getView():getTopLevel(), 0)
	end
end

function AssetsManager:addEntity(entity, level)
--	if entity ~= nil then
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
--	end
end

function AssetsManager:getInput(question, answerFunc)
	if not self.waitingAnswer then
		self.waitingAnswer = true
		self.answerFunc = answerFunc
		
		self.widget:getWindow("InputQuestion_text"):setText(question)
		self.widget:getWindow("InputAnswerTextbox"):setText("")
		self.widget:getWindow("InputWindow"):setVisible(true)
		self.widget:getWindow("InputWindow"):moveToFront()
	end
end

function AssetsManager:fillLODTypeCombobox()
	local types = self.widget:getWindow("LODTypeCombobox")
	types = CEGUI.toCombobox(types)
	
	local item = Ember.OgreView.Gui.ColouredListItem:new("Automatic vertex reduction", 0)
	types:addItem(item)
	
	local item = Ember.OgreView.Gui.ColouredListItem:new("User created LOD", 1)
	types:addItem(item)
end

function AssetsManager:fillLODStrategyCombobox()
	local strategies = self.widget:getWindow("LODStrategyCombobox")
	strategies = CEGUI.toCombobox(strategies)
	
	local item = Ember.OgreView.Gui.ColouredListItem:new("Distance Lod Strategy", 0)
	strategies:addItem(item)
	
	local item = Ember.OgreView.Gui.ColouredListItem:new("Pixel Count Lod Strategy", 1)
	strategies:addItem(item)
end

function AssetsManager:fillLODReductionTypeCombobox()
	local methods = self.widget:getWindow("LODReductionTypeCombobox")
	methods = CEGUI.toCombobox(methods)
	
	local item = Ember.OgreView.Gui.ColouredListItem:new("Proportional", 0)
	methods:addItem(item)
	
	local item = Ember.OgreView.Gui.ColouredListItem:new("Constant", 1)
	methods:addItem(item)
	
	local item = Ember.OgreView.Gui.ColouredListItem:new("Collapse Cost", 2)
	methods:addItem(item)
end

function AssetsManager:LODRegenerateLods()
	if not self.meshes.current.lodUnderRegeneration then
		self.meshes.current.lodNeedsRegenerate = false
		self.meshes.current.lodUnderRegeneration = true
		
		local mesh = self.meshes.current.meshPtr.get()
		local meshName = mesh:getName()
		local lodManager = Ember.OgreView.Lod.LodManager:getSingleton()
		local lodDefManager = Ember.OgreView.Lod.LodDefinitionManager:getSingleton()
		local lodDefPtr = lodDefManager:getByName(self:getLodDefName(meshName))
		local lodDef = lodDefPtr:get()
		
		local manager = Ogre.MeshManager:getSingleton()
		local meshPtr = manager:getByName(meshName)
		
		local combobox = self.widget:getWindow("LODTypeCombobox")
		combobox = CEGUI.toCombobox(combobox)
		-- user created Lod needs recreating entities.
		self.meshes.renderer:unloadEntity()
		lodManager:loadLod(meshPtr, lodDef)
		self.meshes.renderer:setAutoShowFull(false)
		self.meshes.renderer:showEntity(meshName)
		self.meshes.renderer:setAutoShowFull(true)

		-- local entity = self.meshes.renderer:getEntity()
		-- local camera = self.meshes.renderer:getCamera()
		-- entity:_notifyCurrentCamera(camera)
		
		self:LODUpdateForcedLevel()
		self:updatePreviewInfo()
		self:updateMeshInfo()
		
		if lodDef:getLodDistanceCount() == 0 or lodDef:getType() ~= 0 then
			-- signal will not be called with user created Lod or no Lod levels.
			self.meshes.current.lodUnderRegeneration = false
		end
		
	else
		self.meshes.current.lodNeedsRegenerate = true
	end
end

function AssetsManager:LODSave(dist)
	local loddef = self.meshes.current.lodDefPtr.get()
	if dist == nil or dist < 0 or not loddef:hasLodDistance(dist) then return end
	
	distdef = loddef:getLodDistance(dist)
	
	distdef:setMeshName(self.meshes.current.userlistboxSelected)
	
	local combobox = self.widget:getWindow("LODReductionTypeCombobox")
	combobox = CEGUI.toCombobox(combobox)
	local item = combobox:getSelectedItem()
	local reductionMethod
	if item then
		reductionMethod = combobox:getItemIndex(item)
	else
		reductionMethod = 0
	end
	distdef:setReductionMethod(reductionMethod)
	
	local editbox = self.widget:getWindow("LODReductionParameterTextbox")
	local reductionValue = editbox:getText()
	distdef:setReductionValue(reductionValue)
	
	local meshName = self.meshes.current.userlistboxSelected or ""
	distdef:setMeshName(meshName)
end

function AssetsManager:LODLoad(dist)
	local loddef = self.meshes.current.lodDefPtr.get()
	
	local useAuto = loddef:getUseAutomaticLod()
	
	local container = self.widget:getWindow("ManualLODContainer")
	container:setEnabled(not useAuto)
	
	local checkbox = self.widget:getWindow("EnableAutomaticLOD")
	checkbox = CEGUI.toCheckbox(checkbox)
	checkbox:setSelected(useAuto)
	
	local container = self.widget:getWindow("LODConfigContainer")
	if not dist or not loddef:hasLodDistance(dist) then
		container:setEnabled(false)
		return
	end
	
	container:setEnabled(true)
	distdef = loddef:getLodDistance(dist)
	
	local meshName = distdef:getMeshName()
	self.meshes.current.userlistboxSelected = meshName
	local listbox = self.meshes.controls.userlistbox
	if loddef:getType() == 1 and listbox and listbox:getItemCount() > 0 and meshName ~= "" then
		local meshName = self.meshes.current.userlistboxSelected
		listbox:clearAllSelections()
		local begin = listbox:getListboxItemFromIndex(0)
		local selected = listbox:findItemWithText(meshName, begin)
		if selected then
			listbox:setItemSelectState(selected, true)
		end
	end
	
	local combobox = self.widget:getWindow("LODTypeCombobox")
	combobox = CEGUI.toCombobox(combobox)
	combobox:setItemSelectState(loddef:getType(), true)
	
	local combobox = self.widget:getWindow("LODStrategyCombobox")
	combobox = CEGUI.toCombobox(combobox)
	combobox:setItemSelectState(loddef:getStrategy(), true)
	
	local combobox = self.widget:getWindow("LODReductionTypeCombobox")
	combobox = CEGUI.toCombobox(combobox)
	combobox:setItemSelectState(distdef:getReductionMethod(), true)
	
	if not self.meshes.current.scrollOnStack then
		self:LODUpdateScroll(true, true)
	end
		
	self.meshes.current.userlistboxSelected = distdef:getMeshName()
end

function AssetsManager:LODUpdate()
	if not self.meshes.current.updateOnStack then -- recursion protection
		self.meshes.current.updateOnStack = true
		self:LODSave(self.meshes.current.selectedDistance)
		self.meshes.current.selectedDistance = self:LODGetSelected()
		self:LODLoad(self.meshes.current.selectedDistance)
		self:LODUpdateTypes()
		self:LODUpdateForcedLevel()
		self.meshes.current.updateOnStack = false
		self:LODRegenerateLods()
	end
end

function AssetsManager:LODGetSelected()
	local listbox = self.widget:getWindow("LODDistances")
	listbox = CEGUI.toListbox(listbox)
	local item = listbox:getFirstSelectedItem()
	local loddef = self.meshes.current.lodDefPtr.get()
	if (item == nil or loddef:getUseAutomaticLod()) then
		return nil
	else
		return tonumber(item:getText())
	end
end

function AssetsManager:LODUpdateTypes()
	local combobox = self.widget:getWindow("LODTypeCombobox")
	combobox = CEGUI.toCombobox(combobox)
	local item = combobox:getSelectedItem()
	local type = item and combobox:getItemIndex(item) or 0
	local isAutomatic = (type == 0)
	local automatic = self.widget:getWindow("AutomaticVertexReductionContainer")
	automatic:setVisible(isAutomatic)
	local manual = self.widget:getWindow("ManuallyCreatedLODContainer")
	manual:setVisible(not isAutomatic)
end

function AssetsManager:LODUpdateForcedLevel()
	local checkbox = self.widget:getWindow("ForceLodLevelCheckbox")
	checkbox = CEGUI.toCheckbox(checkbox)
	local shouldForce = checkbox:isSelected()
	local dist = self:LODGetSelected()
	if shouldForce and dist then
		local mesh = self.meshes.current.meshPtr.get()
		local loddef = self.meshes.current.lodDefPtr.get()
		-- fix rounding problems.
		if loddef:getStrategy() == 0 then
			-- distance Lod strategy
			dist = dist * 1.00001
		else
			-- pixel count Lod strategy
			dist = dist * 0.99999
		end
		local strategy = mesh:getLodStrategy()
		dist = strategy:transformUserValue(dist)
		local lodIndex = mesh:getLodIndex(dist)
		self.meshes.renderer:setForcedLodLevel(lodIndex)
	else
		self.meshes.renderer:clearForcedLodLevel()
	end
end

function AssetsManager:LODAdd(distance)
	local loddef = self.meshes.current.lodDefPtr.get()
	distance = tonumber(distance)
	if not distance or loddef:hasLodDistance(distance) then return end
	
	distdef = loddef:createDistance(distance)
	local selected = self:LODGetSelected()
	self:LODSave(selected)
	local listbox = self.widget:getWindow("LODDistances")
	listbox = CEGUI.toListbox(listbox)
	local item = Ember.OgreView.Gui.ColouredListItem:new(distance, 0)
	listbox:addItem(item)
	listbox:setItemSelectState(item, true)
	self:LODSortDistances()
	self:LODUpdate()
end

function AssetsManager:LODPaste(distance)
	local loddef = self.meshes.current.lodDefPtr.get()
	distance = tonumber(distance)
	if not distance or loddef:hasLodDistance(distance) then return end
	
	local listbox = self.widget:getWindow("LODDistances")
	listbox = CEGUI.toListbox(listbox)
	distdef = loddef:createDistance(distance)
	
	local type = self.clipboard.type
	loddef:setType(type)
	
	local meshname = self.clipboard.meshName
	distdef:setMeshName(meshname)
	
	local reductionMethod = self.clipboard.reductionMethod
	distdef:setReductionMethod(reductionMethod)
	
	local reductionValue = self.clipboard.reductionValue
	distdef:setReductionValue(reductionValue)

	local selected = self:LODGetSelected()
	self:LODSave(selected)
	local item = Ember.OgreView.Gui.ColouredListItem:new(distance, 0)
	listbox:addItem(item)
	listbox:setItemSelectState(item, true)
	self:LODSortDistances()
	self:LODUpdate()
end

function AssetsManager:LODSortDistances()
	local selected = self:LODGetSelected()
	
	local listbox = self.widget:getWindow("LODDistances")
	listbox = CEGUI.toListbox(listbox)
	
	local distances = {}
	local count = listbox:getItemCount() - 1
	for i = 0, count do
		local item = listbox:getListboxItemFromIndex(i)
		item = item:getText()
		table.insert(distances, tonumber(item))
	end
	
	local loddef = self.meshes.current.lodDefPtr.get()

	if loddef:getStrategy() == 0 then
		table.sort(distances)
	else
		-- Pixel count Lod strategy needs inverted order.
		table.sort(distances, function(a, b) return a > b end)
	end
	
	listbox:resetList()
	
	count = count + 1
	for i = 1, count do
		local item = Ember.OgreView.Gui.ColouredListItem:new(distances[i], i)
		listbox:addItem(item)
		if distances[i] == selected then
			listbox:setItemSelectState(item, true)
		end
	end
end

function AssetsManager:LODUpdateScroll(shouldUpdateText, shouldUpdateScroll, value)
	local dist = self:LODGetSelected()
	local loddef = self.meshes.current.lodDefPtr.get()
	if not dist or not loddef:hasLodDistance(dist) then
		return
	end
	local distdef = loddef:getLodDistance(dist)
	
	if not value then
		value = distdef:getReductionValue()
	end
	
	local scrollSize
	local stepSize
	if distdef:getReductionMethod() == 0 then
		-- Proportional
		scrollSize = 1
		stepSize = 0.01
	elseif distdef:getReductionMethod() == 1 then
		-- Constant
		local mesh = self.meshes.current.meshPtr.get()
		scrollSize = Ember.OgreView.Gui.MeshInfoProvider:calcUniqueVertexCount(mesh)
		stepSize = 1
	else
		-- Collapse cost
		local mesh = self.meshes.current.meshPtr.get()
		scrollSize = mesh:getBoundingSphereRadius()
		stepSize = scrollSize * 0.001
	end
	
	value = math.floor(value / stepSize + 0.5) * stepSize
	value = self:round(value, 6)
	
	if shouldUpdateText then
		local editbox = self.widget:getWindow("LODReductionParameterTextbox")
		editbox:setText(tostring(value))
	end
	
	if shouldUpdateScroll then
		local scroll = self.widget:getWindow("LODValueScroll")
		scroll = CEGUI.toScrollbar(scroll)
		scroll:setDocumentSize(scrollSize * 1.01)
		scroll:setPageSize(scrollSize * 0.01)
		scroll:setStepSize(stepSize)
		scroll:setScrollPosition(value)
	end
end

function AssetsManager:updateMeshInfo()
	if self.meshes.current.submesh then
		local info = self.meshes.rendererStats:getInfo(self.meshes.current.submeshIndex)
		local window = self.widget:getWindow("SubmeshInfoText")
		window:setText(info)
	end
end

function AssetsManager:updatePreviewInfo()
	local info = self.meshes.rendererStats:getPreviewInfo()
	self.meshes.previewInfo:setText(info)
	return true
end

function AssetsManager:buildWidget()
	self.widget = guiManager:createWidget()

	--delay setup of the widget until it's shown for the first time
	local setup = function()

		self.controls.tabs = CEGUI.toTabControl(self.widget:getWindow("MainTabControl"))
		
		--the texture part
		self.textures.controls.listbox = CEGUI.toListbox(self.widget:getWindow("TexturesList"))
		self.textures.controls.filter = CEGUI.toEditbox(self.widget:getWindow("FilterTextures"))
		self.textures.listholder = Ember.OgreView.Gui.ListHolder:new(self.textures.controls.listbox, self.textures.controls.filter)
		self.textures.controls.textureView = self.widget:getWindow("TextureInfo/Image")
		self.textures.adapter = Ember.OgreView.Gui.Adapters.Ogre.ResourceListAdapter:new_local(self.textures.listholder, Ogre.TextureManager:getSingleton())
		self.textures.refresh = function(self)
			self.textures.adapter:update()
		end
		self.widget:getWindow("TexturesRefresh"):subscribeEvent("Clicked", self.TexturesRefresh_Clicked, self)
		self.widget:getWindow("TexturesReload"):subscribeEvent("Clicked", self.TexturesReload_Clicked, self)
		self.widget:getWindow("TexturesList"):subscribeEvent("ItemSelectionChanged", self.TexturesList_ItemSelectionChanged, self)

		
		--the materials part
		self.materials.controls.listbox = CEGUI.toListbox(self.widget:getWindow("MaterialsList"))
		self.materials.controls.filter = CEGUI.toEditbox(self.widget:getWindow("FilterMaterials"))
		self.materials.listholder = Ember.OgreView.Gui.ListHolder:new(self.materials.controls.listbox, self.materials.controls.filter)
		self.materials.controls.textWidget = self.widget:getWindow("MaterialInfo/Text")
		self.materials.adapter = Ember.OgreView.Gui.Adapters.Ogre.ResourceListAdapter:new_local(self.materials.listholder, Ogre.MaterialManager:getSingleton())
		self.materials.refresh = function(self)
			self.materials.adapter:update()
		end
		self.widget:getWindow("MaterialsRefresh"):subscribeEvent("Clicked", self.MaterialsRefresh_Clicked, self)
		self.widget:getWindow("MaterialsReload"):subscribeEvent("Clicked", self.MaterialsReload_Clicked, self)
		self.widget:getWindow("MaterialsList"):subscribeEvent("ItemSelectionChanged", self.MaterialsList_ItemSelectionChanged, self)
		
	
		--the images part
		self.images.controls.listbox = CEGUI.toListbox(self.widget:getWindow("ImagesList"))
		self.images.controls.filter = CEGUI.toEditbox(self.widget:getWindow("FilterImages"))
		self.images.listholder = Ember.OgreView.Gui.ListHolder:new(self.images.controls.listbox, self.images.controls.filter)
		self.images.controls.textureView = self.widget:getWindow("ImagesInfo/Image")
		self.images.refresh = function(self)
			self.images.listholder:resetList()
			manager = CEGUI.ImagesetManager:getSingleton()
			local I = manager:getIterator()
			while I:isAtEnd() == false do
				local value = I:value()
				local J = value:getIterator()
				while J:isAtEnd() == false do
					local name = J:key()
					local item = Ember.OgreView.Gui.ColouredListItem:new(name, 0, J:value())
					self.images.listholder:addItem(item)
					J:next()
				end
				
				I:next()
			end
		end
		self.widget:getWindow("ImagesRefresh"):subscribeEvent("Clicked", self.RefreshImages_Clicked, self)
		self.widget:getWindow("ImagesList"):subscribeEvent("ItemSelectionChanged", self.ImagesList_ItemSelectionChanged, self)
		
		
		--the windows part
		self.windows.controls.listbox = CEGUI.toListbox(self.widget:getWindow("WindowsList"))
		self.windows.controls.filter = CEGUI.toEditbox(self.widget:getWindow("FilterWindows"))
		self.windows.listholder = Ember.OgreView.Gui.ListHolder:new(self.windows.controls.listbox, self.windows.controls.filter)
		self.windows.controls.visibleCheckbox = CEGUI.toCheckbox(self.widget:getWindow("WindowInfo/Visible"))
		self.windows.controls.infoText = self.widget:getWindow("WindowInfo/Text")
		self.windows.controls.widthRel = self.widget:getWindow("WindowInfo/Width_rel")
		self.windows.controls.widthFixed = self.widget:getWindow("WindowInfo/Width_fix")
		self.windows.controls.heightRel = self.widget:getWindow("WindowInfo/Height_rel")
		self.windows.controls.heightFixed = self.widget:getWindow("WindowInfo/Height_fix")
		
		local updateWindowSize = function(args)
			if self.windows.selectedWindow then
				if self.windows.controls.widthRel:getText() ~= "" and self.windows.controls.widthFixed:getText() ~= "" and self.windows.controls.heightRel:getText() ~= "" and self.windows.controls.heightFixed:getText() ~= "" then
					local dim = CEGUI.UVector2(CEGUI.UDim(tonumber(self.windows.controls.widthRel:getText()),tonumber(self.windows.controls.widthFixed:getText())), CEGUI.UDim(tonumber(self.windows.controls.heightRel:getText()),tonumber(self.windows.controls.heightFixed:getText())))
					self.windows.selectedWindow:setSize(dim)
				end
			end
			return true
		end
		
		self.windows.controls.widthRel:subscribeEvent("TextChanged", updateWindowSize)
		self.windows.controls.widthFixed:subscribeEvent("TextChanged", updateWindowSize)
		self.windows.controls.heightRel:subscribeEvent("TextChanged", updateWindowSize)
		self.windows.controls.heightFixed:subscribeEvent("TextChanged", updateWindowSize)
		
		
		self.windows.refresh = function(self)
			self.windows.listholder:resetList()
			
			self.windows.addWindow(self, CEGUI.System:getSingleton():getGUISheet(), 0)
		end

		self.windows.addWindow = function(self, window, depth)		
			if window ~= nil then
				local label = ""
				for i = 0, depth  do
					label = label .. "-"
				end	
				label = label .. window:getName()
				local item = Ember.OgreView.Gui.ColouredListItem:new(label, window:getID(), window)
				self.windows.listholder:addItem(item)
				if window:getChildCount() > 0 then
					for i = 0, window:getChildCount() - 1 do
						local childWindow = window:getChildAtIdx(i)
						self.windows.addWindow(self, childWindow, depth + 1)
					end
				end
			end
		end
		
		self.widget:getWindow("RefreshWindows"):subscribeEvent("Clicked", self.RefreshWindows_Clicked, self)
		self.widget:getWindow("WindowsList"):subscribeEvent("ItemSelectionChanged", self.WindowsList_ItemSelectionChanged, self)
		self.widget:getWindow("WindowInfo/Visible"):subscribeEvent("CheckStateChanged", self.WindowsList_CheckStateChanged, self)

		
		--the meshes part
		self.meshes.controls.listbox = CEGUI.toListbox(self.widget:getWindow("MeshesList"))
		self.meshes.controls.userlistbox = CEGUI.toListbox(self.widget:getWindow("UserMeshList"))
		self.meshes.controls.submeshesListbox = CEGUI.toListbox(self.widget:getWindow("SubMeshesList"))
		self.meshes.controls.materialListbox = CEGUI.toListbox(self.widget:getWindow("SubMeshMaterialsList"))
		self.meshes.controls.materialFilter = CEGUI.toEditbox(self.widget:getWindow("SubMeshMaterialsFilter"))
		self.meshes.controls.skeletonPath = CEGUI.toEditbox(self.widget:getWindow("MeshSkeletonPath"))
		self.meshes.controls.filter = CEGUI.toEditbox(self.widget:getWindow("FilterMeshes"))
		self.meshes.listholder = Ember.OgreView.Gui.ListHolder:new(self.meshes.controls.listbox, self.meshes.controls.filter)
		self.meshes.controls.userfilter = CEGUI.toEditbox(self.widget:getWindow("UserMeshFilter"))
		self.meshes.userlistholder = Ember.OgreView.Gui.ListHolder:new(self.meshes.controls.userlistbox, self.meshes.controls.userfilter)
		self.meshes.controls.textureView = self.widget:getWindow("MeshInfo/Preview")
		self.meshes.renderer = Ember.OgreView.Gui.OgreEntityRenderer:new(self.meshes.controls.textureView)
		self.meshes.rendererManipulator = Ember.OgreView.Gui.CameraEntityTextureManipulator:new(self.meshes.controls.textureView, self.meshes.renderer:getEntityTexture())
		self.meshes.rendererStats = Ember.OgreView.Gui.MeshInfoProvider:new(self.meshes.renderer)
		self.meshes.previewInfo = self.widget:getWindow("PreviewInfo")
		connect(self.connectors, self.meshes.rendererStats.EventLodChanged, self.updatePreviewInfo, self)
		self.meshes.materialListholder = Ember.OgreView.Gui.ListHolder:new(self.meshes.controls.materialListbox, self.meshes.controls.materialFilter)
		self.meshes.refresh = function(self)
			self.meshes.listholder:resetList()
		
			local manager = Ember.OgreView.Model.ModelDefinitionManager:getSingleton()
			local meshes = manager:getAllMeshes()
			
			for i = 0, meshes:size() - 1 do
				local name = meshes[i]
				local item = Ember.OgreView.Gui.ColouredListItem:new(name, i)
				self.meshes.listholder:addItem(item)
			end	
		end
		self.meshes.userrefresh = function(self)
			self.meshes.userlistholder:resetList()
		
			local manager = Ember.OgreView.Model.ModelDefinitionManager:getSingleton()
			local meshes = manager:getAllMeshes()
			
			for i = 0, meshes:size() - 1 do
				local name = meshes[i]
				local item = Ember.OgreView.Gui.ColouredListItem:new(name, i)
				self.meshes.userlistholder:addItem(item)
			end	
		end
		self.widget:getWindow("MeshesRefresh"):subscribeEvent("Clicked", self.MeshesRefresh_Clicked, self)
		self.widget:getWindow("MeshesList"):subscribeEvent("ItemSelectionChanged", self.MeshesList_ItemSelectionChanged, self)
		self.widget:getWindow("UserMeshList"):subscribeEvent("ItemSelectionChanged", self.UserMeshList_ItemSelectionChanged, self)
		self.widget:getWindow("SubMeshesList"):subscribeEvent("ItemSelectionChanged", self.SubMeshesList_ItemSelectionChanged, self)
		self.widget:getWindow("SubMeshMaterialsList"):subscribeEvent("ItemSelectionChanged", self.SubMeshMaterialsList_ItemSelectionChanged, self)
		self.widget:getWindow("MeshInfoSaveMeshButton"):subscribeEvent("Clicked", self.MeshInfoSaveMeshButton_Clicked, self)
	
		--the shaders part
		self.shaders.controls.listbox = CEGUI.toListbox(self.widget:getWindow("ShadersList"))
		self.shaders.controls.filter = CEGUI.toEditbox(self.widget:getWindow("FilterShaders"))
		self.shaders.listholder = Ember.OgreView.Gui.ListHolder:new(self.shaders.controls.listbox, self.shaders.controls.filter)
		self.shaders.controls.textWidget = self.widget:getWindow("ShadersInfo/Text")
		self.shaders.adapter = Ember.OgreView.Gui.Adapters.Ogre.ResourceListAdapter:new_local(self.shaders.listholder, Ogre.HighLevelGpuProgramManager:getSingleton())
		self.shaders.refresh = function(self)
			self.shaders.adapter:update()
		end
		self.widget:getWindow("RefreshShaders"):subscribeEvent("Clicked", self.RefreshShaders_Clicked, self)
		self.widget:getWindow("ShadersReload"):subscribeEvent("Clicked", self.ShadersReload_Clicked, self)
		self.widget:getWindow("ShadersList"):subscribeEvent("ItemSelectionChanged", self.ShadersList_ItemSelectionChanged, self)
	
		
		self.widget:getWindow("MeshInfoSaveLoddefButton"):subscribeEvent("Clicked", function(args)
			self:LODUpdate()
			local mesh = self.meshes.current.meshPtr.get()
			local meshName = mesh:getName()
			local lodDefManager = Ember.OgreView.Lod.LodDefinitionManager:getSingleton()
			local lodDefPtr = lodDefManager:getByName(self:getLodDefName(meshName))
			lodDefManager:exportScript(meshName, lodDefPtr)			
			return true
		end)
		
		self.widget:getWindow("InputOkButton"):subscribeEvent("Clicked", function(args)
			self.widget:getWindow("InputWindow"):setVisible(false)
			self.waitingAnswer = false
			local name = self.widget:getWindow("InputAnswerTextbox"):getText()
			self:answerFunc(name)
			return true
		end)

		self.widget:getWindow("InputCancelButton"):subscribeEvent("Clicked", function(args)
			self.widget:getWindow("InputWindow"):setVisible(false)
			self.waitingAnswer = false
			return true
		end)
		
		-- subscribe LOD events.
		self.widget:getWindow("EnableAutomaticLOD"):subscribeEvent("CheckStateChanged", function(args)
			local checkbox = self.widget:getWindow("EnableAutomaticLOD")
			checkbox = CEGUI.toCheckbox(checkbox)
			local useAuto = checkbox:isSelected()
			
			local loddef = self.meshes.current.lodDefPtr.get()
			loddef:setUseAutomaticLod(useAuto)

			self:LODUpdate()
			return true
		end)
		
		self.widget:getWindow("LODAddButton"):subscribeEvent("Clicked", function(args)
			self:getInput("Please enter the distance.", self.LODAdd)
			return true
		end)
		
		self.widget:getWindow("LODDeleteButton"):subscribeEvent("Clicked", function(args)
			local dist = self:LODGetSelected()
			if dist then
				local listbox = self.widget:getWindow("LODDistances")
				listbox = CEGUI.toListbox(listbox)
				local item = listbox:getFirstSelectedItem()
				listbox:removeItem(item)
				local loddef = self.meshes.current.lodDefPtr.get()
				loddef:removeLodDistance(dist)
				self:LODUpdate()
			end
			return true
		end)
		
		self.widget:getWindow("LODCopyButton"):subscribeEvent("Clicked", function(args)
			local loddef = self.meshes.current.lodDefPtr.get()
			local distance = self:LODGetSelected()
			if not distance or not loddef:hasLodDistance(distance) then return end
			
			distdef = loddef:getLodDistance(distance)
			
			self:LODSave(distance)
			self.clipboard = {}
			self.clipboard.type = loddef:getType()
			self.clipboard.meshName = distdef:getMeshName()
			self.clipboard.reductionMethod = distdef:getReductionMethod()
			self.clipboard.reductionValue = distdef:getReductionValue()
			return true
		end)
		
		self.widget:getWindow("LODPasteButton"):subscribeEvent("Clicked", function(args)
			self:getInput("Please enter the distance.", self.LODPaste)
			return true
		end)
		
		self.widget:getWindow("LODTypeCombobox"):subscribeEvent("ListSelectionAccepted", function(args)
			local combobox = self.widget:getWindow("LODTypeCombobox")
			local item = combobox:getSelectedItem()
			local type = item and combobox:getItemIndex(item) or 0
			local loddef = self.meshes.current.lodDefPtr.get()
			loddef:setType(type)
			if type == 1 then
				self.meshes.userrefresh(self)
			end
			self:LODUpdate()
			return true
		end)
		
		self.widget:getWindow("LODStrategyCombobox"):subscribeEvent("ListSelectionAccepted", function(args)
			local combobox = self.widget:getWindow("LODStrategyCombobox")
			local item = combobox:getSelectedItem()
			local strategy = item and combobox:getItemIndex(item) or 0
			local loddef = self.meshes.current.lodDefPtr.get()
			loddef:setStrategy(strategy)
			self:LODSortDistances()
			self:LODUpdate()
			return true
		end)
		
		self.widget:getWindow("LODReductionTypeCombobox"):subscribeEvent("ListSelectionAccepted", function(args)
			self:LODUpdate()
			return true
		end)
		
		self.widget:getWindow("LODDistances"):subscribeEvent("ItemSelectionChanged", function(args)
			self:LODUpdate()
			return true
		end)
		
		self.widget:getWindow("LODValueScroll"):subscribeEvent("ScrollPosChanged", function(args)
			if not self.meshes.current.scrollOnStack then
				self.meshes.current.scrollOnStack = true
				local scroll = self.widget:getWindow("LODValueScroll")
				scroll = CEGUI.toScrollbar(scroll)
				local value = scroll:getScrollPosition()
				self:LODUpdateScroll(true, false, value)
				self:LODUpdate()
				self.meshes.current.scrollOnStack = false
			end
			return true
		end)
		
		self.widget:getWindow("LODReductionParameterTextbox"):subscribeEvent("TextChanged", function(args)
			if not self.meshes.current.scrollOnStack then
				self.meshes.current.scrollOnStack = true
				self:LODUpdate()
				self:LODUpdateScroll(false, true)
				self.meshes.current.scrollOnStack = false
			end
			return true
		end)
		
		self.widget:getWindow("ShowWireFrameCheckbox"):subscribeEvent("CheckStateChanged", function(args)
			local checkbox = self.widget:getWindow("ShowWireFrameCheckbox")
			checkbox = CEGUI.toCheckbox(checkbox)
			local useWireframe = checkbox:isSelected()
			self.meshes.renderer:setWireframeMode(useWireframe)
			return true
		end)
		
		self.widget:getWindow("ForceLodLevelCheckbox"):subscribeEvent("CheckStateChanged", function(args)
			self:LODUpdateForcedLevel()
			return true
		end)

		-- Fill LOD Comboboxes.
		self:fillLODTypeCombobox()
		self:fillLODStrategyCombobox()
		self:fillLODReductionTypeCombobox()
			
		self.controls.tabs:subscribeEvent("TabSelectionChanged", function(args)
			local selected = self.controls.tabs:getSelectedTabIndex()
			local tab = self.controls.tabs:getTabContentsAtIndex(selected)
			local name = tab:getText()
			if name == "Meshes" then
				local meshesList = self.widget:getWindow("MeshesList")
				local count = meshesList:getItemCount()
				if count == 0 then
					self.meshes.refresh(self)
				end
			end
		end)
	
		self.helper = Ember.OgreView.Gui.AssetsManager:new()
	
		self.widget:enableCloseButton()
	end
	
	connect(self.connectors, self.widget.EventFirstTimeShown, setup)
	self.widget:loadMainSheet("AssetsManager.layout", "AssetsManager/")
	self.widget:registerConsoleVisibilityToggleCommand("assetsManager")
	

	--See if we automatically should show a certain mesh. This is useful for authoring when one wants to inspect a specific mesh.	
	if emberServices:getConfigService():itemExists("authoring", "loadmesh") then
		local meshNameVar = emberServices:getConfigService():getValue("authoring", "loadmesh")
		local meshName = meshNameVar:as_string()
		meshName = self.helper:resolveResourceNameFromFilePath(meshName)
		self.widget:show()
		self.controls.tabs:setSelectedTab("Meshes")
		self:showMesh(meshName)
	end


end

function AssetsManager:shutdown()
	disconnectAll(self.connectors)
	deleteSafe(self.helper)
	deleteSafe(self.shaders.listholder)
	deleteSafe(self.meshes.materialListholder)
	deleteSafe(self.meshes.rendererStats)
	deleteSafe(self.meshes.renderer)
	deleteSafe(self.meshes.rendererManipulator)
	deleteSafe(self.meshes.listholder)
	deleteSafe(self.meshes.userlistholder)
	deleteSafe(self.windows.listholder)
	deleteSafe(self.images.listholder)
	deleteSafe(self.materials.listholder)
	deleteSafe(self.textures.listholder)
	guiManager:destroyWidget(self.widget)
end

local assetsManagerInit = function()
	local assetsManager = {connectors={}, 
		controls = {},
		textures = {controls = {}, listbox = nil,selectedTexture = nil}, 
		materials = {controls = {}, listbox = nil,selectedTexture = nil}, 
		images = {controls = {}, listbox = nil,selectedTexture = nil}, 
		windows = {controls = {}, listbox = nil, selectedWindow = nil},
		meshes = {controls = {}, listbox = nil, selectedWindow = nil, current = {}},
		shaders = {controls = {}, listbox = nil, selectedTexture = nil}
	}
	setmetatable(assetsManager, {__index = AssetsManager})
	
	assetsManager:buildWidget()

	local injector = Ember.OgreView.Lod.PMInjectorSignaler:getSingleton()
	connect(assetsManager.connectors, injector.LodInjected, function()
		if assetsManager and assetsManager.meshes.current and assetsManager.meshes.current.meshPtr then
			assetsManager.meshes.current.lodUnderRegeneration = false
			if assetsManager.meshes.current.lodNeedsRegenerate then
				assetsManager:LODRegenerateLods()
			end
			
			assetsManager:LODUpdateForcedLevel()
			assetsManager:updatePreviewInfo()
			assetsManager:updateMeshInfo()
		end
	end)
	
	connect(assetsManager.connectors, emberOgre.EventGUIManagerBeingDestroyed, function()
		assetsManager:shutdown()
		assetsManager = nil
	end)
end
assetsManagerInit()

