EntityCreator = {}

-- Fills recipes list at start
function EntityCreator:fillRecipesList()
	local list = self.recipesList
	list:resetList()

	local recipeMgr = Ember.OgreView.Authoring.EntityRecipeManager:getSingleton()
	local I = recipeMgr:getResourceIterator()
	while I:hasMoreElements() do
		local recipePtr = I:getNext()
		recipePtr = tolua.cast(recipePtr, "Ember::OgreView::Authoring::EntityRecipePtr")
		local recipe = recipePtr:get()
		local name = recipe:getName()
		local item = Ember.OgreView.Gui.ColouredListItem:new(name)
		list:addItem(item)
	end
end

-- Handles click on recipes list
function EntityCreator:RecipesList_SelectionChanged(args)
	self:clear()

	self.widget:getWindow("CreateSection"):setVisible(false)
	self.widget:getWindow("WaitForTypeInfo"):setVisible(true)
	local item = self.recipesList:getFirstSelectedItem()
	if item ~= nil then
		local name = item:getText()
		local recipeMgr = Ember.OgreView.Authoring.EntityRecipeManager:getSingleton()
		local recipePtr = recipeMgr:getByName(name)
		recipePtr = tolua.cast(recipePtr, "Ember::OgreView::Authoring::EntityRecipePtr")
		self.recipe = recipePtr:get()
		--We just set the recipe here and wait for the EventTypeInfoLoaded event, which will call the showRecipe function when the recipe is ready
		self.helper:setRecipe(self.recipe)
	else
		self.recipe = nil
	end
end

function EntityCreator:createFromType(name, type)
	self.typesCreator.recipe = Ember.OgreView.Authoring.EntityRecipe:new_local(name, type:getName())
	self.recipe = self.typesCreator.recipe
	self.helper:setRecipe(self.typesCreator.recipe)
	self.helper:toggleCreateMode()
end


-- Clears widget if recipe is selected
function EntityCreator:clear()
	if self.recipe ~= nil then
		-- Detaching adapters
		for k,v in self.recipe:getGUIAdapters():pairs() do
			v:detach()
		end
		-- Clearing container, from end to start
		for i = self.container:getChildCount() - 1, 0, -1 do
		    log.verbose("Destroying adapter container N" .. i)
		    local window = self.container:getChildAtIdx(i)
			windowManager:destroyWindow(window)
		end
	end
	self.createButton:setEnabled(false)

end

-- Shows selected recipe
function EntityCreator:showRecipe()
	local recipe = self.recipe
	local author = recipe:getAuthor()
	local description = recipe:getDescription()
	
	self.createButton:setEnabled(true)

	
	self.widget:getWindow("CreateSection"):setVisible(true)
	self.widget:getWindow("WaitForTypeInfo"):setVisible(false)
	if author ~= "" then
		description = "Author: " .. author .. "\n" .. description
	end
	self.recipeDescription:setText(description)

	for k,v in recipe:getGUIAdapters():pairs() do
		log.verbose("Creating adapter '" .. k .. "' of type '" .. v:getType() .. "'.")

		local container = guiManager:createWindow("DefaultWindow")
		v:attach(container)

		self:addAdapter(v, v:getTitle(), container, self.container)
	end
end

-- Adds adapter
function EntityCreator:addAdapter(adapter, title, container, parentContainer)
	local textWidth = 75
	local outercontainer = guiManager:createWindow("DefaultWindow")
	--outercontainer:setRiseOnClickEnabled(false)
	local label = guiManager:createWindow("EmberLook/StaticText")
	
	label:setText(title)
	label:setWidth(CEGUI.UDim(0, textWidth))
	label:setProperty("FrameEnabled", "false");
 	label:setProperty("BackgroundEnabled", "false");
	label:setProperty("VertFormatting", "TopAligned");
	label:setProperty("Tooltip", title);
	
	local width = container:getWidth()
	width = width + CEGUI.UDim(0, textWidth)
	outercontainer:setWidth(width)
	container:setXPosition(CEGUI.UDim(0, textWidth))
	container:setProperty("Tooltip", adapter:getTooltip());
	
	outercontainer:setHeight(container:getHeight())
	
	--make sure that the outer container has the same height as the inner container (so that when we add new child adapters it's updated)
	function syncWindowHeights(args)
		outercontainer:setHeight(container:getHeight())
	end
	local SizedConnection = container:subscribeEvent("Sized", syncWindowHeights)

	outercontainer:addChildWindow(label)
	outercontainer:addChildWindow(container)

	parentContainer:addChildWindow(outercontainer)
	return outercontainer
end

function EntityCreator:shutdown()
	guiManager:destroyWidget(self.widget)
	deleteSafe(self.helper)
	if self.typesCreator then
		deleteSafe(self.typesCreator.helper)
	end
	disconnectAll(self.connectors)
end

-- Builds widget
function EntityCreator.buildWidget(world)

	entityCreator = {connectors={}}
	setmetatable(entityCreator, {__index = EntityCreator})
		
	
	-- Loading widget layout
	entityCreator.widget = guiManager:createWidget()
	local setup = function()
		-- Initializing helper classes
		entityCreator.helper = Ember.OgreView.Gui.EntityCreator:new(world)
		
		connect(entityCreator.connectors, entityCreator.helper.EventCreationStarted, function()
				entityCreator.widget:getMainWindow():setAlpha(0.6)
			end
		)
		
		connect(entityCreator.connectors, entityCreator.helper.EventCreationEnded, function()
				entityCreator.widget:getMainWindow():setAlpha(1)
			end
		)

		connect(entityCreator.connectors, entityCreator.helper.EventTypeInfoLoaded, entityCreator.showRecipe, entityCreator)
		entityCreator.factory = Ember.OgreView.Gui.Adapters.Atlas.AdapterFactory("EntityCreator")
	
		-- Creating container for storing adapters
		entityCreator.recipeDescription = entityCreator.widget:getWindow("RecipeDescription")
		entityCreator.container = entityCreator.widget:getWindow("AdaptersContainer")
		entityCreator.stackableContainer = Ember.OgreView.Gui.StackableContainer(entityCreator.container)
	
		-- Filling list of recipes
		entityCreator.recipesList = CEGUI.toListbox(entityCreator.widget:getWindow("RecipesList"))
		entityCreator.recipesList:subscribeEvent("ItemSelectionChanged", EntityCreator.RecipesList_SelectionChanged, entityCreator) 
		entityCreator:fillRecipesList()
		
		entityCreator.createButton = CEGUI.toPushButton(entityCreator.widget:getWindow("Create"))

		entityCreator.createButton:subscribeEvent("Clicked", function() 
			entityCreator.helper:toggleCreateMode()
		end) 
		entityCreator.createButton:setEnabled(false)

		entityCreator.widget:getWindow("RandomizeOrientation"):subscribeEvent("CheckStateChanged", function()
			local checkbox = entityCreator.widget:getWindow("RandomizeOrientation")
			checkbox = CEGUI.toCheckbox(checkbox)
		
			entityCreator.helper:setRandomizeOrientation(checkbox:isSelected())
		end) 
		entityCreator.widget:enableCloseButton()
	
		local typesTree = CEGUI.toTree(entityCreator.widget:getWindow("Types/TypeList", true))
		local typesName = CEGUI.toEditbox(entityCreator.widget:getWindow("Types/Name", true))
		local typesCreateButton = CEGUI.toPushButton(entityCreator.widget:getWindow("Types/CreateButton", true))
		local typesPreviewImage = entityCreator.widget:getWindow("Types/ModelPreviewImage", true)
		entityCreator.typesCreator = {}

		entityCreator.typesCreator.helper = Ember.OgreView.Gui.EntityCreatorTypeHelper:new(world:getView():getAvatar():getConnection(), typesTree, typesName, typesCreateButton, typesPreviewImage)
		connect(entityCreator.connectors, entityCreator.typesCreator.helper.EventCreateFromType, entityCreator.createFromType, entityCreator)
	
		--Entity exports tab

		local activeOverlay = entityCreator.widget:getWindow("Exports/ActiveOverlay")

		local cancelButton = entityCreator.widget:getWindow("Exports/DumpCancel")
		cancelButton:subscribeEvent("Clicked", function(args)
			if cancelButton.method then
				cancelButton.method()
				return true
			end
		end)
		
		
		local importButton = entityCreator.widget:getWindow("Exports/ImportButton") 
		importButton:subscribeEvent("Clicked", function(args)
			if importButton.filename then
			
			
				local worldLoader = Ember.EntityImporter:new(emberServices:getServerService():getAccount())
				local dumpStatusWindow = entityCreator.widget:getWindow("Exports/DumpStatus") 
				
				dumpStatusWindow:setText("Loading...")
				createConnector(worldLoader.EventCompleted):connect(function()
					dumpStatusWindow:setText("Done loading.")
					activeOverlay:setVisible(false)
					cancelButton.method = nil
					worldLoader:delete()
				end)
				createConnector(worldLoader.EventProgress):connect(function(entitiesToLoad)
					dumpStatusWindow:setText("Loading, " .. entitiesToLoad .. " left")
				end)
				cancelButton.method = function()
					worldLoader:cancel()
					dumpStatusWindow:setText("Cancelled")
				end
				activeOverlay:setVisible(true)
				worldLoader:start(importButton.filename)
			end
			return true
		end)
		
		local populateExportsList = function(listWidget)
			local exports = Ember.EntityImporter:getInfoFromDirectory(emberServices:getConfigService():getHomeDirectory() .. "/entityexport")

			if exports:size() > 0 then 
				for i = 0, exports:size() - 1 do
					local info = exports[i]
					local item = CEGUI.toItemEntry(windowManager:createWindow("EmberLook/ListboxItem"))
					item:setText(info.name)
					
					item:subscribeEvent("SelectionChanged", function(args)
						if item:isSelected() then
							importButton:setEnabled(true)
							importButton.filename = info.filename
							
							local infoWidget = entityCreator.widget:getWindow("Exports/DetailsText")
							local infoString = "Name: " .. info.name .. "\nDescription: " .. info.description .. "\nNumber of entities: " .. info.entityCount
							infoWidget:setText(infoString) 
							
						end
			
						return true
					end
					)
					
					listWidget:addItem(item)			
				end
			end 
		end
		
		populateExportsList(CEGUI.toItemListbox(entityCreator.widget:getWindow("Exports/ExportsList")))
		
	end
	connect(entityCreator.connectors, entityCreator.widget.EventFirstTimeShown, setup)
	entityCreator.widget:registerConsoleVisibilityToggleCommand("advEntityCreator")
	entityCreator.widget:loadMainSheet("EntityCreator.layout", "EntityCreator/")
	
	connect(entityCreator.connectors, emberServices:getServerService().DestroyedAvatar, function()
			entityCreator:shutdown()
			entityCreator = nil
		end)
	
end

connect(connectors, emberOgre.EventWorldCreated, EntityCreator.buildWidget)

