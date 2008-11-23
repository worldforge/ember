Compass = {
connectors={},
map = nil,
widget = nil,
renderImage = nil,
helper = nil,
previousPosX = 0,
previousPosY = 0,
updateNextFrame = false,
zoomInButton = nil
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
	local pos = emberOgre:getAvatar():getAvatarSceneNode():getPosition()
-- 	local pos = emberOgre:getAvatar():getAvatarEmberEntity():getSceneNode():getPosition()
-- 	if pos.x ~= previousPosX or pos.z ~= previousPosY then
		Compass.helper:reposition(pos.x, pos.z)
-- 		Compass.renderImage:requestRedraw()
-- 		previousPosX = pos.x
-- 		previousPosY = pos.z
-- 	end
-- 	console:pushMessage("x: " .. pos.x .. "y: " .. pos.z)
end

function Compass.framestarted(frameEvent)
	if Compass.updateNextFrame then
		Compass.helper:getMap():render()
		Compass.helper:refresh()
		Compass.updateNextFrame = false
	end
end

function Compass.TerrainPageGeometryUpdated(page)
	Compass.updateNextFrame = true;
--[[	Compass.helper:getMap():render()
	Compass.helper:refresh()]]
end

function Compass.CreatedAvatarEntity(avatarEntity)
	Compass.anchor = EmberOgre.Gui.CompassThirdPersonCameraAnchor:new_local(Compass.helper, emberOgre:getMainCamera():getCamera(), emberOgre:getMainCamera():getRootNode())
-- 	connect(Compass.connectors, guiManager.EventFrameStarted, "Compass.framestarted")
	if Compass.widget ~= nil then
		Compass.widget:show()
		
--[[		Compass.zoomInButton = EmberOgre.Gui.IconBase:new("compass_zoom", MainIconBar.images.background, EmberOgre.Gui.IconBase:loadImageFromImageset("iconset_standard", "close2"), MainIconBar.images.borderinactive, MainIconBar.images.borderactive, CEGUI.UVector2(CEGUI.UDim(0, 24), CEGUI.UDim(0, 24)))
		
		Compass.widget:getMainWindow():addChildWindow(Compass.zoomInButton:getContainer())]]
		
	end
	
	connect(Compass.connectors, emberOgre:getTerrainGenerator().EventTerrainPageGeometryUpdated, "Compass.TerrainPageGeometryUpdated")
	connect(Compass.connectors, guiManager.EventFrameStarted, "Compass.framestarted")
	

end

function Compass.buildWidget()
-- 	Compass.helperImpl = EmberOgre.Gui.OverlayCompassImpl:new_local()
--	Compass.helperImpl = EmberOgre.Gui.CEGUICompassImpl:new_local()
	Compass.helperImpl = EmberOgre.Gui.RenderedCompassImpl:new_local()

	Compass.helper = EmberOgre.Gui.Compass:new_local(Compass.helperImpl)
	Compass.map = Compass.helper:getMap()
	
	
-- -- 	local assetManager = EmberOgre.Gui.AssetsManager:new_local()
-- -- 	
-- -- 	local texturePair = assetManager:createTextureImage(Compass.map:getTexture(), "CompassMap")
-- -- 	if texturePair:hasData() then 
-- -- 		Compass.renderImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
-- -- 	end
	Compass.buildCEGUIWidget()
	
	connect(Compass.connectors, emberOgre.EventCreatedAvatarEntity, "Compass.CreatedAvatarEntity")

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
	local texturePair = assetManager:createTextureImage(Compass.helperImpl:getTexture(), "CompassMap")
	if texturePair:hasData() then 
		Compass.renderImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
	end
	
	--also set up the pointer image
	texturePair = assetManager:createTextureImage(Compass.helperImpl:getPointerTexture(), "CompassPointer")
	if texturePair:hasData() then 
		Compass.pointerImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
	end
	
	Compass.widget:hide()
end



Compass.buildWidget()