Tasks = {connectors={}}

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

end

function Tasks.SetCurrentTask(task)
	Tasks.currentTask = task
	Tasks.nameWindow:setText("Task name: " .. task:name())
	--set up bindings
	Tasks.progressAdapter = EmberOgre.LuaConnector:new_local(task.Progressed)
	Tasks.progressAdapter:connect("Tasks.Progressed")
	Tasks.cancelledAdapter = EmberOgre.LuaConnector:new_local(task.Cancelled)
	Tasks.cancelledAdapter:connect("Tasks.Cancelled")
	Tasks.completedAdapter = EmberOgre.LuaConnector:new_local(task.Completed)
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
end

function Tasks.buildWidget()

	Tasks.widget = guiManager:createWidget()
	Tasks.widget:loadMainSheet("Tasks.layout", "Tasks/")
	
	Tasks.progressBar = Tasks.widget:getWindow("Progress")
	Tasks.progressBar =  CEGUI.toProgressBar(Tasks.progressBar)
	
	Tasks.nameWindow = Tasks.widget:getWindow("NameText")

	Tasks.widget:getWindow("StopButton"):subscribeEvent("MouseClick", "Tasks.StopButtonClicked")


	connect(Tasks.connectors, emberOgre.EventCreatedAvatarEntity, "Tasks.createdAvatarEmberEntity")

--	EmberOgre.LuaConnector:new(Tasks.widget:EventFrameStarted):connect("Tasks.frameStarted")


	Tasks.widget:registerConsoleVisibilityToggleCommand("tasks")
	Tasks.widget:enableCloseButton()
	Tasks.widget:hide()

end


Tasks.buildWidget()
