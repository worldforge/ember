--This script is the first one loaded. It's responsible for setting up utility objects, defining some utility functions and loading all scripts that should be loaded at start up


--set up some commonly used objects, these will be available to all scripts

--The main application object
app = Ember.Application:getSingleton()

--The main CEGUI system. This is the main channel for all CEGUI actions.
guiSystem = CEGUI.System:getSingleton()

--The CEGUI WindowManager. Use this if you want to do some fancy window magic. Oftenwhile you don't need to touch this however since most window actions can be handled by the Widget class, which has methods such as loadMainSheet(...) and getWindow(...)
windowManager = CEGUI.WindowManager:getSingleton()

--The root GUI sheet under which all other CEGUI windows reside.
root = guiSystem:getGUISheet()

--This is the main ember object. You can usually use any of it's many get*() methods to access other parts of the system.
emberOgre = EmberOgre.EmberOgre:getSingleton()

--The GUIManager handles higher level gui actions. It doesn't know anything about the lua or CEGUI world, but has some useful events, for example EventFrameStarted which is emitted every frame.
--Also, when you create a new widget you have to use the createWidget() method.
guiManager = EmberOgre.GUIManager:getSingleton()

--All of Ember's services can be accessed from here.
emberServices = Ember.EmberServices:getSingleton()

--Scripting is handled by a service. This will allow you to load other scripts and perform more advanced scripting functions. See also the method loadScript(...) defined below.
scriptingService = emberServices:getScriptingService()

--The console can be used for debug output.
console = Ember.ConsoleBackend:getMainConsole()

--loads a lua script
function loadScript(scriptname)
	--load all lua files
	--they all reside in the same directory
	local dirPrefix = "lua/"
	
	scriptingService:loadScript(dirPrefix .. scriptname)

end

--simple debug method, prints the string representation of the object to the console
function debug(object)
	console:pushMessage(tostring(object))
end


--Load all lua files.
--They all reside in the same directory.
--If you want another window to also load at start up, add it to the list here.

loadScript("Chat.lua")
loadScript("ServerBrowser.lua")
loadScript("Performance.lua")
loadScript("EntityPicker.lua")
loadScript("Give.lua")
loadScript("ScriptEdit.lua")
loadScript("ModelEdit.lua")
loadScript("Tasks.lua")
loadScript("Mover.lua")
loadScript("Admin.lua")
loadScript("IndirectRenderingChecker.lua")
loadScript("EntityEditor.lua")
loadScript("EntityBrowser.lua")

--loadScript("TerrainEditor.lua")






























