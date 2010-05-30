EntityCreator = {}

-- Fills recipes list at start
function EntityCreator:fillRecipesList()
	local list = self.recipesList
	list:resetList()

	local recipeMgr = EmberOgre.Authoring.EntityRecipeManager:getSingleton()
	local I = recipeMgr:getResourceIterator()
	while I:hasMoreElements() do
		local recipePtr = I:getNext()
		recipePtr = tolua.cast(recipePtr, "EmberOgre::Authoring::EntityRecipePtr")
		local recipe = recipePtr:get()
		local name = recipe:getName()
		local item = EmberOgre.Gui.ColouredListItem:new(name)
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
		local recipeMgr = EmberOgre.Authoring.EntityRecipeManager:getSingleton()
		local recipePtr = recipeMgr:getByName(name)
		recipePtr = tolua.cast(recipePtr, "EmberOgre::Authoring::EntityRecipePtr")
		self.recipe = recipePtr:get()
		--We just set the recipe here and wait for the EventTypeInfoLoaded event, which will call the showRecipe function when the recipe is ready
		self.helper:setRecipe(self.recipe)
	else
		self.recipe = nil
	end
end

-- Handles create button press
function EntityCreator:Create_Click(args)
	self.helper:toggleCreateMode()
end

-- Handles toggling the randomizing of the orientation on and off
function EntityCreator:RandomizeOrientation_CheckStateChanged(args)
	local checkbox = self.widget:getWindow("RandomizeOrientation")
	checkbox = CEGUI.toCheckbox(checkbox)

	self.helper:setRandomizeOrientation(checkbox:isSelected())
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

		local container = guiManager:createWindow("DefaultGUISheet")
		v:attach(container)

		self:addAdapter(v, v:getTitle(), container, self.container)
	end
end

-- Adds adapter
function EntityCreator:addAdapter(adapter, title, container, parentContainer)
	local textWidth = 75
	local outercontainer = guiManager:createWindow("DefaultGUISheet")
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
	local SizedConnection = container:subscribeEvent("Sized", syncWindowHeights, self)

	outercontainer:addChildWindow(label)
	outercontainer:addChildWindow(container)

	parentContainer:addChildWindow(outercontainer)
	parentContainer:notifyScreenAreaChanged(true)
	return outercontainer
end

function EntityCreator:shutdown()
	guiManager:destroyWidget(self.widget)
	self.helper:delete()
end

-- Builds widget
function EntityCreator.buildWidget(avatar)

	entityCreator = {connectors={}}
	setmetatable(entityCreator, {__index = EntityCreator})
		
	
	-- Loading widget layout
	entityCreator.widget = guiManager:createWidget()
	local setup = function()
		-- Initializing helper classes
		debugObject(avatar)
		entityCreator.helper = EmberOgre.Gui.EntityCreator:new(avatar:getConnection())

		connect(entityCreator.connectors, entityCreator.helper.EventTypeInfoLoaded, entityCreator.showRecipe, entityCreator)
		entityCreator.helper.mWidget = entityCreator.widget
		entityCreator.factory = EmberOgre.Gui.Adapters.Atlas.AdapterFactory("EntityCreator")
	
		-- Creating container for storing adapters
		entityCreator.recipeDescription = entityCreator.widget:getWindow("RecipeDescription")
		entityCreator.container = entityCreator.widget:getWindow("AdaptersContainer")
		entityCreator.stackableContainer = EmberOgre.Gui.StackableContainer(entityCreator.container)
	
		-- Filling list of recipes
		entityCreator.recipesList = CEGUI.toListbox(entityCreator.widget:getWindow("RecipesList"))
		entityCreator.recipesList:subscribeEvent("ItemSelectionChanged", EntityCreator.RecipesList_SelectionChanged, entityCreator) 
		entityCreator:fillRecipesList()
		
		entityCreator.createButton = CEGUI.toPushButton(entityCreator.widget:getWindow("Create"))
		entityCreator.createButton:subscribeEvent("Clicked", EntityCreator.Create_Click, entityCreator) 
		entityCreator.createButton:setEnabled(false)

		entityCreator.widget:getWindow("RandomizeOrientation"):subscribeEvent("CheckStateChanged", EntityCreator.RandomizeOrientation_CheckStateChanged, entityCreator) 
	
		-- Finalizing
		entityCreator.widget:enableCloseButton()
	end
	connect(entityCreator.connectors, entityCreator.widget.EventFirstTimeShown, setup)
	entityCreator.widget:registerConsoleVisibilityToggleCommand("advEntityCreator")
	entityCreator.widget:loadMainSheet("EntityCreator.layout", "EntityCreator/")
	
	connect(entityCreator.connectors, emberServices:getServerService().DestroyedAccount, function()
			entityCreator:shutDown()
			entityCreator = nil
		end
		, entityCreator)
	
end

connect(connectors, emberServices:getServerService().GotAvatar, EntityCreator.buildWidget)

