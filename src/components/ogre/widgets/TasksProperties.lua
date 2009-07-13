TasksProperties = {connectors={}}

function TasksProperties.startTask()
	emberServices:getServerService():use(TasksProperties.entity, EmberOgre.Convert.toWF_Point3(TasksProperties.position), TasksProperties.operation)
	guiManager:EmitEntityAction("use", TasksProperties.entity)
-- set properties
end

function TasksProperties.setTask(entity, position, operation)
	TasksProperties.entity = entity
	TasksProperties.position = position
	TasksProperties.operation = operation
	TasksProperties.widget:getWindow("MainWindow"):setVisible(true)
end

function TasksProperties.showTask()
	local adapters = TasksProperties.helper:getGUIAdapters()

	if adapters ~= nil then
		return
	end

	for k,v in adapters:pairs() do
		log.verbose("Creating adapter " .. k .. " of type " .. v:getType())

		local container = guiManager:createWindow("DefaultGUISheet")
		v:attach(container)

		TasksProperties.addAdapter(v, v:getTitle(), container, TasksProperties.container)
	end
end

-- Adds adapter
function EntityCreator.addAdapter(adapter, title, container, parentContainer)
	local textWidth = 75
	local outercontainer = guiManager:createWindow("DefaultGUISheet")
	--outercontainer:setRiseOnClickEnabled(false)
	local label = guiManager:createWindow("EmberLook/StaticText")
	
	label:setText(title)
	label:setWidth(CEGUI.UDim(0, textWidth))
	label:setProperty("FrameEnabled", "false");
 	label:setProperty("BackgroundEnabled", "false");
	label:setProperty("VertFormatting", "TopAligned");
	label:setProperty("Tooltip", title);
	
	local width = container:getWidth()
	width = width + CEGUI.UDim(0, textWidth)
	outercontainer:setWidth(width)
	container:setXPosition(CEGUI.UDim(0, textWidth))
	container:setProperty("Tooltip", adapter:getTooltip());
	
	outercontainer:setHeight(container:getHeight())
	
	--make sure that the outer container has the same height as the inner container (so that when we add new child adapters it's updated)
	function syncWindowHeights(args)
		outercontainer:setHeight(container:getHeight())
	end
	local SizedConnection = container:subscribeEvent("Sized", syncWindowHeights)

	outercontainer:addChildWindow(label)
	outercontainer:addChildWindow(container)

	parentContainer:addChildWindow(outercontainer)
	return outercontainer
end

--- Builds widget
function TasksProperties.buildWidget()

	-- Initializing helper classes
	TasksProperties.helper = EmberOgre.Gui.TasksProperties()

	-- Loading widget layout
	TasksProperties.widget = guiManager:createWidget()
	local setup = function()

		connect(TasksProperties.connectors, TasksProperties.helper.EventTypeInfoLoaded, TasksProperties.showTask)
		TasksProperties.helper.mWidget = TasksProperties.widget
		TasksProperties.factory = EmberOgre.Gui.Adapters.Atlas.AdapterFactory("TasksProperties")

		-- Creating container for storing adapters
		TasksProperties.recipeDescription = EntityCreator.widget:getWindow("RecipeDescription")
		TasksProperties.container = EntityCreator.widget:getWindow("AdaptersContainer")
		TasksProperties.stackableContainer = EmberOgre.Gui.StackableContainer(EntityCreator.container)

		-- Finalizing
		TasksProperties.widget:enableCloseButton()
	end
	connect(TasksProperties.connectors, TasksProperties.widget.EventFirstTimeShown, setup)
--	TasksProperties.widget:registerConsoleVisibilityToggleCommand("tasksProperties")
	TasksProperties.widget:loadMainSheet("TasksProperties.layout", "TasksProperties/")

end

TasksProperties.buildWidget()
