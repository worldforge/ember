TypeManager = {}

function TypeManager:buildWidget()

	self.widget = guiManager:createWidget()

	local setup = function()
		
		self.typeTree = tolua.cast(self.widget:getWindow("TypeList"), "CEGUI::Tree")
		self.typeTree:subscribeEvent("ItemSelectionChanged", self.TypeList_SelectionChanged, self)
	
		self.codecTypeCombobox = CEGUI.toCombobox(self.widget:getWindow("CodecType"))
		
		local item = EmberOgre.Gui.ColouredListItem:new("XML", 0)
		self.codecTypeCombobox:addItem(item)
		item = EmberOgre.Gui.ColouredListItem:new("Bach", 1)
		self.codecTypeCombobox:addItem(item)
		item = EmberOgre.Gui.ColouredListItem:new("Packed", 2)
		self.codecTypeCombobox:addItem(item)
		self.codecTypeCombobox:setItemSelectState(0, true)
		self.codecTypeCombobox:setSingleClickEnabled(true)
		self.codecTypeCombobox:subscribeEvent("ListSelectionChanged", self.CodecType_ListSelectionChanged, self)
	
		
		self.typeInfoText = CEGUI.toMultiLineEditbox(self.widget:getWindow("TypeInfoText"))
	
		self.typeAdapter = EmberOgre.Gui.Adapters.Eris.TypeTreeAdapter:new_local(emberServices:getServerService():getConnection():getTypeService(), self.typeTree)
		self.typeAdapter:initialize("root")
		
		self.widget:getWindow("SendToServerButton"):subscribeEvent("Clicked", self.SendToServerButton_Clicked, self)
			
		self.widget:enableCloseButton()
	end

	connect(self.connectors, self.widget.EventFirstTimeShown, setup)
	self.widget:loadMainSheet("TypeManager.layout", "TypeManager/")
	self.widget:registerConsoleVisibilityToggleCommand("typeManager")

end

function TypeManager:CodecType_ListSelectionChanged()
	local item = self.codecTypeCombobox:getSelectedItem()
	if item ~= nil then
		local selectId = item:getID()
		if selectId == 0 then
			self.codecClass = Atlas.Codecs.XML
		elseif selectId == 1 then
			self.codecClass = Atlas.Codecs.Bach
		else
			self.codecClass = Atlas.Codecs.Packed
		end
		self:printType()
	end
end

function TypeManager:sendTypeToServer()
	local outstream = std.stringstream:new_local(self.typeInfoText:getText())
	local decoder = EmberOgre.Authoring.AtlasObjectDecoder:new_local()

	local codec = self.codecClass:new_local(outstream, tolua.cast(decoder, "Atlas::Bridge"))
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

function TypeManager:SendToServerButton_Clicked(args)

	self:sendTypeToServer()
	return true
end

function TypeManager:printType()
	local typeInfo = self.typeAdapter:getSelectedTypeInfo()
	
	if typeInfo ~= nil then
	
		local rawTypeData = self.world:getAuthoringManager():getRawTypeInfoRepository():getRawTypeData(typeInfo:getName())
	
		if rawTypeData:isValid() then
		
			local outstream = std.stringstream:new_local()
			local decoder = Atlas.Message.QueuedDecoder:new_local()
		
			local codec = self.codecClass:new_local(outstream, tolua.cast(decoder, "Atlas::Bridge"))
			local formatter = Atlas.Formatter:new_local(outstream, tolua.cast(codec, "Atlas::Bridge"))
			local encoder = Atlas.Message.Encoder:new_local(tolua.cast(formatter, "Atlas::Bridge"))
			local message = Atlas.Message.MapType:new_local()
			rawTypeData:get():addToMessage(message)
			formatter:streamBegin();
			encoder:streamMessageElement(message);
		
			formatter:streamEnd();
		
			self.typeInfoText:setText(outstream:str())
		end
	end
end

function TypeManager:TypeList_SelectionChanged(args)

	self:printType()
	return true
end

function TypeManager:shutdown()
	disconnectAll(self.connectors)
	guiManager:destroyWidget(self.widget)
end

TypeManager.createdAvatarConnector = EmberOgre.LuaConnector:new_local(emberOgre.EventWorldCreated):connect(function(world)
		typeManager = {connectors={}, codecClass=Atlas.Codecs.XML, world=world}
		setmetatable(typeManager, {__index = TypeManager})
		
		typeManager:buildWidget()
		connect(typeManager.connectors, emberOgre.EventWorldDestroyed, function()
				typeManager:shutdown()
				typeManager = nil
			end
		)
	end
)


