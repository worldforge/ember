Tasks = { connectors = {} }

Tasks.currentTask = nil
Tasks.buttons = {}
Tasks.activeButtons = {}

--function Tasks.frameStarted(time)
--	if Tasks.currentTask then
--		Tasks.progressBar:setProgress(Tasks.currentTask)
--	end
--end













function Tasks.buildWidget()
	local function Completed()
	end

	local function Progressed()
		-- Check if the task has a progress, if not hide the progress bar
		if Tasks.currentTask:progressRate() > 0 then
			Tasks.progressBar:show()
		else
			Tasks.progressBar:hide()
		end
		Tasks.progressBar:setProgress(Tasks.currentTask:progress())
	end

	local function showButton(text, tooltip, clickFn)
		local index = #Tasks.activeButtons
		if index == #Tasks.buttons then
			--We can't show too many buttons, that would just look strange.
			log.warning("Too many buttons to show for entity.")
		else
			local buttonWrapper = Tasks.buttons[index + 1]
			buttonWrapper.clickedHandler = clickFn
			buttonWrapper.button:setText(text)
			buttonWrapper.button:setTooltipText(tooltip)
			local font = buttonWrapper.button:getFont()
			local width = font:getTextExtent(text) + 10
			buttonWrapper.button:setWidth(CEGUI.UDim.new(0, width))
			table.insert(Tasks.activeButtons, buttonWrapper.button)
		end
	end

	local function UsagesChanged()
		--first hide all buttons
		for _, v in pairs(Tasks.activeButtons) do
			v:setWidth(CEGUI.UDim.new(0, 0))
		end

		--As they are hidden they are all now "inactive"
		Tasks.activeButtons = {}

		for i = 0, Tasks.currentTask:usagesSize() do
			local name = Tasks.currentTask:usageName(i)
			if name ~= "" then
				showButton(name, "", function()
					Tasks.avatar:taskUsage(Tasks.currentTaskId, name)
				end)
			end
		end
	end

	local function ResetTask()
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

	local function SetCurrentTask(id, task)
		Tasks.currentTask = task
		Tasks.currentTaskId = id
		Tasks.nameWindow:setText(task:name())
		--set up bindings
		Tasks.progressAdapter = task.Progressed:connect(Progressed)
		Progressed()
		Tasks.completedAdapter = task.Completed:connect(Completed)
		Tasks.usagesAdapter = task.UsagesChanged:connect(UsagesChanged)
		UsagesChanged()
		Tasks.widget:show()
	end

	Tasks.widget = guiManager:createWidget()
	Tasks.widget:loadMainSheet("Tasks.layout", "Tasks")

	Tasks.progressBar = Tasks.widget:getWindow("Progress")
	Tasks.progressBar = CEGUI.toProgressBar(Tasks.progressBar)

	Tasks.nameWindow = Tasks.widget:getWindow("NameText")

	Tasks.buttonsWindow = Tasks.widget:getWindow("Buttons")

	local con
	connect(connectors, emberOgre.EventWorldCreated, function(world)
		con = world.EventGotAvatar:connect(function()
			Tasks.avatar = world:getAvatar()
			local avatarEntity = Tasks.avatar:getEmberEntity()
			connect(Tasks.connectors, avatarEntity.TaskAdded, function(id, task)
				if Tasks.currentTask == nil then
					SetCurrentTask(id, task)
				end
			end)
			connect(Tasks.connectors, avatarEntity.TaskRemoved, function(id, task)
				if task == Tasks.currentTask then
					ResetTask()
				end
			end)
			--If there are already tasks, show the first one
			if avatarEntity:getTasksSize() > 0 then
				SetCurrentTask(avatarEntity:getTaskIdFirst(), avatarEntity:getTaskFirst())
			end

		end)
	end)
	connect(connectors, emberOgre.EventWorldDestroyed, function()
		con = null
	end)
	--	Tasks.widget:EventFrameStarted:connect(Tasks.frameStarted)

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
	button:setHeight(CEGUI.UDim.new(0, 20))
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


