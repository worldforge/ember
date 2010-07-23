Compass = {}

function Compass:Refresh_Clicked(args)
	self.helper:refresh()
	self.helper:getMap():render()
	return true
end

function Compass:ZoomIn_Clicked(args)
	local newResolution = self.helper:getMap():getResolution() - 0.2
	--prevent the user from zooming in to much (at which point only one pixel from the head of the avatar will be seen
	if newResolution > 0.2 then
		self.helper:getMap():setResolution(newResolution)
		self.helper:getMap():render()
		self.helper:refresh()
	end
	return true
end
function Compass:ZoomOut_Clicked(args)
	local newResolution = self.helper:getMap():getResolution() + 0.2
	--we'll use the arbitrary resolution of 5 as the max
	if newResolution < 5 then
		self.helper:getMap():setResolution(self.helper:getMap():getResolution() + 0.2)
		self.helper:getMap():render()
		self.helper:refresh()
	end
	return true
end



function Compass:repositionAtAvatar()
	local pos = emberOgre:getAvatar():getClientSideAvatarPosition()
	self.helper:reposition(pos.x(), -pos.y())
end

function Compass:framestarted(frameEvent)
	if self.updateFrameCountDown > 0 then
		self.updateFrameCountDown = self.updateFrameCountDown - 1
		if self.updateFrameCountDown == 0 then
			--if we haven't created any anchor yet, it means that the whole compass is uninitialized and needs to be shown, else we can just rerender the map
			if self.anchor == nil then
				self:initialize()
			else
				self.helper:getMap():render()
				self.helper:refresh()
			end
			self.updateFrameCountDown = -1
		end
	end
end

function Compass:TerrainPageGeometryUpdated(page)
	--wait six frames until we rerender the map. This is a hack because apparently the event this listens for doesn't actually guarantee that the page will be rendered next frame. We need to add another event which is emitted when a page actually is rendered the first time.
	self.updateFrameCountDown = 6
end

function Compass:initialize()
	self.anchor = EmberOgre.Gui.CompassCameraAnchor:new(self.helper, emberOgre:getMainOgreCamera())
	if self.widget ~= nil then
		self.widget:show()
	end
end

function Compass:CreatedAvatarEntity(avatarEntity)
	connect(self.connectors, guiManager.EventFrameStarted, self.framestarted, self)
end

function Compass:shutdown()
	guiManager:destroyWidget(self.widget)
	deleteSafe(self.helper)
	deleteSafe(self.helperImpl)
	deleteSafe(self.anchor)
	disconnectAll(self.connectors)
end

function Compass.buildWidget(terrainManager)
	compass = {
		connectors={},
		map = nil,
		widget = nil,
		renderImage = nil,
		helper = nil,
		previousPosX = 0,
		previousPosY = 0,
		updateFrameCountDown = -1, --this is used for triggering delayed render updates. If it's more than zero, it's decreased each frame until it's zero, and a render is then carried out. If it's below zero nothing is done.
		zoomInButton = nil,
		anchor = nil
	}
	setmetatable(compass, {__index = Compass})
	

	compass.helperImpl = EmberOgre.Gui.RenderedCompassImpl:new()

	compass.helper = EmberOgre.Gui.Compass:new(compass.helperImpl)
	compass.map = compass.helper:getMap()
	
	compass:buildCEGUIWidget()
	
	--don't show the compass here, instead wait until we've gotten some terrain (by listening 
	connect(compass.connectors, emberOgre.EventCreatedAvatarEntity, compass.CreatedAvatarEntity, compass)
	connect(compass.connectors, terrainManager.EventTerrainPageGeometryUpdated, compass.TerrainPageGeometryUpdated, compass)
	
	connect(compass.connectors, emberOgre.EventTerrainManagerDestroyed, compass.shutdown, compass)

end

-- Call this method to build the cegui widget.
function Compass:buildCEGUIWidget()
	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("Compass.layout", "Compass/")
	self.widget:setIsActiveWindowOpaque(false)
	self.renderImage = self.widget:getWindow("RenderImage")
	self.pointerImage = self.widget:getWindow("Pointer")
	
	local assetManager = EmberOgre.Gui.AssetsManager:new_local()
	
	--set up the main background image
	if self.helperImpl:getTexture():isNull() == false then
		local texturePair = assetManager:createTextureImage(self.helperImpl:getTexture(), "CompassMap")
		if texturePair:hasData() then 
			self.renderImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
		end
	end
		
	if self.helperImpl:getPointerTexture():isNull() == false then
		--also set up the pointer image
		texturePair = assetManager:createTextureImage(self.helperImpl:getPointerTexture(), "CompassPointer")
		if texturePair:hasData() then 
			self.pointerImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
		end
	end
	
	self.widget:getWindow("ZoomOut"):subscribeEvent("Clicked", self.ZoomOut_Clicked, self) 
	self.widget:getWindow("ZoomIn"):subscribeEvent("Clicked", self.ZoomIn_Clicked, self)
	
	self.widget:hide()
end

connect(connectors, emberOgre.EventTerrainManagerCreated, Compass.buildWidget)
