EntityCreator = {connectors={}}

function EntityCreator.fillRecipesList()
	local list = EntityCreator.widget:getWindow("RecipesList")
	list = CEGUI.toListbox(list)
	list:resetList()

	local recipeMgr = EmberOgre.EntityRecipeManager:getSingleton()
	local I = recipeMgr:getResourceIterator()
	local i = 0
	while I:hasMoreElements() do
		local recipePtr = I:getNext()
		recipePtr = tolua.cast(recipePtr, "EmberOgre::EntityRecipePtr")
		local recipe = recipePtr:get()
		local name = recipe:getName()
		local item = EmberOgre.Gui.ColouredListItem:new(name, i, name)
		list:addItem(item)
		i = i + 1
	end
end

function EntityCreator.buildWidget()
	EntityCreator.widget = guiManager:createWidget()
	EntityCreator.widget:loadMainSheet("EntityCreator.layout", "EntityCreator/")

	EntityCreator.helper = EmberOgre.Gui.EntityCreator()

	EntityCreator.fillRecipesList()

	EntityCreator.widget:registerConsoleVisibilityToggleCommand("advEntityCreator")
	EntityCreator.widget:enableCloseButton()
	EntityCreator.widget:hide()
end

function EntityCreator.Create_Click(args)
	console:runCommand("/show_modelEdit")
end

EntityCreator.buildWidget()
