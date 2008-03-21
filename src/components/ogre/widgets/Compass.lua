Compass = {
connectors={},
map = nil,
widget = nil,
renderImage = nil,
helper = nil
}

function Compass.Refresh_Clicked(args)
	local pos = emberOgre:getAvatar():getAvatarSceneNode():getPosition()
	Compass.helper:reposition(pos.x, pos.z)
-- 	Compass.map:render()
end

function Compass.buildWidget()
	Compass.helper = EmberOgre.Gui.Compass:new_local()
	Compass.map = Compass.helper:getMap()
	
	Compass.widget = guiManager:createWidget()
	Compass.widget:loadMainSheet("Compass.layout", "Compass/")
	Compass.widget:setIsActiveWindowOpaque(false)
	Compass.renderImage = Compass.widget:getWindow("RenderImage")
	
	Compass.renderImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(Compass.helper:getViewImage()))
	
-- 	local assetManager = EmberOgre.Gui.AssetsManager:new_local()
-- 	
-- 	local texturePair = assetManager:createTextureImage(Compass.map:getTexture(), "CompassMap")
-- 	if texturePair:hasData() then 
-- 		Compass.renderImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
-- 	end

end

Compass.buildWidget()