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
		
		if item then
			self.contentparts.submeshInfo.materiallist:setItemSelectState(item, true)
			self.contentparts.submeshInfo.materiallist:ensureItemIsVisible(item)
		else
			self.contentparts.submeshInfo.materiallist:clearAllSelections()
		end
	end
end

function ModelEdit:updateMaterial(subentity, material)
	if subentity then
		subentity:setMaterialName(material)
		self:reloadModel()
	end
end

function ModelEdit:updatePartShown(part, shown)
	if part then
		part:setShow(shown)
		self:reloadModel()
--		self:updateModelContentList()
	end
end

function ModelEdit:reloadModel()
	local model = self.renderer:getModel()
	if model then
		model:reload()
	end
end

function ModelEdit:fillMaterialList()
	self.contentparts.submeshInfo.listadapter:update()
end

function ModelEdit:fillMeshList()
	local manager = Ember.OgreView.Model.ModelDefinitionManager:getSingleton()
	local meshes = manager:getAllMeshes()
	
	for i = 0, meshes:size() - 1 do
		local name = meshes[i]
		local item = Ember.OgreView.Gui.ColouredListItem:new(name, i)
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
		local item = Ember.OgreView.Gui.ColouredListItem:new(submeshname, i)
		list:addItem(item)
		i = i + 1
	end	
end

function ModelEdit:getSubMeshName(mesh, index)
	local submeshname = Ember.OgreView.OgreUtils:getSubMeshName(mesh, index)
	if submeshname == "" then
		submeshname = "(index: " .. index .. ")"
	end
	return submeshname
end

function ModelEdit:loadModelDefinition(definitionName)
	local modelDefMgr = Ember.OgreView.Model.ModelDefinitionManager:getSingleton()
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
		local item = Ember.OgreView.Gui.ColouredListItem:new(name, val)
		self.submodels:addItem(item)
	end	
end

function ModelEdit:updatePartsList(submodel)
	self.parts:resetList()
	self.parts:clearAllSelections()
	if submodel then
		local parts = submodel:getPartDefinitions()
		for val = 0, parts:size() - 1 do
			local name = parts[val]:getName()
			local item = Ember.OgreView.Gui.ColouredListItem:new(name, val)
			self.parts:addItem(item)
		end
	end
end



function ModelEdit:updateSubentitiesList(part)
	self.subentities:resetList()
	self.subentities:clearAllSelections()
	if part then
		local subentities = part:getSubEntityDefinitions()
		for val = 0, subentities:size() - 1 do
			local name = subentities[val]:getSubEntityName()
			if name == "" then
				name = subentities[val]:getSubEntityIndex()
			end
			local item = Ember.OgreView.Gui.ColouredListItem:new(name, val)
			self.subentities:addItem(item)
		end
	end
end

function ModelEdit:showPreview(definitionName)
	deleteSafe(self.modelHelper)
	self.renderer:showModel(definitionName)
	self.modelHelper = Ember.OgreView.Gui.ModelEditHelper:new(self.renderer:getModel(), self.renderer:getEntityTexture():getRenderContext())
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
	
	self.attachPointsList:resetList()
	local attachPoints = self.definition:getAttachPointsDefinitions()
	for val = 0, attachPoints:size() - 1 do
		local name = attachPoints[val].Name
		local item = Ember.OgreView.Gui.ColouredListItem:new(name, val)
		self.attachPointsList:addItem(item)
	end	
	
	self:updatePosesList()
end

function ModelEdit:updatePosesList()
	self.posesList:resetList()
	self.posesList.model = {}
	local poses = self.definition:getPoseDefinitions()
	local val = 0
	for k,v in poses:pairs() do
		local name = k
		local item = Ember.OgreView.Gui.ColouredListItem:new(name, val)
		self.posesList.model[val] = {def = v, name = name}
		val = val + 1
		self.posesList:addItem(item)
	end
end

function ModelEdit:fillWindowsFromVector(windowNamePrefix, vector)
		local xWindow = self.widget:getWindow(windowNamePrefix .. "_x");
		local yWindow = self.widget:getWindow(windowNamePrefix .. "_y");
		local zWindow = self.widget:getWindow(windowNamePrefix .. "_z");
	if vector then
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




function ModelEdit:attachPointToolShown_StateChanged(args)
	local shown = self.attachPointToolShown:isSelected()
	local part = self:getSelectedAttachPoint()
	--inspectObject(part)
	self:updatePartShown(part, shown)	
	return true
end


function ModelEdit:AddSubmodelButton_Clicked(args)
	
	local item = self.contentparts.modelInfo.meshlist:getFirstSelectedItem()
	--an item must be selected
	if item then
		local submodel = self.definition:createSubModelDefinition(item:getText())
		--lets create a "main" part automatically
		local part = submodel:createPartDefinition("main");
		--and lets add all submeshes to this new part
		
		--we need to get hold of a mesh instance
		local manager = Ogre.MeshManager:getSingleton()
		local name = submodel:getMeshName()
		local meshPtr = manager:load(name, "General")
		local mesh = meshPtr:get()
		
		if mesh then
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
	return true

end

function ModelEdit:NewModelOk_Clicked(args)
	local modelDefMgr = Ember.OgreView.Model.ModelDefinitionManager:getSingleton()
	local name = self.widget:getWindow("NewModelName"):getText()
	local def = modelDefMgr:create(name, "ModelDefinitions"):get()
	if def then
		def:setValid(true)
		
		--after adding the definition, update the model list
		self:fillModellist()
		local item = self.models:findItemWithText(name, self.models:getListboxItemFromIndex(0))
		if item then
			item:setSelected(true)
			self.models:ensureItemIsVisible(item)	
		end
		self.widget:getWindow("NewModelWindow"):setVisible(false)
		self:loadModelDefinition(item:getText())
		self:reloadModel()
		self:updateModelContentList()
	end
	return true
end


function ModelEdit:previewMesh(meshname)
	self.submeshRenderer:showEntity(meshname)
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
	local actions = self.definition:getActionDefinitions()
	
	local submodelsTreeItem = Ember.OgreView.Gui.ColouredTreeItem:new("Submodels", 0)
	submodelsTreeItem:toggleIsOpen()
	self.modelcontentstree:addItem(submodelsTreeItem)
	 
	local actionsTreeItem = Ember.OgreView.Gui.ColouredTreeItem:new("Actions", 0)
	actionsTreeItem:toggleIsOpen()
	self.modelcontentstree:addItem(actionsTreeItem)
	
	--first, add all submodels
	for val = 0, submodels:size() - 1 do
		local submodel = submodels[val]
		local name = submodel:getMeshName()
		local modelcontentItem = {}
		
		--we need to get hold of a mesh instance
		local manager = Ogre.MeshManager:getSingleton()
		local meshPtr = manager:getByName(name)
		local mesh = meshPtr:get()
		
		
		modelcontentItem.activate = function()
			self:showSubModel(submodel)
		end
		self.modelContentsItems[table.getn(self.modelContentsItems) + 1] = modelcontentItem
		
		local treeItem = Ember.OgreView.Gui.ColouredTreeItem:new(name, table.getn(self.modelContentsItems)) 
		treeItem:toggleIsOpen()
		treeItem:setTooltipText("Mesh '" .. name .. "'")
		submodelsTreeItem:addItem(treeItem)
		
		--add all parts
		if submodel then
			local parts = submodel:getPartDefinitions()
			for val_ = 0, parts:size() - 1 do
				local part = parts[val_]
				local name = part:getName()
				
				local modelcontentItem = {}
				modelcontentItem.activate = function()
					self:showPart(part)
				end
				
--				local partVisible = ""
--				if part:getShow() then
--					partVisible = " (shown)";
--				end
				
				self.modelContentsItems[table.getn(self.modelContentsItems) + 1] = modelcontentItem
				
				local treeItem2 = Ember.OgreView.Gui.ColouredTreeItem:new(name, table.getn(self.modelContentsItems))
				treeItem2:toggleIsOpen()
				treeItem2:setTooltipText("Part '" .. name .. "'")
				treeItem:addItem(treeItem2)
			
				if part then
					local subentities = part:getSubEntityDefinitions()
					for val = 0, subentities:size() - 1 do
						local subentity = subentities[val]
						
						local submeshname = self:getSubMeshName(mesh, subentity:getSubEntityIndex())
 
						
						local modelcontentItem = {}
						modelcontentItem.activate = function()
							self:showSubEntity(subentity)
						end

						self.modelContentsItems[table.getn(self.modelContentsItems) + 1] = modelcontentItem
						
						local treeItem3 = Ember.OgreView.Gui.ColouredTreeItem:new(submeshname, table.getn(self.modelContentsItems))
						treeItem3:toggleIsOpen()
						treeItem3:setTooltipText("Submesh '" .. submeshname .. "'")
						treeItem2:addItem(treeItem3)
					end
				end
			end
		end		
	end	
	
	--then, add all actions
	for val = 0, actions:size() - 1 do
		local action = actions[val]
		local name = action:getName()
		local modelcontentItem = {}
		
		modelcontentItem.activate = function()
			self:showAction(action)
		end
		self.modelContentsItems[table.getn(self.modelContentsItems) + 1] = modelcontentItem
		
		local treeItem = Ember.OgreView.Gui.ColouredTreeItem:new(name, table.getn(self.modelContentsItems)) 
		treeItem:toggleIsOpen()
		treeItem:setTooltipText("Action '" .. name .. "'")
		actionsTreeItem:addItem(treeItem)
		
		local animations = action:getAnimationDefinitions()
		for val_ = 0, animations:size() - 1 do
			local animation = animations[val_]
			local name = "animation"
			
			self.modelContentsItems[table.getn(self.modelContentsItems) + 1] = { activate = function() self:showAnimation(animation) end}
			
			local animationTreeItem = Ember.OgreView.Gui.ColouredTreeItem:new(name, table.getn(self.modelContentsItems)) 
			animationTreeItem:toggleIsOpen()
			animationTreeItem:setTooltipText("Animation")

			treeItem:addItem(animationTreeItem)

			local animationParts = animation:getAnimationPartDefinitions()
			for val__ = 0, animationParts:size() - 1 do
				local animationPart = animationParts[val__]
				local name = animationPart.Name
				
				self.modelContentsItems[table.getn(self.modelContentsItems) + 1] = { activate = function() self:showAnimationPart(animationPart) end }
				
				local animationPartTreeItem = Ember.OgreView.Gui.ColouredTreeItem:new(name, table.getn(self.modelContentsItems)) 
				animationPartTreeItem:toggleIsOpen()
				animationPartTreeItem:setTooltipText(name)
	
				animationTreeItem:addItem(animationPartTreeItem)
				
			end		
			
		end		
	end	
end




function ModelEdit:getCurrentModelContentItem()
	local item = self.modelcontentstree:getFirstSelectedItem()
	if item then
		local itemId= item:getID()
		local contentItem = self.modelContentsItems[itemId]
		return contentItem
	end
	return nil
end

function ModelEdit:getSelectedAttachPoint()
	local item = self.modelcontentstree:getFirstSelectedItem()
	if item then
		local itemId= item:getID()
		local contentItem = self.modelContentsItems[itemId]
		return contentItem
	end
	return nil
end

function ModelEdit:showModelContent(listitem)
	if listitem then
		local itemId = listitem:getID()
		local contentItem = self.modelContentsItems[itemId]
		if contentItem then
			if contentItem.activate then
				contentItem.activate()
			end
		end
	else
		self:showModel()
	end
end

function ModelEdit:hideAllContentParts()
	for key,value in pairs(self.contentparts) do 
		value:setVisible(false) 
	end
end

function ModelEdit:showSubModel(submodelDef)
	self:hideAllContentParts()
	self.contentparts.submodelInfo:setVisible(true)

	self.widget:getWindow("SubModelName"):setText(submodelDef:getMeshName())
	
	local sizeWidget = self.widget:getWindow("SubModelSize")
	
	local model = self.renderer:getModel()
	if model then
		local submodel = model:getSubModel(0)
		if submodel then
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

function ModelEdit:showPart(part)
	self:hideAllContentParts()
	self.contentparts.partInfo:setVisible(true)
	self.widget:getWindow("PartName"):setText(part:getName())
	self.partShown:setSelected(part:getShow())
	
	self:fillSubMeshList(part)
end


function ModelEdit:showSubEntity(subentity)
	self:hideAllContentParts()
	self.contentparts.submeshInfo:setVisible(true)
	self:selectMaterial(subentity)
end

function ModelEdit:showModel()
	self:hideAllContentParts()
	self.contentparts.modelInfo:setVisible(true)
end

function ModelEdit:showAction(action)
	self:hideAllContentParts()
	self.contentparts.actionInfo:setVisible(true)
	self.action = action
	
	self.widget:getWindow("ActionSpeed"):setText(action:getAnimationSpeed())
end

function ModelEdit:showAnimation(animation)
	self:hideAllContentParts()
	self.contentparts.animationInfo:setVisible(true)
	self.animation = animation

	self.widget:getWindow("AnimationIterations"):setText(animation:getIterations())
end

function ModelEdit:showAnimationPart(animationPart)
	self:hideAllContentParts()
--	self.contentparts.actionInfo:setVisible(true)

end

function ModelEdit:buildWidget()
	self.widget = guiManager:createWidget()
	
	--delay setup of the widget until it's shown for the first time
	local setup = function()
		self.renderImage = self.widget:getWindow("Image")
		--self.renderImage = CEGUI.toStaticImage(self.renderImage)
		
		self.models = self.widget:getWindow("Models")
		self.models = CEGUI.toListbox(self.models)
		self.models:subscribeEvent("ItemSelectionChanged", function(args)
			local item = self.models:getFirstSelectedItem()
			self:loadModelDefinition(item:getText())
			return true
		end)
		self.modelsfilter = self.widget:getWindow("FilterModels")
		self.modelsfilter = CEGUI.toEditbox(self.modelsfilter)
		self.modelslistholder = Ember.OgreView.Gui.ListHolder:new(self.models, self.modelsfilter)
		self.modelsAdapter = Ember.OgreView.Gui.Adapters.Ogre.ResourceListAdapter:new(self.modelslistholder, Ember.OgreView.Model.ModelDefinitionManager:getSingleton())
		
		local xW = self.widget:getWindow("ModelTranslate_x")
		local yW = self.widget:getWindow("ModelTranslate_y")
		local zW = self.widget:getWindow("ModelTranslate_z")
		self.translateAdapter = Ember.OgreView.Gui.Vector3Adapter:new(xW,yW ,zW)
		connect(self.connectors, self.translateAdapter.EventValueChanged, function()
			self.definition:setTranslate(self.translateAdapter:getValue())
			self.renderer:repositionSceneNode()
		end)
		self.widget:getWindow("ModelTranslate_reset"):subscribeEvent("Clicked", function(args)
			self.translateAdapter:setValue(Ogre.Vector3.ZERO)
			return true
		end)
		
		
		local xW = self.widget:getWindow("ModelContainedOffset_x")
		local yW = self.widget:getWindow("ModelContainedOffset_y")
		local zW = self.widget:getWindow("ModelContainedOffset_z")
		self.containedOffsetAdapter = Ember.OgreView.Gui.Vector3Adapter:new(xW,yW ,zW)
		connect(self.connectors, self.containedOffsetAdapter.EventValueChanged, function()
			self.definition:setContentOffset(self.containedOffsetAdapter:getValue())
			self.renderer:repositionSceneNode()
		end)
	
		local xW = self.widget:getWindow("ModelRotation_x")
		local yW = self.widget:getWindow("ModelRotation_y")
		local zW = self.widget:getWindow("ModelRotation_z")
		local degreeW = self.widget:getWindow("ModelRotation_degrees")
		self.rotationAdapter = Ember.OgreView.Gui.QuaternionAdapter:new(degreeW, xW,yW ,zW)
		connect(self.connectors, self.rotationAdapter.EventValueChanged, function()
			self.definition:setRotation(self.rotationAdapter:getValue())
			self.renderer:repositionSceneNode()
		end)
		self.widget:getWindow("ModelRotation_reset"):subscribeEvent("Clicked", function(args)
			self.rotationAdapter:setValue(Ogre.Quaternion.IDENTITY)
			return true
		end)
		
	
	
	
	
		self.contentparts = {}
		self.contentparts.partInfo = self.widget:getWindow("PartInfo")
		self.contentparts.modelInfo = self.widget:getWindow("ModelInfo")
		self.contentparts.submodelInfo = self.widget:getWindow("SubModelInfo")
		self.contentparts.submeshInfo = self.widget:getWindow("SubMeshInfo")
		self.contentparts.actionInfo = self.widget:getWindow("ActionInfo")
		self.contentparts.animationInfo = self.widget:getWindow("AnimationInfo")
		
		
		--hide all parts initially
		self:hideAllContentParts()
		
		
		self.contentparts.modelInfo.meshlist = self.widget:getWindow("MeshList")
		self.contentparts.modelInfo.meshlist = CEGUI.toListbox(self.contentparts.modelInfo.meshlist)
		self.contentparts.modelInfo.meshlist:subscribeEvent("ItemSelectionChanged", function()
			local item = self.contentparts.modelInfo.meshlist:getFirstSelectedItem()
			if item then
				self:previewMesh(item:getText())
			end
			return true
		end)
		
		self.contentparts.modelInfo.meshlistfilter =  CEGUI.toEditbox(self.widget:getWindow("MeshListFilter"))
		self.contentparts.modelInfo.meshlistlistholder = Ember.OgreView.Gui.ListHolder:new(self.contentparts.modelInfo.meshlist, self.contentparts.modelInfo.meshlistfilter)
		
		
		self.contentparts.submeshInfo.materiallist = self.widget:getWindow("Materials")
		self.contentparts.submeshInfo.materiallist = CEGUI.toListbox(self.contentparts.submeshInfo.materiallist)
		self.contentparts.submeshInfo.materiallist:subscribeEvent("ItemSelectionChanged", function(args)
			local item = self.contentparts.submeshInfo.materiallist:getFirstSelectedItem()
			if item then 
				local material = item:getText()
				local subentity = self:getSelectedSubEntity()
				--inspectObject(subentity)
				self:updateMaterial(subentity, material)
			end
			return true
		end)
		self.contentparts.submeshInfo.filter = self.widget:getWindow("FilterMaterials")
		self.contentparts.submeshInfo.filter = CEGUI.toEditbox(self.contentparts.submeshInfo.filter)
		self.contentparts.submeshInfo.listholder = Ember.OgreView.Gui.ListHolder:new(self.contentparts.submeshInfo.materiallist, self.contentparts.submeshInfo.filter)
		self.contentparts.submeshInfo.listadapter = Ember.OgreView.Gui.Adapters.Ogre.ResourceListAdapter:new(self.contentparts.submeshInfo.listholder, Ogre.MaterialManager:getSingleton())
		
		self.contentparts.submeshInfo.removeSubMeshButton = self.widget:getWindow("RemoveSubMeshButton")
		self.contentparts.submeshInfo.removeSubMeshButton:subscribeEvent("Clicked", function(args)
			--just remove the subentity definition from the part
			local subentity = self:getSelectedSubEntity()
			local part = subentity:getPartDefinition()
			part:removeSubEntityDefinition(subentity)
			self:reloadModel()
			self:updateModelContentList()
			return true
		end)
		
		self.widget:getWindow("PartRemoveButton"):subscribeEvent("Clicked", function(args)
			--just remove the part definition from the submodel
			local part = self:getSelectedPart()
			local submodel = part:getSubModelDefinition()
			submodel:removePartDefinition(part)
			self:reloadModel()
			self:updateModelContentList()
			return true
		end)
		self.widget:getWindow("AddSubmeshButton"):subscribeEvent("Clicked", function(args)
			--get the selected submesh and add it to the part
			local list = CEGUI.toListbox(self.widget:getWindow("SubmeshList"))
			local item = list:getFirstSelectedItem()
			--an item must be selected
			if item then
				local part = self:getSelectedPart()
				part:createSubEntityDefinition(item:getID())
				self:reloadModel()
				self:updateModelContentList()
			end
			return true
		end)
		self.widget:getWindow("AddPartButton"):subscribeEvent("Clicked", function(args)
			local name = self.widget:getWindow("NewPartName"):getText()
			if name ~= "" then
				local submodel = self:getSelectedSubModel()
				if submodel then
					submodel:createPartDefinition(name);
					self:reloadModel()
					self:updateModelContentList()
				end
			end
			return true
		end)
		self.widget:getWindow("RenamePartButton"):subscribeEvent("Clicked", function(args)
			local editbox = self.widget:getWindow("PartName")
			local name = editbox:getText()
			if name ~= "" then
				local part = self:getSelectedPart()
				if part then
					part:setName(name)
					self:reloadModel()
					self:updateModelContentList()
				end
			end	
			return true
		end)
		self.widget:getWindow("AddModelButton"):subscribeEvent("Clicked", function(args)
			self.widget:getWindow("NewModelWindow"):setVisible(true)
			self.widget:getWindow("NewModelWindow"):moveToFront()
			return true
		end)
		self.widget:getWindow("ReloadModelListButton"):subscribeEvent("Clicked", function(args)
			self:fillModellist()
			return true
		end)
		
		self.widget:getWindow("AddSubmodelButton"):subscribeEvent("Clicked", self.AddSubmodelButton_Clicked, self)
		self.widget:getWindow("SaveModelButton"):subscribeEvent("Clicked", function(args)
			local modelDefMgr = Ember.OgreView.Model.ModelDefinitionManager:getSingleton()
			local exportPath = modelDefMgr:exportScript(self.definitionPtr)
			if exportPath ~= "" then
				console:pushMessage("Model exported to " .. exportPath, "info")
			end	
			return true
		end)
		self.widget:getWindow("NewModelOk"):subscribeEvent("Clicked", self.NewModelOk_Clicked, self)
		self.widget:getWindow("NewModelCancel"):subscribeEvent("Clicked", function(args)
			self.widget:getWindow("NewModelWindow"):setVisible(false)
			return true
		end)
		self.widget:getWindow("RemoveSubmodelButton"):subscribeEvent("Clicked", function(args)
			local submodel = self:getSelectedSubModel()
			self.definition:removeSubModelDefinition(submodel)
				
			self:reloadModel()
			self:updateModelContentList()
			
			return true
		end)
		self.widget:getWindow("ReloadInstancesButton"):subscribeEvent("Clicked", function(args)
			--reload all model instances
			self.definition:reloadAllInstances()
			return true
		end)
		
		local updateModelAdapters = function()
			local translation = self.renderer:getEntityTranslation()
			local orientation = self.renderer:getEntityRotation()
			
			self.definition:setTranslate(translation)
			self.definition:setRotation(orientation)
			self.translateAdapter:updateGui(self.definition:getTranslate());
			self.rotationAdapter:updateGui(self.definition:getRotation());
		end
				
		self.widget:getWindow("YawLeft"):subscribeEvent("Clicked", function(args)
			self.renderer:getEntityTexture():getRenderContext():getSceneNode():yaw(Ogre.Radian:new_local(Ogre.Degree:new_local(-45)))
			updateModelAdapters()
			return true
		end)
		self.widget:getWindow("YawRight"):subscribeEvent("Clicked", function(args)
			self.renderer:getEntityTexture():getRenderContext():getSceneNode():yaw(Ogre.Radian:new_local(Ogre.Degree:new_local(45)))
			updateModelAdapters()
			return true
		end)
		self.widget:getWindow("RollLeft"):subscribeEvent("Clicked",function(args)
			self.renderer:getEntityTexture():getRenderContext():getSceneNode():roll(Ogre.Radian:new_local(Ogre.Degree:new_local(-45)))
			updateModelAdapters()
			return true
		end)
		self.widget:getWindow("RollRight"):subscribeEvent("Clicked", function(args)
			self.renderer:getEntityTexture():getRenderContext():getSceneNode():roll(Ogre.Radian:new_local(Ogre.Degree:new_local(45)))
			updateModelAdapters()
			return true
		end)
		self.widget:getWindow("PitchUp"):subscribeEvent("Clicked", function(args)
			self.renderer:getEntityTexture():getRenderContext():getSceneNode():pitch(Ogre.Radian:new_local(Ogre.Degree:new_local(-45)))
			updateModelAdapters()
			return true
		end)
		self.widget:getWindow("PitchDown"):subscribeEvent("Clicked", function(args)
			self.renderer:getEntityTexture():getRenderContext():getSceneNode():pitch(Ogre.Radian:new_local(Ogre.Degree:new_local(45)))
			updateModelAdapters()
			return true
		end)
		self.widget:getWindow("ResetOrientation"):subscribeEvent("Clicked", function(args)
			self.renderer:getEntityTexture():getRenderContext():getSceneNode():setOrientation(Ogre.Quaternion.IDENTITY)
			updateModelAdapters()
			return true
		end)


		
		local attachPointPreviewCombobox = CEGUI.toCombobox(self.widget:getWindow("AttachPointPreviewType"))
		local attachPointPreviewModelList = CEGUI.toListbox(self.widget:getWindow("AttachPointModelList"))
		local attachPointPreviewModelListFilter = CEGUI.toEditbox(self.widget:getWindow("AttachPointModelListFilter"))
		
		self.attachPointsList = self.widget:getWindow("AttachPointsList")
		self.attachPointsList = CEGUI.toListbox(self.attachPointsList)
		self.attachPointsList.getSelected = function()
			local item = self.attachPointsList:getFirstSelectedItem()
			if item then
				return self.definition:getAttachPointsDefinitions()[item:getID()]
			end
			return nil
		end
		local updateAttachPointPreview = function()
			local attachPoint = self.attachPointsList.getSelected()
			if attachPoint then
				local item = attachPointPreviewCombobox:getSelectedItem()
				if item then
					local selectId = item:getID()
					if selectId == 0 then
						self.modelHelper:showAttachPointHelperEntity(attachPoint.Name)
					elseif selectId == 1 then
						local modelItem = attachPointPreviewModelList:getFirstSelectedItem()
						if modelItem then
							local modelName = modelItem:getText()
							self.modelHelper:showAttachPointHelperModel(attachPoint.Name, modelName)
						end
					end
				end
			else
				self.modelHelper:hideAttachPointHelper()
			end
		end
		self.attachPointsList:subscribeEvent("ItemSelectionChanged", function(args)
			updateAttachPointPreview()
			return true
		end)
		
		attachPointPreviewModelList:subscribeEvent("ItemSelectionChanged", function(args)
			updateAttachPointPreview()
			return true
		end)
		
		
		local attachPointRotateButton = self.widget:getWindow("AttachPointRotate")
		attachPointRotateButton:subscribeEvent("MouseButtonDown", function(args)
			self.modelHelper:startInputRotate()
			return true
		end)
		
		local attachPointTranslateButton = self.widget:getWindow("AttachPointTranslate")
		attachPointTranslateButton:subscribeEvent("MouseButtonDown", function(args)
			self.modelHelper:startInputTranslate()
			return true
		end)

		local item = Ember.OgreView.Gui.ColouredListItem:new("Arrow", 0)
		attachPointPreviewCombobox:addItem(item)
		item = Ember.OgreView.Gui.ColouredListItem:new("Model", 1)
		attachPointPreviewCombobox:addItem(item)
		attachPointPreviewCombobox:setItemSelectState(0, true)
		attachPointPreviewCombobox:subscribeEvent("ListSelectionChanged", function(args)
			local item = attachPointPreviewCombobox:getSelectedItem()
			if item then
				local selectId = item:getID()
				if selectId == 1 then
					attachPointPreviewModelList:setVisible(true)
					attachPointPreviewModelListFilter:setVisible(true)
					if not self.attachPointPreviewModelListAdapter then
						self.attachPointPreviewModelListHolder = Ember.OgreView.Gui.ListHolder:new(attachPointPreviewModelList, attachPointPreviewModelListFilter)
						self.attachPointPreviewModelListAdapter = Ember.OgreView.Gui.Adapters.Ogre.ResourceListAdapter:new(self.attachPointPreviewModelListHolder, Ember.OgreView.Model.ModelDefinitionManager:getSingleton())
						self.attachPointPreviewModelListAdapter:update()
					end
				else
					attachPointPreviewModelList:setVisible(false)
					attachPointPreviewModelListFilter:setVisible(false)
				end
			end
		
			updateAttachPointPreview()
			return true
		end)
		
		self.posesList = CEGUI.toListbox(self.widget:getWindow("PoseList"))
		self.posesList:subscribeEvent("ItemSelectionChanged", function(args)
			local item = self.posesList:getFirstSelectedItem()
			if item then
				local poseDefWrapper = self.posesList.model[item:getID()]
				local poseDef = poseDefWrapper.def
				self.poseRenderer.poseDefWrapper = poseDefWrapper
				self.poseRenderer:showModel(self.definition:getName(), poseDef.Translate, poseDef.Rotate)
			else
				self.poseRenderer:showModel("")
			end
			
			return true
		end)
		
		
		local poseImage = self.widget:getWindow("PoseImage")
		self.poseRenderer = Ember.OgreView.Gui.ModelRenderer:new(poseImage)
		self.poseRendererManipulator = Ember.OgreView.Gui.CombinedEntityTextureManipulator:new(poseImage, self.poseRenderer:getEntityTexture())
		self.poseRenderer:showAxis();
		self.poseRenderer:setCameraPositionMode(Ember.OgreView.SimpleRenderContext.CPM_WORLDCENTER)
	
		self.contentparts.modelInfo.renderImage = self.widget:getWindow("MeshPreviewImage")
		
		
		
		connect(self.connectors, self.poseRendererManipulator.EventMovementStopped, function()
			local translation = self.poseRenderer:getEntityTranslation()
			local orientation = self.poseRenderer:getEntityRotation()
			local poseDef = self.poseRenderer.poseDefWrapper.def
			poseDef.Rotate = orientation
			poseDef.Translate = translation
			
			self.definition:addPoseDefinition(self.poseRenderer.poseDefWrapper.name, poseDef)
		end)
		
		connect(self.connectors, self.poseRendererManipulator.EventMovementStarted, function()
			local helpWindow = self.widget:getWindow("PoseImageHelpText") 
			helpWindow:fireEvent("StartHideTransition", CEGUI.WindowEventArgs:new_local(helpWindow))
		end)
		
		
		local newPoseButton = self.widget:getWindow("PoseNewButton")
		newPoseButton:setEnabled(false)
		local newPoseName = self.widget:getWindow("PoseNewName")
		
		newPoseName:subscribeEvent("TextChanged", function(args)
			if newPoseName:getText() ~= '' then
				newPoseButton:setEnabled(true)
			else
				newPoseButton:setEnabled(false)
			end
			return true
		end)


		newPoseButton:subscribeEvent("Clicked", function(args)
			local name = newPoseName:getText()
			local poseDef = Ember.OgreView.Model.PoseDefinition:new_local()
			poseDef.Rotate = self.poseRenderer:getEntityRotation()
			poseDef.Translate = self.poseRenderer:getEntityTranslation()
			
			self.definition:addPoseDefinition(name, poseDef)
			self:updatePosesList()
			local newItem = self.posesList:findItemWithText(name)
			if newItem then
				newItem:setSelected(true)
			end
			return true
		end)
		
	
	
		self.modelcontentstree = self.widget:getWindow("ModelContentsTree")
		self.modelcontentstree = tolua.cast(self.modelcontentstree,"CEGUI::Tree")
		self.modelcontentstree:setMultiselectEnabled(false)
		self.modelcontentstree:subscribeEvent("ItemSelectionChanged", function(args)
			self:showModelContent(self.modelcontentstree:getFirstSelectedItem())
			return true
		end)
		self.modelcontentstree:subscribeEvent("ListItemsChanged", function(args)
			self:showModelContent(self.modelcontentstree:getFirstSelectedItem())
			return true
		end)
		--self.modelcontents:setMutedState(true)
	
		
		self.partShown = self.widget:getWindow("Shown")
		self.partShown = CEGUI.toCheckbox(self.partShown)
		self.partShown:subscribeEvent("CheckStateChanged", function(args)
			local shown = self.partShown:isSelected()
			local part = self:getSelectedPart()
			self:updatePartShown(part, shown)	
			return true
		end)
			
		
			
		self.renderer = Ember.OgreView.Gui.ModelRenderer:new(self.renderImage)
		self.renderer:showAxis();
		self.renderer:setCameraPositionMode(Ember.OgreView.SimpleRenderContext.CPM_WORLDCENTER)
		self.rendererManipulator = Ember.OgreView.Gui.CombinedEntityTextureManipulator:new(self.renderImage, self.renderer:getEntityTexture())
		
		
		connect(self.connectors, self.rendererManipulator.EventMovementStopped, updateModelAdapters)
		
		connect(self.connectors, self.rendererManipulator.EventMovementStarted, function()
			local helpWindow = self.widget:getWindow("ImageHelpText") 
			helpWindow:fireEvent("StartHideTransition", CEGUI.WindowEventArgs:new_local(helpWindow))
		end)

		local cameraPosCombobox = CEGUI.toCombobox(self.widget:getWindow("ImageCameraPositioning"))
		local item = Ember.OgreView.Gui.ColouredListItem:new("World center", 0)
		cameraPosCombobox:addItem(item)
		item = Ember.OgreView.Gui.ColouredListItem:new("Object center", 1)
		cameraPosCombobox:addItem(item)
		cameraPosCombobox:setItemSelectState(0, true)
		cameraPosCombobox:subscribeEvent("ListSelectionChanged", function(args) 
			local item = cameraPosCombobox:getSelectedItem()
			if item then
				local selectId = item:getID()
				if selectId == 0 then
					self.renderer:setCameraPositionMode(Ember.OgreView.SimpleRenderContext.CPM_WORLDCENTER)
				elseif selectId == 1 then
					self.renderer:setCameraPositionMode(Ember.OgreView.SimpleRenderContext.CPM_OBJECTCENTER)
				end
			end
			return true 
		end)

		
		local subMeshPreviewImage = self.widget:getWindow("SubMeshPreviewImage")
		--subMeshPreviewImage = CEGUI.toStaticImage(subMeshPreviewImage)
		self.subMeshPartRenderer = Ember.OgreView.Gui.OgreEntityRenderer:new(subMeshPreviewImage)
		self.subMeshPartRendererManipulator = Ember.OgreView.Gui.CameraEntityTextureManipulator:new(subMeshPreviewImage, self.subMeshPartRenderer:getEntityTexture())
		
		local meshPreviewImage = self.widget:getWindow("MeshPreviewImage")
		--meshPreviewImage = CEGUI.toStaticImage(meshPreviewImage)
		self.submeshRenderer = Ember.OgreView.Gui.OgreEntityRenderer:new(meshPreviewImage)
		self.submeshRendererManipulator = Ember.OgreView.Gui.CameraEntityTextureManipulator:new(meshPreviewImage, self.submeshRenderer:getEntityTexture())
		
		--self.contentparts.modelInfo.renderer = Ember.OgreView.Gui.ModelRenderer:new_local(self.contentparts.modelInfo.renderImage)
		
		self.zoomSlider = self.widget:getWindow("Zoom")
		self.zoomSlider = CEGUI.toSlider(self.zoomSlider)
		
		self.zoomSlider:subscribeEvent("ValueChanged", function(args)
			self.renderer:setCameraDistance(self.zoomSlider:getCurrentValue())
			return true
		end)
		
		self.scaleTextbox = self.widget:getWindow("ModelScale")
		self.scaleTextbox:subscribeEvent("TextChanged", function(args)
			self.definition:setScale(tonumber(self.scaleTextbox:getText()));
			return true
		end)
		
		local showAxesCheckbox = CEGUI.toCheckbox(self.widget:getWindow("Image_ShowAxes"))
		showAxesCheckbox:subscribeEvent("CheckStateChanged", function(args)
			if showAxesCheckbox:isSelected() then
				self.renderer:showAxis()
			else
				self.renderer:hideAxis()
			end
			return true
		end)
		
		local showSkeletonCheckbox = CEGUI.toCheckbox(self.widget:getWindow("Image_ShowSkeleton"))
		showSkeletonCheckbox:subscribeEvent("CheckStateChanged", function(args)
			local model = self.renderer:getModel()
			if model then
				if showSkeletonCheckbox:isSelected() then
					model:setDisplaySkeleton(true)
				else
					model:setDisplaySkeleton(false)
				end
			end
			return true
		end)
				
		
		self.widget:getWindow("ExportAsAtlasTypeButton"):subscribeEvent("Clicked", function(args)
			local model = self.renderer:getModel()
			if model then
				local composer = Ember.OgreView.Model.ModelDefinitionAtlasComposer:new_local()
				local exportPath = composer:composeToFile(model, self.definition:getName(), "thing", self.definition:getScale())
				if exportPath ~= "" then
					console:pushMessage("Atlas exported to " .. exportPath, "info")
				end	
			end	
			return true
		end)
		self.widget:getWindow("GetIconFromPreviewButton"):subscribeEvent("Clicked", function(args)
			if self.definition then
				local definition = self.definition:createViewDefinition("icon")
				definition.Rotation = self.renderer:getEntityRotation():Inverse()
				--only set the camera distance if it has been moved from the optimal position
				if self.renderer:getCameraDistance() ~= 1 then
					definition.Distance = self.renderer:getAbsoluteCameraDistance()
				end
			end
			return true
		end)
		
		
		local playEndFunction = nil
		self.widget:getWindow("PlayAction"):subscribeEvent("Clicked", function(args)
		
			
			if playEndFunction then
				playEndFunction()
				playEndFunction = nil
			else
				if self.action then
					local actionName = self.action:getName()
					local currentConnection = createConnector(self.widget.EventFrameStarted):connect(function(timeslice)
						local model = self.renderer:getModel()
						if model then
							local modelAction = model:getAction(actionName)
							if modelAction then
								modelAction:getAnimations():addTime(timeslice)
							end
						end
					end)
					
					playEndFunction = function()
						currentConnection:disconnect()
						currentConnection = nil
						local model = self.renderer:getModel()
						if model then
							local modelAction = model:getAction(actionName)
							if modelAction then
								modelAction:getAnimations():reset()
							end
						end
						self.widget:getWindow("PlayAction"):setText("Play")
					end
					
					self.widget:getWindow("PlayAction"):setText("Stop")
				end

			end
			return true
		end)

		local animationIterationsWindow = self.widget:getWindow("AnimationIterations") 
		animationIterationsWindow:subscribeEvent("TextChanged", function(args)
				if self.animation then
					local iterations = tonumber(animationIterationsWindow:getText())
					self.animation:setIterations(iterations)
				end
			return true
		end)		
		
		local actionSpeedWindow = self.widget:getWindow("ActionSpeed") 
		actionSpeedWindow:subscribeEvent("TextChanged", function(args)
				if self.action then
					local speed = tonumber(actionSpeedWindow:getText())
					self.action:setAnimationSpeed(speed)
				end
			return true
		end)
		

		
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
	
	local item = Ember.OgreView.Gui.ColouredListItem:new("all", 0)
	self.scaleTypes:addItem(item)
	local item = Ember.OgreView.Gui.ColouredListItem:new("none", 1)
	self.scaleTypes:addItem(item)
	local item = Ember.OgreView.Gui.ColouredListItem:new("width", 2)
	self.scaleTypes:addItem(item)
	local item = Ember.OgreView.Gui.ColouredListItem:new("depth", 3)
	self.scaleTypes:addItem(item)
	local item = Ember.OgreView.Gui.ColouredListItem:new("height", 4)
	self.scaleTypes:addItem(item)
	
	self.scaleTypes:subscribeEvent("ListSelectionChanged", function(args)
		local model = self.definition
		local item = self.scaleTypes:getSelectedItem()
		if item then
			model:setUseScaleOf(item:getID())
		end
		return true
	end)

end

function ModelEdit:shutdown()
	self.definitionPtr = nil
	self.definition = nil
	deleteSafe(self.modelHelper)
	deleteSafe(self.submeshRendererManipulator)
	deleteSafe(self.submeshRenderer)
	deleteSafe(self.renderer)
	deleteSafe(self.subMeshPartRendererManipulator)
	deleteSafe(self.subMeshPartRenderer)
	if self.contentparts then
		if self.contentparts.submeshInfo then
			deleteSafe(self.contentparts.submeshInfo.listadapter)
			deleteSafe(self.contentparts.submeshInfo.listholder)
		end
		if self.contentparts.modelInfo then
			deleteSafe(self.contentparts.modelInfo.meshlistlistholder)
		end
	end
	deleteSafe(self.rotationAdapter)
	deleteSafe(self.containedOffsetAdapter)
	deleteSafe(self.translateAdapter)
	deleteSafe(self.modelsAdapter)
	deleteSafe(self.modelslistholder)
	deleteSafe(self.attachPointPreviewModelListAdapter)
	deleteSafe(self.attachPointPreviewModelListHolder)
	deleteSafe(self.poseRenderer)
	deleteSafe(self.poseRendererManipulator)
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

connect(connectors, emberOgre.EventGUIManagerInitialized, function(guiManager)
	local modelEdit = {connectors={}, 
		zoomRatio = 10, 
		modelContentsItems = {}
	}
	setmetatable(modelEdit, {__index = ModelEdit})
	
	modelEdit:buildWidget()

	connect(modelEdit.connectors, emberOgre.EventGUIManagerBeingDestroyed, function()
		modelEdit:shutdown()
		modelEdit = nil
	end)
end)
