EntityCreator = {connectors={}}

-- Fills recipes list at start
function EntityCreator.fillRecipesList()
	local list = EntityCreator.recipesList
	list:resetList()

	local recipeMgr = EmberOgre.EntityRecipeManager:getSingleton()
	local I = recipeMgr:getResourceIterator()
	while I:hasMoreElements() do
		local recipePtr = I:getNext()
		recipePtr = tolua.cast(recipePtr, "EmberOgre::EntityRecipePtr")
		local recipe = recipePtr:get()
		local name = recipe:getName()
		local item = EmberOgre.Gui.ColouredListItem:new(name)
		list:addItem(item)
	end
end

-- Handles click on recipes list
function EntityCreator.RecipesList_SelectionChanged(args)
	local item = EntityCreator.recipesList:getFirstSelectedItem()
	if item ~= nil then
		local name = item:getText()
		local recipeMgr = EmberOgre.EntityRecipeManager:getSingleton()
		local recipePtr = recipeMgr:getByName(name)
		recipePtr = tolua.cast(recipePtr, "EmberOgre::EntityRecipePtr")
		EntityCreator.recipe = recipePtr:get()
		EntityCreator.showRecipe(EntityCreator.recipe)
	end
end

-- Handles create button press
function EntityCreator.Create_Click(args)
	EntityCreator.recipe:createEntity()
end

-- Shows selected recipe
function EntityCreator.showRecipe(recipe)
--	local element = Atlas.Message.Element("Booo!")
--	local container = EntityCreator.widget:getWindow("AdaptersContainer")
--	local adapter = EntityCreator.factory:createStaticAdapter(container, "testAdapter", element)

	EntityCreator.helper:showRecipe(recipe, EntityCreator.container)
end

-- Builds widget
function EntityCreator.buildWidget()
	-- Loading widget layout
	EntityCreator.widget = guiManager:createWidget()
	EntityCreator.widget:loadMainSheet("EntityCreator.layout", "EntityCreator/")

	-- Initializing helper classes
	EntityCreator.helper = EmberOgre.Gui.EntityCreator()
	EntityCreator.factory = EmberOgre.Gui.Adapters.Atlas.AdapterFactory("EntityCreator")

	-- Creating container for storing adapters
	EntityCreator.container = EntityCreator.widget:getWindow("AdaptersContainer")
	EntityCreator.stackableContainer = EmberOgre.Gui.StackableContainer(EntityCreator.container)

	-- Filling list of recipes
	EntityCreator.recipesList = CEGUI.toListbox(EntityCreator.widget:getWindow("RecipesList"))
	EntityCreator.fillRecipesList()

	-- Finalizing
	EntityCreator.widget:registerConsoleVisibilityToggleCommand("advEntityCreator")
	EntityCreator.widget:enableCloseButton()
	EntityCreator.widget:hide()
end

EntityCreator.buildWidget()
