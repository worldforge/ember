ActionBarDefaultAction = {}

function ActionBarDefaultAction:wieldCommandObject()
	debugObject("attempting to wield")
	emberServices:getServerService():wield(self.mCommandObject)
end

function ActionBarDefaultAction:setEntityDefaultFunction()
	--TODO: Based on the entity type, set the default function
	self.mDefaultFunction = ActionBarDefaultAction.wieldCommandObject
	debugObject(self.mDefaultFunction)
	debugObject(ActionBarDefaultAction.wieldCommandObject)
end

function ActionBarDefaultAction:getCommandObject(commandObjectIcon)
	--Check if our command object contains an entity.
	if commandObjectIcon:getEntity() ~= nil then
		self.mCommandObject = commandObjectIcon:getEntity()
		self:setEntityDefaultFunction()
	end
end

function ActionBarDefaultAction:executeAction()
	debugObject(self.mDefaultFunction)
	self:mDefaultFunction()
end

function ActionBarDefaultAction:init(commandObjectIcon)
	self:getCommandObject(commandObjectIcon)
end

function ActionBarDefaultAction:new()
	local actionbarDefaultAction = {
			mCommandObject = nil,
			mDefaultFunction = nil,
		}
	setmetatable(actionbarDefaultAction,{__index=ActionBarDefaultAction})
	return actionbarDefaultAction
end