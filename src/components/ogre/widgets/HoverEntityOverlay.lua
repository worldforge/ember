
HoverEntityOverlay = {}

function HoverEntityOverlay:buildWidget(world)

	self.world = world
	self.widget = guiManager:createWidget()
	self.widget:hide()

	local entityPickListener = world:getEntityPickListener()
 	connect(self.connectors, entityPickListener.EventPickedEntity, self.pickedEntity, self)
    
	self.widget:loadMainSheet("HoverEntityOverlay.layout", "HoverEntityOverlay/")
	self.mainView = self.widget:getWindow("MainWindow")
	self.entityName = self.widget:getWindow("EntityName")

	connect(self.connectors, Ember.Input:getSingleton().EventMouseMoved, self.input_MouseMoved, self)
	connect(self.connectors, Ember.Input:getSingleton().EventMouseButtonPressed, self.input_MouseButtonReleased, self)

end

--called when an entity has been picked
function HoverEntityOverlay:pickedEntity(results, args)

	if args.pickType == Ember.OgreView.MPT_HOVER then
		local localPosition = CEGUI.Vector2:new_local(args.windowX, args.windowY)
		local entity = results[0].entity
		self.widget:show()
		self.overlayShown = true
		
		local name
		--if the entity has a name, use it, else use the type name
		--perhaps we should prefix the type name with an "a" or "an"?
		if entity:getName() ~= "" then
			if entity:getName() ~= entity:getType():getName() then
				name = entity:getName() .. " (a " .. entity:getType():getName() .. ")"
			else
				name = entity:getName()
			end
		else
			name = entity:getType():getName()
		end	
		self.entityName:setText(name)
		
		
		localPosition.x = localPosition.x - self.widget:getMainWindow():getWidth():asAbsolute(0) * 0.5
		localPosition.y = localPosition.y - self.widget:getMainWindow():getHeight():asAbsolute(0) - 5
		
		--Make sure the menu is fully contained within the main window
		if localPosition.x < 0 then
			localPosition.x = 0
		end
		if localPosition.y < 0 then
			localPosition.y = 0
		end
	
--[[		local mainWindowSize = root:getPixelSize()
		if localPosition.x + width > mainWindowSize.width then
			localPosition.x = mainWindowSize.width - width
		end
		if localPosition.y + height > mainWindowSize.height then
			localPosition.y = mainWindowSize.height - height
		end
	--]]	
		
		local uPosition = CEGUI.UVector2:new_local(CEGUI.UDim(0,localPosition.x), CEGUI.UDim(0,localPosition.y))
		self.widget:getMainWindow():setPosition(uPosition )
		
	end
end


function HoverEntityOverlay:input_MouseMoved()
	if self.overlayShown then
		self.mainView:fireEvent("StartHideTransition", CEGUI.WindowEventArgs:new_local(self.mainView))
		self.overlayShown = false
	end
end

function HoverEntityOverlay:input_MouseButtonReleased(button,  mode)
	if self.overlayShown then
		self.mainView:fireEvent("StartHideTransition", CEGUI.WindowEventArgs:new_local(self.mainView))
		self.overlayShown = false
	end
end

function HoverEntityOverlay:shutdown()
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

connect(connectors, emberOgre.EventWorldCreated, function(world)
	createConnector(world.EventGotAvatar):connect(function()
		hoverEntityOverlay = {connectors={}, overlayShown=false}
		setmetatable(hoverEntityOverlay, {__index = HoverEntityOverlay})
		
		hoverEntityOverlay:buildWidget(world)
		connect(hoverEntityOverlay.connectors, emberOgre.EventWorldDestroyed, function()
				hoverEntityOverlay:shutdown()
				hoverEntityOverlay = nil
			end
		)
	end
	)
end
)

