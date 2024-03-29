--This script is the first one loaded. It's responsible for setting up utility objects, defining some utility functions and loading all scripts that should be loaded at start up


--set up some commonly used objects, these will be available to all scripts

--The main CEGUI system. This is the main channel for all CEGUI actions.
guiSystem = CEGUI.System.getSingleton()

--The CEGUI WindowManager. Use this if you want to do some fancy window magic. Oftenwhile you don't need to touch this however since most window actions can be handled by the Widget class, which has methods such as loadMainSheet(...) and getWindow(...)
windowManager = CEGUI.WindowManager.getSingleton()

--This is the main ember object. You can usually use any of it's many get*() methods to access other parts of the system.
emberOgre = Ember.OgreView.EmberOgre.getSingleton()


--The GUIManager handles higher level gui actions. It doesn't know anything about the lua or CEGUI world, but has some useful events, for example EventFrameStarted which is emitted every frame.
--Also, when you create a new widget you have to use the createWidget() method.
guiManager = Ember.OgreView.GUIManager.getSingleton()

--The root GUI sheet under which all other CEGUI windows reside.
root = guiManager:getMainSheet()

--All of Ember's services can be accessed from here.
emberServices = Ember.EmberServices.getSingleton()

--Scripting is handled by a service. This will allow you to load other scripts and perform more advanced scripting functions. See also the method loadScript(...) defined below.
scriptingService = emberServices:getScriptingService()

--The console can be used for debug output.
console = Ember.ConsoleBackend.getSingleton()

--Global list of connectors, mainly used for the bootstrapping of widgets
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
	local connector = event:connect(functionName, selfRef)
	if connectorTable then
		table.insert(connectorTable, connector)
	else
		log.warning("No connector table supplied when creating connector.")
		if (debug and debug.traceback) then
			log.warning(debug.traceback())
		end
	end
	return connector
end

--creates a CEGUI connection between the supplied event and a function, stores the connection object in the supplied table and returns it
function subscribe(connectorTable, window, event, callback, self)
	local connector
	if self then
		connector = window:subscribeEvent(event, callback, self)
	else
		connector = window:subscribeEvent(event, callback)
	end
	if connectorTable then
		table.insert(connectorTable, connector)
	else
		log.warning("No connector table supplied when creating subscription.")
		if (debug and debug.traceback) then
			log.warning(debug.traceback())
		end
	end
	return connector
end

--Iterates through all entries in a table of connectors (for example used in the connect(...) function) and disconnects them
function disconnectAll(connectorTable)
	for _, value in pairs(connectorTable) do
		value:disconnect()
	end
end

--A method for safely deleting C++ instances. This removes the need to have to check for nil all the time.
--@param objectToDelete The instance to delete. This can be null
--@returns True if a valid object was submitted, else false.
function deleteSafe(objectToDelete)
	if objectToDelete and objectToDelete.delete then
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
	for n in pairs(t) do
		table.insert(a, n)
	end
	table.sort(a, f)
	local i = 0      -- iterator variable
	local iter = function()
		-- iterator function
		i = i + 1
		if a[i] == nil then
			return nil
		else
			return a[i], t[a[i]]
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
loadScript("ServerBrowser.lua")
loadScript("Performance.lua")
loadScript("EntityPicker.lua")
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
loadScript("Compass.lua")
loadScript("ServerLogger.lua")
loadScript("RuleManager.lua")
loadScript("TerrainEditor.lua")
loadScript("IngameMerchantWidget.lua")
loadScript("Settings.lua")
loadScript("HoverEntityOverlay.lua")



