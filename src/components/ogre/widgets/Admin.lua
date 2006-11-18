Admin = {}

function Admin_buildWidget()
	Admin.widget = guiManager:createWidget()
	
	Admin.widget:loadMainSheet("Admin.layout", "Admin/")
	
	Admin.widget:getWindow("ModelEditor"):subscribeEvent("MouseClick", "Admin_ModelEditor_Click")
	Admin.widget:getWindow("TerrainEditor"):subscribeEvent("MouseClick", "Admin_TerrainEditor_Click")
	Admin.widget:getWindow("SwitchCamera"):subscribeEvent("MouseClick", "Admin_SwitchCamera_Click")
	Admin.widget:getWindow("EntityCreator"):subscribeEvent("MouseClick", "Admin_EntityCreator_Click")
	Admin.widget:getWindow("ScriptEditor"):subscribeEvent("MouseClick", "Admin_ScriptEditor_Click")

end

function Admin_ModelEditor_Click(args)
	console:runCommand("/show_modelEdit")
end

function Admin_TerrainEditor_Click(args)
	if TerrainEditor == nil then
		loadScript("TerrainEditor.lua")
	end
	console:runCommand("/show_terrainEditor")
	
end

function Admin_SwitchCamera_Click(args)
	console:runCommand("/toggle_cameraattached")
end

function Admin_EntityCreator_Click(args)
	console:runCommand("/show_entitycreator")
end

function Admin_ScriptEditor_Click(args)
	console:runCommand("/show_scriptEdit")
end


function Admin_createdAvatarEmberEntity(avatarEntity)
	if avatarEntity:getAvatar():isAdmin() then
		Admin_buildWidget()
	end
end


EmberOgre.LuaConnector:new(emberOgre.EventCreatedAvatarEntity):connect("Admin_createdAvatarEmberEntity")
