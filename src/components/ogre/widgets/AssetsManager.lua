--Lists all of the graphical resources available

AssetsManager = {}

--Reloads a resource
function AssetsManager:reloadResource(manager, resourceName)
    local resourcePtr = manager:getResourceByName(resourceName, Ogre.ResourceGroupManager.AUTODETECT_RESOURCE_GROUP_NAME)
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

function AssetsManager:TexturesList_SelectionChanged(args)
    local item = self.textures.controls.listbox:getFirstSelectedItem()
    if item ~= nil then
        local textureName = item:getText()
        local texturePair = self.helper:showTexture(textureName)
        if texturePair:hasData() then
            self.textures.controls.textureView:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair.textureImage()))
            if texturePair.ogreTexture:get():isReloadable() == true then
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

function AssetsManager:MaterialsList_SelectionChanged(args)
    local item = self.materials.controls.listbox:getFirstSelectedItem()
    if item ~= nil then
        local manager = Ogre.MaterialManager:getSingleton()
        local materialName = item:getText()
        local res = manager:getByName(materialName, Ogre.ResourceGroupManager.AUTODETECT_RESOURCE_GROUP_NAME)
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

function AssetsManager:ShadersList_SelectionChanged(args)
    local item = self.shaders.controls.listbox:getFirstSelectedItem()
    if item ~= nil then
        local manager = Ogre.HighLevelGpuProgramManager:getSingleton()
        local materialName = item:getText()
        local res = manager:getByName(materialName, Ogre.ResourceGroupManager.AUTODETECT_RESOURCE_GROUP_NAME)
        if res ~= nil then
            res = tolua.cast(res, "Ogre::GpuProgramPtr")
            local resPtr = res:get()
            if resPtr ~= nil then
                resPtr:load()
                self.shaders.controls.textWidget:setProperty("Text", resPtr:getSource())
            end
        end
    end
end

function AssetsManager:ShadersReload_Clicked(args)
    self:reloadResourceFromList(self.shaders.controls.listbox, Ogre.HighLevelGpuProgramManager:getSingleton())
    self:ShadersList_SelectionChanged(args)
end

function AssetsManager:RefreshImages_Clicked(args)
    self.images.refresh(self)
end

function AssetsManager:ImagesList_SelectionChanged(args)
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

function AssetsManager:MeshesList_SelectionChanged(args)
    local item = self.meshes.controls.listbox:getFirstSelectedItem()
    local meshInfo = self.widget:getWindow("MeshInfo")
    meshInfo:setEnabled(item ~= nil)
    if item then
        self:showMesh(item:getText())
    end
    return true
end

function AssetsManager:UserMeshList_SelectionChanged(args)
    local item = self.meshes.controls.userlistbox:getFirstSelectedItem()
    if item then
        self.meshes.current.userlistboxSelected = item:getText()
        self.meshes.controls.userlistbox:ensureItemIsVisible(item)
    end
    return true
end

function AssetsManager:SubMeshesList_SelectionChanged(args)
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

function AssetsManager:SubMeshMaterialsList_SelectionChanged(args)
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
    local meshPtr = manager:getByName(mesh:getName(), Ogre.ResourceGroupManager.AUTODETECT_RESOURCE_GROUP_NAME)
    self.meshes.renderer:unloadEntity()
    mesh:removeLodLevels()
    self.helper:exportMesh(meshPtr, self.helper:resolveFilePathForMesh(meshPtr))
    self:LODRegenerateLods()
    return true
end

function AssetsManager:MeshInfoLoadMeshButton_Clicked(args)
    local mesh = self.meshes.current.meshPtr.get()
    --first reload the skeleton, then the mesh
    if mesh:hasSkeleton() and mesh:getSkeleton() and mesh:getSkeleton():get() then
        mesh:getSkeleton():get():reload()
    end
    mesh:reload()
    return true
end

function AssetsManager:CreateModel_Clicked(args)
    local mesh = self.meshes.current.meshPtr.get()
    local manager = Ogre.MeshManager:getSingleton()
    local meshPtr = manager:getByName(mesh:getName(), Ogre.ResourceGroupManager.AUTODETECT_RESOURCE_GROUP_NAME)
    self.helper:createModel(meshPtr)
    return true
end

function AssetsManager:showMesh(meshName)
    self.meshes.renderer:showEntity(meshName)

    --we need to get hold of a mesh instance
    local manager = Ogre.MeshManager:getSingleton()
    local meshPtr = manager:getByName(meshName, Ogre.ResourceGroupManager.AUTODETECT_RESOURCE_GROUP_NAME)
    self.meshes.current = {}
    self.meshes.current.userlistboxSelected = ""
    self.meshes.current.meshPtr = {}
    self.meshes.current.meshPtr.get = function()
        return manager:getByName(meshName, Ogre.ResourceGroupManager.AUTODETECT_RESOURCE_GROUP_NAME):get()
    end

    local animationNames = self.meshes.renderer:getEntityAnimationNames()
    self.meshes.controls.animationCombobox:resetList()
    self.meshes.controls.animationCombobox:clearAllSelections()
    if animationNames:size() > 0 then
        for i = 0, animationNames:size() - 1 do
            local animationName = animationNames[i]
            local animationItem = Ember.OgreView.Gui.ColouredListItem:new(animationName)
            self.meshes.controls.animationCombobox:addItem(animationItem)
        end
    end

    self:fillSubMeshList(meshPtr)
    local mesh = meshPtr:get()
    self.meshes.controls.skeletonPath:setText(mesh:getSkeletonName())

    -- Fill distances list.
    local lodDefManager = Ember.OgreView.Lod.LodDefinitionManager:getSingleton()
    self.meshes.current.lodDefPtr = {}
    self.meshes.current.lodDefPtr.get = function()
        return lodDefManager:getByName(self:getLodDefName(meshName), Ogre.ResourceGroupManager.AUTODETECT_RESOURCE_GROUP_NAME):get()
    end
    local lodlevelsList = self.widget:getWindow("LODDistances")
    lodlevelsList = CEGUI.toListbox(lodlevelsList)
    lodlevelsList:resetList()
    local loddef = self.meshes.current.lodDefPtr.get()
    if not loddef then
        lodDefManager:create(self:getLodDefName(mesh:getName()), mesh:getGroup())
        loddef = self.meshes.current.lodDefPtr.get()
    end
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
    if num == 0 then
        return 0
    end
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
            log.info("There's no material '" .. materialName .. "' available.")
        end
    end
end

function AssetsManager:RefreshWindows_Clicked(args)
    self.windows.refresh(self)
end

function AssetsManager:WindowsList_SelectionChanged(args)
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
        info = info .. "Size: " .. CEGUI.PropertyHelper:usizeToString(self.windows.selectedWindow:getSize()) .. "\n"
        self.windows.controls.infoText:setText(info)
    end
end

function AssetsManager:WindowsList_SelectStateChanged(args)
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
    for i = 0, level do
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
        local lodDefPtr = lodDefManager:getByName(self:getLodDefName(meshName), Ogre.ResourceGroupManager.AUTODETECT_RESOURCE_GROUP_NAME)
        local lodDef = lodDefPtr:get()

        local manager = Ogre.MeshManager:getSingleton()
        local meshPtr = manager:getByName(meshName, Ogre.ResourceGroupManager.AUTODETECT_RESOURCE_GROUP_NAME)

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
    if dist == nil or dist < 0 or not loddef:hasLodDistance(dist) then
        return
    end

    local distdef = loddef:getLodDistance(dist)

    distdef.meshName = self.meshes.current.userlistboxSelected

    local combobox = self.widget:getWindow("LODReductionTypeCombobox")
    combobox = CEGUI.toCombobox(combobox)
    local item = combobox:getSelectedItem()
    local reductionMethodIndex
    if item then
        reductionMethodIndex = combobox:getItemIndex(item)
    else
        reductionMethodIndex = 0
    end

    if reductionMethodIndex == 0 then
        distdef.reductionMethod = Ogre.LodLevel.VRM_PROPORTIONAL
    elseif reductionMethodIndex == 1 then
        distdef.reductionMethod = Ogre.LodLevel.VRM_CONSTANT
    else
        distdef.reductionMethod = Ogre.LodLevel.VRM_COLLAPSE_COST
    end

    local editbox = self.widget:getWindow("LODReductionParameterTextbox")
    local reductionValue = editbox:getText()
    distdef.reductionValue = reductionValue

    local meshName = self.meshes.current.userlistboxSelected or ""
    distdef.meshName = meshName
end

function AssetsManager:LODLoad(dist)
    local loddef = self.meshes.current.lodDefPtr.get()

    local useAuto = loddef:getUseAutomaticLod()

    local container = self.widget:getWindow("ManualLODContainer")
    container:setEnabled(not useAuto)

    local checkbox = self.widget:getWindow("EnableAutomaticLOD")
    checkbox = CEGUI.toToggleButton(checkbox)
    checkbox:setSelected(useAuto)

    local container = self.widget:getWindow("LODConfigContainer")
    if not dist or not loddef:hasLodDistance(dist) then
        container:setEnabled(false)
        return
    end

    container:setEnabled(true)
    distdef = loddef:getLodDistance(dist)

    local meshName = distdef.meshName
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
    local index = 0
    if distdef.reductionMethod == Ogre.LodLevel.VRM_PROPORTIONAL then
        index = 0
    elseif distdef.reductionMethod == Ogre.LodLevel.VRM_CONSTANT then
        index = 1
    else
        index = 2
    end

    combobox:setItemSelectState(index, true)

    if not self.meshes.current.scrollOnStack then
        self:LODUpdateScroll(true, true)
    end

    self.meshes.current.userlistboxSelected = distdef.meshName
end

function AssetsManager:LODUpdate()

    local loddef = self.meshes.current.lodDefPtr.get()
    if not self.meshes.current.updateOnStack and loddef then
        -- recursion protection
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
    checkbox = CEGUI.toToggleButton(checkbox)
    local shouldForce = checkbox:isSelected()
    local dist = self:LODGetSelected()
    if shouldForce and dist then
        local mesh = self.meshes.current.meshPtr.get()
        local loddef = self.meshes.current.lodDefPtr.get()
        if not loddef then
            return
        end
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
    if not loddef then
        return
    end
    distance = tonumber(distance)
    if not distance or loddef:hasLodDistance(distance) then
        return
    end

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
    if not distance or loddef:hasLodDistance(distance) then
        return
    end

    local listbox = self.widget:getWindow("LODDistances")
    listbox = CEGUI.toListbox(listbox)
    distdef = loddef:createDistance(distance)

    local type = self.clipboard.type
    loddef:setType(type)

    local meshname = self.clipboard.meshName
    distdef.meshName = meshname

    local reductionMethod = self.clipboard.reductionMethod
    distdef.reductionMethod = reductionMethod

    local reductionValue = self.clipboard.reductionValue
    distdef.reductionValue = reductionValue

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
        table.sort(distances, function(a, b)
            return a > b
        end)
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
        value = distdef.reductionValue
    end

    local scrollSize
    local stepSize
    if distdef.reductionMethod == 0 then
        -- Proportional
        scrollSize = 1
        stepSize = 0.01
    elseif distdef.reductionMethod == 1 then
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
        self.textures.controls.textureView = self.widget:getWindow("TextureInfo_Image")
        self.textures.adapter = Ember.OgreView.Gui.Adapters.Ogre.ResourceListAdapter:new_local(self.textures.listholder, Ogre.TextureManager:getSingleton())
        self.textures.refresh = function(self)
            self.textures.adapter:update()
        end
        self.widget:getWindow("TexturesRefresh"):subscribeEvent("Clicked", self.TexturesRefresh_Clicked, self)
        self.widget:getWindow("TexturesReload"):subscribeEvent("Clicked", self.TexturesReload_Clicked, self)
        self.textures.controls.listbox:subscribeEvent("SelectionChanged", self.TexturesList_SelectionChanged, self)


        --the materials part
        self.materials.controls.listbox = CEGUI.toListbox(self.widget:getWindow("MaterialsList"))
        self.materials.controls.filter = CEGUI.toEditbox(self.widget:getWindow("FilterMaterials"))
        self.materials.listholder = Ember.OgreView.Gui.ListHolder:new(self.materials.controls.listbox, self.materials.controls.filter)
        self.materials.controls.textWidget = self.widget:getWindow("MaterialInfo_Text")
        self.materials.adapter = Ember.OgreView.Gui.Adapters.Ogre.ResourceListAdapter:new_local(self.materials.listholder, Ogre.MaterialManager:getSingleton())
        self.materials.refresh = function(self)
            self.materials.adapter:update()
        end
        self.widget:getWindow("MaterialsRefresh"):subscribeEvent("Clicked", self.MaterialsRefresh_Clicked, self)
        self.widget:getWindow("MaterialsReload"):subscribeEvent("Clicked", self.MaterialsReload_Clicked, self)
        self.widget:getWindow("MaterialsList"):subscribeEvent("SelectionChanged", self.MaterialsList_SelectionChanged, self)


        --the images part
        self.images.controls.listbox = CEGUI.toListbox(self.widget:getWindow("ImagesList"))
        self.images.controls.filter = CEGUI.toEditbox(self.widget:getWindow("FilterImages"))
        self.images.listholder = Ember.OgreView.Gui.ListHolder:new(self.images.controls.listbox, self.images.controls.filter)
        self.images.controls.textureView = self.widget:getWindow("ImagesInfo_Image")
        self.images.refresh = function(self)
            self.images.listholder:resetList()
            local manager = CEGUI.ImageManager:getSingleton()
            local I = manager:getIterator()
            while I:isAtEnd() == false do
                local name = I:key()
                local image = I:value().first
                local item = Ember.OgreView.Gui.ColouredListItem:new(name, 0, image)
                self.images.listholder:addItem(item)

                I:next()
            end
        end
        self.widget:getWindow("ImagesRefresh"):subscribeEvent("Clicked", self.RefreshImages_Clicked, self)
        self.widget:getWindow("ImagesList"):subscribeEvent("SelectionChanged", self.ImagesList_SelectionChanged, self)


        --the windows part
        self.windows.controls.listbox = CEGUI.toListbox(self.widget:getWindow("WindowsList"))
        self.windows.controls.filter = CEGUI.toEditbox(self.widget:getWindow("FilterWindows"))
        self.windows.listholder = Ember.OgreView.Gui.ListHolder:new(self.windows.controls.listbox, self.windows.controls.filter)
        self.windows.controls.visibleCheckbox = CEGUI.toToggleButton(self.widget:getWindow("WindowInfo_Visible"))
        self.windows.controls.infoText = self.widget:getWindow("WindowInfo_Text")
        self.windows.controls.widthRel = self.widget:getWindow("WindowInfo_Width_rel")
        self.windows.controls.widthFixed = self.widget:getWindow("WindowInfo_Width_fix")
        self.windows.controls.heightRel = self.widget:getWindow("WindowInfo_Height_rel")
        self.windows.controls.heightFixed = self.widget:getWindow("WindowInfo_Height_fix")

        local updateWindowSize = function(args)
            if self.windows.selectedWindow then
                if self.windows.controls.widthRel:getText() ~= "" and self.windows.controls.widthFixed:getText() ~= "" and self.windows.controls.heightRel:getText() ~= "" and self.windows.controls.heightFixed:getText() ~= "" then
                    local dim = CEGUI.USize(CEGUI.UDim(tonumber(self.windows.controls.widthRel:getText()), tonumber(self.windows.controls.widthFixed:getText())), CEGUI.UDim(tonumber(self.windows.controls.heightRel:getText()), tonumber(self.windows.controls.heightFixed:getText())))
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

            self.windows.addWindow(self, CEGUI.System:getSingleton():getDefaultGUIContext():getRootWindow(), 0)
        end

        self.windows.addWindow = function(self, window, depth)
            if window ~= nil then
                local label = ""
                for i = 0, depth do
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
        self.widget:getWindow("WindowsList"):subscribeEvent("SelectionChanged", self.WindowsList_SelectionChanged, self)
        self.widget:getWindow("WindowInfo_Visible"):subscribeEvent("SelectStateChanged", self.WindowsList_SelectStateChanged, self)


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
        self.meshes.controls.textureView = self.widget:getWindow("MeshInfo_Preview")
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
        self.meshes.controls.animationCombobox = CEGUI.toCombobox(self.widget:getWindow("MeshAnimation"))
        self.meshes.controls.showSkeletonCheckbox = CEGUI.toToggleButton(self.widget:getWindow("MeshShowSkeleton"))

        self.widget:getWindow("MeshesRefresh"):subscribeEvent("Clicked", self.MeshesRefresh_Clicked, self)
        self.widget:getWindow("MeshesList"):subscribeEvent("SelectionChanged", self.MeshesList_SelectionChanged, self)
        self.widget:getWindow("UserMeshList"):subscribeEvent("SelectionChanged", self.UserMeshList_SelectionChanged, self)
        self.widget:getWindow("SubMeshesList"):subscribeEvent("SelectionChanged", self.SubMeshesList_SelectionChanged, self)
        self.widget:getWindow("SubMeshMaterialsList"):subscribeEvent("SelectionChanged", self.SubMeshMaterialsList_SelectionChanged, self)
        self.widget:getWindow("MeshInfoSaveMeshButton"):subscribeEvent("Clicked", self.MeshInfoSaveMeshButton_Clicked, self)
        self.widget:getWindow("MeshInfoLoadMeshButton"):subscribeEvent("Clicked", self.MeshInfoLoadMeshButton_Clicked, self)
        self.widget:getWindow("CreateModel"):subscribeEvent("Clicked", self.CreateModel_Clicked, self)


        --the shaders part
        self.shaders.controls.listbox = CEGUI.toListbox(self.widget:getWindow("ShadersList"))
        self.shaders.controls.filter = CEGUI.toEditbox(self.widget:getWindow("FilterShaders"))
        self.shaders.listholder = Ember.OgreView.Gui.ListHolder:new(self.shaders.controls.listbox, self.shaders.controls.filter)
        self.shaders.controls.textWidget = self.widget:getWindow("ShadersInfo_Text")
        self.shaders.adapter = Ember.OgreView.Gui.Adapters.Ogre.ResourceListAdapter:new_local(self.shaders.listholder, Ogre.HighLevelGpuProgramManager:getSingleton())
        self.shaders.refresh = function(self)
            self.shaders.adapter:update()
        end
        self.widget:getWindow("RefreshShaders"):subscribeEvent("Clicked", self.RefreshShaders_Clicked, self)
        self.widget:getWindow("ShadersReload"):subscribeEvent("Clicked", self.ShadersReload_Clicked, self)
        self.widget:getWindow("ShadersList"):subscribeEvent("SelectionChanged", self.ShadersList_SelectionChanged, self)

        self.widget:getWindow("MeshInfoSaveLoddefButton"):subscribeEvent("Clicked", function(args)
            self:LODUpdate()
            local mesh = self.meshes.current.meshPtr.get()
            local meshName = mesh:getName()
            local lodDefManager = Ember.OgreView.Lod.LodDefinitionManager:getSingleton()
            local lodDefPtr = lodDefManager:getByName(self:getLodDefName(meshName), Ogre.ResourceGroupManager.AUTODETECT_RESOURCE_GROUP_NAME)
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
        self.widget:getWindow("EnableAutomaticLOD"):subscribeEvent("SelectStateChanged", function(args)
            local checkbox = self.widget:getWindow("EnableAutomaticLOD")
            checkbox = CEGUI.toToggleButton(checkbox)
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
            if not distance or not loddef:hasLodDistance(distance) then
                return
            end

            local distdef = loddef:getLodDistance(distance)

            self:LODSave(distance)
            self.clipboard = {}
            self.clipboard.type = loddef:getType()
            self.clipboard.meshName = distdef.meshName
            self.clipboard.reductionMethod = distdef.reductionMethod
            self.clipboard.reductionValue = distdef.reductionValue
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

        self.widget:getWindow("LODDistances"):subscribeEvent("SelectionChanged", function(args)
            self:LODUpdate()
            return true
        end)

        self.widget:getWindow("LODValueScroll"):subscribeEvent("ScrollPositionChanged", function(args)
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

        self.widget:getWindow("ShowWireFrameCheckbox"):subscribeEvent("SelectStateChanged", function(args)
            local checkbox = self.widget:getWindow("ShowWireFrameCheckbox")
            checkbox = CEGUI.toToggleButton(checkbox)
            local useWireframe = checkbox:isSelected()
            self.meshes.renderer:setWireframeMode(useWireframe)
            return true
        end)

        self.widget:getWindow("ShowMovingLight"):subscribeEvent("SelectStateChanged", function(args)
            local checkbox = self.widget:getWindow("ShowMovingLight")
            checkbox = CEGUI.toToggleButton(checkbox)
            local showMovingLight = checkbox:isSelected()
            self.meshes.renderer:setShowMovingLight(showMovingLight)
            return true
        end)

        self.widget:getWindow("MeshAnimation"):subscribeEvent("ListSelectionAccepted", function(args)
            local combobox = self.widget:getWindow("MeshAnimation")
            local item = combobox:getSelectedItem()
            if item then
                local animation = item:getText()
                self.meshes.renderer:enableAnimation(animation)
            end
            return true
        end)

        self.widget:getWindow("MeshShowSkeleton"):subscribeEvent("SelectStateChanged", function(args)
            local checkbox = self.widget:getWindow("MeshShowSkeleton")
            checkbox = CEGUI.toToggleButton(checkbox)
            local isSelected = checkbox:isSelected()
            self.meshes.renderer:setShowSkeleton(isSelected)
            return true
        end)

        self.widget:getWindow("MeshResetView"):subscribeEvent("Clicked", function(args)
            self.meshes.renderer:resetCameraOrientation()
            return true
        end)

        self.widget:getWindow("ForceLodLevelCheckbox"):subscribeEvent("SelectStateChanged", function(args)
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


        connect(self.connectors, self.helper:getLodInjectorSignaler().LodInjected, function()
            if self.meshes.current and self.meshes.current.meshPtr then
                self.meshes.current.lodUnderRegeneration = false
                if self.meshes.current.lodNeedsRegenerate then
                    self:LODRegenerateLods()
                end

                self:LODUpdateForcedLevel()
                self:updatePreviewInfo()
                self:updateMeshInfo()
            end
        end)

        self.widget:enableCloseButton()
    end

    connect(self.connectors, self.widget.EventFirstTimeShown, setup)
    self.widget:loadMainSheet("AssetsManager.layout", "AssetsManager")
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
    local assetsManager = {
        connectors = {},
        controls = {},
        textures = { controls = {}, listbox = nil, selectedTexture = nil },
        materials = { controls = {}, listbox = nil, selectedTexture = nil },
        images = { controls = {}, listbox = nil, selectedTexture = nil },
        windows = { controls = {}, listbox = nil, selectedWindow = nil },
        meshes = { controls = {}, listbox = nil, selectedWindow = nil, current = {} },
        shaders = { controls = {}, listbox = nil, selectedTexture = nil }
    }
    setmetatable(assetsManager, { __index = AssetsManager })

    assetsManager:buildWidget()



    connect(assetsManager.connectors, emberOgre.EventGUIManagerBeingDestroyed, function()
        assetsManager:shutdown()
        assetsManager = nil
    end)
end
assetsManagerInit()

