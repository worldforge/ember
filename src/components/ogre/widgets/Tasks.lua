Tasks = {}

Tasks.currentTask = nil

--function Tasks_frameStarted(time)
--	if Tasks.currentTask ~= nil then
--		Tasks.progressBar:setProgress(Tasks.currentTask)
--	end
--end

function Tasks_Progressed()
	console:pushMessage(Tasks.currentTask:progress())
	Tasks.progressBar:setProgress(Tasks.currentTask:progress())
end

function Tasks_Cancelled()
end

function Tasks_Completed()
end

function Tasks_ResetTask()
	Tasks.currentTask = nil
	--tear down bindings
	Tasks.progressAdapter = nil
	Tasks.cancelledAdapter = nil
	Tasks.completedAdapter = nil
	Tasks.nameWindow:setText("Task name: ")
	Tasks.widget:hide()

end

function Tasks_SetCurrentTask(task)
	Tasks.currentTask = task
	Tasks.nameWindow:setText("Task name: " .. task:name())
	--set up bindings
	Tasks.progressAdapter = EmberOgre.LuaConnector:new_local(task.Progressed)
	Tasks.progressAdapter:connect("Tasks_Progressed")
	Tasks.cancelledAdapter = EmberOgre.LuaConnector:new_local(task.Cancelled)
	Tasks.cancelledAdapter:connect("Tasks_Cancelled")
	Tasks.completedAdapter = EmberOgre.LuaConnector:new_local(task.Completed)
	Tasks.completedAdapter:connect("Tasks_Completed")
	Tasks.widget:show()
end

function Tasks_TaskAdded(task)
	if Tasks.currentTask == nil then
		Tasks_SetCurrentTask(task)
	end
end

function Tasks_TaskRemoved(task)
	if task == Tasks.currentTask then
		Tasks_ResetTask()
	end
end

function Tasks_createdAvatarEmberEntity(avatarEntity)
	EmberOgre.LuaConnector:new(avatarEntity.TaskAdded):connect("Tasks_TaskAdded")
	EmberOgre.LuaConnector:new(avatarEntity.TaskRemoved):connect("Tasks_TaskRemoved")
end

function Tasks_StopButtonClicked(args)
	emberServices:getServerService():useStop()
end

function Tasks_buildWidget()

	Tasks.widget = guiManager:createWidget()
	Tasks.widget:loadMainSheet("Tasks.layout", "Tasks/")
	
	Tasks.progressBar = Tasks.widget:getWindow("Progress")
	Tasks.progressBar =  CEGUI.toProgressBar(Tasks.progressBar)
	
	Tasks.nameWindow = Tasks.widget:getWindow("NameText")

	Tasks.widget:getWindow("StopButton"):subscribeEvent("MouseClick", "Tasks_StopButtonClicked")


	EmberOgre.LuaConnector:new(emberOgre.EventCreatedAvatarEntity):connect("Tasks_createdAvatarEmberEntity")

--	EmberOgre.LuaConnector:new(Tasks.widget:EventFrameStarted):connect("Tasks_frameStarted")


	Tasks.widget:registerConsoleVisibilityToggleCommand("tasks")
	Tasks.widget:enableCloseButton()
	Tasks.widget:hide()

end


Tasks_buildWidget()
