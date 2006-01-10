-----------------------------------------
-- Start of handler functions
-----------------------------------------

-----------------------------------------
-- Script Entry Point
-----------------------------------------
guiSystem = CEGUI.System:getSingleton()
windowManager = CEGUI.WindowManager:getSingleton()
root = guiSystem:getGUISheet()

emberOgre = EmberOgre.EmberOgre:getSingleton()
guiManager = EmberOgre.GUIManager:getSingleton()
emberServices = Ember.EmberServices:getSingleton()
scriptingService = emberServices:getScriptingService()
console = Ember.ConsoleBackend:getMainConsole()

function loadScript(scriptname)
	--load all lua files
	--they all reside in the same directory
	local dirPrefix = "cegui/datafiles/lua_scripts/"
	
	scriptingService:loadScript(dirPrefix .. scriptname)

end

function debug(object)
	console:pushMessage(tostring(object))
end
--load all lua files
--they all reside in the same directory

loadScript("Chat.lua")
loadScript("ServerBrowser.lua")
loadScript("Performance.lua")
loadScript("EntityPicker.lua")
loadScript("Give.lua")
loadScript("ScriptEdit.lua")
loadScript("ModelEdit.lua")






























