Tasks = {connectors={}, columns=9, iconSize=32, slotcounter=0, activeSlots={}}

Tasks.currentTask = nil

--function Tasks.frameStarted(time)
--	if Tasks.currentTask ~= nil then
--		Tasks.progressBar:setProgress(Tasks.currentTask)
--	end
--end

function Tasks.Progressed()
	Tasks.progressBar:setProgress(Tasks.currentTask:progress())
end

function Tasks.Cancelled()
end

function Tasks.Completed()
end

function Tasks.ResetTask()
	Tasks.currentTask = nil
	--tear down bindings
	if Tasks.progressAdapter ~= nil then
		Tasks.progressAdapter:disconnect()
	end
	if Tasks.cancelledAdapter ~= nil then
		Tasks.cancelledAdapter:disconnect()
	end
	if Tasks.completedAdapter ~= nil then
		Tasks.completedAdapter:disconnect()
	end
	Tasks.progressAdapter = nil
	Tasks.cancelledAdapter = nil
	Tasks.completedAdapter = nil
	Tasks.nameWindow:setText("Task name: ")
	Tasks.widget:hide()
	Tasks.removeActiveSlots()
end

function Tasks.SetCurrentTask(task)
	Tasks.currentTask = task
	Tasks.nameWindow:setText("Task name: " .. task:name())
	--set up bindings
	Tasks.progressAdapter = createConnector(task.Progressed)
	Tasks.progressAdapter:connect("Tasks.Progressed")
	Tasks.cancelledAdapter = createConnector(task.Cancelled)
	Tasks.cancelledAdapter:connect("Tasks.Cancelled")
	Tasks.completedAdapter = createConnector(task.Completed)
	Tasks.completedAdapter:connect("Tasks.Completed")
	Tasks.widget:show()
end

function Tasks.TaskAdded(task)
	if Tasks.currentTask == nil then
		Tasks.SetCurrentTask(task)
	end
end

function Tasks.TaskRemoved(task)
	if task == Tasks.currentTask then
		Tasks.ResetTask()
	end
end

function Tasks.createdAvatarEmberEntity(avatarEntity)
	connect(Tasks.connectors, avatarEntity.TaskAdded, "Tasks.TaskAdded")
	connect(Tasks.connectors, avatarEntity.TaskRemoved, "Tasks.TaskRemoved")
end

function Tasks.StopButtonClicked(args)
	emberServices:getServerService():useStop()
	return true
end

function Tasks.ExpandParametersButtonClicked(args)

	Tasks.parametersDropArea:setVisible(not Tasks.parametersDropArea:isVisible())
	if Tasks.parametersDropArea:isVisible() == false then
		Tasks.widget:getMainWindow():setHeight(CEGUI.UDim:new_local(0, 90))
	else
		Tasks.widget:getMainWindow():setHeight(CEGUI.UDim:new_local(0, 200))
	end
	return true
end

function Tasks.iconDropped(entityIcon)

	if Tasks.activeSlots[entityIcon:getEntity():getId()] then
		return
	end
	local yPosition = math.floor(Tasks.slotcounter / Tasks.columns)
	local xPosition = Tasks.slotcounter % Tasks.columns
	
	Tasks.slotcounter = Tasks.slotcounter + 1

	local slot = guiManager:getEntityIconManager():createSlot(Tasks.iconSize)
	slot:getWindow():setPosition(CEGUI.UVector2(CEGUI.UDim(0, Tasks.iconSize * xPosition), CEGUI.UDim(0, Tasks.iconSize * yPosition)))
--	slot:getWindow():setAlpha(0.6)
	slot:getWindow():setProperty("FrameEnabled", "false")
	slot:getWindow():setProperty("BackgroundEnabled", "false")
	
	local icon = guiManager:getIconManager():getIcon(Tasks.iconSize, entityIcon:getEntity())
	local newEntityIcon = guiManager:getEntityIconManager():createIcon(icon, entityIcon:getEntity(), Tasks.iconSize)
	slot:addEntityIcon(newEntityIcon)
	Tasks.parametersDropArea:addChildWindow(slot:getWindow())
	local activeSlot = {
		slot = slot,
		icon = icon,
		entityIcon = newEntityIcon
	}
	Tasks.activeSlots[entityIcon:getEntity():getId()] = activeSlot
end

function Tasks.moveManager_StartMoving(entity, mover)
	--When the user is moving something we should start to listen for the mouse entering the task widget, and if so activate the ability to drop the thing that is being moved here.
	local overlay = Tasks.widget:createWindow("DefaultGUISheet")
	Tasks.parametersDropArea:addChildWindow(overlay)
	overlay:subscribeEvent("MouseEnter", function(args)
			local icon = guiManager:getIconManager():getIcon(Tasks.iconSize, entity)
			
			if icon then
				mover:setIsActive(false)
--				self.DragDrop:setActive(false)
				local entityIcon = guiManager:getEntityIconManager():createIcon(icon, entity, Tasks.iconSize)	
				Tasks.parametersDropArea:addChildWindow(entityIcon:getDragContainer())
				entityIcon:getDragContainer():pickUp(true)
			end
			overlay:subscribeEvent("MouseLeave", function(args)
					debugObject("Mouse leaves")
					guiManager:getEntityIconManager():destroyIcon(icon)
					Tasks.parametersDropArea:removeChildWindow(overlay)
					mover:setIsActive(true)
					return true
				end
			)
			--self.widget:getMainWindow():removeChildWindow(overlay)
			
			return true
		end
	)
	local finishedConnector
	local cancelledConnector
	local function endMovement()
		finishedConnector:disconnect()
		finishedConnector = nil
		cancelledConnector:disconnect()
		cancelledConnector = nil
		windowManager:destroyWindow(overlay)
	end
	
	finishedConnector = createConnector(emberOgre:getWorld():getMoveManager().EventFinishedMoving):connect(endMovement)
	cancelledConnector = createConnector(emberOgre:getWorld():getMoveManager().EventCancelledMoving):connect(endMovement)
	
end


function Tasks.removeActiveSlots()
	for k,v in pairs(Tasks.activeSlots) do
		guiManager:getEntityIconManager():destroySlot(v.slot)
		guiManager:getEntityIconManager():destroyIcon(v.entityIcon)
	end
	Tasks.activeSlots = {}
	Tasks.slotcounter = 0
end

function Tasks.buildWidget()

	Tasks.widget = guiManager:createWidget()
	Tasks.widget:loadMainSheet("Tasks.layout", "Tasks/")
	
	Tasks.progressBar = Tasks.widget:getWindow("Progress")
	Tasks.progressBar =  CEGUI.toProgressBar(Tasks.progressBar)
	
	Tasks.nameWindow = Tasks.widget:getWindow("NameText")
	Tasks.parametersDropArea = Tasks.widget:getWindow("ParametersDropArea")
	Tasks.iconDropTarget = Ember.OgreView.Gui.EntityIconDragDropTarget:new(Tasks.parametersDropArea)
	connect(Tasks.connectors, Tasks.iconDropTarget.EventIconDropped, "Tasks.iconDropped")
	

	Tasks.widget:getWindow("StopButton"):subscribeEvent("Clicked", "Tasks.StopButtonClicked")
	Tasks.widget:getWindow("ExpandParametersButton"):subscribeEvent("Clicked", "Tasks.ExpandParametersButtonClicked")


	--Set up listeners so that when something is moved in the world it can be dropped on the inventory
	connect(Tasks.connectors, emberOgre:getWorld():getMoveManager().EventStartMoving, Tasks.moveManager_StartMoving)

--	createConnector(Tasks.widget:EventFrameStarted):connect("Tasks.frameStarted")


	Tasks.widget:registerConsoleVisibilityToggleCommand("tasks")
	Tasks.widget:enableCloseButton()
	Tasks.widget:hide()

end

Tasks.createdAvatarEntityConnector = createConnector(emberOgre.EventCreatedAvatarEntity):connect(function(avatarEntity)
		if not Tasks.widget then
			Tasks.buildWidget()
		end
		Tasks.createdAvatarEmberEntity(avatarEntity)
	end
)
