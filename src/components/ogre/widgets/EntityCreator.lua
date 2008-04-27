EntityCreator = {connectors={}}

function EntityCreator.buildWidget()
	EntityCreator.widget = guiManager:createWidget()
	EntityCreator.widget:loadMainSheet("EntityCreator.layout", "EntityCreator/")

	EntityCreator.helper = EmberOgre.Gui.EntityCreator:new_local()

	EntityCreator.widget:registerConsoleVisibilityToggleCommand("advEntityCreator")
	EntityCreator.widget:enableCloseButton()
	EntityCreator.widget:hide()
end

function EntityCreator.Create_Click(args)
	console:runCommand("/show_modelEdit")
end

EntityCreator.buildWidget()
