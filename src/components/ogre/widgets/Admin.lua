--[[
When the user is logged in as admin we want to show a special admin menu which contains options for showing various widgets and tools specially suited for admins.
We'll use the existing icon bar and add a new icon to it, so the MainIconBar.lua script must be loaded.
The creation of the amdmin menu is delayed until we've actually logged in as an admin; if not it will never be created.
]]--

Admin = {connectors={}, adminIcon=nil}

--adds a standard menu item to the main popup menu.
function Admin.addMenuItem(labelText, clickMethod, tooltipText)
	local menuItem = windowManager:createWindow("EmberLook/MenuItem")
	menuItem:setText(labelText)
	menuItem:subscribeEvent("MouseClick", clickMethod)
	menuItem:setTooltipText(tooltipText)
	Admin.popup:addChildWindow(menuItem)
	return menuItem
end

--[[
Add an icon to the icon bar and build a popup menu which will be shown when the icon is clicked
]]--
function Admin.buildWidget()

	if MainIconBar.addIcon ~= nil then
		--we'll use the "spell" icon from the "iconset_mason" for now until we get a better icon
		local foreground = EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_mason", "spell")
		Admin.adminIcon = MainIconBar.addIcon("admin", foreground, "Click here to access the admin menu.")
		Admin.adminIcon:getButton():subscribeEvent("MouseClick", "Admin.admin_MouseClick")
	
		Admin.popup = windowManager:createWindow("EmberLook/PopupMenu")
		Admin.popup = CEGUI.toPopupMenu(Admin.popup)
		Admin.popup:subscribeEvent("MouseLeave", "Admin.popupMenu_MouseLeaves")
		
		Admin.addMenuItem("Model editor", "Admin.ModelEditor_Click", "Shows the model editor.")
		Admin.addMenuItem("Terrain editor", "Admin.TerrainEditor_Click", "Shows the terrain editor.")
		Admin.addMenuItem("Switch camera", "Admin.SwitchCamera_Click", "Switches camera mode between free flying and attached to the avatar.")
		Admin.addMenuItem("Entity creator", "Admin.EntityCreator_Click", "Shows the entity creator.")
		Admin.addMenuItem("Script editor", "Admin.ScriptEditor_Click", "Shows the script editor.")
		Admin.addMenuItem("Entity browser", "Admin.EntityBrowser_Click", "Shows the entity browser.")
		Admin.addMenuItem("Assets manager", "Admin.AssetsManager_Click", "Shows the assets manager.")
		Admin.addMenuItem("Picking info", "Admin.PickingInfo_Click", "Shows mouse picking info.")
		
	
		root:addChildWindow(Admin.popup)
	end 
end

--hide the menu when the mouse leaves it
function Admin.popupMenu_MouseLeaves(args)
	--only hide if we're not moved to another menu item
	local mousePosition = CEGUI.MouseCursor:getSingleton():getPosition()
	local windowUnderTheCursor = Admin.popup:getTargetChildAtPosition(mousePosition)
	if windowUnderTheCursor == nil then
		Admin.popup:closePopupMenu()
	end
	return true
end

--[[
Show the admin menu at the mouse position
]]--
function Admin.admin_MouseClick(args)
	Admin.popup:openPopupMenu()
	
--[[	local adminContainer = Admin.adminIcon:getContainer()
	local x = adminContainer:getXPosition():asAbsolute(0) + (adminContainer:getWidth():asAbsolute(0) * 0.5)
	local y = adminContainer:getYPosition():asAbsolute(0) - Admin.popup.getHeight():asAbsolute(0);]]
	
	local mousePosition = CEGUI.MouseCursor:getSingleton():getPosition()
	local x = mousePosition.x - Admin.popup:getWidth():asAbsolute(0) * 0.5
	local y = mousePosition.y - Admin.popup:getHeight():asAbsolute(0) + 5
	
	local uPosition = CEGUI.UVector2:new_local(CEGUI.UDim(0,x), CEGUI.UDim(0,y))
	
	Admin.popup:setPosition(uPosition)
	Admin.popup:moveToFront()
	return true
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

function Admin.EntityBrowser_Click(args)
	console:runCommand("/show_entityBrowser")
end

function Admin.AssetsManager_Click(args)
	console:runCommand("/show_assetsManager")
end

function Admin.PickingInfo_Click(args)
	console:runCommand("/show_pickingInfo")
end

--only show the admin menu if we're logged in as an admin
function Admin.createdAvatarEmberEntity(avatarEntity)
	if avatarEntity:getAvatar():isAdmin() then
		Admin.buildWidget()
	end
end

--listen for when we've gotten an avatar entity, and see if we're admin
connect(Admin.connectors, emberOgre.EventCreatedAvatarEntity, "Admin.createdAvatarEmberEntity")
