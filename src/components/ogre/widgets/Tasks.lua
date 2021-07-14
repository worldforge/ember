Tasks = { connectors = {} }

Tasks.currentTask = nil
Tasks.buttons = {}
Tasks.activeButtons = {}

--function Tasks.frameStarted(time)
--	if Tasks.currentTask ~= nil then
--		Tasks.progressBar:setProgress(Tasks.currentTask)
--	end
--end

function Tasks.Progressed()
    -- Check if the task has a progress, if not hide the progress bar
    if Tasks.currentTask:progressRate() > 0 then
        Tasks.progressBar:show()
    else
        Tasks.progressBar:hide()
    end
    Tasks.progressBar:setProgress(Tasks.currentTask:progress())
end

function Tasks.Completed()
end

function Tasks.UsagesChanged()
    --first hide all buttons
    for _, v in pairs(Tasks.activeButtons) do
        v:setWidth(CEGUI.UDim(0, 0))
    end

    --As they are hidden they are all now "inactive"
    Tasks.activeButtons = {}

    for i = 1, Tasks.currentTask:usagesSize() do
        local name = Tasks.currentTask:usageName(i - 1)
        log.verbose(name)
        if name ~= "" then
            Tasks:showButton(name, "", function()
                Tasks.avatar:taskUsage(Tasks.currentTaskId, name)
            end)
        end
    end


end

function Tasks:showButton(text, tooltip, clickFn)
    local index = table.getn(Tasks.activeButtons)
    if index == table.getn(Tasks.buttons) then
        --We can't show too many buttons, that would just look strange.
        log.warning("Too many buttons to show for entity.")
    else
        local buttonWrapper = Tasks.buttons[index + 1]
        buttonWrapper.clickedHandler = clickFn
        buttonWrapper.button:setText(text)
        buttonWrapper.button:setTooltipText(tooltip)
        local font = tolua.cast(buttonWrapper.button:getFont(), "CEGUI::Font")
        local width = font:getTextExtent(text) + 10
        buttonWrapper.button:setWidth(CEGUI.UDim(0, width))
        table.insert(Tasks.activeButtons, buttonWrapper.button)
    end
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
    if Tasks.usagesAdapter then
        Tasks.usagesAdapter:disconnect()
    end
    Tasks.progressAdapter = nil
    Tasks.completedAdapter = nil
    Tasks.usagesAdapter = nil
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
    Tasks.usagesAdapter = createConnector(task.UsagesChanged)
    Tasks.usagesAdapter:connect("Tasks.UsagesChanged")
    Tasks.UsagesChanged()
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

function Tasks.buildWidget()

    Tasks.widget = guiManager:createWidget()
    Tasks.widget:loadMainSheet("Tasks.layout", "Tasks")

    Tasks.progressBar = Tasks.widget:getWindow("Progress")
    Tasks.progressBar = CEGUI.toProgressBar(Tasks.progressBar)

    Tasks.nameWindow = Tasks.widget:getWindow("NameText")

    Tasks.buttonsWindow = Tasks.widget:getWindow("Buttons")

	local con
	connect(connectors, emberOgre.EventWorldCreated, function(world)
		con = createConnector(world.EventGotAvatar):connect(function()
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
	connect(connectors, emberOgre.EventWorldDestroyed, function()
		con = null
	end)
    --	createConnector(Tasks.widget:EventFrameStarted):connect("Tasks.frameStarted")

    -- Start with 6 buttons
    for _ = 1, 6 do
        Tasks:createButton()
    end

    Tasks.widget:registerConsoleVisibilityToggleCommand("tasks")
    Tasks.widget:enableCloseButton()
    Tasks.widget:hide()

end

function Tasks:createButton()
    local button = guiManager:createWindow("EmberLook/Button")
    button:setHeight(CEGUI.UDim(0, 20))
    local wrapper = { button = button, clickedHandler = nil }
    button:subscribeEvent("Clicked", function()
        if wrapper.clickedHandler then
            wrapper.clickedHandler()
        end
    end)
    Tasks.buttonsWindow:addChild(wrapper.button)

    table.insert(Tasks.buttons, wrapper)
end

Tasks.buildWidget()


