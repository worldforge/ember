Compass = {
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

function Compass.Refresh_Clicked(args)
	Compass.helper:refresh()
	Compass.helper:getMap():render()
	return true
end

function Compass.ZoomIn_Clicked(args)
	local newResolution = Compass.helper:getMap():getResolution() - 0.2
	--prevent the user from zooming in to much (at which point only one pixel from the head of the avatar will be seen
	if newResolution > 0.2 then
		Compass.helper:getMap():setResolution(newResolution)
		Compass.helper:getMap():render()
		Compass.helper:refresh()
	end
	return true
end
function Compass.ZoomOut_Clicked(args)
	local newResolution = Compass.helper:getMap():getResolution() + 0.2
	--we'll use the arbitrary resolution of 5 as the max
	if newResolution < 5 then
		Compass.helper:getMap():setResolution(Compass.helper:getMap():getResolution() + 0.2)
		Compass.helper:getMap():render()
		Compass.helper:refresh()
	end
	return true
end



function Compass.repositionAtAvatar()
	local pos = emberOgre:getAvatar():getClientSideAvatarPosition()
	Compass.helper:reposition(pos.x(), -pos.y())
end

function Compass.framestarted(frameEvent)
	if Compass.updateFrameCountDown > 0 then
		Compass.updateFrameCountDown = Compass.updateFrameCountDown - 1
		if Compass.updateFrameCountDown == 0 then
			--if we haven't created any anchor yet, it means that the whole compass is uninitialized and needs to be shown, else we can just rerender the map
			if Compass.anchor == nil then
				Compass.initialize()
			else
				Compass.helper:getMap():render()
				Compass.helper:refresh()
			end
			Compass.updateFrameCountDown = -1
		end
	end
end

function Compass.TerrainPageGeometryUpdated(page)
	--wait six frames until we rerender the map. This is a hack because apparently the event this listens for doesn't actually guarantee that the page will be rendered next frame. We need to add another event which is emitted when a page actually is rendered the first time.
	Compass.updateFrameCountDown = 6
end

function Compass.initialize()
	Compass.anchor = EmberOgre.Gui.CompassCameraAnchor:new_local(Compass.helper, emberOgre:getMainOgreCamera())
	if Compass.widget ~= nil then
		Compass.widget:show()
	end
end

function Compass.CreatedAvatarEntity(avatarEntity)
	connect(Compass.connectors, guiManager.EventFrameStarted, "Compass.framestarted")
end

function Compass.buildWidget()
	Compass.helperImpl = EmberOgre.Gui.RenderedCompassImpl:new_local()

	Compass.helper = EmberOgre.Gui.Compass:new_local(Compass.helperImpl)
	Compass.map = Compass.helper:getMap()
	
	
	Compass.buildCEGUIWidget()
	
	--don't show the compass here, instead wait until we've gotten some terrain (by listening 
	connect(Compass.connectors, emberOgre.EventCreatedAvatarEntity, "Compass.CreatedAvatarEntity")
	connect(Compass.connectors, emberOgre:getTerrainManager().EventTerrainPageGeometryUpdated, "Compass.TerrainPageGeometryUpdated")

end

-- Call this method to build the cegui widget.
function Compass.buildCEGUIWidget()
	Compass.widget = guiManager:createWidget()
	Compass.widget:loadMainSheet("Compass.layout", "Compass/")
	Compass.widget:setIsActiveWindowOpaque(false)
	Compass.renderImage = Compass.widget:getWindow("RenderImage")
	Compass.pointerImage = Compass.widget:getWindow("Pointer")
	
	local assetManager = EmberOgre.Gui.AssetsManager:new_local()
	
	--set up the main background image
	if Compass.helperImpl:getTexture():isNull() == false then
		local texturePair = assetManager:createTextureImage(Compass.helperImpl:getTexture(), "CompassMap")
		if texturePair:hasData() then 
			Compass.renderImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
		end
	end
		
	if Compass.helperImpl:getPointerTexture():isNull() == false then
		--also set up the pointer image
		texturePair = assetManager:createTextureImage(Compass.helperImpl:getPointerTexture(), "CompassPointer")
		if texturePair:hasData() then 
			Compass.pointerImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
		end
	end
	
	Compass.widget:hide()
end



Compass.buildWidget()