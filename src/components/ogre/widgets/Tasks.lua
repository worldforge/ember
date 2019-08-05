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

function Tasks.Completed()
end

function Tasks.ResetTask()
	Tasks.currentTask = nil
	--tear down bindings
	if Tasks.progressAdapter then
		Tasks.progressAdapter:disconnect()
	end
	if Tasks.completedAdapter then
		Tasks.completedAdapter:disconnect()
	end
	Tasks.progressAdapter = nil
	Tasks.completedAdapter = nil
	Tasks.nameWindow:setText("")
	Tasks.widget:hide()

end

function Tasks.SetCurrentTask(id, task)
	Tasks.currentTask = task
	Tasks.currentTaskId = id
	Tasks.nameWindow:setText(task:name())
	--set up bindings
	Tasks.progressAdapter = createConnector(task.Progressed)
	Tasks.progressAdapter:connect("Tasks.Progressed")
	Tasks.Progressed()
	Tasks.completedAdapter = createConnector(task.Completed)
	Tasks.completedAdapter:connect("Tasks.Completed")
	Tasks.widget:show()
end

function Tasks.TaskAdded(id, task)
	if Tasks.currentTask == nil then
		Tasks.SetCurrentTask(id, task)
	end
end

function Tasks.TaskRemoved(id, task)
	if task == Tasks.currentTask then
		Tasks.ResetTask()
	end
end

function Tasks.StopButtonClicked()
	Tasks.avatar:taskUsage(Tasks.currentTaskId, "stop")
end

function Tasks.buildWidget()

	Tasks.widget = guiManager:createWidget()
	Tasks.widget:loadMainSheet("Tasks.layout", "Tasks")
	
	Tasks.progressBar = Tasks.widget:getWindow("Progress")
	Tasks.progressBar = CEGUI.toProgressBar(Tasks.progressBar)
	
	Tasks.nameWindow = Tasks.widget:getWindow("NameText")

	Tasks.widget:getWindow("StopButton"):subscribeEvent("Clicked", "Tasks.StopButtonClicked")


	connect(connectors, emberOgre.EventWorldCreated, function(world)
		createConnector(world.EventGotAvatar):connect(function()
			Tasks.avatar = world:getAvatar()
			local avatarEntity = Tasks.avatar:getEmberEntity()
			connect(Tasks.connectors, avatarEntity.TaskAdded, "Tasks.TaskAdded")
			connect(Tasks.connectors, avatarEntity.TaskRemoved, "Tasks.TaskRemoved")
			--If there are already tasks, show the first one
			if avatarEntity:getTasksSize() > 0 then
				Tasks.SetCurrentTask(avatarEntity:getTaskIdFirst(), avatarEntity:getTaskFirst())
			end

		end)
	end)

--	createConnector(Tasks.widget:EventFrameStarted):connect("Tasks.frameStarted")


	Tasks.widget:registerConsoleVisibilityToggleCommand("tasks")
	Tasks.widget:enableCloseButton()
	Tasks.widget:hide()

end


Tasks.buildWidget()


