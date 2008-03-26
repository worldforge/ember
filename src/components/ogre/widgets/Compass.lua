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
	Compass.helper:rotate(emberOgre:getMainCamera():getOrientation():getYaw())
end

function Compass.framestarted(timeSinceLastFrame)
	Compass.repositionAtAvatar()
	Compass.rotate()
end

function Compass.CreatedAvatarEntity(avatarEntity)
	connect(Compass.connectors, guiManager.EventFrameStarted, "Compass.framestarted")
end

function Compass.buildWidget()
	Compass.helperImpl = EmberOgre.Gui.OverlayCompassImpl:new_local()
	Compass.helper = EmberOgre.Gui.Compass:new_local(Compass.helperImpl)
	Compass.map = Compass.helper:getMap()
	
--[[	Compass.widget = guiManager:createWidget()
	Compass.widget:loadMainSheet("Compass.layout", "Compass/")
	Compass.widget:setIsActiveWindowOpaque(false)
	Compass.renderImage = Compass.widget:getWindow("RenderImage")
	
	Compass.renderImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(Compass.helper:getViewImage()))]]
	
-- 	local assetManager = EmberOgre.Gui.AssetsManager:new_local()
-- 	
-- 	local texturePair = assetManager:createTextureImage(Compass.map:getTexture(), "CompassMap")
-- 	if texturePair:hasData() then 
-- 		Compass.renderImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
-- 	end

	connect(Compass.connectors, emberOgre.EventCreatedAvatarEntity, "Compass.CreatedAvatarEntity")

end

Compass.buildWidget()