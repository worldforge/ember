-----------------------------------------

--Check if we're using indirect rendering, and if so show a little warning about this.

-----------------------------------------
IndirectRenderingChecker = {}


if EmberOgre.OgreInfo:isIndirect() then
	IndirectRenderingChecker.widget = guiManager:createWidget()
	IndirectRenderingChecker.widget:loadMainSheet("IndirectRenderingChecker.layout", "IndirectRenderingChecker/")
	IndirectRenderingChecker.widget:show()
	IndirectRenderingChecker.widget:getMainWindow():setAlwaysOnTop(true)
	
	IndirectRenderingChecker.widget:getWindow("OkButton"):subscribeEvent("MouseClick", 
		function()
			IndirectRenderingChecker.widget:hide()
		end)

end
