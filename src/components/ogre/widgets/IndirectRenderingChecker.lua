-----------------------------------------

--Check if we're using indirect rendering, and if so show a little warning about this.

-----------------------------------------
IndirectRenderingChecker = {}

local ogreInfo = EmberOgre.OgreInfo:new_local()

if ogreInfo:isIndirect() then
	IndirectRenderingChecker.widget = guiManager:createWidget()
	IndirectRenderingChecker.widget:loadMainSheet("IndirectRenderingChecker.layout", "IndirectRenderingChecker/")
	
	IndirectRenderingChecker.widget:getWindow("OkButton"):subscribeEvent("MouseClick", "IndirectRenderingChecker.OkButton_Click")

end

function IndirectRenderingChecker.OkButton_Click(args)
	IndirectRenderingChecker.widget:hide()
end
