ModelEdit = {}

function ModelEdit:selectMaterial(subentity)
	if subentity == nil then
		--there is no subentity (for example if the part doesn't have any defined), just clear the selection
		self.materials:clearAllSelections()
	else
		local materialName = subentity:getMaterialName()
		local item
--		if materialName == "" then
			--no material specified, get the default for this subentity
--			self.model
--		else 
		if materialName ~= "" then
			item = self.contentparts.submeshInfo.materiallist:findItemWithText(materialName, self.contentparts.submeshInfo.materiallist:getListboxItemFromIndex(0))
		end
		
		if item ~= nil then
			self.contentparts.submeshInfo.materiallist:setItemSelectState(item, true)
			self.contentparts.submeshInfo.materiallist:ensureItemIsVisible(item)
		else
			self.contentparts.submeshInfo.materiallist:clearAllSelections()
		end
	end
end

function ModelEdit:updateMaterial(subentity, material)
	if subentity ~= nil then
		subentity:setMaterialName(material)
		self:reloadModel()
	end
end

function ModelEdit:updatePartShown(part, shown)
	if part ~= nil then
		part:setShow(shown)
		self:reloadModel()
--		self:updateModelContentList()
	end
end

function ModelEdit:reloadModel()
	local model = self.renderer:getModel()
	if model ~= nil then
		model:reload()
	end
end

function ModelEdit:fillMaterialList()
	self.contentparts.submeshInfo.listadapter:update()
end

function ModelEdit:fillMeshList()
	local manager = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	local meshes = manager:getAllMeshes()
	
	for i = 0, meshes:size() - 1 do
		local name = meshes[i]
		local item = EmberOgre.Gui.ColouredListItem:new(name, i)
		self.contentparts.modelInfo.meshlistlistholder:addItem(item)
--		self.contentparts.modelInfo.meshlist:addItem(item)
		
	end	
end

function ModelEdit:fillSubMeshList(part)
	local list = self.widget:getWindow("SubmeshList")
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
		local submeshname = self:getSubMeshName(mesh, i)
		local item = EmberOgre.Gui.ColouredListItem:new(submeshname, i)
		list:addItem(item)
		i = i + 1
	end	
end

function ModelEdit:getSubMeshName(mesh, index)
	local submeshname = EmberOgre.OgreUtils:getSubMeshName(mesh, index)
	if submeshname == "" then
		submeshname = "(index: " .. index .. ")"
	end
	return submeshname
end

function ModelEdit:loadModelDefinition(definitionName)
	modelDefMgr = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	self.definitionPtr = modelDefMgr:getByName(definitionName)
	self.definition = self.definitionPtr:get()
	self:showPreview(definitionName)
	
	--self:updateSubmodelsList()
	self:updateModelContentList()
	self:updateModelInfo()
	self:showModel()

end

function ModelEdit:updateSubmodelsList()
	self.submodels:resetList()
	self.submodels:clearAllSelections()
	local submodels = self.definition:getSubModelDefinitions()
	for val = 0, submodels:size() - 1 do
		local name = submodels[val]:getMeshName()
		local item = EmberOgre.Gui.ColouredListItem:new(name, val)
		self.submodels:addItem(item)
	end	
end

function ModelEdit:updatePartsList(submodel)
	self.parts:resetList()
	self.parts:clearAllSelections()
	if submodel ~= nil then
		local parts = submodel:getPartDefinitions()
		for val = 0, parts:size() - 1 do
			local name = parts[val]:getName()
			local item = EmberOgre.Gui.ColouredListItem:new(name, val)
			self.parts:addItem(item)
		end
	end
end



function ModelEdit:updateSubentitiesList(part)
	self.subentities:resetList()
	self.subentities:clearAllSelections()
	if part ~= nil then
		local subentities = part:getSubEntityDefinitions()
		for val = 0, subentities:size() - 1 do
			local name = subentities[val]:getSubEntityName()
			if name == "" then
				name = subentities[val]:getSubEntityIndex()
			end
			local item = EmberOgre.Gui.ColouredListItem:new(name, val)
			self.subentities:addItem(item)
		end
	end
end

function ModelEdit:showPreview(definitionName)
	self.renderer:showModel(definitionName)
	--self.renderer:showFullModel()

end

function ModelEdit:fillModellist()
	self.modelsAdapter:update()
end

function ModelEdit:updateModelInfo()
	self.widget:getWindow("ModelName_Text"):setText("Name: " .. self.definition:getName())
	self.scaleTextbox:setText(self.definition:getScale())
	--self.widget:getWindow("ModelRotation"):setText(self.definition:getRotation())
	self.rotationAdapter:updateGui(self.definition:getRotation());
	self.translateAdapter:updateGui(self.definition:getTranslate());
	self.containedOffsetAdapter:updateGui(self.definition:getContentOffset());
--	self:fillWindowsFromVector("ModelContainedOffset", self.definition:getContentOffset())
--	self:fillWindowsFromVector("ModelTranslate", self.definition:getTranslate())
	
	local showContent = self.widget:getWindow("ModelShowContent")
	
	showContent = CEGUI.toCheckbox(showContent)
	showContent:setSelected(self.definition:getShowContained())
	
	self.scaleTypes:clearAllSelections()
	self.scaleTypes:setItemSelectState(self.definition:getUseScaleOf(), true)
	
	self.zoomSlider:setCurrentValue(self.renderer:getCameraDistance())

end

function ModelEdit:translateAdapter_update()
	self.definition:setTranslate(self.translateAdapter:getValue())
	self.renderer:repositionSceneNode()
end

function ModelEdit:containedOffsetAdapter_update()
	self.definition:setContentOffset(self.containedOffsetAdapter:getValue())
	self.renderer:repositionSceneNode()
end

function ModelEdit:rotationAdapter_update()
	self.definition:setRotation(self.rotationAdapter:getValue())
	self.renderer:repositionSceneNode()
end



function ModelEdit:fillWindowsFromVector(windowNamePrefix, vector)
		local xWindow = self.widget:getWindow(windowNamePrefix .. "_x");
		local yWindow = self.widget:getWindow(windowNamePrefix .. "_y");
		local zWindow = self.widget:getWindow(windowNamePrefix .. "_z");
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

function ModelEdit:fillVectorFromWindows(windowNamePrefix, vector)
	local xWindow = self.widget:getWindow(windowNamePrefix .. "_x");
	local yWindow = self.widget:getWindow(windowNamePrefix .. "_y");
	local zWindow = self.widget:getWindow(windowNamePrefix .. "_z");
	
	vector.x = tonumber(xWindow:getText())
	vector.y = tonumber(xWindow:getText())
	vector.z = tonumber(xWindow:getText())
	
end


function ModelEdit:models_SelectionChanged(args)
	local item = self.models:getFirstSelectedItem()
	self:loadModelDefinition(item:getText())
end

function ModelEdit:parts_SelectionChanged(args)
	local part = self:getSelectedPart()
	self:updateSubentitiesList(part)
	self:updateShownCheckbox(part)
end

function ModelEdit:submodels_SelectionChanged(args)
	local submodel = self:getSelectedSubModel()
	--inspectObject(submodel)
	self:updatePartsList(submodel)
end

function ModelEdit:subentities_SelectionChanged(args)
	local subentity = self:getSelectedSubEntity()
	self:selectMaterial(subentity)
end

function ModelEdit:submeshinfomaterials_SelectionChanged(args)
	local item = self.contentparts.submeshInfo.materiallist:getFirstSelectedItem()
	if item ~= nil then 
		local material = item:getText()
		local subentity = self:getSelectedSubEntity()
		--inspectObject(subentity)
		self:updateMaterial(subentity, material)
	end
end

function ModelEdit:ModelUseScaleOf_SelectionChanged(args)
	local model = self.definition
	local item = self.scaleTypes:getSelectedItem()
	if item ~= nil then
		model:setUseScaleOf(item:getID())
	end
end

function ModelEdit:ModelScale_TextChanged(args)
	local model = self.definition
	model:setScale(tonumber(self.scaleTextbox:getText()));
end

function ModelEdit:YawLeft_Clicked(args)
	self.renderer:yaw(Ogre.Degree:new_local(-45))
end

function ModelEdit:YawRight_Clicked(args)
	self.renderer:yaw(Ogre.Degree:new_local(45))
end

function ModelEdit:PitchUp_Clicked(args)
	self.renderer:pitch(Ogre.Degree:new_local(-45))
end

function ModelEdit:PitchDown_Clicked(args)
	self.renderer:pitch(Ogre.Degree:new_local(45))
end

function ModelEdit:RollLeft_Clicked(args)
	self.renderer:roll(Ogre.Degree:new_local(-45))
end

function ModelEdit:RollRight_Clicked(args)
	self.renderer:roll(Ogre.Degree:new_local(45))
end

function ModelEdit:ResetOrientation_Clicked(args)
	self.renderer:resetCameraOrientation()
end


function ModelEdit:submeshinforemovesubmesh_Clicked(args)
	--just remove the subentity definition from the part
	local subentity = self:getSelectedSubEntity()
	local part = subentity:getPartDefinition()
	part:removeSubEntityDefinition(subentity)
	self:reloadModel()
	self:updateModelContentList()
end

function ModelEdit:removePart_Clicked(args)
	--just remove the part definition from the submodel
	local part = self:getSelectedPart()
	local submodel = part:getSubModelDefinition()
	submodel:removePartDefinition(part)
	self:reloadModel()
	self:updateModelContentList()
end

function ModelEdit:addSubmesh_Clicked(args)
--get the selected submesh and add it to the part
	local list = self.widget:getWindow("SubmeshList")
	list = CEGUI.toListbox(list)
	local item = list:getFirstSelectedItem()
	--an item must be selected
	if item ~= nil then
		local part = self:getSelectedPart()
		part:createSubEntityDefinition(item:getID())
		self:reloadModel()
		self:updateModelContentList()
	end

end

function ModelEdit:ReloadModelListButton_Clicked(args)
	self:fillModellist()
end

function ModelEdit:SaveModelButton_Clicked(args)
	local modelDefMgr = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	modelDefMgr:exportScript(self.definitionPtr)	
	inspectObject(self.definition:getName())

end

function ModelEdit:ExportAsAtlasTypeButton_Clicked(args)
	local model = self.renderer:getModel()
	if model ~= nil then
		local composer = EmberOgre.Model.ModelDefinitionAtlasComposer:new_local()
		composer:composeToFile(model, self.definition:getName(), "thing", self.definition:getScale())
	end	
end

function ModelEdit:AddSubmodelButton_Clicked(args)
	
	local item = self.contentparts.modelInfo.meshlist:getFirstSelectedItem()
	--an item must be selected
	if item ~= nil then
		local submodel = self.definition:createSubModelDefinition(item:getText())
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
		
		
		self:reloadModel()
		--update the renderer so that the camera is repositioned and the complete model is shown
		self.renderer:showModel(self.definition:getName())
		self:updateModelContentList()
	end

end

function ModelEdit:ReloadInstancesButton_Clicked(args)
	--reload all model instances
	self.definition:reloadAllInstances()
end


function ModelEdit:GetRotationFromPreviewButton_Clicked(args)
	--Get the rotation from the preview window

	self.rotationAdapter:setValue(self.renderer:getEntityRotation())
end

function ModelEdit:GetIconFromPreviewButton_Clicked(args)
	if self.definition ~= nil then
		local definition = self.definition:createViewDefinition("icon")
		definition.Rotation = self.renderer:getEntityRotation():Inverse()
		--only set the camera distance if it has been moved from the optimal position
		if self.renderer:getCameraDistance() ~= 1 then
			definition.Distance = self.renderer:getAbsoluteCameraDistance()
		end
	end
end

function ModelEdit:RemoveSubmodelButton_Clicked(args)

	local submodel = self:getSelectedSubModel()
	self.definition:removeSubModelDefinition(submodel)
		
	self:reloadModel()
	self:updateModelContentList()
	
end


function ModelEdit:addPart_Clicked(args)
	local editbox = self.widget:getWindow("NewPartName")
	local name = editbox:getText()
	if name ~= "" then
		local submodel = self:getSelectedSubModel()
		if submodel ~= nil then
			submodel:createPartDefinition(name);
			self:reloadModel()
			self:updateModelContentList()
		end
	end
end

function ModelEdit:renamePart_Clicked(args)
	local editbox = self.widget:getWindow("PartName")
	local name = editbox:getText()
	if name ~= "" then
		local part = self:getSelectedPart()
		if part ~= nil then
			part:setName(name)
			self:reloadModel()
			self:updateModelContentList()
		end
	end	
end


function ModelEdit:AddModelButton_Clicked(args)
	self.widget:getWindow("NewModelWindow"):setVisible(true)
	self.widget:getWindow("NewModelWindow"):moveToFront()
end

function ModelEdit:NewModelCancel_Clicked(args)
	self.widget:getWindow("NewModelWindow"):setVisible(false)
end

function ModelEdit:NewModelOk_Clicked(args)
	local modelDefMgr = EmberOgre.Model.ModelDefinitionManager:getSingleton()
	local name = self.widget:getWindow("NewModelName"):getText()
	def = modelDefMgr:create(name, "ModelDefinitions"):get()
	def:setValid(true)
	
	--after adding the definition, update the model list
	self:fillModellist()
	local item = self.models:findItemWithText(name, self.models:getListboxItemFromIndex(0))
	if item ~= nil then
		item:setSelected(true)
		self.models:ensureItemIsVisible(item)	
	end
	self.widget:getWindow("NewModelWindow"):setVisible(false)
	self:loadModelDefinition(item:getText())
	self:reloadModel()
	self:updateModelContentList()
end


function ModelEdit:previewMesh(meshname)
	self.submeshRenderer:showEntity(meshname)
end

function ModelEdit:modelinfoMeshlist_SelectionChanged()
	local item = self.contentparts.modelInfo.meshlist:getFirstSelectedItem()
	self:previewMesh(item:getText())

end


function ModelEdit:partShown_StateChanged(args)
	local shown = self.partShown:isSelected()
	local part = self:getSelectedPart()
	--inspectObject(part)
	self:updatePartShown(part, shown)	
end

function ModelEdit:modelcontents_SelectionChanged(args)
	local item = self.modelcontentstree:getFirstSelectedItem()
	self:showModelContent(item)
end


function ModelEdit:zoom_ValueChanged(args)
	local zoomValue = self.zoomSlider:getCurrentValue()
	self.renderer:setCameraDistance(zoomValue)
end

function ModelEdit:getSelectedSubModel()
	local item = self:getCurrentModelContentItem()
	
	return item.submodel
end

function ModelEdit:getSelectedPart()
	local item = self:getCurrentModelContentItem()
	
	return item.part
	
end

function ModelEdit:getSelectedSubEntity()
	local item = self:getCurrentModelContentItem()
	
	return item.subentity
end


function ModelEdit:updateModelContentList()
	self.modelcontentstree:resetList()
	--self.modelcontents:clearAllSelections()
	
	self.modelContentsItems = {}
	

	
	local submodels = self.definition:getSubModelDefinitions()
	
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
		self.modelContentsItems[table.getn(self.modelContentsItems) + 1] = modelcontentItem
		
		local treeItem = EmberOgre.Gui.ColouredTreeItem:new(name, table.getn(self.modelContentsItems)) 
		treeItem:toggleIsOpen()
		treeItem:setTooltipText("Mesh '" .. name .. "'")
		self.modelcontentstree:addItem(treeItem)
		
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
				
				self.modelContentsItems[table.getn(self.modelContentsItems) + 1] = modelcontentItem
				
				local treeItem2 = EmberOgre.Gui.ColouredTreeItem:new(name .. partVisible, table.getn(self.modelContentsItems))
				treeItem2:toggleIsOpen()
				treeItem2:setTooltipText("Part '" .. name .. "'")
				treeItem:addItem(treeItem2)
			
				if part ~= nil then
					local subentities = part:getSubEntityDefinitions()
					for val = 0, subentities:size() - 1 do
						local subentity = subentities[val]
						
						local submeshname = self:getSubMeshName(mesh, subentity:getSubEntityIndex())
 
						
						local modelcontentItem = {}
						modelcontentItem.type = "subentity"
						modelcontentItem.name = submeshname
						modelcontentItem.subentity = subentity
						self.modelContentsItems[table.getn(self.modelContentsItems) + 1] = modelcontentItem
						
						local treeItem3 = EmberOgre.Gui.ColouredTreeItem:new(submeshname, table.getn(self.modelContentsItems))
						treeItem3:toggleIsOpen()
						treeItem3:setTooltipText("Submesh '" .. submeshname .. "'")
						treeItem2:addItem(treeItem3)
					end
				end
			end
		end		
	end	
end


function ModelEdit:Image_ShowAxes_CheckStateChanged(args)

	local checkbox = self.widget:getWindow("Image_ShowAxes")
	checkbox = CEGUI.toCheckbox(checkbox)
	
	if checkbox:isSelected() then
		self.renderer:showAxis()
	else
		self.renderer:hideAxis()
	end

end

function ModelEdit:getCurrentModelContentItem()
	local item = self.modelcontentstree:getFirstSelectedItem()
	if item ~= nil then
		local itemId= item:getID()
		local contentItem = self.modelContentsItems[itemId]
		return contentItem
	end
	return nil
end

function ModelEdit:showModelContent(listitem)
	if listitem ~= nil then
		local itemId= listitem:getID()
		local contentItem = self.modelContentsItems[itemId]
		--inspectObject(contentItem.type)
		
		if contentItem.type == "submodel" then
			self:showSubModel(contentItem)
		elseif contentItem.type == "part" then
			self:showPart(contentItem)
		elseif contentItem.type == "subentity" then
			self:showSubEntity(contentItem)
		else
			self:showModel()
		end
	else
		self:showModel()
	end
end

function ModelEdit:hideAllContentParts()
	self.contentparts.partInfo:setVisible(false)
	self.contentparts.modelInfo:setVisible(false)
	self.contentparts.submodelInfo:setVisible(false)
	self.contentparts.submeshInfo:setVisible(false)
end

function ModelEdit:showSubModel(contentItem)
	self:hideAllContentParts()
	self.contentparts.submodelInfo:setVisible(true)
	local submodelDef = self:getSelectedSubModel()
	
	self.widget:getWindow("SubModelName"):setText(submodelDef:getMeshName())
	
	local sizeWidget = self.widget:getWindow("SubModelSize")
	
	local model = self.renderer:getModel()
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

function ModelEdit:showPart(contentItem)
	self:hideAllContentParts()
	self.contentparts.partInfo:setVisible(true)
	local part = contentItem.part
	self.widget:getWindow("PartName"):setText(part:getName())
	self.partShown:setSelected(part:getShow())
	
	self:fillSubMeshList(part)
end


function ModelEdit:showSubEntity(contentItem)
	self:hideAllContentParts()
	self.contentparts.submeshInfo:setVisible(true)
	self:selectMaterial(contentItem.subentity)
end

function ModelEdit:showModel()
	self:hideAllContentParts()
	self.contentparts.modelInfo:setVisible(true)
end

function ModelEdit:buildWidget()
	self.widget = guiManager:createWidget()
	
	--delay setup of the widget until it's shown for the first time
	local setup = function()
		self.renderImage = self.widget:getWindow("Image")
		--self.renderImage = CEGUI.toStaticImage(self.renderImage)
	
		self.models = self.widget:getWindow("Models")
		self.models = CEGUI.toListbox(self.models)
		self.models:subscribeEvent("ItemSelectionChanged", self.models_SelectionChanged, self)
		self.modelsfilter = self.widget:getWindow("FilterModels")
		self.modelsfilter = CEGUI.toEditbox(self.modelsfilter)
		self.modelslistholder = EmberOgre.Gui.ListHolder:new_local(self.models, self.modelsfilter)
		self.modelsAdapter = EmberOgre.Gui.Adapters.Ogre.ResourceListAdapter:new_local(self.modelslistholder, EmberOgre.Model.ModelDefinitionManager:getSingleton())
		
		local xW = self.widget:getWindow("ModelTranslate_x")
		local yW = self.widget:getWindow("ModelTranslate_y")
		local zW = self.widget:getWindow("ModelTranslate_z")
		self.translateAdapter = EmberOgre.Gui.Vector3Adapter:new_local(xW,yW ,zW)
		connect(self.connectors, self.translateAdapter.EventValueChanged, self.translateAdapter_update, self)
		
		local xW = self.widget:getWindow("ModelContainedOffset_x")
		local yW = self.widget:getWindow("ModelContainedOffset_y")
		local zW = self.widget:getWindow("ModelContainedOffset_z")
		self.containedOffsetAdapter = EmberOgre.Gui.Vector3Adapter:new_local(xW,yW ,zW)
		connect(self.connectors, self.containedOffsetAdapter.EventValueChanged, self.containedOffsetAdapter_update, self)
	
		local xW = self.widget:getWindow("ModelRotation_x")
		local yW = self.widget:getWindow("ModelRotation_y")
		local zW = self.widget:getWindow("ModelRotation_z")
		local degreeW = self.widget:getWindow("ModelRotation_degrees")
		self.rotationAdapter = EmberOgre.Gui.QuaternionAdapter:new_local(degreeW, xW,yW ,zW)
		connect(self.connectors, self.rotationAdapter.EventValueChanged, self.rotationAdapter_update, self)
	
	
	
	
	
		self.contentparts = {}
		self.contentparts.partInfo = self.widget:getWindow("PartInfo")
		self.contentparts.modelInfo = self.widget:getWindow("ModelInfo")
		self.contentparts.submodelInfo = self.widget:getWindow("SubModelInfo")
		self.contentparts.submeshInfo = self.widget:getWindow("SubMeshInfo")
		
		--hide all parts initially
		self:hideAllContentParts()
		
		
		self.contentparts.modelInfo.meshlist = self.widget:getWindow("MeshList")
		self.contentparts.modelInfo.meshlist = CEGUI.toListbox(self.contentparts.modelInfo.meshlist)
		self.contentparts.modelInfo.meshlist:subscribeEvent("ItemSelectionChanged", self.modelinfoMeshlist_SelectionChanged, self)
		
		self.contentparts.modelInfo.meshlistfilter =  CEGUI.toEditbox(self.widget:getWindow("MeshListFilter"))
		self.contentparts.modelInfo.meshlistlistholder = EmberOgre.Gui.ListHolder:new_local(self.contentparts.modelInfo.meshlist, self.contentparts.modelInfo.meshlistfilter)
		
		
		self.contentparts.submeshInfo.materiallist = self.widget:getWindow("Materials")
		self.contentparts.submeshInfo.materiallist = CEGUI.toListbox(self.contentparts.submeshInfo.materiallist)
		self.contentparts.submeshInfo.materiallist:subscribeEvent("ItemSelectionChanged", self.submeshinfomaterials_SelectionChanged, self)
		self.contentparts.submeshInfo.filter = self.widget:getWindow("FilterMaterials")
		self.contentparts.submeshInfo.filter = CEGUI.toEditbox(self.contentparts.submeshInfo.filter)
		self.contentparts.submeshInfo.listholder = EmberOgre.Gui.ListHolder:new_local(self.contentparts.submeshInfo.materiallist, self.contentparts.submeshInfo.filter)
		self.contentparts.submeshInfo.listadapter = EmberOgre.Gui.Adapters.Ogre.ResourceListAdapter:new_local(self.contentparts.submeshInfo.listholder, Ogre.MaterialManager:getSingleton())
		
		self.contentparts.submeshInfo.removeSubMeshButton = self.widget:getWindow("RemoveSubMeshButton")
		self.contentparts.submeshInfo.removeSubMeshButton:subscribeEvent("Clicked", self.submeshinforemovesubmesh_Clicked, self)
		
		self.widget:getWindow("PartRemoveButton"):subscribeEvent("Clicked", self.removePart_Clicked, self)
		self.widget:getWindow("AddSubmeshButton"):subscribeEvent("Clicked", self.addSubmesh_Clicked, self)
		self.widget:getWindow("AddPartButton"):subscribeEvent("Clicked", self.addPart_Clicked, self)
		self.widget:getWindow("RenamePartButton"):subscribeEvent("Clicked", self.renamePart_Clicked, self)
		self.widget:getWindow("AddModelButton"):subscribeEvent("Clicked", self.AddModelButton_Clicked, self)
		self.widget:getWindow("ReloadModelListButton"):subscribeEvent("Clicked", self.ReloadModelListButton_Clicked, self)
		
		self.widget:getWindow("AddSubmodelButton"):subscribeEvent("Clicked", self.AddSubmodelButton_Clicked, self)
		self.widget:getWindow("SaveModelButton"):subscribeEvent("Clicked", self.SaveModelButton_Clicked, self)
		self.widget:getWindow("NewModelOk"):subscribeEvent("Clicked", self.NewModelOk_Clicked, self)
		self.widget:getWindow("NewModelCancel"):subscribeEvent("Clicked", self.NewModelCancel_Clicked, self)
		self.widget:getWindow("RemoveSubmodelButton"):subscribeEvent("Clicked", self.RemoveSubmodelButton_Clicked, self)
		self.widget:getWindow("ReloadInstancesButton"):subscribeEvent("Clicked", self.ReloadInstancesButton_Clicked, self)
		self.widget:getWindow("GetRotationFromPreviewButton"):subscribeEvent("Clicked", self.GetRotationFromPreviewButton_Clicked, self)
		
		self.widget:getWindow("YawLeft"):subscribeEvent("Clicked", self.YawLeft_Clicked, self)
		self.widget:getWindow("YawRight"):subscribeEvent("Clicked", self.YawRight_Clicked, self)
		self.widget:getWindow("RollLeft"):subscribeEvent("Clicked", self.RollLeft_Clicked, self)
		self.widget:getWindow("RollRight"):subscribeEvent("Clicked", self.RollRight_Clicked, self)
		self.widget:getWindow("PitchUp"):subscribeEvent("Clicked", self.PitchUp_Clicked, self)
		self.widget:getWindow("PitchDown"):subscribeEvent("Clicked", self.PitchDown_Clicked, self)
		self.widget:getWindow("ResetOrientation"):subscribeEvent("Clicked", self.ResetOrientation_Clicked, self)
	
	
	
		self.contentparts.modelInfo.renderImage =  self.widget:getWindow("MeshPreviewImage")
		--self.contentparts.modelInfo.renderImage = CEGUI.toStaticImage(self.contentparts.modelInfo.renderImage)
		
	--	self.materials = self.widget:getWindow("Materials")
	--	self.materials = CEGUI.toListbox(self.materials)
	--	self.materials:subscribeEvent("ItemSelectionChanged", self.materials_SelectionChanged, self)
		
		
	
		self.modelcontentstree = self.widget:getWindow("ModelContentsTree")
		self.modelcontentstree = tolua.cast(self.modelcontentstree,"CEGUI::Tree")
		self.modelcontentstree:setMultiselectEnabled(false)
		self.modelcontentstree:subscribeEvent("ItemSelectionChanged", self.modelcontents_SelectionChanged, self)
		self.modelcontentstree:subscribeEvent("ListItemsChanged", self.modelcontents_SelectionChanged, self)
		--self.modelcontents:setMutedState(true)
	
		
		self.partShown = self.widget:getWindow("Shown")
		self.partShown = CEGUI.toCheckbox(self.partShown)
		self.partShown:subscribeEvent("CheckStateChanged", self.partShown_StateChanged, self)
			
		
			
		self.renderer = EmberOgre.Gui.ModelRenderer:new_local(self.renderImage)
		self.renderer:showAxis();
		self.renderer:setCameraPositionMode(EmberOgre.SimpleRenderContext.CPM_WORLDCENTER)
		
		local subMeshPreviewImage = self.widget:getWindow("SubMeshPreviewImage")
		--subMeshPreviewImage = CEGUI.toStaticImage(subMeshPreviewImage)
		self.subMeshPartRenderer = EmberOgre.Gui.OgreEntityRenderer:new_local(subMeshPreviewImage)
		
		local meshPreviewImage = self.widget:getWindow("MeshPreviewImage")
		--meshPreviewImage = CEGUI.toStaticImage(meshPreviewImage)
		self.submeshRenderer = EmberOgre.Gui.OgreEntityRenderer:new_local(meshPreviewImage)
		
		--self.contentparts.modelInfo.renderer = EmberOgre.Gui.ModelRenderer:new_local(self.contentparts.modelInfo.renderImage)
		
		self.zoomSlider  = self.widget:getWindow("Zoom")
		self.zoomSlider = CEGUI.toSlider(self.zoomSlider)
		self.zoomSlider:subscribeEvent("ValueChanged", self.zoom_ValueChanged, self)
		
		self.scaleTextbox = self.widget:getWindow("ModelScale")
		self.scaleTextbox:subscribeEvent("TextChanged", self.ModelScale_TextChanged, self)
		
		self.widget:getWindow("Image_ShowAxes"):subscribeEvent("CheckStateChanged", self.Image_ShowAxes_CheckStateChanged, self)
		self.widget:getWindow("ExportAsAtlasTypeButton"):subscribeEvent("Clicked", self.ExportAsAtlasTypeButton_Clicked, self)
		self.widget:getWindow("GetIconFromPreviewButton"):subscribeEvent("Clicked", self.GetIconFromPreviewButton_Clicked, self)

		
		self:fillMaterialList()
		self:fillModellist()
		self:fillMeshList()
		self:fillScaleTypesList()
		--def:setValid(true)	
		--model = self.renderer:getModel()
		--def = model:getDefinition():get()
		
		self.widget:enableCloseButton()
	
	end

	connect(self.connectors, self.widget.EventFirstTimeShown, setup, self)
	self.widget:loadMainSheet("ModelEdit.layout", "ModelEdit/")
	self.widget:registerConsoleVisibilityToggleCommand("modelEdit")

end

function ModelEdit:fillScaleTypesList()
	self.scaleTypes  = self.widget:getWindow("ModelUseScaleOf")
	self.scaleTypes = CEGUI.toCombobox( self.scaleTypes)
	
	local item = EmberOgre.Gui.ColouredListItem:new("all", 0)
	self.scaleTypes:addItem(item)
	local item = EmberOgre.Gui.ColouredListItem:new("none", 1)
	self.scaleTypes:addItem(item)
	local item = EmberOgre.Gui.ColouredListItem:new("width", 2)
	self.scaleTypes:addItem(item)
	local item = EmberOgre.Gui.ColouredListItem:new("depth", 3)
	self.scaleTypes:addItem(item)
	local item = EmberOgre.Gui.ColouredListItem:new("height", 4)
	self.scaleTypes:addItem(item)
	
	self.scaleTypes:subscribeEvent("ListSelectionChanged", self.ModelUseScaleOf_SelectionChanged, self)

end

function ModelEdit:shutdown()
	
end

connect(connectors, emberOgre.EventGUIManagerInitialized, function(guiManager)
	local modelEdit = {connectors={}, 
		zoomRatio = 10, 
		modelContentsItems = {}
	}
	setmetatable(modelEdit, {__index = ModelEdit})
	
	modelEdit:buildWidget()

	connect(console.connectors, emberOgre.EventGUIManagerBeingDestroyed, function()
		modelEdit:shutdown()
		modelEdit = nil
	end)
end)
