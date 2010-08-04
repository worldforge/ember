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
		TerrainEditor.overlay:createAction(true)
	end
end

function TerrainEditor.UndoButton_Click(args)
	TerrainEditor.overlay:undoLastAction()
end

function TerrainEditor.RedoButton_Click(args)
	TerrainEditor.overlay:redoAction()
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
	TerrainEditor.overlay:sendChangesToServer()
end

function TerrainEditor.MainWindow_Hidden(args)
	TerrainEditor.HideOverlay()
end

function TerrainEditor.Radius_ValueChanged(args)
	TerrainEditor.editor:setRadius(TerrainEditor.radiusSlider:getCurrentValue() * 1024)
end

function TerrainEditor.overlayCreated(overlay)
	TerrainEditor.overlay = overlay
	connect(TerrainEditor.connectors, overlay.EventPickedBasePoint, "TerrainEditor.pickedBasePoint")
	connect(TerrainEditor.connectors, overlay.EventSelectedBasePointUpdatedPosition, "TerrainEditor.selectedBasePointUpdatedPosition")

end

function TerrainEditor.buildWidget()
	local entityFactory = emberOgre:getWorld():getEntityFactory()
	if entityFactory ~= nil then
		local worldEntity = entityFactory:getWorld()
		if worldEntity ~= nil then
			local terrainManager = worldEntity:getTerrainManager()
			local mainCamera = emberOgre:getWorld():getMainCamera()
			if mainCamera ~= nil then
				TerrainEditor.widget = guiManager:createWidget()
				TerrainEditor.widget:loadMainSheet("TerrainEditor.layout", "TerrainEditor/")
				
				TerrainEditor.heightSpinner = TerrainEditor.widget:getWindow("Height")
				TerrainEditor.heightSpinner = CEGUI.toEditbox(TerrainEditor.heightSpinner)
				
				TerrainEditor.radiusSlider = TerrainEditor.widget:getWindow("Radius")
				TerrainEditor.radiusSlider = CEGUI.toSlider(TerrainEditor.radiusSlider)
	
				TerrainEditor.editor = EmberOgre.Terrain.TerrainEditor:new_local(terrainManager, mainCamera)
				connect(TerrainEditor.connectors, TerrainEditor.editor.EventOverlayCreated, "TerrainEditor.overlayCreated")
				TerrainEditor.editor:showOverlay()
				
				
				--TerrainEditor.heightSpinner:subscribeEvent("ValueChanged", "TerrainEditor.HeightSpinner_ValueChanged")
				
				TerrainEditor.showOverlayButton = TerrainEditor.widget:getWindow("ShowOverlayButton")
				TerrainEditor.showOverlayButton = CEGUI.toPushButton(TerrainEditor.showOverlayButton)
				TerrainEditor.showOverlayButton:subscribeEvent("Clicked", "TerrainEditor.ShowOverlayButton_Click")
				
				--hook up the undo and redo buttons
				TerrainEditor.widget:getWindow("UndoButton"):subscribeEvent("Clicked", "TerrainEditor.UndoButton_Click")
				TerrainEditor.widget:getWindow("RedoButton"):subscribeEvent("Clicked", "TerrainEditor.RedoButton_Click")
				
				TerrainEditor.widget:getMainWindow():subscribeEvent("Hidden", "TerrainEditor.MainWindow_Hidden")
				
				TerrainEditor.widget:getWindow("SendToServerButton"):subscribeEvent("Clicked", "TerrainEditor.SendToServerButton_Click")
				
				
				TerrainEditor.widget:registerConsoleVisibilityToggleCommand("terrainEditor")
				TerrainEditor.widget:enableCloseButton()
				--TerrainEditor.widget:hide()
			end
		end
	end
	

end

connect(connectors, emberOgre.EventTerrainManagerCreated, TerrainEditor.buildWidget)
