TerrainEditor = {}
 
 
 
function TerrainEditor_pickedBasePoint(userObject)
	TerrainEditor.currentObject = userObject
	local terrainPosition = userObject:getPosition()
	
	TerrainEditor.widget:getWindow("SelectedBasePointInfo"):setText("Selected basepoint: x=" .. terrainPosition:x() .. " y=" .. terrainPosition:y())
	
	TerrainEditor.heightSpinner:setText(userObject:getHeight())
end

function TerrainEditor_selectedBasePointUpdatedPosition(userObject)
	--inspectObject(userObject:getHeight())
	TerrainEditor.heightSpinner:setText(userObject:getHeight())

end

function TerrainEditor_HeightSpinner_ValueChanged(args)
	if TerrainEditor.currentObject ~= nil then
		local translation = TerrainEditor.heightSpinner:getCurrentValue() - TerrainEditor.currentObject:getHeight()
		TerrainEditor.currentObject:translate(translation)
		TerrainEditor.editor:createAction(true)
	end
end

function TerrainEditor_UndoButton_Click(args)
	TerrainEditor.editor:undoLastAction()
end

function TerrainEditor_RedoButton_Click(args)
	TerrainEditor.editor:redoAction()
end

function TerrainEditor_ShowOverlayButton_Click(args)

	if TerrainEditor.editor:isOverlayShown() then
		TerrainEditor.showOverlayButton:setText("Show overlay")
		TerrainEditor.editor:hideOverlay()
	else 
		TerrainEditor.showOverlayButton:setText("Hide overlay")
		TerrainEditor.editor:showOverlay()
	end
end

function TerrainEditor_SendToServerButton_Click(args)
	TerrainEditor.editor:sendChangesToServer()
end

function TerrainEditor_buildWidget()
	TerrainEditor.widget = guiManager:createWidget()
	TerrainEditor.widget:loadMainSheet("TerrainEditor.layout", "TerrainEditor/")
	
	TerrainEditor.heightSpinner = TerrainEditor.widget:getWindow("Height")
	TerrainEditor.heightSpinner = CEGUI.toEditbox(TerrainEditor.heightSpinner)
	
	
	TerrainEditor.editor = EmberOgre.TerrainEditor:new()
	TerrainEditor.editor:createOverlay()
	TerrainEditor.editor:showOverlay()
	
	EmberOgre.LuaConnector:new(TerrainEditor.editor.EventPickedBasePoint):connect("TerrainEditor_pickedBasePoint")
	EmberOgre.LuaConnector:new(TerrainEditor.editor.EventSelectedBasePointUpdatedPosition):connect("TerrainEditor_selectedBasePointUpdatedPosition")
	
	--TerrainEditor.heightSpinner:subscribeEvent("ValueChanged", "TerrainEditor_HeightSpinner_ValueChanged")
	
	TerrainEditor.showOverlayButton = TerrainEditor.widget:getWindow("ShowOverlayButton")
	TerrainEditor.showOverlayButton = CEGUI.toPushButton(TerrainEditor.showOverlayButton)
	TerrainEditor.showOverlayButton:subscribeEvent("MouseClick", "TerrainEditor_ShowOverlayButton_Click")
	
	--hook up the undo and redo buttons
	TerrainEditor.widget:getWindow("UndoButton"):subscribeEvent("MouseClick", "TerrainEditor_UndoButton_Click")
	TerrainEditor.widget:getWindow("RedoButton"):subscribeEvent("MouseClick", "TerrainEditor_RedoButton_Click")
	
	
	TerrainEditor.widget:getWindow("SendToServerButton"):subscribeEvent("MouseClick", "TerrainEditor_SendToServerButton_Click")
	
	
	TerrainEditor.widget:registerConsoleVisibilityToggleCommand("terrainEditor")
	TerrainEditor.widget:enableCloseButton()
	--TerrainEditor.widget:hide()

end
TerrainEditor_buildWidget()