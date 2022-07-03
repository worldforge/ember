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

function DefaultEntityActions:wield(entity)
	mDefaultWearableFunction = function()
		emberOgre:getWorld():getAvatar():getErisAvatar():wield(entity, "hand_primary")
	end
end

function DefaultEntityActions:eat(entity)
	--TODO: initiate the "consume" task
	print("'eat' not currently implemented in the client code")
	--mDefaultWearableFunction = emberServices:getServerService():eat(entity)
end

function DefaultEntityActions.new()
	local defaultEntityActions = {
		mDefaultWearableFunction = nil,
		mDefaultBioMassFunction = nil
	}
	setmetatable(defaultEntityActions, { __index = DefaultEntityActions })
	defaultEntityActions.mDefaultWearableFunction = defaultEntityActions.wield
	defaultEntityActions.mDefaultBioMassFunction = defaultEntityActions.eat
	return defaultEntityActions
end