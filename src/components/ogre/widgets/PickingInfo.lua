--[[ PickingInfo
A simple widget for showing picking info. It will show both ogre and worldforge positions and the distance to the picking. This is mainly useful for when you need to measure things in the world in order to do server side scripting and similiar stuff.
]]--

connect(connectors, emberOgre.EventWorldCreated, function(world)
	pickingInfo = { connectors = {}, infoBox = nil }

	pickingInfo.widget = guiManager:createWidget()

	connect(pickingInfo.connectors, world:getEntityPickListener().EventPickedEntity, function(result, args)
		if pickingInfo.widget:getMainWindow():isVisible() then
			if result:size() > 0 then
				if args.pickType == Ember.OgreView.MPT_CLICK then

					local firstPickResult = result[1]
					--we must make a copy, else the vector object will be deleted by C++ and we'll end up with garbage
					local ogrePos = Ogre.Vector3.new(firstPickResult.position)
					--we should use a worldforge position, but we haven't yet added lua bindings for wfmath, so we'll just manually convert the ogre values
					-- 		local wfPos = Ember.OgreView.Convert:toWF_Vector3(ogrePos)
					local infoString = string.format([[Entity: %q (id: %q type: %q)
Pos:
  x: %.2f y: %.2f z: %.2f
Distance: %.2f]], firstPickResult.entity:getName(), firstPickResult.entity:getId(), firstPickResult.entity:getType():getName(), ogrePos.x, ogrePos.y, ogrePos.z, firstPickResult.distance)
					pickingInfo.infoBox:setText(infoString)
				end
			end
		end
	end)

	pickingInfo.widget:loadMainSheet("PickingInfo.layout", "PickingInfo")

	pickingInfo.infoBox = pickingInfo.widget:getWindow("InfoBox")
	pickingInfo.widget:registerConsoleVisibilityToggleCommand("pickingInfo")
	pickingInfo.widget:enableCloseButton()
	pickingInfo.widget:hide()

	connect(pickingInfo.connectors, emberOgre.EventWorldDestroyed, function()
		disconnectAll(pickingInfo.connectors)
		guiManager:destroyWidget(pickingInfo.widget)
		pickingInfo = nil
	end
	)
end
)

