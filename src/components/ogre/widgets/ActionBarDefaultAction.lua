--[[ ActionBarDefaultAction
Responsible for managing the default action executed when a user clicks on an action bar icon.
]]--

ActionBarDefaultAction = {}

--Executed on entities that can be equipped.
function ActionBarDefaultAction:wieldableCommandObject()
	if self.mCommandObject ~= nil then
		self.wearFunction = self.mDefaultActionList:getDefaultWearableFunction()
		emberOgre:doWithEntity(self.mCommandObject,function(entity) self:wearFunction(entity) end)
	end
end

--Executed on entities that are edible.
function ActionBarDefaultAction:edibleCommandObject()
	if self.mCommandObject ~= nil then
		self.eatFunction = self.mDefaultActionList:getDefaultBioMassFunction()
		emberOgre:doWithEntity(self.mCommandObject,function(entity) self:eatFunction(entity) end)
	end
end

--Based on the entity, we attempt to figure out what action to take
function ActionBarDefaultAction:setEntityDefaultFunction()
	if emberOgre:getWorld():getEmberEntity(self.mCommandObject):hasProperty("biomass") then
		self.mDefaultFunction = self.edibleCommandObject
	else
		self.mDefaultFunction = self.wieldableCommandObject
	end
end

--Execute our default action on the command object.
function ActionBarDefaultAction:executeAction()
	self:mDefaultFunction()
end

--We intialize the default action setting the command object to the entity, and attempt to pick the best action to take on it.
function ActionBarDefaultAction:initFromEntityIcon(entityId)
	if entityId ~= nil then
		self.mCommandObject = entityId
		self:setEntityDefaultFunction()
	end
end

--Create a new action bar action.
function ActionBarDefaultAction.new(defaultActionList)
	local actionbarDefaultAction = {
			mCommandObject = nil,
			mDefaultFunction = nil,
			mDefaultActionList = defaultActionList
		}
	setmetatable(actionbarDefaultAction,{__index=ActionBarDefaultAction})
	return actionbarDefaultAction
end