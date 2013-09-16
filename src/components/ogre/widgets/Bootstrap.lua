--This script is the first one loaded. It's responsible for setting up utility objects, defining some utility functions and loading all scripts that should be loaded at start up

CEGUI.toCheckbox = function(w)
    return tolua.cast(w,"CEGUI::ToggleButton")
end

CEGUI.Element.getChildCount = function(window)
	return Ember.Cegui.Helper:Window_getChildCount(tolua.cast(window,"CEGUI::Window"))
end

CEGUI.ImageManager.getSingleton = function(self)
	return Ember.Cegui.Helper:ImageManager_getSingleton()
end 

local oldGetChildRecursive = CEGUI.Window.getChildRecursive
local window_getChildRecursive = function(window, id)
	if type(id) == "string" then
		return Ember.Cegui.Helper:Window_getChildRecursive(tolua.cast(window,"CEGUI::Window"), id)
	else
		return oldGetChildRecursive(window, id)
	end
end

CEGUI.Window.getChildRecursive = window_getChildRecursive

CEGUI.Window.removeChild = function(window, child)
	Ember.Cegui.Helper:Window_removeChild(window, child)
end 



--set up some commonly used objects, these will be available to all scripts

--The main CEGUI system. This is the main channel for all CEGUI actions.
guiSystem = CEGUI.System:getSingleton()

--The CEGUI WindowManager. Use this if you want to do some fancy window magic. Oftenwhile you don't need to touch this however since most window actions can be handled by the Widget class, which has methods such as loadMainSheet(...) and getWindow(...)
windowManager = CEGUI.WindowManager:getSingleton()

--This is the main ember object. You can usually use any of it's many get*() methods to access other parts of the system.
emberOgre = Ember.OgreView.EmberOgre:getSingleton()

--Tries to find the selected entity, and if found performs a function on it. If it's not found, and optional function can be performed with the entity id as argument.
--This allows you to safely perform actions on an entity which might have disappeared from the scene.
--@param entityId The id of the entity.
--@param doIfEntityFound The function to call if the entity is found. The first argument is the entity.
--@param doIfEntityNotFound An optional function which will be called if the entity isn't found. The first argument is the entity id.
emberOgre.doWithEntity = function(this, entityId, doIfEntityFound, doIfEntityNotFound)
	if entityId then
		local entity = this:getWorld():getEmberEntity(entityId)
		if entity then
			doIfEntityFound(entity)
		else 
			if doIfEntityNotFound then
				doIfEntityNotFound(entityId)
			end
		end
	end
end


--The GUIManager handles higher level gui actions. It doesn't know anything about the lua or CEGUI world, but has some useful events, for example EventFrameStarted which is emitted every frame.
--Also, when you create a new widget you have to use the createWidget() method.
guiManager = Ember.OgreView.GUIManager:getSingleton()

--The root GUI sheet under which all other CEGUI windows reside.
root = guiManager:getMainSheet()

--All of Ember's services can be accessed from here.
emberServices = Ember.EmberServices:getSingleton()

--Scripting is handled by a service. This will allow you to load other scripts and perform more advanced scripting functions. See also the method loadScript(...) defined below.
scriptingService = emberServices:getScriptingService()

--The console can be used for debug output.
console = Ember.ConsoleBackend:getSingleton()

--Global list of connectors, mainly used for the bootrapping of widgets 
connectors = {}

--loads a lua script
function loadScript(scriptname)
	--load all lua files
	--they all reside in the same directory
	local dirPrefix = "lua/"
	
	scriptingService:loadScript(dirPrefix .. scriptname)

end

--simple debug method, prints the string representation of the object to the console
function debugObject(object)
	console:pushMessage(tostring(object), "info")
end

--creates a connection between the supplied event and a function, stores the connection object in the supplied table and returns it
function connect(connectorTable, event, functionName, selfRef)
	local connector = createConnector(event):connect(functionName, selfRef)
	if connectorTable then
		table.insert(connectorTable, connector)
	end
	return connector
end

--Iterates through all entries in a table of connectors (for example used in the connect(...) function) and disconnects them
function disconnectAll(connectorTable)
	for key,value in pairs(connectorTable) do
		value:disconnect()
	end
end

--A method for safely deleting C++ instances. This removes the need to have to check for nil all the time.
--@param objectToDelete The instance to delete. This can be null
--@returns True if a valid object was submitted, else false.
function deleteSafe(objectToDelete)
	if objectToDelete then
		objectToDelete:delete()
		return true
	end
	return false
end

--CEGUI uses [tag=something] as a formatting syntax, we have to make sure special character [ gets escaped out
function escapeForCEGUI(message)
	--Only the starting [ character needs to be escaped for CEGUI, escaping ] will cause CEGUI to show \]
	--Note that we need to put the result of gsub into a local variable, because we want to throw away the second return value (which is a number).
	local escapedMessage = string.gsub(message, "%[", "\\%[") 
	return escapedMessage
end

--Iterates over a table alphabetically
function pairsByKeys (t, f)
	local a = {}
	for n in pairs(t) do table.insert(a, n) end
	table.sort(a, f)
	local i = 0      -- iterator variable
	local iter = function ()   -- iterator function
		i = i + 1
		if a[i] == nil then return nil
		else return a[i], t[a[i]]
		end
	end
	return iter
end


-- Helper math library
loadScript("Math.lua")

--Load all lua files.
--They all reside in the same directory.
--If you want another window to also load at start up, add it to the list here.

loadScript("MainIconBar.lua")
loadScript("Console.lua")
loadScript("Server.lua")
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
loadScript("Status.lua")
loadScript("Environment.lua")
loadScript("AssetsManager.lua")
loadScript("Inventory.lua")
loadScript("QuickHelp.lua")
loadScript("ActionBar.lua")
loadScript("ActionBarCreator.lua")
loadScript("PickingInfo.lua")
loadScript("Inspect.lua")
loadScript("Compass.lua")
loadScript("EntityCreator.lua")
loadScript("ServerLogger.lua")
loadScript("TypeManager.lua")
loadScript("TerrainEditor.lua")
loadScript("IngameMerchantWidget.lua")
loadScript("Settings.lua")
loadScript("HoverEntityOverlay.lua")
