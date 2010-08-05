--Shows a simple help window when moving an entity.

Mover = {}

function Mover:updateEntityText(entity)
	--if the entity has a name, use it, else use the type name
	--perhaps we should prefix the type name with an "a" or "an"?
	name = ""
	if entity:getName() ~= "" then
		name = entity:getName()
	else
		name = entity:getType():getName()
	end	

	self.widget:getWindow("EntityText"):setText("Moving " .. name .. ". Press left mouse button to place, escape to cancel. Pressing and holding shift decreases movement speed. Mouse wheel rotates.")
end

function Mover:StartMoving(entity, mover)
	self.widget:show()
	self:updateEntityText(entity)
end 

function Mover:FinishedMoving()
	self.widget:hide()
end 

function Mover:CancelledMoving()
	self.widget:hide()
end 

function Mover:buildWidget(world)

	self.widget = guiManager:createWidget()
	self.widget:loadMainSheet("Mover.layout", "Mover/")
	
	local moveManager = world:getMoveManager()
	
	connect(self.connectors, moveManager.EventStartMoving, self.StartMoving, self)
	connect(self.connectors, moveManager.EventFinishedMoving, self.FinishedMoving, self)
	connect(self.connectors, moveManager.EventCancelledMoving, self.CancelledMoving, self)
	

	self.widget:hide()

end

function Mover:shutdown()
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

connect(connectors, emberOgre.EventWorldCreated, function(world) 
	mover = {connectors={}}
	setmetatable(mover, {__index = Mover})

	mover:buildWidget(world)
	connect(mover.connectors, emberOgre.EventWorldDestroyed, function()
			mover:shutdown()
			mover = nil
		end
	)	
end
)
