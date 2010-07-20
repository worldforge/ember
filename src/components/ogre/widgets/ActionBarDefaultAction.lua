ActionBarDefaultAction = {}

function ActionBarDefaultAction:wieldCommandObject()
	debugObject("attempting to wield")
	if self.mCommandObject ~= nil then
		emberServices:getServerService():wield(self.mCommandObject)
	end
end

function ActionBarDefaultAction:setEntityDefaultFunction()
	--TODO: Based on the entity type, set the default function
	self.mDefaultFunction = ActionBarDefaultAction.wieldCommandObject
end

function ActionBarDefaultAction:executeAction()
	debugObject(self.mDefaultFunction)
	self:mDefaultFunction()
end

function ActionBarDefaultAction:initFromEntityIcon(entity)
	self.mCommandObject = entity
	self:setEntityDefaultFunction()
end

function ActionBarDefaultAction:new()
	local actionbarDefaultAction = {
			mCommandObject = nil,
			mDefaultFunction = nil,
		}
	setmetatable(actionbarDefaultAction,{__index=ActionBarDefaultAction})
	return actionbarDefaultAction
end