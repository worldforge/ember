-- -----------------------------------------
-- Start of handler functions
-----------------------------------------

-----------------------------------------
-- Script Entry Point
-----------------------------------------

ScriptEdit = {connectors={}}


ScriptEdit.widget = guiManager:createWidget()

function ScriptEdit.buildWidget()
	
	ScriptEdit.widget:loadMainSheet("ScriptEdit.layout", "ScriptEdit/")
	
	--set up some useful values
	ScriptEdit.scriptList = CEGUI.toCombobox(ScriptEdit.widget:getWindow("ScriptType"))
	ScriptEdit.scriptList:getDropList():setProperty("ClippedByParent", "false")
	ScriptEdit.scriptText = ScriptEdit.widget:getWindow("ScriptText")
	ScriptEdit.scriptText = CEGUI.toMultiLineEditbox(ScriptEdit.scriptText)
	ScriptEdit.output = ScriptEdit.widget:getWindow("Output")
	ScriptEdit.execute = ScriptEdit.widget:getWindow("ExecuteButton")
	ScriptEdit.inspect = ScriptEdit.widget:getWindow("InspectButton")
	ScriptEdit.inspectText = ScriptEdit.widget:getWindow("InspectText")
	
	ScriptEdit.widget:getWindow("ClearScriptTextButton"):subscribeEvent("MouseClick", "ScriptEdit.ClearScriptTextButtonClick")
	ScriptEdit.widget:getWindow("ClearOutputButton"):subscribeEvent("MouseClick", "ScriptEdit.ClearOutputButtonClick")
	
	
	
	--subscribe event
	ScriptEdit.execute:subscribeEvent("MouseClick", "ScriptEdit.executeClick")
	ScriptEdit.inspect:subscribeEvent("MouseClick", "ScriptEdit.inspectClick")
	connect(ScriptEdit.connectors, scriptingService:getEventScriptError(), "ScriptEdit.scriptError")
	
	ScriptEdit.dynamicBindings = CEGUI.toCheckbox(ScriptEdit.widget:getWindow("DynamicBindings"))
	ScriptEdit.dynamicBindings:subscribeEvent("CheckStateChanged", "ScriptEdit.dynamicBindings_CheckStateChanged")
	
	
	
	local providerNames = scriptingService:getProviderNames()
	
	--add all scriptproviders
	for val = 0, providerNames:size() - 1 do
		local name = providerNames[val]
		local item = EmberOgre.Gui.ColouredListItem:new(name, val)
		ScriptEdit.scriptList:addItem(item)
		--select the first item from start
		if val == 0 then
			ScriptEdit.scriptList:setItemSelectState(tolua.cast(item, "CEGUI::ListboxItem"), true)
			ScriptEdit.scriptList:getEditbox():setText(name)
		end
	end
	
	
	ScriptEdit.widget:registerConsoleVisibilityToggleCommand("scriptEdit")
	ScriptEdit.widget:enableCloseButton()
	ScriptEdit.widget:hide()

end

function ScriptEdit.getSelectedScriptingProvider()
	local provider = scriptingService:getProviderFor(ScriptEdit.scriptList:getSelectedItem():getText())
	return provider
end

--handler for script error
--updates the output textbox
function ScriptEdit.scriptError(error)
	ScriptEdit.output:setText(error .. "\n" .. ScriptEdit.output:getText())
end

--execute a block of code
function ScriptEdit.executeCode(providerType)
	scriptingService:executeCode(ScriptEdit.getSelectedText(), providerType)
end

function ScriptEdit.getSelectedText()
	local text = ScriptEdit.scriptText:getText()
	if ScriptEdit.scriptText:getSelectionLength() > 0 then
		text = string.sub(text, ScriptEdit.scriptText:getSelectionStartIndex() + 1, ScriptEdit.scriptText:getSelectionEndIndex())
	end
	return text
end
--handler for the execute click
--will execute the code in the script text window
function ScriptEdit.executeClick(args)

	local item = ScriptEdit.scriptList:getSelectedItem()
	if item ~= nil then
		ScriptEdit.executeCode(item:getText())
	end

end

function ScriptEdit.dynamicBindings_CheckStateChanged(args)
	scriptingService:setAlwaysLookup(ScriptEdit.dynamicBindings:isSelected())
end


--will try to inspect the type of the current selected code
function ScriptEdit.inspectClick(args)
	--see if something is selected
	if ScriptEdit.scriptText:getSelectionLength() > 0 then
		
		local inspectCodeString = "return " .. ScriptEdit.getSelectedText()
		local inspectFunction = loadstring(inspectCodeString)
		if inspectFunction == nil then
			inspectObject(nil)
		else
			local anObject = inspectFunction()
			inspectObject(anObject)
		end
	end
	

end

function inspectObject(anObject)
	local text = "nil"
	if anObject ~= nil then
		local typeName = tolua.type(anObject)
		local value = tostring(anObject)
		if typeName ~= nil then
			text = typeName .. " (".. value .. ")"
		end
	end
	ScriptEdit.inspectText:setText(text)
	ScriptEdit.inspectText:setTooltipText(text)
end

--will clear the text of the script edit box
function ScriptEdit.ClearScriptTextButtonClick(args)
	ScriptEdit.scriptText:setText("")
end

--will clear the text of the output edit box
function ScriptEdit.ClearOutputButtonClick(args)
	ScriptEdit.output:setText("")
end

function ScriptEdit.ForceGCButton_MouseClick(args)
	local provider = ScriptEdit.getSelectedScriptingProvider()
	provider:forceGC()
end

ScriptEdit.buildWidget()