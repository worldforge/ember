DefaultEntityActions = {}

function DefaultEntityActions:getDefaultWearableFunction()
	return self.mDefaultWearableFunction
end

function DefaultEntityActions:setDefaultWearableFunction(defaultWearableFunction)
	self.mDefaultWearableFunction = defaultWearableFunction
end

function DefaultEntityActions:getDefaultBioMassFunction()
	return self.mDefaultBioMassFunction
end

function DefaultEntityActions:setDefaultBioMassFunction(defaultBioMassFunction)
	self.mDefaultBioMassFunction = defaultBioMassFunction
end

function DefaultEntityActions.new()
	local defaultEntityActions = 
	{
		mDefaultWearableFunction = emberServices:getServerService().wield,
		mDefaultBioMassFunction = emberServices:getServerService().eat
	}
	setmetatable(defaultEntityActions,{__index=DefaultEntityActions})
	return defaultEntityActions
end