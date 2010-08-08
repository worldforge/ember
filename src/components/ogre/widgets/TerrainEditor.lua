TerrainEditor = {}
 
 
 
function TerrainEditor:pickedBasePoint(userObject)
	self.currentObject = userObject
	local terrainPosition = userObject:getPosition()
	
	self.widget:getWindow("SelectedBasePointInfo"):setText("Selected basepoint: x=" .. terrainPosition:x() .. " y=" .. terrainPosition:y())
	
	self.heightSpinner:setText(userObject:getHeight())
end

function TerrainEditor:selectedBasePointUpdatedPosition(userObject)
	--inspectObject(userObject:getHeight())
	self.heightSpinner:setText(userObject:getHeight())

end

function TerrainEditor:HeightSpinner_ValueChanged(args)
	if self.currentObject ~= nil then
		local translation = self.heightSpinner:getCurrentValue() - self.currentObject:getHeight()
		self.currentObject:translate(translation)
		self.overlay:createAction(true)
	end
end

function TerrainEditor:UndoButton_Click(args)
	self.overlay:undoLastAction()
end

function TerrainEditor:RedoButton_Click(args)
	self.overlay:redoAction()
end

function TerrainEditor:ShowOverlayButton_Click(args)
	self:ToggleOverlayVisibility()
end

--Toggles the visibility of the overlay, which is the blue movable dots
function TerrainEditor:ToggleOverlayVisibility()
	if self.editor:isOverlayShown() then
		self:HideOverlay()
	else 
		self:ShowOverlay()
	end
end

function TerrainEditor:HideOverlay()
	self.showOverlayButton:setText("Show overlay")
	self.editor:hideOverlay()
end

function TerrainEditor:ShowOverlay()
	self.showOverlayButton:setText("Hide overlay")
	self.editor:showOverlay()
end

function TerrainEditor:SendToServerButton_Click(args)
	self.overlay:sendChangesToServer()
end

function TerrainEditor:MainWindow_Hidden(args)
	self:HideOverlay()
end

function TerrainEditor:Radius_ValueChanged(args)
	self.editor:setRadius(self.radiusSlider:getCurrentValue() * 1024)
end

function TerrainEditor:overlayCreated(overlay)
	self.overlay = overlay
	connect(self.connectors, overlay.EventPickedBasePoint, self.pickedBasePoint, self)
	connect(self.connectors, overlay.EventSelectedBasePointUpdatedPosition, self.selectedBasePointUpdatedPosition, self)

end

function TerrainEditor:buildWidget(terrainManager)
	local mainCamera = emberOgre:getWorld():getMainCamera()
	if mainCamera ~= nil then
		self.widget = guiManager:createWidget()
		
		local setup = function()
			self.heightSpinner = self.widget:getWindow("Height")
			self.heightSpinner = CEGUI.toEditbox(self.heightSpinner)
			
			self.radiusSlider = self.widget:getWindow("Radius")
			self.radiusSlider = CEGUI.toSlider(self.radiusSlider)
		
			self.editor = EmberOgre.Terrain.TerrainEditor:new(terrainManager, mainCamera)
			connect(self.connectors, self.editor.EventOverlayCreated, self.overlayCreated, self)
			self.editor:showOverlay()
			
			
			--self.heightSpinner:subscribeEvent("ValueChanged", self.HeightSpinner_ValueChanged, self)
			
			self.showOverlayButton = self.widget:getWindow("ShowOverlayButton")
			self.showOverlayButton = CEGUI.toPushButton(self.showOverlayButton)
			self.showOverlayButton:subscribeEvent("Clicked", self.ShowOverlayButton_Click, self)
			
			--hook up the undo and redo buttons
			self.widget:getWindow("UndoButton"):subscribeEvent("Clicked", self.UndoButton_Click, self)
			self.widget:getWindow("RedoButton"):subscribeEvent("Clicked", self.RedoButton_Click, self)
			
			self.widget:getMainWindow():subscribeEvent("Hidden", self.MainWindow_Hidden, self)
			
			self.widget:getWindow("SendToServerButton"):subscribeEvent("Clicked", self.SendToServerButton_Click, self)
			self.widget:enableCloseButton()
		end
		
		connect(self.connectors, self.widget.EventFirstTimeShown, setup)
		self.widget:loadMainSheet("TerrainEditor.layout", "TerrainEditor/")
		self.widget:registerConsoleVisibilityToggleCommand("terrainEditor")
		--self.widget:hide()
	end
end

function TerrainEditor:shutdown()
	deleteSafe(self.editor)
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

connect(connectors, emberOgre.EventTerrainManagerCreated, function(terrainManager)
	terrainEditor = {connectors={}}
	setmetatable(terrainEditor, {__index = TerrainEditor})
	
	terrainEditor:buildWidget(terrainManager)
	
	connect(terrainEditor.connectors, emberOgre.EventWorldDestroyed, function()
			terrainEditor:shutdown()
			terrainEditor = nil
		end
	)	
	
end)
