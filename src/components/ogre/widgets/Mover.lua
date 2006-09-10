--Shows a simple help window when moving an entity.

Mover = {}

function Mover_updateEntityText(entity)
	--if the entity has a name, use it, else use the type name
	--perhaps we should prefix the type name with an "a" or "an"?
	name = ""
	if entity:getName() ~= "" then
		name = entity:getName()
	else
		name = entity:getType():getName()
	end	

	Mover.widget:getWindow("EntityText"):setText("Moving " .. name .. ". Press left mouse button to place, escape to cancel.")
end

function Mover_StartMoving(entity)
	Mover.widget:show()
	Mover_updateEntityText(entity)
end 

function Mover_FinishedMoving(entity)
	Mover.widget:hide()
end 

function Mover_CancelledMoving(entity)
	Mover.widget:hide()
end 

function Mover_buildWidget()

	Mover.widget = guiManager:createWidget()
	Mover.widget:loadMainSheet("Mover.layout", "Mover/")
	
	EmberOgre.LuaConnector:new(emberOgre:getMoveManager().EventStartMoving):connect("Mover_StartMoving")
	EmberOgre.LuaConnector:new(emberOgre:getMoveManager().EventFinishedMoving):connect("Mover_FinishedMoving")
	EmberOgre.LuaConnector:new(emberOgre:getMoveManager().EventCancelledMoving):connect("Mover_CancelledMoving")
	

	Mover.widget:hide()

end


Mover_buildWidget()
