TypeManager = {connectors={}, codecClass=Atlas.Codecs.XML}


function TypeManager.buildWidget()

	TypeManager.widget = guiManager:createWidget()

	local setup = function()
		
		TypeManager.typeTree = tolua.cast(TypeManager.widget:getWindow("TypeList"), "CEGUI::Tree")
		TypeManager.typeTree:subscribeEvent("ItemSelectionChanged", "TypeManager.TypeList_SelectionChanged")
	
		TypeManager.codecTypeCombobox = CEGUI.toCombobox(TypeManager.widget:getWindow("CodecType"))
		
		local item = EmberOgre.Gui.ColouredListItem:new("XML", 0)
		TypeManager.codecTypeCombobox:addItem(item)
		item = EmberOgre.Gui.ColouredListItem:new("Bach", 1)
		TypeManager.codecTypeCombobox:addItem(item)
		item = EmberOgre.Gui.ColouredListItem:new("Packed", 2)
		TypeManager.codecTypeCombobox:addItem(item)
		TypeManager.codecTypeCombobox:setItemSelectState(0, true)
		TypeManager.codecTypeCombobox:setSingleClickEnabled(true)
		TypeManager.codecTypeCombobox:subscribeEvent("ListSelectionChanged", "TypeManager.CodecType_ListSelectionChanged")
	
		
		TypeManager.typeInfoText = CEGUI.toMultiLineEditbox(TypeManager.widget:getWindow("TypeInfoText"))
	
		TypeManager.typeAdapter = EmberOgre.Gui.Adapters.Eris.TypeTreeAdapter:new_local(emberServices:getServerService():getConnection():getTypeService(), TypeManager.typeTree)
		TypeManager.typeAdapter:initialize("root")
		
		TypeManager.widget:getWindow("SendToServerButton"):subscribeEvent("Clicked", "TypeManager.SendToServerButton_Clicked")
			
		TypeManager.widget:enableCloseButton()
	end

	connect(TypeManager.connectors, TypeManager.widget.EventFirstTimeShown, setup)
	TypeManager.widget:loadMainSheet("TypeManager.layout", "TypeManager/")
	TypeManager.widget:registerConsoleVisibilityToggleCommand("typeManager")

end

function TypeManager.CreatedAvatarEntity()
	TypeManager.buildWidget()
end

function TypeManager.CodecType_ListSelectionChanged()
	local selectId = TypeManager.codecTypeCombobox:getSelectedItem():getID()
	if selectId == 0 then
		TypeManager.codecClass = Atlas.Codecs.XML
	elseif selectId == 1 then
		TypeManager.codecClass = Atlas.Codecs.Bach
	else
		TypeManager.codecClass = Atlas.Codecs.Packed
	end
	TypeManager.printType()
end

function TypeManager.sendTypeToServer()
	local outstream = std.stringstream:new_local(TypeManager.typeInfoText:getText())
	local decoder = EmberOgre.Authoring.AtlasObjectDecoder:new_local()

	local codec = TypeManager.codecClass:new_local(outstream, tolua.cast(decoder, "Atlas::Bridge"))
	codec:poll(true)
	
	local parsedObject = decoder:getLastObject()
	
	if parsedObject:isValid() then
		--If the type already exists, we need to send a "set" operation, else we need to send a "create" operation
		if emberServices:getServerService():getConnection():getTypeService():findTypeByName(parsedObject:get():getId()) == nil then
			emberServices:getServerService():createTypeInfo(parsedObject)
		else
			emberServices:getServerService():setTypeInfo(parsedObject)
		end
	end
end

function TypeManager.SendToServerButton_Clicked(args)

	TypeManager.sendTypeToServer()
	return true
end

function TypeManager.printType()
	local typeInfo = TypeManager.typeAdapter:getSelectedTypeInfo()
	
	if typeInfo ~= nil then
	
		local rawTypeData = emberOgre:getEntityFactory():getAuthoringManager():getRawTypeInfoRepository():getRawTypeData(typeInfo:getName())
	
		if rawTypeData:isValid() then
		
			local outstream = std.stringstream:new_local()
			local decoder = Atlas.Message.QueuedDecoder:new_local()
		
			local codec = TypeManager.codecClass:new_local(outstream, tolua.cast(decoder, "Atlas::Bridge"))
			local formatter = Atlas.Formatter:new_local(outstream, tolua.cast(codec, "Atlas::Bridge"))
			local encoder = Atlas.Message.Encoder:new_local(tolua.cast(formatter, "Atlas::Bridge"))
			local message = Atlas.Message.MapType:new_local()
			rawTypeData:get():addToMessage(message)
			formatter:streamBegin();
			encoder:streamMessageElement(message);
		
			formatter:streamEnd();
		
			TypeManager.typeInfoText:setText(outstream:str())
		end
	end
end

function TypeManager.TypeList_SelectionChanged(args)

	TypeManager.printType()
	return true
end

connect(TypeManager.connectors, emberOgre.EventCreatedAvatarEntity, TypeManager.CreatedAvatarEntity)

