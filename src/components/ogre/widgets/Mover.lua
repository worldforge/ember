--Shows a simple help window when moving an entity.

Mover = {connectors={}}

function Mover.updateEntityText(entity)
	--if the entity has a name, use it, else use the type name
	--perhaps we should prefix the type name with an "a" or "an"?
	name = ""
	if entity:getName() ~= "" then
		name = entity:getName()
	else
		name = entity:getType():getName()
	end	

	Mover.widget:getWindow("EntityText"):setText("Moving " .. name .. ". Press left mouse button to place, escape to cancel. Pressing and holding shift decreases movement speed. Mouse wheel rotates.")
end

function Mover.StartMoving(entity, mover)
	Mover.widget:show()
	Mover.updateEntityText(entity)
end 

function Mover.FinishedMoving()
	Mover.widget:hide()
end 

function Mover.CancelledMoving()
	Mover.widget:hide()
end 

function Mover.buildWidget()

	Mover.widget = guiManager:createWidget()
	Mover.widget:loadMainSheet("Mover.layout", "Mover/")
	
	local moveManager = emberOgre:getWorld():getMoveManager()
	
	connect(Mover.connectors, moveManager.EventStartMoving, Mover.StartMoving)
	connect(Mover.connectors, moveManager.EventFinishedMoving, Mover.FinishedMoving)
	connect(Mover.connectors, moveManager.EventCancelledMoving, Mover.CancelledMoving)
	

	Mover.widget:hide()

end

connect(connectors, emberOgre.EventWorldCreated, Mover.buildWidget)
