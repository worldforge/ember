Compass = {}

--function Compass:Refresh_Clicked(args)
--	self.helper:refresh()
--	self.helper:getMap():render()
--	return true
--end
--
--function Compass:repositionAtAvatar()
--	local pos = emberOgre:getWorld():getAvatar():getClientSideAvatarPosition()
--	self.helper:reposition(pos.x(), -pos.y())
--end


function Compass:buildWidget(terrainManager)
	self.widget:loadMainSheet("Compass.layout", "Compass")
	self.widget:setIsActiveWindowOpaque(false)
	self.renderImage = self.widget:getWindow("RenderImage")
	self.pointerImage = self.widget:getWindow("Pointer")

	self.helperImpl = Ember.OgreView.Gui.RenderedCompassImpl.new(self.pointerImage)

	self.helper = Ember.OgreView.Gui.Compass.new(self.helperImpl, terrainManager:getScene():getSceneManager(), terrainManager:getTerrainAdapter())
	self.map = self.helper:getMap()

	local assetManager = Ember.OgreView.Gui.AssetsManager.new()

	--set up the main background image
	if self.helperImpl:getTexture() then
		local texturePair = assetManager:createTextureImage(self.helperImpl:getTexture(), "CompassMap")
		if texturePair:hasData() then
			self.renderImage:setProperty("Image", CEGUI.PropertyHelper.imageToString(texturePair.textureImage))
		end
	end

	subscribe(self.connectors, self.widget:getWindow("ZoomOut"), "Clicked", function()
		local newResolution = self.helper:getMap():getResolution() + 0.2
		--we'll use the arbitrary resolution of 5 as the max
		if newResolution < 5 then
			self.helper:getMap():setResolution(self.helper:getMap():getResolution() + 0.2)
			self.helper:getMap():render()
			self.helper:refresh()
		end
		return true
	end)

	subscribe(self.connectors, self.widget:getWindow("ZoomIn"), "Clicked", function()
		local newResolution = self.helper:getMap():getResolution() - 0.2
		--prevent the user from zooming in to much (at which point only one pixel from the head of the avatar will be seen
		if newResolution > 0.2 then
			self.helper:getMap():setResolution(newResolution)
			self.helper:getMap():render()
			self.helper:refresh()
		end
		return true
	end)

	self.widget:hide()



	--don't show the compass here, instead wait until we've gotten some terrain (by listening 
	connect(self.connectors, emberOgre.EventCreatedAvatarEntity, function()
		connect(self.connectors, self.widget.EventFrameStarted, function()
			if self.updateFrameCountDown > 0 then
				self.updateFrameCountDown = self.updateFrameCountDown - 1
				if self.updateFrameCountDown == 0 then
					--if we haven't created any anchor yet, it means that the whole compass is uninitialized and needs to be shown, else we can just rerender the map
					if self.anchor == nil then
						self.anchor = Ember.OgreView.Gui.CompassCameraAnchor.new(self.helper, emberOgre:getWorld():getMainCamera():getCamera())
						if self.widget then
							self.widget:show()
						end
					else
						self.helper:getMap():render()
						self.helper:refresh()
					end
					self.updateFrameCountDown = -1
				end
			end
		end)
	end)
	connect(self.connectors, terrainManager.EventTerrainPageGeometryUpdated, function()
		--TODO SK: see if this can be done better
		--wait six frames until we rerender the map. This is a hack because apparently the event this listens for doesn't actually guarantee that the page will be rendered next frame. We need to add another event which is emitted when a page actually is rendered the first time.
		self.updateFrameCountDown = 6
	end)


end

Compass.createConnector = emberOgre.EventTerrainManagerCreated:connect(function(terrainManager)
	Compass.instance = {
		connectors = {},
		widget = guiManager:createWidget(),
		previousPosX = 0,
		previousPosY = 0,
		updateFrameCountDown = -1 --this is used for triggering delayed render updates. If it's more than zero, it's decreased each frame until it's zero, and a render is then carried out. If it's below zero nothing is done.
	}
	setmetatable(Compass.instance, { __index = Compass })

	Compass.instance:buildWidget(terrainManager)
	connect(Compass.instance.connectors, emberOgre.EventTerrainManagerBeingDestroyed, function()
		guiManager:destroyWidget(Compass.instance.widget)
		disconnectAll(Compass.instance.connectors)
		Compass.instance = nil
		-- We need to destroy the compass while we still have the scene manager, else we get into segfaults.
		collectgarbage()
	end)

end)
