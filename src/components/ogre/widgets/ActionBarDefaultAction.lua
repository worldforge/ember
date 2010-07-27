--[[
Responsible for managing the default action executed when a user clicks on an action bar icon.
]]--

ActionBarDefaultAction = {}

--Executed on entities that can be equipped.
function ActionBarDefaultAction:wieldCommandObject()
	debugObject("attempting to wield")
	if self.mCommandObject ~= nil then
		self.wearFunction = self.mDefaultActionList:getDefaultWearableFunction()
		self:wearFunction(self.mCommandObject)
	end
end

--Based on the entity, we attempt to figure out what action to take
function ActionBarDefaultAction:setEntityDefaultFunction()
	--TODO: Based on the entity type, set the default function
	self.mDefaultFunction = ActionBarDefaultAction.wieldCommandObject
end

--Execute our default action on the command object.
function ActionBarDefaultAction:executeAction()
	self:mDefaultFunction()
end

--We intialize the default action setting the command object to the entity, and attempt to pick the best action to take on it.
function ActionBarDefaultAction:initFromEntityIcon(entity)
	self.mCommandObject = entity
	self:setEntityDefaultFunction()
end

--Create a new action bar actio.
function ActionBarDefaultAction:new(defaultActionList)
	local actionbarDefaultAction = {
			mCommandObject = nil,
			mDefaultFunction = nil,
			mDefaultActionList = defaultActionList
		}
	setmetatable(actionbarDefaultAction,{__index=ActionBarDefaultAction})
	return actionbarDefaultAction
end