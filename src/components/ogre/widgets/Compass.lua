Compass = {
connectors={},
map = nil,
widget = nil,
renderImage = nil
}

function Compass.Refresh_Clicked(args)
	local pos = emberOgre:getAvatar():getAvatarSceneNode():getPosition()
	Compass.map:reposition(pos.x, pos.z)
	Compass.map:render()
end

function Compass.buildWidget()
	Compass.map = EmberOgre.Terrain.Map:new_local()
	Compass.map:initialize()
	
	Compass.widget = guiManager:createWidget()
	Compass.widget:loadMainSheet("Compass.layout", "Compass/")
	Compass.widget:setIsActiveWindowOpaque(false)
	Compass.renderImage = Compass.widget:getWindow("RenderImage")
	
	local assetManager = EmberOgre.Gui.AssetsManager:new_local()
	
	local texturePair = assetManager:createTextureImage(Compass.map:getTexture(), "CompassMap")
	if texturePair:hasData() then 
		Compass.renderImage:setProperty("Image", CEGUI.PropertyHelper:imageToString(texturePair:getTextureImage()))
	end

end

Compass.buildWidget()