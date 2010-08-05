--[[
A simple widget for showing picking info. It will show both ogre and worldforge positions and the distance to the picking. This is mainly useful for when you need to measure things in the world in order to do server side scripting and similiar stuff.
]]--


PickingInfo = {}


function PickingInfo:buildWidget(world)

	self.widget = guiManager:createWidget()
	
 	connect(self.connectors, world:getEntityPickListener().EventPickedEntity, self.pickedEntity, self)
    
	self.widget:loadMainSheet("PickingInfo.layout", "PickingInfo/")

	
	self.infoBox = self.widget:getWindow("InfoBox")
	self.widget:registerConsoleVisibilityToggleCommand("pickingInfo")
	self.widget:enableCloseButton()
	self.widget:hide()

end


--called when an entity has been picked
function PickingInfo:pickedEntity(result, args)
	--only catch single clicks
	if args.pickType == EmberOgre.MPT_CLICK then
		if self.widget:getMainWindow():isVisible() then
			if result:size() > 0 then
			
				local firstPickResult = result[0]
				--we must make a copy, else the vector object will be deleted by C++ and we'll end up with garbage
				local ogrePos = Ogre.Vector3:new_local(firstPickResult.position)
				--we should use a worldforge position, but we haven't yet added lua bindings for wfmath, so we'll just manually convert the ogre values
		-- 		local wfPos = EmberOgre.Convert:toWF_Vector3(ogrePos)
				local infoString = string.format([[Entity: %q (id: %q type: %q)
WF pos:
  x: %.2f y: %.2f z: %.2f
Ogre pos:
  x: %.2f y: %.2f z: %.2f
Distance: %.2f]], firstPickResult.entity:getName(), firstPickResult.entity:getId(), firstPickResult.entity:getType():getName(), ogrePos.x, -ogrePos.z, ogrePos.y, ogrePos.x, ogrePos.y, ogrePos.z, firstPickResult.distance)
				self.infoBox:setText(infoString)
			end
		end
	end
end

function PickingInfo:shutdown()
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

connect(connectors, emberOgre.EventWorldCreated, function(world) 
	pickingInfo = {connectors={}, infoBox=nil}
	setmetatable(pickingInfo, {__index = PickingInfo})

	pickingInfo:buildWidget(world)
	connect(pickingInfo.connectors, emberOgre.EventWorldDestroyed, function()
			pickingInfo:shutdown()
			pickingInfo = nil
		end
	)	
end
)

