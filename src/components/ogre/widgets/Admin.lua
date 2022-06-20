--[[ Admin
When the user is logged in as admin we want to show a special admin menu which contains options for showing various widgets and tools specially suited for admins.
We'll use the existing icon bar and add a new icon to it, so the MainIconBar.lua script must be loaded.
The creation of the amdmin menu is delayed until we've actually logged in as an admin; if not it will never be created.
]]--

Admin = {}

--adds a standard menu item to the main popup menu.
function Admin:addMenuItem(labelText, clickMethod, tooltipText)
	local menuItem = windowManager:createWindow("EmberLook/MenuItem")
	menuItem:setText(labelText)
	menuItem:subscribeEvent("Clicked", clickMethod, self)
	menuItem:setTooltipText(tooltipText)
	self.popup:addChild(menuItem)
	return menuItem
end

--hide the menu when the mouse leaves it
function Admin:popupMenu_MouseLeaves(args)
	self.popup:closePopupMenu()
	return true
end

--[[
Show the admin menu at the mouse position
]]--
function Admin:admin_MouseClick(args)
	self.popup:openPopupMenu()

	--[[	local adminContainer = self.adminIcon:getContainer()
		local x = adminContainer:getXPosition():asAbsolute(0) + (adminContainer:getWidth():asAbsolute(0) * 0.5)
		local y = adminContainer:getYPosition():asAbsolute(0) - self.popup.getHeight():asAbsolute(0);]]

	local mousePosition = CEGUI.System.getSingleton():getDefaultGUIContext():getMouseCursor():getPosition()
	local x = mousePosition.x - self.popup:getPixelSize().width * 0.5
	local y = mousePosition.y - self.popup:getPixelSize().height + 5

	local uPosition = CEGUI.UVector2.new(CEGUI.UDim.new(0, x), CEGUI.UDim.new(0, y))

	self.popup:setPosition(uPosition)
	self.popup:moveToFront()
	return true
end

function Admin:ModelEditor_Click(args)
	console:runCommand("/show_modelEdit")
	return true
end

function Admin:TerrainEditor_Click(args)
	console:runCommand("/show_terrainEditor")
	return true
end

function Admin:SwitchCamera_Click(args)
	console:runCommand("/toggle_cameraattached")
	return true
end

function Admin:EntityCreator_Click(args)
	console:runCommand("/show_entityCreator")
	return true
end

function Admin:ScriptEditor_Click(args)
	console:runCommand("/show_scriptEdit")
	return true
end

function Admin:EntityBrowser_Click(args)
	console:runCommand("/show_entityBrowser")
	return true
end

function Admin:AssetsManager_Click(args)
	console:runCommand("/show_assetsManager")
	return true
end

function Admin:PickingInfo_Click(args)
	console:runCommand("/show_pickingInfo")
	return true
end

function Admin:Environment_Click(args)
	console:runCommand("/show_environment")
	return true
end

function Admin:NetworkLog_Click(args)
	console:runCommand("/show_serverLogger")
	return true
end

function Admin:VisualizeEntities_Click(args)
	if emberServices:getConfigService():itemExists("authoring", "visualizations") then
		local visualizeVariable = emberServices:getConfigService():getValue("authoring", "visualizations")
		if visualizeVariable ~= nil then
			if visualizeVariable:is_bool() then
				local visualize = visualizeVariable:as_bool()
				local new_value = varconf.Variable.new(not visualize)
				emberServices:getConfigService():setValue("authoring", "visualizations", new_value, varconf.USER)
			end
		end
	end
	return true

end

--[[
Add an icon to the icon bar and build a popup menu which will be shown when the icon is clicked
]]--
function Admin:buildWidget()

	if MainIconBar.addIcon then
		--we'll use the "spell" icon from the "iconset_mason" for now until we get a better icon
		local foreground = Ember.OgreView.Gui.IconBase.loadImageFromImageset("iconset_mason", "spell")
		self.adminIcon = MainIconBar.addIcon("admin", foreground, "Click here to access the admin menu.")
		self.adminIcon:getButton():subscribeEvent("MouseClick", self.admin_MouseClick, self)

		self.popup = CEGUI.toPopupMenu(windowManager:createWindow("EmberLook/PopupMenu"))
		self.popup:subscribeEvent("MouseLeavesArea", self.popupMenu_MouseLeaves, self)

		self:addMenuItem("Assets manager", self.AssetsManager_Click, "Shows the assets manager.")
		self:addMenuItem("Entity creator", self.EntityCreator_Click, "Shows the entity creator.")
		self:addMenuItem("Entity browser", self.EntityBrowser_Click, "Shows the entity browser.")
		self:addMenuItem("Environment", self.Environment_Click, "Shows environment widget.")
		self:addMenuItem("Model editor", self.ModelEditor_Click, "Shows the model editor.")
		self:addMenuItem("Network log", self.NetworkLog_Click, "Shows network log widget.")
		self:addMenuItem("Picking info", self.PickingInfo_Click, "Shows mouse picking info.")
		self:addMenuItem("Rule manager",
				function(args)
					console:runCommand("/show_ruleManager")
					return true
				end,
				"Manage server rules.")
		self:addMenuItem("Script editor", self.ScriptEditor_Click, "Shows the script editor.")
		self:addMenuItem("Switch camera", self.SwitchCamera_Click, "Switches camera mode between free flying and attached to the avatar.")
		self:addMenuItem("Terrain editor", self.TerrainEditor_Click, "Shows the terrain editor.")
		self:addMenuItem("Visualize entities", self.VisualizeEntities_Click, "Visualizes entities.")

		root:addChild(self.popup)
	end
end

function Admin:shutdown()
	if self.popup ~= nil then
		windowManager:destroyWindow(self.popup)
		self.popup = nil
	end
	if self.adminIcon ~= nil then
		MainIconBar.removeIcon(self.adminIcon)
		self.adminIcon = nil
	end
	disconnectAll(self.connectors)
end

--listen for when we've gotten an avatar entity, and see if we're admin
Admin.createdAvatarConnector = emberOgre.EventCreatedAvatarEntity:connect(
		function(avatarEntity)
			--only show the admin menu if we're logged in as an admin
			if emberOgre:getWorld():getAvatar():isAdmin() then
				Admin.instance = { connectors = {}, adminIcon = nil }
				setmetatable(Admin.instance, { __index = Admin })

				Admin.instance:buildWidget()
			end
		end
)

Admin.destroyedConnector = emberServices:getServerService().DestroyedAvatar:connect(function()
	if Admin.instance then
		Admin.instance:shutdown()
		Admin.instance = nil
		collectgarbage()
	end
end
)
