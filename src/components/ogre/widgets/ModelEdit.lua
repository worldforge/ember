ModelEdit = {}
ModelEdit.zoomRatio = 10
ModelEdit.modelContentsItems = {}

function ModelEdit_selectMaterial(subentity)
	if subentity == nil then
		--there is no subentity (for example if the part doesn't have any defined), just clear the selection
		ModelEdit.materials:clearAllSelections()
	else
		local materialName = subentity:getMaterialName()
		local item
--		if materialName == "" then
			--no material specified, get the default for this subentity
--			ModelEdit.model
--		else 
		if materialName ~= "" then
			item = ModelEdit.contentparts.submeshInfo.materiallist:findItemWithText(subentity:getMaterialName(), ModelEdit.contentparts.submeshInfo.materiallist:getListboxItemFromIndex(0))
		end
		
		if item ~= nil then
			ModelEdit.contentparts.submeshInfo.materiallist:setItemSelectState(item, true)
			ModelEdit.contentparts.submeshInfo.materiallist:ensureItemIsVisible(item)
		else
			ModelEdit.contentparts.submeshInfo.materiallist:clearAllSelections()
		end
	end
end

function ModelEdit_updateMaterial(subentity, material)
	if subentity ~= nil then
		subentity:setMaterialName(material)
		ModelEdit_reloadModel()
	end
end

function ModelEdit_updatePartShown(part, shown)
	if part ~= nil then
		part:setShow(shown)
		ModelEdit_reloadModel()
	end
end

function ModelEdit_reloadModel()
	local model = ModelEdit.renderer:getModel()
	if model ~= nil then
		model:reload()
	end
end

function ModelEdit_fillMaterialList()
	ModelEdit.contentparts.submeshInfo.listholder:resetList()
	materialDefMgr = Ogre.MaterialManager:getSingleton()
	local I = materialDefMgr:getResourceIterator()
	local i = 0
	while I:hasMoreElements() do
		local materialDef = I:getNext()
		materialDef = tolua.cast(materialDef, "Ogre::MaterialPtr")
		local material = materialDef:get()
		local name = material:getName()
		local item = EmberOgre.ColoredListItem:new(name, i)
		ModelEdit.contentparts.submeshInfo.listholder:addItem(item)
		i = i + 1
	end
end

function ModelEdit_fillMeshList()
	local manager = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	local meshes = manager:getAllMeshes()
	
	for i = 0, meshes:size() - 1 do
		local name = meshes[i]
		local item = EmberOgre.ColoredListItem:new(name, i)
		ModelEdit.contentparts.modelInfo.meshlist:addItem(item)
		
	end	
end

function ModelEdit_fillSubMeshList(part)
	local list = ModelEdit.widget:getWindow("SubmeshList")
	list = CEGUI.toListbox(list)
	list:resetList()
	
	--we need to get hold of a mesh instance
	local manager = Ogre.MeshManager:getSingleton()
	local name = part:getSubModelDefinition():getMeshName()
	local meshPtr = manager:getByName(name)
	--meshPtr = tolua.cast(meshPtr, "Ogre::MeshPtr")
	local mesh = meshPtr:get()
	
	
	--for now, since we don't have any good method for getting the submodel names yet we'll just use the index numbers
	local numberOfSubmeshes = mesh:getNumSubMeshes()
	local i = 0
	while i < numberOfSubmeshes do
		local item = EmberOgre.ColoredListItem:new(i, i)
		list:addItem(item)
		i = i + 1
	end	
end

function ModelEdit_loadModelDefinition(definitionName)
	modelDefMgr = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	ModelEdit.definitionPtr = modelDefMgr:getByName(definitionName)
	ModelEdit.definition = ModelEdit.definitionPtr:get()
	ModelEdit_showPreview(definitionName)
	
	--ModelEdit_updateSubmodelsList()
	ModelEdit_updateModelContentList()
	ModelEdit_updateModelInfo()
	ModelEdit_showModel()

end

function ModelEdit_updateSubmodelsList()
	ModelEdit.submodels:resetList()
	ModelEdit.submodels:clearAllSelections()
	local submodels = ModelEdit.definition:getSubModelDefinitions()
	for val = 0, submodels:size() - 1 do
		local name = submodels[val]:getMeshName()
		local item = EmberOgre.ColoredListItem:new(name, val)
		ModelEdit.submodels:addItem(item)
	end	
end

function ModelEdit_updatePartsList(submodel)
	ModelEdit.parts:resetList()
	ModelEdit.parts:clearAllSelections()
	if submodel ~= nil then
		local parts = submodel:getPartDefinitions()
		for val = 0, parts:size() - 1 do
			local name = parts[val]:getName()
			local item = EmberOgre.ColoredListItem:new(name, val)
			ModelEdit.parts:addItem(item)
		end
	end
end



function ModelEdit_updateSubentitiesList(part)
	ModelEdit.subentities:resetList()
	ModelEdit.subentities:clearAllSelections()
	if part ~= nil then
		local subentities = part:getSubEntityDefinitions()
		for val = 0, subentities:size() - 1 do
			local name = subentities[val]:getSubEntityName()
			if name == "" then
				name = subentities[val]:getSubEntityIndex()
			end
			local item = EmberOgre.ColoredListItem:new(name, val)
			ModelEdit.subentities:addItem(item)
		end
	end
end

function ModelEdit_showPreview(definitionName)
	ModelEdit.renderer:showModel(definitionName)
	--ModelEdit.renderer:showFullModel()

end

function ModelEdit_fillModellist()
	ModelEdit.modelslistholder:resetList()
	ModelEdit.models:clearAllSelections()
	local modelDefMgr = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	local I = modelDefMgr:getResourceIterator()
	local i = 0
	while I:hasMoreElements() do
		local modelPtr = I:getNext()
		modelPtr = tolua.cast(modelPtr, "EmberOgre::Model::ModelDefnPtr")
		local model = modelPtr:get()
		local name = model:getName()
		local item = EmberOgre.ColoredListItem:new(name, i)
		ModelEdit.modelslistholder:addItem(item)
		i = i + 1
	end

end

function ModelEdit_updateModelInfo()
	ModelEdit.widget:getWindow("ModelName_Text"):setText("Name: " .. ModelEdit.definition:getName())
	ModelEdit.widget:getWindow("ModelScale"):setText(ModelEdit.definition:getScale())
	ModelEdit.widget:getWindow("ModelRotation"):setText(ModelEdit.definition:getRotation())
	ModelEdit_fillWindowsFromVector("ModelContainedOffset", ModelEdit.definition:getContentOffset())
	ModelEdit_fillWindowsFromVector("ModelTranslate", ModelEdit.definition:getTranslate())
	
	local showContent = ModelEdit.widget:getWindow("ModelShowContent")
	
	showContent = CEGUI.toCheckbox(showContent)
	showContent:setSelected(ModelEdit.definition:getShowContained())
	
	ModelEdit.scaleTypes:clearAllSelections()
	ModelEdit.scaleTypes:setItemSelectState(ModelEdit.definition:getUseScaleOf(), true)
end

function ModelEdit_fillWindowsFromVector(windowNamePrefix, vector)
		local xWindow = ModelEdit.widget:getWindow(windowNamePrefix .. "_x");
		local yWindow = ModelEdit.widget:getWindow(windowNamePrefix .. "_y");
		local zWindow = ModelEdit.widget:getWindow(windowNamePrefix .. "_z");
	if vector ~= nil then
		xWindow:setText(vector.x)
		yWindow:setText(vector.y)
		zWindow:setText(vector.z)
	else 
		xWindow:setText("")
		yWindow:setText("")
		zWindow:setText("")
	end
	
end

function ModelEdit_fillVectorFromWindows(windowNamePrefix, vector)
	local xWindow = ModelEdit.widget:getWindow(windowNamePrefix .. "_x");
	local yWindow = ModelEdit.widget:getWindow(windowNamePrefix .. "_y");
	local zWindow = ModelEdit.widget:getWindow(windowNamePrefix .. "_z");
	
	vector.x = tonumber(xWindow:getText())
	vector.y = tonumber(xWindow:getText())
	vector.z = tonumber(xWindow:getText())
	
end


function ModelEdit_models_SelectionChanged(args)
	local item = ModelEdit.models:getFirstSelectedItem()
	ModelEdit_loadModelDefinition(item:getText())
end

function ModelEdit_parts_SelectionChanged(args)
	local part = ModelEdit_getSelectedPart()
	ModelEdit_updateSubentitiesList(part)
	ModelEdit_updateShownCheckbox(part)
end

function ModelEdit_submodels_SelectionChanged(args)
	local submodel = ModelEdit_getSelectedSubModel()
	--inspectObject(submodel)
	ModelEdit_updatePartsList(submodel)
end

function ModelEdit_subentities_SelectionChanged(args)
	local subentity = ModelEdit_getSelectedSubEntity()
	ModelEdit_selectMaterial(subentity)
end

function ModelEdit_submeshinfomaterials_SelectionChanged(args)
	local item = ModelEdit.contentparts.submeshInfo.materiallist:getFirstSelectedItem()
	if item ~= nil then 
		local material = item:getText()
		local subentity = ModelEdit_getSelectedSubEntity()
		--inspectObject(subentity)
		ModelEdit_updateMaterial(subentity, material)
	end
end

function ModelEdit_ModelUseScaleOf_SelectionChanged(args)
	local model = ModelEdit.definition
	local item = ModelEdit.scaleTypes:getSelectedItem()
	if item ~= nil then
		model:setUseScaleOf(item:getID())
	end
end

function ModelEdit_submeshinforemovesubmesh_MouseClick(args)
	--just remove the subentity definition from the part
	local subentity = ModelEdit_getSelectedSubEntity()
	local part = subentity:getPartDefinition()
	part:removeSubEntityDefinition(subentity)
	ModelEdit_reloadModel()
	ModelEdit_updateModelContentList()
end

function ModelEdit_removePart_MouseClick(args)
	--just remove the part definition from the submodel
	local part = ModelEdit_getSelectedPart()
	local submodel = part:getSubModelDefinition()
	submodel:removePartDefinition(part)
	ModelEdit_reloadModel()
	ModelEdit_updateModelContentList()
end

function ModelEdit_addSubmesh_MouseClick(args)
--get the selected submesh and add it to the part
	local list = ModelEdit.widget:getWindow("SubmeshList")
	list = CEGUI.toListbox(list)
	local item = list:getFirstSelectedItem()
	--an item must be selected
	if item ~= nil then
		local part = ModelEdit_getSelectedPart()
		part:createSubEntityDefinition(item:getID())
		ModelEdit_reloadModel()
		ModelEdit_updateModelContentList()
	end

end

function ModelEdit_SaveModelButton_MouseClick(args)
	local modelDefMgr = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	modelDefMgr:exportScript(ModelEdit.definitionPtr)	
	inspectObject(ModelEdit.definition:getName())

end

function ModelEdit_AddSubmodelButton_MouseClick(args)
	
	local item = ModelEdit.contentparts.modelInfo.meshlist:getFirstSelectedItem()
	--an item must be selected
	if item ~= nil then
		local submodel = ModelEdit.definition:createSubModelDefinition(item:getText())
		--lets create a "main" part automatically
		local part = submodel:createPartDefinition("main");
		--and lets add all submeshes to this new part
		
		--we need to get hold of a mesh instance
		local manager = Ogre.MeshManager:getSingleton()
		local name = submodel:getMeshName()
		local meshPtr = manager:load(name, "General")
		local mesh = meshPtr:get()
		
		if mesh ~= nil then
			--for now, since we don't have any good method for getting the submodel names yet we'll just use the index numbers
			local numberOfSubmeshes = mesh:getNumSubMeshes()
			for i = 0, numberOfSubmeshes - 1 do
				part:createSubEntityDefinition(i)
			end
		end
		
		
		ModelEdit_reloadModel()
		--update the renderer so that the camera is repositioned and the complete model is shown
		ModelEdit.renderer:showModel(ModelEdit.definition:getName())
		ModelEdit_updateModelContentList()
	end

end

function ModelEdit_RemoveSubmodelButton_MouseClick(args)

	local submodel = ModelEdit_getSelectedSubModel()
	ModelEdit.definition:removeSubModelDefinition(submodel)
		
	ModelEdit_reloadModel()
	ModelEdit_updateModelContentList()
	
end


function ModelEdit_addPart_MouseClick(args)
	local editbox = ModelEdit.widget:getWindow("NewPartName")
	local name = editbox:getText()
	if name ~= "" then
		local submodel = ModelEdit_getSelectedSubModel()
		if submodel ~= nil then
			submodel:createPartDefinition(name);
			ModelEdit_reloadModel()
			ModelEdit_updateModelContentList()
		end
	end
end

function ModelEdit_renamePart_MouseClick(args)
	local editbox = ModelEdit.widget:getWindow("PartName")
	local name = editbox:getText()
	if name ~= "" then
		local part = ModelEdit_getSelectedPart()
		if part ~= nil then
			part:setName(name)
			ModelEdit_reloadModel()
			ModelEdit_updateModelContentList()
		end
	end	
end


function ModelEdit_AddModelButton_MouseClick(args)
	ModelEdit.widget:getWindow("NewModelWindow"):setVisible(true)
	ModelEdit.widget:getWindow("NewModelWindow"):moveToFront()
end

function ModelEdit_NewModelCancel_MouseClick(args)
	ModelEdit.widget:getWindow("NewModelWindow"):setVisible(false)
end

function ModelEdit_NewModelOk_MouseClick(args)
	local modelDefMgr = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	local name = ModelEdit.widget:getWindow("NewModelName"):getText()
	def = modelDefMgr:create(name, "ModelDefinitions"):get()
	def:setValid(true)
	
	--after adding the definition, update the model list
	ModelEdit_fillModellist()
	local item = ModelEdit.models:findItemWithText(name, ModelEdit.models:getListboxItemFromIndex(0))
	if item ~= nil then
		item:setSelected(true)
		ModelEdit.models:ensureItemIsVisible(item)	
	end
	ModelEdit.widget:getWindow("NewModelWindow"):setVisible(false)
	ModelEdit_loadModelDefinition(item:getText())
	ModelEdit_reloadModel()
	ModelEdit_updateModelContentList()
end


function ModelEdit_previewMesh(meshname)
	ModelEdit.submeshRenderer:showEntity(meshname)
end

function ModelEdit_modelinfoMeshlist_SelectionChanged()
	local item = ModelEdit.contentparts.modelInfo.meshlist:getFirstSelectedItem()
	ModelEdit_previewMesh(item:getText())

end


function ModelEdit_partShown_StateChanged(args)
	local shown = ModelEdit.partShown:isSelected()
	local part = ModelEdit_getSelectedPart()
	--inspectObject(part)
	ModelEdit_updatePartShown(part, shown)	
end

function ModelEdit_modelcontents_SelectionChanged(args)
	local item = ModelEdit.modelcontents:getFirstSelectedItem()
	ModelEdit_showModelContent(item)
end


function ModelEdit_zoom_ValueChanged(args)
	local zoomValue = ModelEdit.zoomSlider:getCurrentValue()
	ModelEdit.renderer:setCameraDistance(zoomValue)
end

function ModelEdit_getSelectedSubModel()
	local item = ModelEdit_getCurrentModelContentItem()
	
	return item.submodel
end

function ModelEdit_getSelectedPart()
	local item = ModelEdit_getCurrentModelContentItem()
	
	return item.part
	
end

function ModelEdit_getSelectedSubEntity()
	local item = ModelEdit_getCurrentModelContentItem()
	
	return item.subentity
end


function ModelEdit_updateModelContentList()
	ModelEdit.modelcontents:resetList()
	ModelEdit.modelcontents:clearAllSelections()
	
	ModelEdit.modelContentsItems = {}
	

	
	local submodels = ModelEdit.definition:getSubModelDefinitions()
	
	--first, add all submodels
	for val = 0, submodels:size() - 1 do
		local submodel = submodels[val]
		local name = submodel:getMeshName()
		local modelcontentItem = {}
		modelcontentItem.type = "submodel"
		modelcontentItem.name = name
		modelcontentItem.submodel = submodel
		ModelEdit.modelContentsItems[table.getn(ModelEdit.modelContentsItems) + 1] = modelcontentItem
		
		local item = EmberOgre.ColoredListItem:new(name, table.getn(ModelEdit.modelContentsItems))
		ModelEdit.modelcontents:addItem(item)
		
		--add all parts
		if submodel ~= nil then
			local parts = submodel:getPartDefinitions()
			for val_ = 0, parts:size() - 1 do
				local part = parts[val_]
				local name = part:getName()
				
				local modelcontentItem = {}
				modelcontentItem.type = "part"
				modelcontentItem.name = name
				modelcontentItem.part = part
				ModelEdit.modelContentsItems[table.getn(ModelEdit.modelContentsItems) + 1] = modelcontentItem
				
				--prefix parts with "-"
				local item = EmberOgre.ColoredListItem:new(" - " .. name, table.getn(ModelEdit.modelContentsItems))
				ModelEdit.modelcontents:addItem(item)
			
				if part ~= nil then
					local subentities = part:getSubEntityDefinitions()
					for val = 0, subentities:size() - 1 do
						local subentity = subentities[val]
						local name = subentity:getSubEntityName()
						if name == "" then
							name = subentity:getSubEntityIndex()
						end
						
						
						local modelcontentItem = {}
						modelcontentItem.type = "subentity"
						modelcontentItem.name = name
						modelcontentItem.subentity = subentity
						ModelEdit.modelContentsItems[table.getn(ModelEdit.modelContentsItems) + 1] = modelcontentItem
						
						--prefix subentities with " -- "
						local item = EmberOgre.ColoredListItem:new(" -- " .. name, table.getn(ModelEdit.modelContentsItems))
						ModelEdit.modelcontents:addItem(item)
					end
				end
			
			
			end
		end		
		
	end	
	
	
end


function ModelEdit_getCurrentModelContentItem()
	local item = ModelEdit.modelcontents:getFirstSelectedItem()
	if item ~= nil then
		local itemId= item:getID()
		local contentItem = ModelEdit.modelContentsItems[itemId]
		return contentItem
	end
	return nil
end

function ModelEdit_showModelContent(listitem)
	if listitem ~= nil then
		local itemId= listitem:getID()
		local contentItem = ModelEdit.modelContentsItems[itemId]
		--inspectObject(contentItem.type)
		
		if contentItem.type == "submodel" then
			ModelEdit_showSubModel(contentItem)
		elseif contentItem.type == "part" then
			ModelEdit_showPart(contentItem)
		elseif contentItem.type == "subentity" then
			ModelEdit_showSubEntity(contentItem)
		else
			ModelEdit_showModel()
		end
	else
		ModelEdit_showModel()
	end
end

function ModelEdit_hideAllContentParts()
	ModelEdit.contentparts.partInfo:setVisible(false)
	ModelEdit.contentparts.modelInfo:setVisible(false)
	ModelEdit.contentparts.submodelInfo:setVisible(false)
	ModelEdit.contentparts.submeshInfo:setVisible(false)
end

function ModelEdit_showSubModel(contentItem)
	ModelEdit_hideAllContentParts()
	ModelEdit.contentparts.submodelInfo:setVisible(true)
	local submodel = ModelEdit_getSelectedSubModel()
	
	ModelEdit.widget:getWindow("SubModelName"):setText(submodel:getMeshName())
end

function ModelEdit_showPart(contentItem)
	ModelEdit_hideAllContentParts()
	ModelEdit.contentparts.partInfo:setVisible(true)
	local part = contentItem.part
	ModelEdit.widget:getWindow("PartName"):setText(part:getName())
	ModelEdit.partShown:setSelected(part:getShow())
	
	ModelEdit_fillSubMeshList(part)
end


function ModelEdit_showSubEntity(contentItem)
	ModelEdit_hideAllContentParts()
	ModelEdit.contentparts.submeshInfo:setVisible(true)
	ModelEdit_selectMaterial(contentItem.subentity)
end

function ModelEdit_showModel()
	ModelEdit_hideAllContentParts()
	ModelEdit.contentparts.modelInfo:setVisible(true)
end

function ModelEdit_buildWidget()
	ModelEdit.widget = guiManager:createWidget()

	ModelEdit.widget:loadMainSheet("ModelEdit.layout", "ModelEdit/")
	
	ModelEdit.renderImage = ModelEdit.widget:getWindow("Image")
	ModelEdit.renderImage = CEGUI.toStaticImage(ModelEdit.renderImage)

	ModelEdit.models = ModelEdit.widget:getWindow("Models")
	ModelEdit.models = CEGUI.toListbox(ModelEdit.models)
	ModelEdit.models:subscribeEvent("ItemSelectionChanged", "ModelEdit_models_SelectionChanged")
	ModelEdit.modelsfilter = ModelEdit.widget:getWindow("FilterModels")
	ModelEdit.modelsfilter = CEGUI.toEditbox(ModelEdit.modelsfilter)
	ModelEdit.modelslistholder = EmberOgre.ListHolder:new(ModelEdit.models, ModelEdit.modelsfilter)
	
	

	ModelEdit.contentparts = {}
	ModelEdit.contentparts.partInfo = ModelEdit.widget:getWindow("PartInfo")
	ModelEdit.contentparts.modelInfo = ModelEdit.widget:getWindow("ModelInfo")
	ModelEdit.contentparts.submodelInfo = ModelEdit.widget:getWindow("SubModelInfo")
	ModelEdit.contentparts.submeshInfo = ModelEdit.widget:getWindow("SubMeshInfo")
	
	--hide all parts initially
	ModelEdit_hideAllContentParts()
	
	
	ModelEdit.contentparts.modelInfo.meshlist = ModelEdit.widget:getWindow("MeshList")
	ModelEdit.contentparts.modelInfo.meshlist = CEGUI.toListbox(ModelEdit.contentparts.modelInfo.meshlist)
	ModelEdit.contentparts.modelInfo.meshlist:subscribeEvent("ItemSelectionChanged", "ModelEdit_modelinfoMeshlist_SelectionChanged")
	
	ModelEdit.contentparts.submeshInfo.materiallist = ModelEdit.widget:getWindow("Materials")
	ModelEdit.contentparts.submeshInfo.materiallist = CEGUI.toListbox(ModelEdit.contentparts.submeshInfo.materiallist)
	ModelEdit.contentparts.submeshInfo.materiallist:subscribeEvent("ItemSelectionChanged", "ModelEdit_submeshinfomaterials_SelectionChanged")
	ModelEdit.contentparts.submeshInfo.filter = ModelEdit.widget:getWindow("FilterMaterials")
	ModelEdit.contentparts.submeshInfo.filter = CEGUI.toEditbox(ModelEdit.contentparts.submeshInfo.filter)
	ModelEdit.contentparts.submeshInfo.listholder = EmberOgre.ListHolder:new(ModelEdit.contentparts.submeshInfo.materiallist, ModelEdit.contentparts.submeshInfo.filter)
	
	ModelEdit.contentparts.submeshInfo.removeSubMeshButton = ModelEdit.widget:getWindow("RemoveSubMeshButton")
	ModelEdit.contentparts.submeshInfo.removeSubMeshButton:subscribeEvent("MouseClick", "ModelEdit_submeshinforemovesubmesh_MouseClick")
	
	ModelEdit.widget:getWindow("PartRemoveButton"):subscribeEvent("MouseClick", "ModelEdit_removePart_MouseClick")
	ModelEdit.widget:getWindow("AddSubmeshButton"):subscribeEvent("MouseClick", "ModelEdit_addSubmesh_MouseClick")
	ModelEdit.widget:getWindow("AddPartButton"):subscribeEvent("MouseClick", "ModelEdit_addPart_MouseClick")
	ModelEdit.widget:getWindow("RenamePartButton"):subscribeEvent("MouseClick", "ModelEdit_renamePart_MouseClick")
	ModelEdit.widget:getWindow("AddModelButton"):subscribeEvent("MouseClick", "ModelEdit_AddModelButton_MouseClick")
	ModelEdit.widget:getWindow("AddSubmodelButton"):subscribeEvent("MouseClick", "ModelEdit_AddSubmodelButton_MouseClick")
	ModelEdit.widget:getWindow("SaveModelButton"):subscribeEvent("MouseClick", "ModelEdit_SaveModelButton_MouseClick")
	ModelEdit.widget:getWindow("NewModelOk"):subscribeEvent("MouseClick", "ModelEdit_NewModelOk_MouseClick")
	ModelEdit.widget:getWindow("NewModelCancel"):subscribeEvent("MouseClick", "ModelEdit_NewModelCancel_MouseClick")
	ModelEdit.widget:getWindow("RemoveSubmodelButton"):subscribeEvent("MouseClick", "ModelEdit_RemoveSubmodelButton_MouseClick")
	

	ModelEdit.contentparts.modelInfo.renderImage =  ModelEdit.widget:getWindow("MeshPreviewImage")
	ModelEdit.contentparts.modelInfo.renderImage = CEGUI.toStaticImage(ModelEdit.contentparts.modelInfo.renderImage)
	
--	ModelEdit.materials = ModelEdit.widget:getWindow("Materials")
--	ModelEdit.materials = CEGUI.toListbox(ModelEdit.materials)
--	ModelEdit.materials:subscribeEvent("ItemSelectionChanged", "ModelEdit_materials_SelectionChanged")
	
	
	ModelEdit.modelcontents = ModelEdit.widget:getWindow("ModelContents")
	ModelEdit.modelcontents = CEGUI.toListbox(ModelEdit.modelcontents)
	ModelEdit.modelcontents:subscribeEvent("ItemSelectionChanged", "ModelEdit_modelcontents_SelectionChanged")
	ModelEdit.modelcontents:subscribeEvent("ListItemsChanged", "ModelEdit_modelcontents_SelectionChanged")
	
	--ModelEdit.modelcontents:setMutedState(true)

	
	ModelEdit.partShown = ModelEdit.widget:getWindow("Shown")
	ModelEdit.partShown = CEGUI.toCheckbox(ModelEdit.partShown)
	ModelEdit.partShown:subscribeEvent("CheckStateChanged", "ModelEdit_partShown_StateChanged")
		
	
		
	ModelEdit.renderer = EmberOgre.ModelRenderer:new(ModelEdit.renderImage)
	
	local subMeshPreviewImage = ModelEdit.widget:getWindow("SubMeshPreviewImage")
	subMeshPreviewImage = CEGUI.toStaticImage(subMeshPreviewImage)
	ModelEdit.subMeshPartRenderer = EmberOgre.OgreEntityRenderer:new(subMeshPreviewImage)
	
	local meshPreviewImage = ModelEdit.widget:getWindow("MeshPreviewImage")
	meshPreviewImage = CEGUI.toStaticImage(meshPreviewImage)
	ModelEdit.submeshRenderer = EmberOgre.OgreEntityRenderer:new(meshPreviewImage)
	
	--ModelEdit.contentparts.modelInfo.renderer = EmberOgre.ModelRenderer:new(ModelEdit.contentparts.modelInfo.renderImage)
	
	ModelEdit.zoomSlider  = ModelEdit.widget:getWindow("Zoom")
	ModelEdit.zoomSlider = CEGUI.toSlider(ModelEdit.zoomSlider)
	ModelEdit.zoomSlider:subscribeEvent("ValueChanged", "ModelEdit_zoom_ValueChanged")
	
	
	ModelEdit_fillMaterialList()
	ModelEdit_fillModellist()
	ModelEdit_fillMeshList()
	ModelEdit_fillScaleTypesList()
	--def:setValid(true)	
	--model = ModelEdit.renderer:getModel()
	--def = model:getDefinition():get()
	
	ModelEdit.widget:registerConsoleVisibilityToggleCommand("modelEdit")
	ModelEdit.widget:enableCloseButton()
	ModelEdit.widget:hide()

end

function ModelEdit_fillScaleTypesList()
	ModelEdit.scaleTypes  = ModelEdit.widget:getWindow("ModelUseScaleOf")
	ModelEdit.scaleTypes = CEGUI.toCombobox( ModelEdit.scaleTypes)
	
	local item = EmberOgre.ColoredListItem:new("all", 0)
	ModelEdit.scaleTypes:addItem(item)
	local item = EmberOgre.ColoredListItem:new("none", 1)
	ModelEdit.scaleTypes:addItem(item)
	local item = EmberOgre.ColoredListItem:new("width", 2)
	ModelEdit.scaleTypes:addItem(item)
	local item = EmberOgre.ColoredListItem:new("depth", 3)
	ModelEdit.scaleTypes:addItem(item)
	local item = EmberOgre.ColoredListItem:new("height", 4)
	ModelEdit.scaleTypes:addItem(item)
	
	ModelEdit.scaleTypes:subscribeEvent("ListSelectionChanged", "ModelEdit_ModelUseScaleOf_SelectionChanged")

end


function test()
	ModelEdit.renderer:showModel("settler")
	model = ModelEdit.renderer:getModel()
	def = model:getDefinition()
	modelDefMgr:exportScript(def)

end

function test2()
	modelDefMgr = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	def = modelDefMgr:create("test2", "ModelDefinitions"):get()
	def:setValid(true)
	submodel = def:createSubModelDefinition("3d_objects/items/cylinder/models/barrel/cylinder_barrel_closed.mesh")
	part = submodel:createPartDefinition("main")
	part:setShow(true)
	
	ModelEdit.renderer:showModel("test2")
end

ModelEdit_buildWidget()

--model:reload()

--submodel2 = def:createSubModelDefinition("3d_objects/items/building_primitives/models/2_room_house/2_room_house.mesh")
--part2 = submodel:createPartDefinition("main")
--part2:setShow(true)









