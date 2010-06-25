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

	
	local message = EmberOgre.Gui.HelpMessage:new_local("Entity Move", "Moving " .. name .. ". Press left mouse button to place, escape to cancel. Pressing and holding shift decreases movement speed. Mouse wheel rotates.", "entity move help", "moveMessage")
	EmberOgre.Gui.QuickHelp:getSingleton():updateText(message)
end

function Mover.StartMoving(entity, mover)
	Mover.updateEntityText(entity)
end 

function Mover.buildWidget()

	
	local moveManager = emberOgre:getWorld():getMoveManager()
	
	connect(Mover.connectors, moveManager.EventStartMoving, Mover.StartMoving)
	connect(Mover.connectors, moveManager.EventFinishedMoving, Mover.FinishedMoving)
	connect(Mover.connectors, moveManager.EventCancelledMoving, Mover.CancelledMoving)
	


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
