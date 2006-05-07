--This script is the first one loaded. It's responsible for setting up utility objects, defining some utility functions and loading all scripts that should be loaded at start up


--set up some commonly used objects, these will be available to all scripts
guiSystem = CEGUI.System:getSingleton()
windowManager = CEGUI.WindowManager:getSingleton()
root = guiSystem:getGUISheet()

emberOgre = EmberOgre.EmberOgre:getSingleton()
guiManager = EmberOgre.GUIManager:getSingleton()
emberServices = Ember.EmberServices:getSingleton()
scriptingService = emberServices:getScriptingService()
console = Ember.ConsoleBackend:getMainConsole()

--loads a lua script
function loadScript(scriptname)
	--load all lua files
	--they all reside in the same directory
	local dirPrefix = "cegui/datafiles/lua_scripts/"
	
	scriptingService:loadScript(dirPrefix .. scriptname)

end

--simple debug method, prints the string representation of the object to the console
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
loadScript("Tasks.lua")






























