EntityCreator = {connectors={}}

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

function EntityCreator.RecipesList_SelectionChanged(args)
    local item = EntityCreator.recipesList:getFirstSelectedItem()
	if item ~= nil then
		local name = item:getText()
		local recipeMgr = EmberOgre.EntityRecipeManager:getSingleton()
		local recipe = recipeMgr:getByName(name):get()
		EntityCreator.showRecipe(recipe)
	end
end

function EntityCreator.Create_Click(args)
	console:runCommand("/show_modelEdit")
end

function EntityCreator.showRecipe(recipe)
	local element = Atlas.Message.Element("Booo!")
	local container = EntityCreator.widget:getWindow("AdaptersContainer")
	local adapter = EntityCreator.factory:createStaticAdapter(container, "testAdapter", element)
end

function EntityCreator.buildWidget()
	EntityCreator.widget = guiManager:createWidget()
	EntityCreator.widget:loadMainSheet("EntityCreator.layout", "EntityCreator/")

	EntityCreator.helper = EmberOgre.Gui.EntityCreator()
	EntityCreator.factory = EmberOgre.Gui.Adapters.Atlas.AdapterFactory("EntityCreator")

	EntityCreator.recipesList = CEGUI.toListbox(EntityCreator.widget:getWindow("RecipesList"))

	EntityCreator.fillRecipesList()

	EntityCreator.widget:registerConsoleVisibilityToggleCommand("advEntityCreator")
	EntityCreator.widget:enableCloseButton()
	EntityCreator.widget:hide()
end

EntityCreator.buildWidget()
