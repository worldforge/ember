TerrainEditor = {connectors={}}
 
 
 
function TerrainEditor.pickedBasePoint(userObject)
	TerrainEditor.currentObject = userObject
	local terrainPosition = userObject:getPosition()
	
	TerrainEditor.widget:getWindow("SelectedBasePointInfo"):setText("Selected basepoint: x=" .. terrainPosition:x() .. " y=" .. terrainPosition:y())
	
	TerrainEditor.heightSpinner:setText(userObject:getHeight())
end

function TerrainEditor.selectedBasePointUpdatedPosition(userObject)
	--inspectObject(userObject:getHeight())
	TerrainEditor.heightSpinner:setText(userObject:getHeight())

end

function TerrainEditor.HeightSpinner_ValueChanged(args)
	if TerrainEditor.currentObject ~= nil then
		local translation = TerrainEditor.heightSpinner:getCurrentValue() - TerrainEditor.currentObject:getHeight()
		TerrainEditor.currentObject:translate(translation)
		TerrainEditor.editor:createAction(true)
	end
end

function TerrainEditor.UndoButton_Click(args)
	TerrainEditor.editor:undoLastAction()
end

function TerrainEditor.RedoButton_Click(args)
	TerrainEditor.editor:redoAction()
end

function TerrainEditor.ShowOverlayButton_Click(args)
	TerrainEditor.ToggleOverlayVisibility()
end

--Toggles the visibility of the overlay, which is the blue movable dots
function TerrainEditor.ToggleOverlayVisibility()
	if TerrainEditor.editor:isOverlayShown() then
		TerrainEditor.HideOverlay()
	else 
		TerrainEditor.ShowOverlay()
	end
end

function TerrainEditor.HideOverlay()
	TerrainEditor.showOverlayButton:setText("Show overlay")
	TerrainEditor.editor:hideOverlay()
end

function TerrainEditor.ShowOverlay()
	TerrainEditor.showOverlayButton:setText("Hide overlay")
	TerrainEditor.editor:showOverlay()
end

function TerrainEditor.SendToServerButton_Click(args)
	TerrainEditor.editor:sendChangesToServer()
end

function TerrainEditor.MainWindow_Hidden(args)
	TerrainEditor.HideOverlay()
end

function TerrainEditor.buildWidget()
	TerrainEditor.widget = guiManager:createWidget()
	TerrainEditor.widget:loadMainSheet("TerrainEditor.layout", "TerrainEditor/")
	
	TerrainEditor.heightSpinner = TerrainEditor.widget:getWindow("Height")
	TerrainEditor.heightSpinner = CEGUI.toEditbox(TerrainEditor.heightSpinner)
	
	
	TerrainEditor.editor = EmberOgre.TerrainEditor:new_local()
	TerrainEditor.editor:createOverlay()
	TerrainEditor.editor:showOverlay()
	
	connect(TerrainEditor.connectors, TerrainEditor.editor.EventPickedBasePoint, "TerrainEditor.pickedBasePoint")
	connect(TerrainEditor.connectors, TerrainEditor.editor.EventSelectedBasePointUpdatedPosition, "TerrainEditor.selectedBasePointUpdatedPosition")
	
	--TerrainEditor.heightSpinner:subscribeEvent("ValueChanged", "TerrainEditor.HeightSpinner_ValueChanged")
	
	TerrainEditor.showOverlayButton = TerrainEditor.widget:getWindow("ShowOverlayButton")
	TerrainEditor.showOverlayButton = CEGUI.toPushButton(TerrainEditor.showOverlayButton)
	TerrainEditor.showOverlayButton:subscribeEvent("MouseClick", "TerrainEditor.ShowOverlayButton_Click")
	
	--hook up the undo and redo buttons
	TerrainEditor.widget:getWindow("UndoButton"):subscribeEvent("MouseClick", "TerrainEditor.UndoButton_Click")
	TerrainEditor.widget:getWindow("RedoButton"):subscribeEvent("MouseClick", "TerrainEditor.RedoButton_Click")
	
	TerrainEditor.widget:getMainWindow():subscribeEvent("Hidden", "TerrainEditor.MainWindow_Hidden")
	
	TerrainEditor.widget:getWindow("SendToServerButton"):subscribeEvent("MouseClick", "TerrainEditor.SendToServerButton_Click")
	
	
	TerrainEditor.widget:registerConsoleVisibilityToggleCommand("terrainEditor")
	TerrainEditor.widget:enableCloseButton()
	--TerrainEditor.widget:hide()

end
TerrainEditor.buildWidget()