--[[
A simple widget for showing picking info. It will show both ogre and worldforge positions and the distance to the picking. This is mainly useful for when you need to measure things in the world in order to do server side scripting and similiar stuff.
]]--


PickingInfo = {connectors={}, infoBox=nil}


function PickingInfo.buildWidget()

	PickingInfo.widget = guiManager:createWidget()
	
 	connect(PickingInfo.connectors, guiManager:getEntityPickListener().EventPickedEntity, "PickingInfo.pickedEntity")
    
	PickingInfo.widget:loadMainSheet("PickingInfo.layout", "PickingInfo/")

	
	PickingInfo.infoBox = PickingInfo.widget:getWindow("InfoBox")
	PickingInfo.widget:registerConsoleVisibilityToggleCommand("pickingInfo")
	PickingInfo.widget:enableCloseButton()
	PickingInfo.widget:hide()

end


--called when an entity has been picked
function PickingInfo.pickedEntity(result, args)
	--only catch single clicks
	if args.pickType == EmberOgre.MPT_CLICK then
		if PickingInfo.widget:getMainWindow():isVisible() then
			--we must make a copy, else the vector object will be deleted by C++ and we'll end up with garbage
			local ogrePos = Ogre.Vector3:new_local(result.position)
			--we should use a worldforge position, but we haven't yet added lua bindings for wfmath, so we'll just manually convert the ogre values
	-- 		local wfPos = EmberOgre.Convert:toWF_Vector3(ogrePos)
			local infoString = string.format([[Entity: %q (id: %q type: %q)
WF pos:
  x: %.2f y: %.2f z: %.2f
Ogre pos:
  x: %.2f y: %.2f z: %.2f
Distance: %.2f]], result.entity:getName(), result.entity:getId(), result.entity:getType():getName(), ogrePos.x, -ogrePos.z, ogrePos.y, ogrePos.x, ogrePos.y, ogrePos.z, result.distance)
			PickingInfo.infoBox:setText(infoString)
		end
	end
end



PickingInfo.buildWidget()
