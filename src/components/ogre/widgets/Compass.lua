Compass = {
connectors={},
map = nil,
widget = nil,
renderImage = nil,
helper = nil,
previousPosX = 0,
previousPosY = 0
}

function Compass.Refresh_Clicked(args)
	Compass.repositionAtAvatar()
	Compass.helper:getMap():render()
end


function Compass.repositionAtAvatar()
	local pos = emberOgre:getAvatar():getAvatarSceneNode():getPosition()
-- 	local pos = emberOgre:getAvatar():getAvatarEmberEntity():getSceneNode():getPosition()
	if pos.x ~= previousPosX or pos.z ~= previousPosY then
		Compass.helper:reposition(pos.x, pos.z)
-- 		Compass.renderImage:requestRedraw()
		previousPosX = pos.x
		previousPosY = pos.z
	end
-- 	console:pushMessage("x: " .. pos.x .. "y: " .. pos.z)
end

function Compass.rotate()
	local cameraOrientation = emberOgre:getMainCamera():getOrientation():getYaw()
	Compass.helper:rotate(-cameraOrientation)
end

function Compass.framestarted(timeSinceLastFrame)
	Compass.repositionAtAvatar()
	Compass.rotate()
end

function Compass.CreatedAvatarEntity(avatarEntity)
	Compass.anchor = EmberOgre.Gui.CompassCameraAnchor:new_local(Compass.helper, emberOgre:getMainCamera():getCamera())
-- 	connect(Compass.connectors, guiManager.EventFrameStarted, "Compass.framestarted")
end

function Compass.buildWidget()
-- 	Compass.helperImpl = EmberOgre.Gui.OverlayCompassImpl:new_local()
--	Compass.helperImpl = EmberOgre.Gui.CEGUICompassImpl:new_local()
	Compass.helperImpl = EmberOgre.Gui.CompositorCompassImpl:new_local()

	Compass.helper = EmberOgre.Gui.Compass:new_local(Compass.helperImpl)
	Compass.map = Compass.helper:getMap()
	
	
-- -- 	local assetManager = EmberOgre.Gui.AssetsManager:new_local()
-- -- 	
-- -- 	local texturePair = assetManager:createTextureImage(Compass.map:getTexture(), "CompassMap")
-- -- 	if texturePair:hasData() then 
-- -- 		Compass.renderImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
-- -- 	end

	connect(Compass.connectors, emberOgre.EventCreatedAvatarEntity, "Compass.CreatedAvatarEntity")

end

-- Call this method to build the cegui widget.
function Compass.buildCEGUIWidget()
	Compass.widget = guiManager:createWidget()
	Compass.widget:loadMainSheet("Compass.layout", "Compass/")
	Compass.widget:setIsActiveWindowOpaque(false)
	Compass.renderImage = Compass.widget:getWindow("RenderImage")
	
-- 	local assetManager = EmberOgre.Gui.AssetsManager:new_local()
-- 	
-- 	local texturePair = assetManager:createTextureImage(Compass.map:getTexture(), "CompassMap")
-- 	if texturePair:hasData() then 
-- 		Compass.renderImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
-- 	end
	
	
-- 	Compass.renderImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(Compass.helperImpl:getViewImage()))
end



Compass.buildWidget()