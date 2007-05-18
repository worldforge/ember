Admin = {}

function Admin.buildWidget()
	Admin.widget = guiManager:createWidget()
	
	Admin.widget:loadMainSheet("Admin.layout", "Admin/")
	
	local innerWindow = Admin.widget:getWindow("InnerWindow")
	if innerWindow ~= nil then
		Admin.stackableContainer = EmberOgre.Gui.StackableContainer:new_local(innerWindow)
		Admin.stackableContainer:setInnerContainerWindow(innerWindow)
		Admin.stackableContainer:setFlowDirection(EmberOgre.Gui.StackableContainer.Horizontal)
		Admin.stackableContainer:repositionWindows()
	end
	
	Admin.widget:getWindow("ModelEditor"):subscribeEvent("MouseClick", "Admin.ModelEditor_Click")
	Admin.widget:getWindow("TerrainEditor"):subscribeEvent("MouseClick", "Admin.TerrainEditor_Click")
	Admin.widget:getWindow("SwitchCamera"):subscribeEvent("MouseClick", "Admin.SwitchCamera_Click")
	Admin.widget:getWindow("EntityCreator"):subscribeEvent("MouseClick", "Admin.EntityCreator_Click")
	Admin.widget:getWindow("ScriptEditor"):subscribeEvent("MouseClick", "Admin.ScriptEditor_Click")
	Admin.widget:getWindow("EntityBrowser"):subscribeEvent("MouseClick", "Admin.EntityBrowser_Click")

end

function Admin.ModelEditor_Click(args)
	console:runCommand("/show_modelEdit")
end

function Admin.TerrainEditor_Click(args)
	if TerrainEditor == nil then
		loadScript("TerrainEditor.lua")
	end
	console:runCommand("/show_terrainEditor")
	
end

function Admin.SwitchCamera_Click(args)
	console:runCommand("/toggle_cameraattached")
end

function Admin.EntityCreator_Click(args)
	console:runCommand("/show_entitycreator")
end

function Admin.ScriptEditor_Click(args)
	console:runCommand("/show_scriptEdit")
end

function Admin.ScriptEditor_Click(args)
	console:runCommand("/show_entityBrowser")
end

function Admin.createdAvatarEmberEntity(avatarEntity)
	if avatarEntity:getAvatar():isAdmin() then
		Admin.buildWidget()
	end
end


EmberOgre.LuaConnector:new(emberOgre.EventCreatedAvatarEntity):connect("Admin.createdAvatarEmberEntity")
