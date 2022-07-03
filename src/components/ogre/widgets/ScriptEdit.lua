--[[ScriptEdit
]]--


ScriptEdit = { connectors = {}, widget = guiManager:createWidget() }

function ScriptEdit.buildWidget()

	ScriptEdit.widget:loadMainSheet("ScriptEdit.layout", "ScriptEdit")

	function getSelectedText()
		local text = ScriptEdit.scriptText:getText()
		if ScriptEdit.scriptText:getSelectionLength() > 0 then
			text = string.sub(text, ScriptEdit.scriptText:getSelectionStartIndex() + 1, ScriptEdit.scriptText:getSelectionEndIndex())
		end
		return text
	end


	--execute a block of code
	function executeCode(providerType)
		scriptingService:executeCode(getSelectedText(), providerType)
	end

	function inspectObject(anObject)
		local text = "nil"
		if anObject then
			local typeName = type(anObject)
			local value = tostring(anObject)
			if typeName then
				text = typeName .. " (" .. value .. ")"
			end
		end
		ScriptEdit.inspectText:setText(text)
		ScriptEdit.inspectText:setTooltipText(text)
	end

	--set up some useful values
	ScriptEdit.scriptList = CEGUI.toCombobox(ScriptEdit.widget:getWindow("ScriptType"))
	ScriptEdit.scriptList:getDropList():setProperty("ClippedByParent", "false")
	ScriptEdit.scriptText = ScriptEdit.widget:getWindow("ScriptText")
	ScriptEdit.scriptText = CEGUI.toMultiLineEditbox(ScriptEdit.scriptText)
	ScriptEdit.output = ScriptEdit.widget:getWindow("Output")
	local execute = ScriptEdit.widget:getWindow("ExecuteButton")
	ScriptEdit.inspect = ScriptEdit.widget:getWindow("InspectButton")
	ScriptEdit.inspectText = ScriptEdit.widget:getWindow("InspectText")
	--will clear the text of the script edit box
	ScriptEdit.widget:getWindow("ClearScriptTextButton"):subscribeEvent("Clicked", function()
		ScriptEdit.scriptText:setText("")
	end)
	--will clear the text of the output edit box
	ScriptEdit.widget:getWindow("ClearOutputButton"):subscribeEvent("Clicked",
			function()
				ScriptEdit.output:setText("")
			end)

	function getSelectedScriptingProvider()
		local provider = scriptingService:getProviderFor(ScriptEdit.scriptList:getSelectedItem():getText())
		return provider
	end
	ScriptEdit.widget:getWindow("ForceGCButton"):subscribeEvent("Clicked", function()
		local provider = getSelectedScriptingProvider()
		provider:forceGC()
	end)


	--handler for the execute click
	--will execute the code in the script text window
	execute:subscribeEvent("Clicked", function()
		local item = ScriptEdit.scriptList:getSelectedItem()
		if item then
			executeCode(item:getText())
		end

	end)
	--will try to inspect the type of the current selected code
	ScriptEdit.inspect:subscribeEvent("Clicked", function(args)
		--see if something is selected
		if ScriptEdit.scriptText:getSelectionLength() > 0 then

			local inspectCodeString = "return " .. getSelectedText()
			local inspectFunction = loadstring(inspectCodeString)
			if inspectFunction == nil then
				inspectObject(nil)
			else
				local anObject = inspectFunction()
				inspectObject(anObject)
			end
		end
	end)
	--handler for script error
	--updates the output textbox
	connect(ScriptEdit.connectors, scriptingService.EventScriptError,
			function(error)
				ScriptEdit.output:setText(error .. "\n" .. ScriptEdit.output:getText())
			end)

	local providerNames = scriptingService:getProviderNames()

	--add all scriptproviders
	for val = 1, providerNames:size() do
		local name = providerNames[val]
		local item = Ember.OgreView.Gui.ColouredListItem.new(name, val)
		ScriptEdit.scriptList:addItem(item)
		--select the first item from start
		if val == 1 then
			ScriptEdit.scriptList:setItemSelectState(item, true)
			ScriptEdit.scriptList:getEditbox():setText(name)
		end
	end

	ScriptEdit.widget:registerConsoleVisibilityToggleCommand("scriptEdit")
	ScriptEdit.widget:enableCloseButton()
	ScriptEdit.widget:hide()

end

ScriptEdit.buildWidget()