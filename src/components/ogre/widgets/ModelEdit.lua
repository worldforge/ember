ModelEdit = {connectors={}}
ModelEdit.zoomRatio = 10
ModelEdit.modelContentsItems = {}

function ModelEdit.selectMaterial(subentity)
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

function ModelEdit.updateMaterial(subentity, material)
	if subentity ~= nil then
		subentity:setMaterialName(material)
		ModelEdit.reloadModel()
	end
end

function ModelEdit.updatePartShown(part, shown)
	if part ~= nil then
		part:setShow(shown)
		ModelEdit.reloadModel()
--		ModelEdit.updateModelContentList()
	end
end

function ModelEdit.reloadModel()
	local model = ModelEdit.renderer:getModel()
	if model ~= nil then
		model:reload()
	end
end

function ModelEdit.fillMaterialList()
	ModelEdit.contentparts.submeshInfo.listholder:resetList()
	materialDefMgr = Ogre.MaterialManager:getSingleton()
	local I = materialDefMgr:getResourceIterator()
	local i = 0
	while I:hasMoreElements() do
		local materialDef = I:getNext()
		materialDef = tolua.cast(materialDef, "Ogre::MaterialPtr")
		local material = materialDef:get()
		local name = material:getName()
		local item = EmberOgre.Gui.ColouredListItem:new(name, i)
		ModelEdit.contentparts.submeshInfo.listholder:addItem(item)
		i = i + 1
	end
end

function ModelEdit.fillMeshList()
	local manager = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	local meshes = manager:getAllMeshes()
	
	for i = 0, meshes:size() - 1 do
		local name = meshes[i]
		local item = EmberOgre.Gui.ColouredListItem:new(name, i)
		ModelEdit.contentparts.modelInfo.meshlistlistholder:addItem(item)
--		ModelEdit.contentparts.modelInfo.meshlist:addItem(item)
		
	end	
end

function ModelEdit.fillSubMeshList(part)
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
		local submeshname = ModelEdit.getSubMeshName(mesh, i)
		local item = EmberOgre.Gui.ColouredListItem:new(submeshname, i)
		list:addItem(item)
		i = i + 1
	end	
end

function ModelEdit.getSubMeshName(mesh, index)
	local submeshname = EmberOgre.OgreUtils:getSubMeshName(mesh, index)
	if submeshname == "" then
		submeshname = "(index: " .. index .. ")"
	end
	return submeshname
end

function ModelEdit.loadModelDefinition(definitionName)
	modelDefMgr = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	ModelEdit.definitionPtr = modelDefMgr:getByName(definitionName)
	ModelEdit.definition = ModelEdit.definitionPtr:get()
	ModelEdit.showPreview(definitionName)
	
	--ModelEdit.updateSubmodelsList()
	ModelEdit.updateModelContentList()
	ModelEdit.updateModelInfo()
	ModelEdit.showModel()

end

function ModelEdit.updateSubmodelsList()
	ModelEdit.submodels:resetList()
	ModelEdit.submodels:clearAllSelections()
	local submodels = ModelEdit.definition:getSubModelDefinitions()
	for val = 0, submodels:size() - 1 do
		local name = submodels[val]:getMeshName()
		local item = EmberOgre.Gui.ColouredListItem:new(name, val)
		ModelEdit.submodels:addItem(item)
	end	
end

function ModelEdit.updatePartsList(submodel)
	ModelEdit.parts:resetList()
	ModelEdit.parts:clearAllSelections()
	if submodel ~= nil then
		local parts = submodel:getPartDefinitions()
		for val = 0, parts:size() - 1 do
			local name = parts[val]:getName()
			local item = EmberOgre.Gui.ColouredListItem:new(name, val)
			ModelEdit.parts:addItem(item)
		end
	end
end



function ModelEdit.updateSubentitiesList(part)
	ModelEdit.subentities:resetList()
	ModelEdit.subentities:clearAllSelections()
	if part ~= nil then
		local subentities = part:getSubEntityDefinitions()
		for val = 0, subentities:size() - 1 do
			local name = subentities[val]:getSubEntityName()
			if name == "" then
				name = subentities[val]:getSubEntityIndex()
			end
			local item = EmberOgre.Gui.ColouredListItem:new(name, val)
			ModelEdit.subentities:addItem(item)
		end
	end
end

function ModelEdit.showPreview(definitionName)
	ModelEdit.renderer:showModel(definitionName)
	--ModelEdit.renderer:showFullModel()

end

function ModelEdit.fillModellist()
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
		local item = EmberOgre.Gui.ColouredListItem:new(name, i)
		ModelEdit.modelslistholder:addItem(item)
		i = i + 1
	end

end

function ModelEdit.updateModelInfo()
	ModelEdit.widget:getWindow("ModelName_Text"):setText("Name: " .. ModelEdit.definition:getName())
	ModelEdit.scaleTextbox:setText(ModelEdit.definition:getScale())
	--ModelEdit.widget:getWindow("ModelRotation"):setText(ModelEdit.definition:getRotation())
	ModelEdit.rotationAdapter:updateGui(ModelEdit.definition:getRotation());
	ModelEdit.translateAdapter:updateGui(ModelEdit.definition:getTranslate());
	ModelEdit.containedOffsetAdapter:updateGui(ModelEdit.definition:getContentOffset());
--	ModelEdit.fillWindowsFromVector("ModelContainedOffset", ModelEdit.definition:getContentOffset())
--	ModelEdit.fillWindowsFromVector("ModelTranslate", ModelEdit.definition:getTranslate())
	
	local showContent = ModelEdit.widget:getWindow("ModelShowContent")
	
	showContent = CEGUI.toCheckbox(showContent)
	showContent:setSelected(ModelEdit.definition:getShowContained())
	
	ModelEdit.scaleTypes:clearAllSelections()
	ModelEdit.scaleTypes:setItemSelectState(ModelEdit.definition:getUseScaleOf(), true)
	
	ModelEdit.zoomSlider:setCurrentValue(ModelEdit.renderer:getCameraDistance())

end

function ModelEdit.translateAdapter_update()
	ModelEdit.definition:setTranslate(ModelEdit.translateAdapter:getValue())
	ModelEdit.renderer:repositionSceneNode()
end

function ModelEdit.containedOffsetAdapter_update()
	ModelEdit.definition:setContentOffset(ModelEdit.containedOffsetAdapter:getValue())
	ModelEdit.renderer:repositionSceneNode()
end

function ModelEdit.rotationAdapter_update()
	ModelEdit.definition:setRotation(ModelEdit.rotationAdapter:getValue())
	ModelEdit.renderer:repositionSceneNode()
end



function ModelEdit.fillWindowsFromVector(windowNamePrefix, vector)
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

function ModelEdit.fillVectorFromWindows(windowNamePrefix, vector)
	local xWindow = ModelEdit.widget:getWindow(windowNamePrefix .. "_x");
	local yWindow = ModelEdit.widget:getWindow(windowNamePrefix .. "_y");
	local zWindow = ModelEdit.widget:getWindow(windowNamePrefix .. "_z");
	
	vector.x = tonumber(xWindow:getText())
	vector.y = tonumber(xWindow:getText())
	vector.z = tonumber(xWindow:getText())
	
end


function ModelEdit.models_SelectionChanged(args)
	local item = ModelEdit.models:getFirstSelectedItem()
	ModelEdit.loadModelDefinition(item:getText())
end

function ModelEdit.parts_SelectionChanged(args)
	local part = ModelEdit.getSelectedPart()
	ModelEdit.updateSubentitiesList(part)
	ModelEdit.updateShownCheckbox(part)
end

function ModelEdit.submodels_SelectionChanged(args)
	local submodel = ModelEdit.getSelectedSubModel()
	--inspectObject(submodel)
	ModelEdit.updatePartsList(submodel)
end

function ModelEdit.subentities_SelectionChanged(args)
	local subentity = ModelEdit.getSelectedSubEntity()
	ModelEdit.selectMaterial(subentity)
end

function ModelEdit.submeshinfomaterials_SelectionChanged(args)
	local item = ModelEdit.contentparts.submeshInfo.materiallist:getFirstSelectedItem()
	if item ~= nil then 
		local material = item:getText()
		local subentity = ModelEdit.getSelectedSubEntity()
		--inspectObject(subentity)
		ModelEdit.updateMaterial(subentity, material)
	end
end

function ModelEdit.ModelUseScaleOf_SelectionChanged(args)
	local model = ModelEdit.definition
	local item = ModelEdit.scaleTypes:getSelectedItem()
	if item ~= nil then
		model:setUseScaleOf(item:getID())
	end
end

function ModelEdit.ModelScale_TextChanged(args)
	local model = ModelEdit.definition
	model:setScale(tonumber(ModelEdit.scaleTextbox:getText()));
end

function ModelEdit.YawLeft_MouseClick(args)
	ModelEdit.renderer:yaw(Ogre.Degree:new_local(-45))
end

function ModelEdit.YawRight_MouseClick(args)
	ModelEdit.renderer:yaw(Ogre.Degree:new_local(45))
end

function ModelEdit.PitchUp_MouseClick(args)
	ModelEdit.renderer:pitch(Ogre.Degree:new_local(-45))
end

function ModelEdit.PitchDown_MouseClick(args)
	ModelEdit.renderer:pitch(Ogre.Degree:new_local(45))
end

function ModelEdit.RollLeft_MouseClick(args)
	ModelEdit.renderer:roll(Ogre.Degree:new_local(-45))
end

function ModelEdit.RollRight_MouseClick(args)
	ModelEdit.renderer:roll(Ogre.Degree:new_local(45))
end

function ModelEdit.ResetOrientation_MouseClick(args)
	ModelEdit.renderer:resetCameraOrientation()
end


function ModelEdit.submeshinforemovesubmesh_MouseClick(args)
	--just remove the subentity definition from the part
	local subentity = ModelEdit.getSelectedSubEntity()
	local part = subentity:getPartDefinition()
	part:removeSubEntityDefinition(subentity)
	ModelEdit.reloadModel()
	ModelEdit.updateModelContentList()
end

function ModelEdit.removePart_MouseClick(args)
	--just remove the part definition from the submodel
	local part = ModelEdit.getSelectedPart()
	local submodel = part:getSubModelDefinition()
	submodel:removePartDefinition(part)
	ModelEdit.reloadModel()
	ModelEdit.updateModelContentList()
end

function ModelEdit.addSubmesh_MouseClick(args)
--get the selected submesh and add it to the part
	local list = ModelEdit.widget:getWindow("SubmeshList")
	list = CEGUI.toListbox(list)
	local item = list:getFirstSelectedItem()
	--an item must be selected
	if item ~= nil then
		local part = ModelEdit.getSelectedPart()
		part:createSubEntityDefinition(item:getID())
		ModelEdit.reloadModel()
		ModelEdit.updateModelContentList()
	end

end

function ModelEdit.ReloadModelListButton_MouseClick(args)
	ModelEdit.fillModellist()
end

function ModelEdit.SaveModelButton_MouseClick(args)
	local modelDefMgr = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	modelDefMgr:exportScript(ModelEdit.definitionPtr)	
	inspectObject(ModelEdit.definition:getName())

end

function ModelEdit.ExportAsAtlasTypeButton_MouseClick(args)
	local model = ModelEdit.renderer:getModel()
	if model ~= nil then
		local composer = EmberOgre.Model.ModelDefinitionAtlasComposer:new_local()
		composer:composeToFile(model, ModelEdit.definition:getName(), "thing", ModelEdit.definition:getScale())
	end	
end

function ModelEdit.AddSubmodelButton_MouseClick(args)
	
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
		
		
		ModelEdit.reloadModel()
		--update the renderer so that the camera is repositioned and the complete model is shown
		ModelEdit.renderer:showModel(ModelEdit.definition:getName())
		ModelEdit.updateModelContentList()
	end

end

function ModelEdit.ReloadInstancesButton_MouseClick(args)
	--reload all model instances
	ModelEdit.definition:reloadAllInstances()
end


function ModelEdit.GetRotationFromPreviewButton_MouseClick(args)
	--Get the rotation from the preview window

	ModelEdit.rotationAdapter:setValue(ModelEdit.renderer:getEntityRotation())
end

function ModelEdit.GetIconFromPreviewButton_MouseClick(args)
	if ModelEdit.definition ~= nil then
		local definition = ModelEdit.definition:createViewDefinition("icon")
		definition.Rotation = ModelEdit.renderer:getEntityRotation():Inverse()
		--only set the camera distance if it has been moved from the optimal position
		if ModelEdit.renderer:getCameraDistance() ~= 1 then
			definition.Distance = ModelEdit.renderer:getAbsoluteCameraDistance()
		end
	end
end

function ModelEdit.RemoveSubmodelButton_MouseClick(args)

	local submodel = ModelEdit.getSelectedSubModel()
	ModelEdit.definition:removeSubModelDefinition(submodel)
		
	ModelEdit.reloadModel()
	ModelEdit.updateModelContentList()
	
end


function ModelEdit.addPart_MouseClick(args)
	local editbox = ModelEdit.widget:getWindow("NewPartName")
	local name = editbox:getText()
	if name ~= "" then
		local submodel = ModelEdit.getSelectedSubModel()
		if submodel ~= nil then
			submodel:createPartDefinition(name);
			ModelEdit.reloadModel()
			ModelEdit.updateModelContentList()
		end
	end
end

function ModelEdit.renamePart_MouseClick(args)
	local editbox = ModelEdit.widget:getWindow("PartName")
	local name = editbox:getText()
	if name ~= "" then
		local part = ModelEdit.getSelectedPart()
		if part ~= nil then
			part:setName(name)
			ModelEdit.reloadModel()
			ModelEdit.updateModelContentList()
		end
	end	
end


function ModelEdit.AddModelButton_MouseClick(args)
	ModelEdit.widget:getWindow("NewModelWindow"):setVisible(true)
	ModelEdit.widget:getWindow("NewModelWindow"):moveToFront()
end

function ModelEdit.NewModelCancel_MouseClick(args)
	ModelEdit.widget:getWindow("NewModelWindow"):setVisible(false)
end

function ModelEdit.NewModelOk_MouseClick(args)
	local modelDefMgr = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	local name = ModelEdit.widget:getWindow("NewModelName"):getText()
	def = modelDefMgr:create(name, "ModelDefinitions"):get()
	def:setValid(true)
	
	--after adding the definition, update the model list
	ModelEdit.fillModellist()
	local item = ModelEdit.models:findItemWithText(name, ModelEdit.models:getListboxItemFromIndex(0))
	if item ~= nil then
		item:setSelected(true)
		ModelEdit.models:ensureItemIsVisible(item)	
	end
	ModelEdit.widget:getWindow("NewModelWindow"):setVisible(false)
	ModelEdit.loadModelDefinition(item:getText())
	ModelEdit.reloadModel()
	ModelEdit.updateModelContentList()
end


function ModelEdit.previewMesh(meshname)
	ModelEdit.submeshRenderer:showEntity(meshname)
end

function ModelEdit.modelinfoMeshlist_SelectionChanged()
	local item = ModelEdit.contentparts.modelInfo.meshlist:getFirstSelectedItem()
	ModelEdit.previewMesh(item:getText())

end


function ModelEdit.partShown_StateChanged(args)
	local shown = ModelEdit.partShown:isSelected()
	local part = ModelEdit.getSelectedPart()
	--inspectObject(part)
	ModelEdit.updatePartShown(part, shown)	
end

function ModelEdit.modelcontents_SelectionChanged(args)
	local item = ModelEdit.modelcontentstree:getFirstSelectedItem()
	ModelEdit.showModelContent(item)
end


function ModelEdit.zoom_ValueChanged(args)
	local zoomValue = ModelEdit.zoomSlider:getCurrentValue()
	ModelEdit.renderer:setCameraDistance(zoomValue)
end

function ModelEdit.getSelectedSubModel()
	local item = ModelEdit.getCurrentModelContentItem()
	
	return item.submodel
end

function ModelEdit.getSelectedPart()
	local item = ModelEdit.getCurrentModelContentItem()
	
	return item.part
	
end

function ModelEdit.getSelectedSubEntity()
	local item = ModelEdit.getCurrentModelContentItem()
	
	return item.subentity
end


function ModelEdit.updateModelContentList()
	ModelEdit.modelcontentstree:resetList()
	--ModelEdit.modelcontents:clearAllSelections()
	
	ModelEdit.modelContentsItems = {}
	

	
	local submodels = ModelEdit.definition:getSubModelDefinitions()
	
	--first, add all submodels
	for val = 0, submodels:size() - 1 do
		local submodel = submodels[val]
		local name = submodel:getMeshName()
		local modelcontentItem = {}
		
		--we need to get hold of a mesh instance
		local manager = Ogre.MeshManager:getSingleton()
		local meshPtr = manager:getByName(name)
		local mesh = meshPtr:get()
		
		
		modelcontentItem.type = "submodel"
		modelcontentItem.name = name
		modelcontentItem.submodel = submodel
		ModelEdit.modelContentsItems[table.getn(ModelEdit.modelContentsItems) + 1] = modelcontentItem
		
		local treeItem = EmberOgre.Gui.ColouredTreeItem:new(name, table.getn(ModelEdit.modelContentsItems)) 
		treeItem:toggleIsOpen()
		treeItem:setTooltipText("Mesh '" .. name .. "'")
		ModelEdit.modelcontentstree:addItem(treeItem)
		
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
				
				local partVisible = ""
--				if part:getShow() then
--					partVisible = " (shown)";
--				end
				
				ModelEdit.modelContentsItems[table.getn(ModelEdit.modelContentsItems) + 1] = modelcontentItem
				
				local treeItem2 = EmberOgre.Gui.ColouredTreeItem:new(name .. partVisible, table.getn(ModelEdit.modelContentsItems))
				treeItem2:toggleIsOpen()
				treeItem2:setTooltipText("Part '" .. name .. "'")
				treeItem:addItem(treeItem2)
			
				if part ~= nil then
					local subentities = part:getSubEntityDefinitions()
					for val = 0, subentities:size() - 1 do
						local subentity = subentities[val]
						
						local submeshname = ModelEdit.getSubMeshName(mesh, subentity:getSubEntityIndex())
 
						
						local modelcontentItem = {}
						modelcontentItem.type = "subentity"
						modelcontentItem.name = submeshname
						modelcontentItem.subentity = subentity
						ModelEdit.modelContentsItems[table.getn(ModelEdit.modelContentsItems) + 1] = modelcontentItem
						
						local treeItem3 = EmberOgre.Gui.ColouredTreeItem:new(submeshname, table.getn(ModelEdit.modelContentsItems))
						treeItem3:toggleIsOpen()
						treeItem3:setTooltipText("Submesh '" .. submeshname .. "'")
						treeItem2:addItem(treeItem3)
					end
				end
			end
		end		
	end	
end


function ModelEdit.getCurrentModelContentItem()
	local item = ModelEdit.modelcontentstree:getFirstSelectedItem()
	if item ~= nil then
		local itemId= item:getID()
		local contentItem = ModelEdit.modelContentsItems[itemId]
		return contentItem
	end
	return nil
end

function ModelEdit.showModelContent(listitem)
	if listitem ~= nil then
		local itemId= listitem:getID()
		local contentItem = ModelEdit.modelContentsItems[itemId]
		--inspectObject(contentItem.type)
		
		if contentItem.type == "submodel" then
			ModelEdit.showSubModel(contentItem)
		elseif contentItem.type == "part" then
			ModelEdit.showPart(contentItem)
		elseif contentItem.type == "subentity" then
			ModelEdit.showSubEntity(contentItem)
		else
			ModelEdit.showModel()
		end
	else
		ModelEdit.showModel()
	end
end

function ModelEdit.hideAllContentParts()
	ModelEdit.contentparts.partInfo:setVisible(false)
	ModelEdit.contentparts.modelInfo:setVisible(false)
	ModelEdit.contentparts.submodelInfo:setVisible(false)
	ModelEdit.contentparts.submeshInfo:setVisible(false)
end

function ModelEdit.showSubModel(contentItem)
	ModelEdit.hideAllContentParts()
	ModelEdit.contentparts.submodelInfo:setVisible(true)
	local submodelDef = ModelEdit.getSelectedSubModel()
	
	ModelEdit.widget:getWindow("SubModelName"):setText(submodelDef:getMeshName())
	
	local sizeWidget = ModelEdit.widget:getWindow("SubModelSize")
	
	local model = ModelEdit.renderer:getModel()
	if model ~= nil then
		local submodel = model:getSubModel(0)
		if submodel ~= nil then
			local bbox = submodel:getEntity():getMesh():get():getBounds()
			local minPos = bbox:getMinimum()
			local maxPos = bbox:getMaximum()
			
			local totalX = maxPos.x - minPos.x
			local totalY = maxPos.y - minPos.y
			local totalZ =  maxPos.z - minPos.z
			
			local theLargest = 0
			if totalX > totalY and totalX > totalZ then
				theLargest = totalX
			elseif totalY > totalZ and totalY > totalZ then
				theLargest = totalY
			else
				theLargest = totalZ
			end
			
			local sizeString = string.format("min: %.3f   %.3f   %.3f\nmax: %.3f   %.3f   %.3f\ntotal: %.3f   %.3f   %.3f\nprop.: %.3f   %.3f   %.3f ", minPos.x, minPos.y, minPos.z, maxPos.x, maxPos.y, maxPos.z, totalX, totalY, totalZ, totalX / theLargest, totalY / theLargest, totalZ / theLargest)
			sizeWidget:setText(sizeString)
		end
	end
end

function ModelEdit.showPart(contentItem)
	ModelEdit.hideAllContentParts()
	ModelEdit.contentparts.partInfo:setVisible(true)
	local part = contentItem.part
	ModelEdit.widget:getWindow("PartName"):setText(part:getName())
	ModelEdit.partShown:setSelected(part:getShow())
	
	ModelEdit.fillSubMeshList(part)
end


function ModelEdit.showSubEntity(contentItem)
	ModelEdit.hideAllContentParts()
	ModelEdit.contentparts.submeshInfo:setVisible(true)
	ModelEdit.selectMaterial(contentItem.subentity)
end

function ModelEdit.showModel()
	ModelEdit.hideAllContentParts()
	ModelEdit.contentparts.modelInfo:setVisible(true)
end

function ModelEdit.buildWidget()
	ModelEdit.widget = guiManager:createWidget()

	ModelEdit.widget:loadMainSheet("ModelEdit.layout", "ModelEdit/")
	
	ModelEdit.renderImage = ModelEdit.widget:getWindow("Image")
	--ModelEdit.renderImage = CEGUI.toStaticImage(ModelEdit.renderImage)

	ModelEdit.models = ModelEdit.widget:getWindow("Models")
	ModelEdit.models = CEGUI.toListbox(ModelEdit.models)
	ModelEdit.models:subscribeEvent("ItemSelectionChanged", "ModelEdit.models_SelectionChanged")
	ModelEdit.modelsfilter = ModelEdit.widget:getWindow("FilterModels")
	ModelEdit.modelsfilter = CEGUI.toEditbox(ModelEdit.modelsfilter)
	ModelEdit.modelslistholder = EmberOgre.Gui.ListHolder:new_local(ModelEdit.models, ModelEdit.modelsfilter)
	
	local xW = ModelEdit.widget:getWindow("ModelTranslate_x")
	local yW = ModelEdit.widget:getWindow("ModelTranslate_y")
	local zW = ModelEdit.widget:getWindow("ModelTranslate_z")
	ModelEdit.translateAdapter = EmberOgre.Gui.Vector3Adapter:new_local(xW,yW ,zW)
	connect(ModelEdit.connectors, ModelEdit.translateAdapter.EventValueChanged, "ModelEdit.translateAdapter_update")
	
	local xW = ModelEdit.widget:getWindow("ModelContainedOffset_x")
	local yW = ModelEdit.widget:getWindow("ModelContainedOffset_y")
	local zW = ModelEdit.widget:getWindow("ModelContainedOffset_z")
	ModelEdit.containedOffsetAdapter = EmberOgre.Gui.Vector3Adapter:new_local(xW,yW ,zW)
	connect(ModelEdit.connectors, ModelEdit.containedOffsetAdapter.EventValueChanged, "ModelEdit.containedOffsetAdapter_update")

	local xW = ModelEdit.widget:getWindow("ModelRotation_x")
	local yW = ModelEdit.widget:getWindow("ModelRotation_y")
	local zW = ModelEdit.widget:getWindow("ModelRotation_z")
	local degreeW = ModelEdit.widget:getWindow("ModelRotation_degrees")
	ModelEdit.rotationAdapter = EmberOgre.Gui.QuaternionAdapter:new_local(degreeW, xW,yW ,zW)
	connect(ModelEdit.connectors, ModelEdit.rotationAdapter.EventValueChanged, "ModelEdit.rotationAdapter_update")





	ModelEdit.contentparts = {}
	ModelEdit.contentparts.partInfo = ModelEdit.widget:getWindow("PartInfo")
	ModelEdit.contentparts.modelInfo = ModelEdit.widget:getWindow("ModelInfo")
	ModelEdit.contentparts.submodelInfo = ModelEdit.widget:getWindow("SubModelInfo")
	ModelEdit.contentparts.submeshInfo = ModelEdit.widget:getWindow("SubMeshInfo")
	
	--hide all parts initially
	ModelEdit.hideAllContentParts()
	
	
	ModelEdit.contentparts.modelInfo.meshlist = ModelEdit.widget:getWindow("MeshList")
	ModelEdit.contentparts.modelInfo.meshlist = CEGUI.toListbox(ModelEdit.contentparts.modelInfo.meshlist)
	ModelEdit.contentparts.modelInfo.meshlist:subscribeEvent("ItemSelectionChanged", "ModelEdit.modelinfoMeshlist_SelectionChanged")
	
	ModelEdit.contentparts.modelInfo.meshlistfilter =  CEGUI.toEditbox(ModelEdit.widget:getWindow("MeshListFilter"))
	ModelEdit.contentparts.modelInfo.meshlistlistholder = EmberOgre.Gui.ListHolder:new_local(ModelEdit.contentparts.modelInfo.meshlist, ModelEdit.contentparts.modelInfo.meshlistfilter)
	
	
	ModelEdit.contentparts.submeshInfo.materiallist = ModelEdit.widget:getWindow("Materials")
	ModelEdit.contentparts.submeshInfo.materiallist = CEGUI.toListbox(ModelEdit.contentparts.submeshInfo.materiallist)
	ModelEdit.contentparts.submeshInfo.materiallist:subscribeEvent("ItemSelectionChanged", "ModelEdit.submeshinfomaterials_SelectionChanged")
	ModelEdit.contentparts.submeshInfo.filter = ModelEdit.widget:getWindow("FilterMaterials")
	ModelEdit.contentparts.submeshInfo.filter = CEGUI.toEditbox(ModelEdit.contentparts.submeshInfo.filter)
	ModelEdit.contentparts.submeshInfo.listholder = EmberOgre.Gui.ListHolder:new_local(ModelEdit.contentparts.submeshInfo.materiallist, ModelEdit.contentparts.submeshInfo.filter)
	
	ModelEdit.contentparts.submeshInfo.removeSubMeshButton = ModelEdit.widget:getWindow("RemoveSubMeshButton")
	ModelEdit.contentparts.submeshInfo.removeSubMeshButton:subscribeEvent("MouseClick", "ModelEdit.submeshinforemovesubmesh_MouseClick")
	
	ModelEdit.widget:getWindow("PartRemoveButton"):subscribeEvent("MouseClick", "ModelEdit.removePart_MouseClick")
	ModelEdit.widget:getWindow("AddSubmeshButton"):subscribeEvent("MouseClick", "ModelEdit.addSubmesh_MouseClick")
	ModelEdit.widget:getWindow("AddPartButton"):subscribeEvent("MouseClick", "ModelEdit.addPart_MouseClick")
	ModelEdit.widget:getWindow("RenamePartButton"):subscribeEvent("MouseClick", "ModelEdit.renamePart_MouseClick")
	ModelEdit.widget:getWindow("AddModelButton"):subscribeEvent("MouseClick", "ModelEdit.AddModelButton_MouseClick")
	ModelEdit.widget:getWindow("ReloadModelListButton"):subscribeEvent("MouseClick", "ModelEdit.ReloadModelListButton_MouseClick")
	
	ModelEdit.widget:getWindow("AddSubmodelButton"):subscribeEvent("MouseClick", "ModelEdit.AddSubmodelButton_MouseClick")
	ModelEdit.widget:getWindow("SaveModelButton"):subscribeEvent("MouseClick", "ModelEdit.SaveModelButton_MouseClick")
	ModelEdit.widget:getWindow("NewModelOk"):subscribeEvent("MouseClick", "ModelEdit.NewModelOk_MouseClick")
	ModelEdit.widget:getWindow("NewModelCancel"):subscribeEvent("MouseClick", "ModelEdit.NewModelCancel_MouseClick")
	ModelEdit.widget:getWindow("RemoveSubmodelButton"):subscribeEvent("MouseClick", "ModelEdit.RemoveSubmodelButton_MouseClick")
	ModelEdit.widget:getWindow("ReloadInstancesButton"):subscribeEvent("MouseClick", "ModelEdit.ReloadInstancesButton_MouseClick")
	ModelEdit.widget:getWindow("GetRotationFromPreviewButton"):subscribeEvent("MouseClick", "ModelEdit.GetRotationFromPreviewButton_MouseClick")
	
	ModelEdit.widget:getWindow("YawLeft"):subscribeEvent("MouseClick", "ModelEdit.YawLeft_MouseClick")
	ModelEdit.widget:getWindow("YawRight"):subscribeEvent("MouseClick", "ModelEdit.YawRight_MouseClick")
	ModelEdit.widget:getWindow("RollLeft"):subscribeEvent("MouseClick", "ModelEdit.RollLeft_MouseClick")
	ModelEdit.widget:getWindow("RollRight"):subscribeEvent("MouseClick", "ModelEdit.RollRight_MouseClick")
	ModelEdit.widget:getWindow("PitchUp"):subscribeEvent("MouseClick", "ModelEdit.PitchUp_MouseClick")
	ModelEdit.widget:getWindow("PitchDown"):subscribeEvent("MouseClick", "ModelEdit.PitchDown_MouseClick")
	ModelEdit.widget:getWindow("ResetOrientation"):subscribeEvent("MouseClick", "ModelEdit.ResetOrientation_MouseClick")



	ModelEdit.contentparts.modelInfo.renderImage =  ModelEdit.widget:getWindow("MeshPreviewImage")
	--ModelEdit.contentparts.modelInfo.renderImage = CEGUI.toStaticImage(ModelEdit.contentparts.modelInfo.renderImage)
	
--	ModelEdit.materials = ModelEdit.widget:getWindow("Materials")
--	ModelEdit.materials = CEGUI.toListbox(ModelEdit.materials)
--	ModelEdit.materials:subscribeEvent("ItemSelectionChanged", "ModelEdit.materials_SelectionChanged")
	
	

	ModelEdit.modelcontentstree = ModelEdit.widget:getWindow("ModelContentsTree")
	ModelEdit.modelcontentstree = tolua.cast(ModelEdit.modelcontentstree,"CEGUI::Tree")
	ModelEdit.modelcontentstree:setMultiselectEnabled(false)
	ModelEdit.modelcontentstree:subscribeEvent("ItemSelectionChanged", "ModelEdit.modelcontents_SelectionChanged")
	ModelEdit.modelcontentstree:subscribeEvent("ListItemsChanged", "ModelEdit.modelcontents_SelectionChanged")
	--ModelEdit.modelcontents:setMutedState(true)

	
	ModelEdit.partShown = ModelEdit.widget:getWindow("Shown")
	ModelEdit.partShown = CEGUI.toCheckbox(ModelEdit.partShown)
	ModelEdit.partShown:subscribeEvent("CheckStateChanged", "ModelEdit.partShown_StateChanged")
		
	
		
	ModelEdit.renderer = EmberOgre.Gui.ModelRenderer:new_local(ModelEdit.renderImage)
	ModelEdit.renderer:showAxis();
	ModelEdit.renderer:setCameraPositionMode(EmberOgre.SimpleRenderContext.CPM_WORLDCENTER)
	
	local subMeshPreviewImage = ModelEdit.widget:getWindow("SubMeshPreviewImage")
	--subMeshPreviewImage = CEGUI.toStaticImage(subMeshPreviewImage)
	ModelEdit.subMeshPartRenderer = EmberOgre.Gui.OgreEntityRenderer:new_local(subMeshPreviewImage)
	
	local meshPreviewImage = ModelEdit.widget:getWindow("MeshPreviewImage")
	--meshPreviewImage = CEGUI.toStaticImage(meshPreviewImage)
	ModelEdit.submeshRenderer = EmberOgre.Gui.OgreEntityRenderer:new_local(meshPreviewImage)
	
	--ModelEdit.contentparts.modelInfo.renderer = EmberOgre.Gui.ModelRenderer:new_local(ModelEdit.contentparts.modelInfo.renderImage)
	
	ModelEdit.zoomSlider  = ModelEdit.widget:getWindow("Zoom")
	ModelEdit.zoomSlider = CEGUI.toSlider(ModelEdit.zoomSlider)
	ModelEdit.zoomSlider:subscribeEvent("ValueChanged", "ModelEdit.zoom_ValueChanged")
	
	ModelEdit.scaleTextbox = ModelEdit.widget:getWindow("ModelScale")
	ModelEdit.scaleTextbox:subscribeEvent("TextChanged", "ModelEdit.ModelScale_TextChanged")
	
	ModelEdit.fillMaterialList()
	ModelEdit.fillModellist()
	ModelEdit.fillMeshList()
	ModelEdit.fillScaleTypesList()
	--def:setValid(true)	
	--model = ModelEdit.renderer:getModel()
	--def = model:getDefinition():get()
	
	ModelEdit.widget:registerConsoleVisibilityToggleCommand("modelEdit")
	ModelEdit.widget:enableCloseButton()
	ModelEdit.widget:hide()

end

function ModelEdit.fillScaleTypesList()
	ModelEdit.scaleTypes  = ModelEdit.widget:getWindow("ModelUseScaleOf")
	ModelEdit.scaleTypes = CEGUI.toCombobox( ModelEdit.scaleTypes)
	
	local item = EmberOgre.Gui.ColouredListItem:new("all", 0)
	ModelEdit.scaleTypes:addItem(item)
	local item = EmberOgre.Gui.ColouredListItem:new("none", 1)
	ModelEdit.scaleTypes:addItem(item)
	local item = EmberOgre.Gui.ColouredListItem:new("width", 2)
	ModelEdit.scaleTypes:addItem(item)
	local item = EmberOgre.Gui.ColouredListItem:new("depth", 3)
	ModelEdit.scaleTypes:addItem(item)
	local item = EmberOgre.Gui.ColouredListItem:new("height", 4)
	ModelEdit.scaleTypes:addItem(item)
	
	ModelEdit.scaleTypes:subscribeEvent("ListSelectionChanged", "ModelEdit.ModelUseScaleOf_SelectionChanged")

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

ModelEdit.buildWidget()

--model:reload()

--submodel2 = def:createSubModelDefinition("3d_objects/items/building_primitives/models/2_room_house/2_room_house.mesh")
--part2 = submodel:createPartDefinition("main")
--part2:setShow(true)









