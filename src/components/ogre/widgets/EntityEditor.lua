--Allows the editing of entities

EntityEditor = {

	adapters = {
		map = {
			name = "Map",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("VerticalLayoutContainer")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createMapAdapter(wrapper.container, self.instance.entity:getId(), element)
				if wrapper.adapter == nil then
					return nil
				end

				local attributeNames = wrapper.adapter:getAttributeNames()
				for i = 0, attributeNames:size() - 1 do
					local name = attributeNames[i]
					local childElement = wrapper.adapter:valueOfAttr(name)
					local adapterWrapper = self:createAdapter(name, childElement)
					if adapterWrapper then
						if adapterWrapper.adapter then
							self:addNamedAdapterContainer(name, adapterWrapper.adapter, adapterWrapper.container, wrapper.container, adapterWrapper.prototype)
							wrapper.adapter:addAttributeAdapter(name, adapterWrapper.adapter, adapterWrapper.outercontainer)
						end
					end
				end

				if prototype.readonly == nil then
					local newElementWrapper = self.adapters.map.createNewElementWidget(self, wrapper.adapter, wrapper.container, element)
					wrapper.container:addChildWindow(newElementWrapper.container)
				end
				return wrapper
			end,
			createNewElement = function(self)
				return self.instance.helper:createMapElement()
			end,
			createNewElementWidget = function(self, mapAdapter, outercontainer, outerElement)
				local wrapper = {}
				wrapper.adapter = mapAdapter
				wrapper.outercontainer = outercontainer
				wrapper.container = guiManager:createWindow("DefaultWindow")
				self.factory:loadLayoutIntoContainer(wrapper.container, "newNamedElement", "adapters/atlas/MapAdapterNewElement.layout")
				wrapper.button = CEGUI.toPushButton(windowManager:getWindow(self.factory:getCurrentPrefix().. "NewElementButton"))
				wrapper.container:setHeight(CEGUI.UDim(0, 25))
				wrapper.typeCombobox = CEGUI.toCombobox(windowManager:getWindow(self.factory:getCurrentPrefix().. "ElementType"))
				local helpWindow = windowManager:getWindow(self.factory:getCurrentPrefix().. "ElementHelp")
				wrapper.newAdapters = self:fillNewElementCombobox(wrapper.typeCombobox, "", outerElement)

				--Depending on whether we have suggestions or not we'll either show a combobox or an editbox.
				local nameEditboxCombobox = CEGUI.toCombobox(windowManager:getWindow(self.factory:getCurrentPrefix().. "ElementName_combobox"))
				local nameEditboxEditbox = CEGUI.toEditbox(windowManager:getWindow(self.factory:getCurrentPrefix().. "ElementName_editbox"))

				local checkSuggestions = function()
					nameEditboxCombobox:resetList()
					for attr,value in pairsByKeys(self.prototypes) do
						if mapAdapter:hasAdapter(attr) == false and value.shouldAddSuggestion then
							if value.shouldAddSuggestion(outerElement, self.instance.entity) then
								local item = Ember.OgreView.Gui.ColouredListItem:new(attr)
								nameEditboxCombobox:addItem(item)
							end
						end
					end

					if nameEditboxCombobox:getItemCount() > 0 then
						wrapper.nameEditbox = nameEditboxCombobox
						nameEditboxEditbox:setVisible(false)
						nameEditboxCombobox:setVisible(true)
					else
						wrapper.nameEditbox = nameEditboxEditbox
						nameEditboxCombobox:setVisible(false)
						nameEditboxEditbox:setVisible(true)
					end
				end
				checkSuggestions()

				wrapper.nameChanged = function(args)
					local attribute = wrapper.nameEditbox:getText()
					--check that the name doesn't already exists in the map adapter
					if mapAdapter:hasAdapter(attribute) then
						wrapper.button:setEnabled(false)
					else
						wrapper.newAdapters = self:fillNewElementCombobox(wrapper.typeCombobox, attribute, outerElement)
						wrapper.buttonEnableChecker(args)
					end
					if self.prototypes[attribute] and self.prototypes[attribute].help then
						helpWindow:setText(self.prototypes[attribute].help)
					end
					return true
				end
				nameEditboxCombobox:getEditbox():subscribeEvent("TextChanged", wrapper.nameChanged)
				nameEditboxEditbox:subscribeEvent("TextChanged", wrapper.nameChanged)

				nameEditboxCombobox:subscribeEvent("ListSelectionChanged", function(args)
					if nameEditboxCombobox:getSelectedItem() and self.prototypes[nameEditboxCombobox:getSelectedItem():getText()].help then
						helpWindow:setText(self.prototypes[nameEditboxCombobox:getSelectedItem():getText()].help)
					else
						helpWindow:setText("")
					end
					return true;
				end)


				wrapper.buttonEnableChecker = function(args)
					if wrapper.typeCombobox:getSelectedItem() ~= nil and wrapper.nameEditbox:getText() ~= "" then
						wrapper.button:setEnabled(true)
					else
						wrapper.button:setEnabled(false)
					end
					return true
				end
				wrapper.typeCombobox:subscribeEvent("ListSelectionChanged", wrapper.buttonEnableChecker)

				wrapper.buttonPressed = function(args)
					local name = wrapper.nameEditbox:getText()
					local newAdapter = wrapper.newAdapters[wrapper.typeCombobox:getSelectedItem():getID()]
					if newAdapter.createNewElement then
						local element = newAdapter.createNewElement(self)
						local adapterWrapper = newAdapter.createAdapter(self, element, self:getPrototype(name, element))

						self.instance.addNewElement(self, element)

						if adapterWrapper then
							local newPrototype = {}
							wrapper.adapter:addAttributeAdapter(name, adapterWrapper.adapter, adapterWrapper.outercontainer)
							self:addNamedAdapterContainer(name, adapterWrapper.adapter, adapterWrapper.container, wrapper.outercontainer, newPrototype)
							--by adding the window again we make sure that it's at the bottom of the child window list
							wrapper.outercontainer:addChildWindow(wrapper.container)
							nameEditboxCombobox:getEditbox():setText("")
							nameEditboxEditbox:setText("")
							helpWindow:setText("")
							checkSuggestions()
						end
					end
					return true
				end
				wrapper.buttonSubscriber = wrapper.button:subscribeEvent("Clicked", wrapper.buttonPressed)

				wrapper.buttonEnableChecker(nil)

				return wrapper
			end
		},
		list = {
			name = "List",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("VerticalLayoutContainer")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createListAdapter(wrapper.container, self.instance.entity:getId(), element)
				if wrapper.adapter == nil then
					return nil
				end
				for i = 0, wrapper.adapter:getSize() - 1 do
					local childElement = wrapper.adapter:valueOfAttr(i)
					local childPrototype = self:getPrototype("", childElement)
					--if the prototype for the list have it marked as nodelete, mark the child elements too
					if prototype.readonly then
						childPrototype.readonly = true
						childPrototype.nodelete = true
					end
					local adapterWrapper = self:createAdapterFromPrototype(childElement, childPrototype)
					if adapterWrapper then
						self:addUnNamedAdapterContainer(adapterWrapper.adapter, adapterWrapper.container, wrapper.container, adapterWrapper.prototype)
						wrapper.adapter:addAttributeAdapter(adapterWrapper.adapter, adapterWrapper.outercontainer)
					end
				end

				if prototype.readonly == nil then
					local newElementWrapper = self.adapters.list.createNewElementWidget(self, wrapper.adapter, wrapper.container)
					wrapper.container:addChildWindow(newElementWrapper.container)
				end

				return wrapper
			end,
			createNewElement = function(self)
				return self.instance.helper:createListElement()
			end,
			createNewElementWidget = function(self, listAdapter, outercontainer, prototype)
				local wrapper = {}
				wrapper.adapter = listAdapter
				wrapper.outercontainer = outercontainer
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				self.factory:loadLayoutIntoContainer(wrapper.container, "newUnnamedElement", "adapters/atlas/ListAdapterNewElement.layout")
				wrapper.button = CEGUI.toPushButton(windowManager:getWindow(self.factory:getCurrentPrefix().. "NewElementButton"))
				wrapper.container:setHeight(CEGUI.UDim(0, 25))
				wrapper.typeCombobox = CEGUI.toCombobox(windowManager:getWindow(self.factory:getCurrentPrefix().. "ElementType"))
				wrapper.newAdapters = self:fillNewElementCombobox(wrapper.typeCombobox, "")
				wrapper.buttonPressed = function(args)
					local newAdapter = wrapper.newAdapters[wrapper.typeCombobox:getSelectedItem():getID()]
					if newAdapter.createNewElement then
						local element = newAdapter.createNewElement(self)
						local adapterWrapper = newAdapter.createAdapter(self, element, self:getPrototype("", element))

						--store a reference to the element so it isn't garbage collected
						self.instance.addNewElement(self, element)

						if adapterWrapper then
							wrapper.adapter:addAttributeAdapter(adapterWrapper.adapter, adapterWrapper.outercontainer)
							local newPrototype = {}
							self:addUnNamedAdapterContainer(adapterWrapper.adapter, adapterWrapper.container, wrapper.outercontainer, newPrototype)
							--by adding the window again we make sure that it's at the bottom of the child window list
							wrapper.outercontainer:addChildWindow(wrapper.container)
						end
					end
				end
				wrapper.button:subscribeEvent("Clicked", wrapper.buttonPressed)

				wrapper.buttonEnableChecker = function(args)
					if wrapper.typeCombobox:getSelectedItem() then
						wrapper.button:setEnabled(true)
					else
						wrapper.button:setEnabled(false)
					end
					return true
				end
				wrapper.typeCombobox:subscribeEvent("ListSelectionChanged", wrapper.buttonEnableChecker)

				wrapper.buttonEnableChecker(nil)

				return wrapper
			end
		},
		static = {
			name = "Static",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createStaticAdapter(wrapper.container, self.instance.entity:getId(), element)
				return wrapper
			end
		},
		size = {
			name = "Size",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createSizeAdapter(wrapper.container, self.instance.entity:getId(), element)
				return wrapper
			end,
			createNewElement = function(self)
				return self.instance.helper:createListElement()
			end
		},
		position = {
			name = "Position",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createPositionAdapter(wrapper.container, self.instance.entity:getId(), element)
				wrapper.moveButtonPressed = function()
					guiManager:EmitEntityAction("move", self.instance.entity)
				end
				wrapper.moveButtonPressedListener = createConnector(wrapper.adapter.EventMoveClicked):connect(wrapper.moveButtonPressed)
				return wrapper
			end
		},
		position2d = {
			name = "Position",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createPosition2DAdapter(wrapper.container, self.instance.entity:getId(), element)
				return wrapper
			end,
			createNewElement = function(self)
				return self.instance.helper:createPosition2dElement()
			end
		},
		orientation = {
			name = "Orientation",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createOrientationAdapter(wrapper.container, self.instance.entity:getId(), element)
				return wrapper
			end
		},
		points = {
			name = "Points",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("VerticalLayoutContainer")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createListAdapter(wrapper.container, self.instance.entity:getId(), element)
				if wrapper.adapter == nil then
					return nil
				end
				for i = 0, wrapper.adapter:getSize() - 1 do
					local childElement = wrapper.adapter:valueOfAttr(i)
					local childPrototype = self.adapters.position2d
					local adapterWrapper = self:createAdapterFromPrototype(childElement, childPrototype)
					if adapterWrapper then
						self:addUnNamedAdapterContainer(adapterWrapper.adapter, adapterWrapper.container, wrapper.container, adapterWrapper.prototype)
						wrapper.adapter:addAttributeAdapter(adapterWrapper.adapter, adapterWrapper.outercontainer)
					end
				end

				local newElementWrapper = self.adapters.points.createNewElementWidget(self, wrapper.adapter, wrapper.container)
				wrapper.container:addChildWindow(newElementWrapper.container)

				return wrapper
			end,
			createNewElement = function(self)
				return self.instance.helper:createListElement()
			end,
			createNewElementWidget = function(self, listAdapter, outercontainer, prototype)
				local wrapper = {}
				wrapper.adapter = listAdapter
				wrapper.outercontainer = outercontainer
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				self.factory:loadLayoutIntoContainer(wrapper.container, "newUnnamedElement", "adapters/atlas/ListAdapterNewElement.layout")
				wrapper.container:setHeight(CEGUI.UDim(0, 25))
				wrapper.typeCombobox = CEGUI.toCombobox(windowManager:getWindow(self.factory:getCurrentPrefix().. "ElementType"))

				local item = Ember.OgreView.Gui.ColouredListItem:new("Point", 0)
				wrapper.typeCombobox:addItem(item)
				wrapper.typeCombobox:setHeight(CEGUI.UDim(0, 100))
				--combobox:setProperty("ReadOnly", "true")

				wrapper.button = CEGUI.toPushButton(windowManager:getWindow(self.factory:getCurrentPrefix().. "NewElementButton"))
				wrapper.buttonPressed = function(args)
					local newAdapter = newAdapters[wrapper.typeCombobox:getSelectedItem():getID()]
					if newAdapter.createNewElement then
						local element = newAdapter.createNewElement(self)
						local adapterWrapper = newAdapter.createAdapter(self, element, self:getPrototype("", element))

						--[[					local adapterWrapper = nil
						local element = nil

						if wrapper.typeCombobox:getSelectedItem():getID() == 0 then
						element = self.instance.helper:createPosition2dElement()
						adapterWrapper = self.adapters.position2d.createAdapter(self, element, self:getPrototype("", element))
						end]]

						self.instance.addNewElement(self, element)

						if adapterWrapper then
							local newPrototype = {}
							wrapper.adapter:addAttributeAdapter(adapterWrapper.adapter, adapterWrapper.outercontainer)
							self:addUnNamedAdapterContainer(adapterWrapper.adapter, adapterWrapper.container, wrapper.outercontainer, newPrototype)
							--by adding the window again we make sure that it's at the bottom of the child window list
							wrapper.outercontainer:addChildWindow(wrapper.container)
						end
					end
				end
				wrapper.button:subscribeEvent("Clicked", wrapper.buttonPressed)

				return wrapper
			end

		},
		string = {
			name = "String",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createStringAdapter(wrapper.container, self.instance.entity:getId(), element)
				return wrapper
			end,
			createNewElement = function(self)
				return self.instance.helper:createStringElement()
			end
		},
		number = {
			name = "Number",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createNumberAdapter(wrapper.container, self.instance.entity:getId(), element)
				return wrapper
			end
		},
		float = {
			name = "Float",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createNumberAdapter(wrapper.container, self.instance.entity:getId(), element)
				return wrapper
			end,
			createNewElement = function(self)
				return self.instance.helper:createFloatElement()
			end

		},
		integer = {
			name = "Integer",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createNumberAdapter(wrapper.container, self.instance.entity:getId(), element)
				return wrapper
			end,
			createNewElement = function(self)
				return self.instance.helper:createIntElement()
			end
		},
		boolean = {
			name = "Boolean",
			--We'll use an integer adapter and UI for now until we've created a checkbox UI
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createNumberAdapter(wrapper.container, self.instance.entity:getId(), element)
				return wrapper
			end,
			createNewElement = function(self)
				return self.instance.helper:createIntElement()
			end
		},
		area = {
			name = "Area",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createAreaAdapter(wrapper.container, self.instance.entity:getId(), element, self.instance.entity)

				wrapper.adapter:addAreaSuggestion(0, "none")

				--fill the area adapter with suggested areas, which we get from the terrain layer definitions
				local layerDefinitions = Ember.OgreView.Terrain.TerrainLayerDefinitionManager:getSingleton():getDefinitions()
				for index,value in layerDefinitions:ipairs() do
					if value:getAreaId() ~= 0 then
						local name = value:getName()
						--fall back to the area id if there's no name given
						if name == "" then
							name = value:getAreaId()
						end
						wrapper.adapter:addAreaSuggestion(value:getAreaId(), name)
					end
				end
				return wrapper
			end,
			createNewElement = function(self)
				return self.instance.helper:createMapElement()
			end
		},
		terrainmod = {
			name = "Terrain mod",
			createAdapter = function(self, element, prototype)
				local wrapper = {}
				wrapper.container = guiManager:createWindow("DefaultWindow")
				wrapper.container:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
				wrapper.adapter = self.factory:createTerrainModAdapter(wrapper.container, self.instance.entity:getId(), element, self.instance.entity)
				return wrapper
			end,
			createNewElement = function(self)
				return self.instance.helper:createMapElement()
			end
		}
	}
}
EntityEditor.prototypes =
{
	external = {
		adapter = EntityEditor.adapters.static,
		help = "Marks the entity as externally controlled."
	},
	parents = {
		nodelete = true,
		-- 		adapter = EntityEditor.adapters.static
		adapter = nil
	},
	objtype = {
		nodelete = true,
		adapter = EntityEditor.adapters.static,
		help = "The type of object in the world. Should always be 'obj' and not editable."
	},
	velocity = {
		adapter = nil,
		help = "The current velocity of the entity.",
	},
	stamp = {
		adapter = nil,
		nodelete = true
	},
	name = {
		adapter = EntityEditor.adapters.string,
		help = "The name of the entity.",
		nodelete = true
	},
	bbox = {
		adapter = EntityEditor.adapters.size,
		help = "Defines the 3d bounding box of the entity. This influences the size, which also influences visibility.",
		nodelete = true
	},
	pos = {
		adapter = EntityEditor.adapters.position,
		help = "The 3d position of the entity.",
		nodelete = true
	},
	orientation = {
		adapter = EntityEditor.adapters.orientation,
		help = "The 3d orientation of the entity.",
		nodelete = true
	},
	calendar = {
		adapter = EntityEditor.adapters.map,
		help = "Specifies the calendar for the world. Should only be applied to the top entity.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level and for the top entity
			return ownerElement == nil and entity:getId() == "0"
		end
	},
	area = {
		adapter = EntityEditor.adapters.area,
		help = "An area, which will be applied to the terrain.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	points = {
		adapter = EntityEditor.adapters.points,
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	style = {
		adapter = EntityEditor.adapters.string,
		suggestions = {
			"gnarly",
			"knotted",
			"weathered"
		}
	},
	terrain = {
		--TODO: supply a terrain adapter which opens the terrain editor
		adapter = EntityEditor.adapters.map,
		help = "Defines the terrain. Currently only applicable to the top level entity.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level and for the top entity
			return ownerElement == nil and entity:getId() == "0"
		end
	},
	terrainmod = {
		adapter = EntityEditor.adapters.terrainmod,
		help = "Allows the entity to perform modifications to the terrain.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	stamina = {
		adapter = EntityEditor.adapters.float,
		help = "The current stamina of the entity. Only applicable to living things.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	decays = {
		adapter = EntityEditor.adapters.string,
		help = "Specify the entity type which will be generated when this entity is deleted.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	outfit = {
		adapter = EntityEditor.adapters.map,
		help = "Determines how contained entities are attached. Often used for clothing.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	solid = {
		adapter = EntityEditor.adapters.boolean,
		help = "Determines if the entity is solid, i.e. reacts to collisions.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end,
		suggestions = {
			0,1
		}
	},
	simple = {
		adapter = EntityEditor.adapters.boolean,
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end,
		suggestions = {
			0,1
		}
	},
	status = {
		adapter = EntityEditor.adapters.float,
		help = "The current status of the entity. If the status reaches zero the entity is destroyed.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	mode = {
		adapter = EntityEditor.adapters.string,
		help = "Positioning mode. Available values:\n none: regular physics applies\nfixed: position is fixed in space\nfloating: floats to top of liquids\nprojectile: flies as a projectile\nswimming: free movement in liquids",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end,
		suggestions = {
			" ",
			"fixed",
			"floating",
			"projectile",
			"swimming"
		}
	},
	statistics = {
		adapter = EntityEditor.adapters.map,
		help = "Allows for game rule specific values.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	biomass = {
		adapter = EntityEditor.adapters.float,
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	burn_speed = {
		adapter = EntityEditor.adapters.float,
		help = "Determines how fast the entity burns when set on fire. This will affect 'status' and 'mass'.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	transient = {
		adapter = EntityEditor.adapters.float,
		help = "Makes the entity ephemeral. Any value higher than zero indicates the number of seconds until the entity is auto deleted.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	food = {
		adapter = EntityEditor.adapters.float,
		help = "The current amount of food in the entities stomach. This interacts with metabolism and is only applicable to living entities.",
		shouldAddSuggestion = function(ownerElement, entity)
			--TODO: check that the entity is a living entity
			--only show on top level
			return ownerElement == nil
		end
	},
	mass = {
		adapter = EntityEditor.adapters.float,
		help = "The mass of the entity, in kilograms.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	ticks = {
		adapter = EntityEditor.adapters.float,
		help = "Makes the world send a Tick operation to the entity at the specified interval (in seconds).",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	spawn = {
		--TODO: make spawn adapter
		adapter = EntityEditor.adapters.map,
		help = "Defines a new spawn point, in which new entities can be created.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	visibility = {
		adapter = EntityEditor.adapters.float,
		help = "Allows overriding of visibility calculated by the 'bbox' property.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	},
	linked = {
		adapter = EntityEditor.adapters.string,
		help = "Make this entity act as a teleportation device to another server.",
		shouldAddSuggestion = function(ownerElement, entity)
			--only show on top level
			return ownerElement == nil
		end
	}
}
EntityEditor.defaultPrototypes =
{
	string = {
		adapter = EntityEditor.adapters.string
	},
	integer = {
		adapter = EntityEditor.adapters.integer
	},
	float = {
		adapter = EntityEditor.adapters.float
	},
	list = {
		adapter = EntityEditor.adapters.list
	},
	map = {
		adapter = EntityEditor.adapters.map
	}
}

EntityEditor.goalPrototypes = {
	mason = {
		welcome = {
			proto="welcome(message, type)",
			args = {message="The greeting message.",type="The type of entity to react to."},
			help = "Welcome entities of a given type that are created nearby."
		},
		add_help={
			proto="add_help([messages], [responses])",
			args = {messages="The messages which will be spoken.",type="An optional list of suggested responses."},
			help = "Set off a help goal if we get a touch operation."
		},
		hire_trade={
			proto="hire_trade()",
			help="Allows to be hired for one day. Price is governed by knowledge of 'price' for 'services'.",
			knowledge={
				{predicate="price", subject="services", help="Determines the price of services for one day."}
			}
		},
		buy_from={
			proto="buy_from(what, cost, who)",
			args = {what="What type of entity to buy.", cost="What to pay at most.", who="From whom."},
			help="Buy entities from somone."
		},
		buy_livestock={
			proto="buy_livestock(what, cost)",
			args={what="What type of entity to buy.", cost="What to pay per kilo."},
			help="Buy livestock by the kilo."
		},
		keep_livestock={
			proto="keep_livestock(what, where, call)",
			args={what="What type of livestock to keep.", where="Where it should be kept.", call="How to call for livestock to return."},
			help="Keep livestock that we own in a given location, calling them if required."
		},
		gather={
			proto="gather(what)",
			args={what="The entity type to gather."},
			help="Gather freely available resources."
		},
		harvest_resource={
			proto="harvest_resource(what, source, place, tool)",
			args={what="What to harvest.", source="?", place="A place where to harvest.", tool="The tool used to harvest." },
			help="Gather something from a given location, by using a tool on something."
		},
		plant_seeds={
			proto="plant_seeds(what, source, place, tool)",
			args={what="The seed type.", source="From where we can obtain the seed.", place="A place where to plant", tool="The tool used to plant." },
			help="Use a tool to plant a given kind of seed in a given location."
		},
		move_me={
			proto="move_me(location)",
			args={location="The location to move to."},
			help="Move me to a certain place."
		},
		move_me_area={
			proto="move_me(location, range)",
			args={location="The location to move to.", range="Size of area from location."},
			help="Move me to a certain area."
		},
		move_me_place={
			proto="move_me_place(place)",
			args={place="A named place."},
			help="Move me to a place by name."
		},
		move_it_outof_me={
			proto="move_it_outof_me(what)",
			args={what="The entity type to move out of me."},
			help="Put something down."
		},
		move_me_to_possession={
			proto="move_me_to_possession(what)",
			args={what="The place I own."},
			help="Move me to the same place as something I own."
		},
		move_me_to_focus={
			proto="move_me_to_focus([what])",
			args={what="The types that I'm interested in."},
			help="Move me to something I am interested in."
		},
		pick_up_possession={
			proto="pick_up_possession(what)",
			args={what="The type of thing I own."},
			help="Pick up something I own."
		},
		pick_up_focus={
			proto="pick_up_focus([what])",
			args={what="What I want to pick up."},
			help="Pick up something I am interested in."
		},
		wander={
			proto="wander()",
			help="Move in a non-specific way."
		},
		search={
			proto="search(what)",
			args={what="What to search for."},
			help="Move in a non-specific way looking for something."
		},
		avoid={
			proto="avoid([what], range)",
			args={what="What to avoid.", range="?"},
			help="Avoid something at range."
		},
		hunt={
			proto="hunt([what], range)",
			args={what="What to hunt.", range="?"},
			help="Hunt something at range."
		},
		hunt_for={
			proto="hunt_for(what, range, proximity)",
			args={what="What to hunt.", range="?", proximity="?"},
			help="Hunt something at range."
		},
		patrol={
			proto="patrol([locations])",
			args={locations="A list of locations to visit."},
			help="Move around an area defined by some waypoints."
		},
		accompany={
			proto="accompany(who)",
			args={who="Who to follow."},
			help="Move around staying close to someone."
		},
		driven={
			proto="driven()",
			help="Move away from a herder when touched."
		},
		summons= {
			proto="summons(verb)",
			args={verb="The summoning verb."},
			help="Stop moving when the herder gives a cry."
		},
		school= {
			proto="school([members])",
			args={members="A list of members in the school."},
			help="Move in a shoal with other animals of the same type."
		},
		herd={
			proto="herd([members])",
			args={members="A list of members in the herd."},
			help="Move in a herd with other animals of the same type."
		}
	}
}

EntityEditor.knowledge= {
	predicates = {
		mason = {
			location = {help="Provides a location. This can either be the name of a known thing, or a point in space. For the latter, use this format: \"('entityid',(x,y,z))\"."},
			importance = {help="Makes one goal more important than another. The 'subject' should be of the format \"('goal1', 'goal2')\" and the object either '>' or '<'."},
			about = {help="Know something about a subject."},
			price = {help="Know the price of a thing."}
		}
	}
}
function editEntity(id)
	local entity = emberOgre:getWorld():getEmberEntity(id)
	if entity then
		self:editEntity(entity)
	end
end

function EntityEditor:clearEditing()
	if self.instance then

		if self.instance.entity then
			--as we're not editing anymore, hide the bounding boxes
			self.world:getAuthoringManager():hideSimpleEntityVisualization(self.instance.entity)
			self.instance.entity:setVisualize("OgreBBox", false)
		end

		--we want to disconnect all stackable containers before we start
		for index,value in ipairs(self.instance.stackableContainers) do
			value:disconnect()
		end

		if self.instance.rootMapAdapter then
			self.instance.rootMapAdapter:removeAdapters()
		end
		if self.instance.outercontainer then
			windowManager:destroyWindow(self.instance.outercontainer)
		end
		if self.instance.deleteListener then
			self.instance.deleteListener:disconnect()
		end
		if self.instance.entityChangeConnection then
			self.instance.entityChangeConnection:disconnect()
		end
		if self.instance.helper then
			self.instance.helper:removeMarker()
		end
		deleteSafe(self.instance.helper)
		self.instance = nil
	end
	self.instance = {knowledge={model={}}}
	self.instance.stackableContainers = {}
	self.instance.newElements = {}
	self.instance.addNewElement = function(self, element)
		table.insert(self.instance.newElements, element)
	end
	self.exportFilenameWindow:setText("")
	self.exportNameWindow:setText("")
	self.exportDescriptionWindow:setText("")


end

function EntityEditor:editEntity(entity)
	self.widget:show()
	self.widget:getMainWindow():setVisible(true)

	self:clearEditing()

	self.instance.model = {}
	self.instance.entity = entity

	--show the bounding boxes by default when editing
	self.instance.entity:setVisualize("OgreBBox", false)
	self.world:getAuthoringManager():displaySimpleEntityVisualization(self.instance.entity)

	self.instance.deleteListener = createConnector(entity.BeingDeleted):connect(self.Entity_BeingDeleted, self)


	self:refreshChildren(entity)
	self:refreshModelInfo(entity)


	self.instance.entityChangeConnection = createConnector(entity.Changed):connect(self.Entity_Changed, self)
	self.instance.outercontainer = guiManager:createWindow("VerticalLayoutContainer")
	self.instance.outercontainer:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
	local adapter = self.factory:createMapAdapter(self.instance.outercontainer, self.instance.entity:getId(), self.instance.entity)
	self.instance.rootMapAdapter = adapter
	self.instance.helper = Ember.OgreView.Gui.EntityEditor:new(self.world, entity, self.instance.rootMapAdapter)
	self.attributesContainer:addChildWindow(self.instance.outercontainer)

	local attributeNames = self.instance.rootMapAdapter:getAttributeNames()
	for i = 0, attributeNames:size() - 1 do
		local name = attributeNames[i]
		local element = self.instance.rootMapAdapter:valueOfAttr(name)
		local prototype = self:getPrototype(name, element)
		--there's currently no way to delete from the root attributes, so we'll just disallow that
		prototype.nodelete = true
		local adapterWrapper = self:createAdapterFromPrototype(element, prototype)
		if adapterWrapper then
			self.instance.rootMapAdapter:addAttributeAdapter(name, adapterWrapper.adapter, adapterWrapper.outercontainer)
			self:addNamedAdapterContainer(name, adapterWrapper.adapter, adapterWrapper.container, self.instance.outercontainer, adapterWrapper.prototype)
		end
	end
	self.instance.model.newAdapter = self.adapters.map.createNewElementWidget(self, adapter, self.instance.outercontainer)
	--Set the height of the new property adapter to be enough to contain the combobox dropdown.
	--The reason for this is that else the horizontal scroll bar of the scrollable pane will overlap the dropdown (this might be a bug in CEGUI though).
	self.instance.model.newAdapter.container:setHeight(CEGUI.UDim(0, 100))
	self.instance.outercontainer:addChildWindow(self.instance.model.newAdapter.container)


	self.infoWindow:setText('Id: ' .. entity:getId() .. ' Name: ' .. entity:getName())

	self.knowledgelistbox:resetList()
	self.goallistbox:resetList()

	local exportFileName = "entityexport_" .. entity:getId() .. ".xml"
	self.exportFilenameWindow:setText(exportFileName)

	self.instance.goals = {}

	createConnector(self.instance.helper.EventGotThought):connect(function(element)
		if element:isMap() then
			if self.instance.clearThoughts then
				self.knowledgelistbox:resetList()
				self.goallistbox:resetList()
				self.instance.clearThoughts = false
				self.instance.goals = {}
			end
			local thoughtMap = element:asMap()
			local item = CEGUI.toItemEntry(windowManager:createWindow("EmberLook/ItemEntry"))

			local modelItem = {predicate = "", subject = "", object = ""}
			if thoughtMap:get("predicate") and thoughtMap:get("predicate"):isString() then
				modelItem.predicate = thoughtMap:get("predicate"):asString()
				if thoughtMap:get("subject") and thoughtMap:get("subject"):isString() then
					modelItem.subject = thoughtMap:get("subject"):asString()
					if thoughtMap:get("object") then
						local object = thoughtMap:get("object")

						if modelItem.predicate ~= "goal" then
							if object:isString() then
								modelItem.object = thoughtMap:get("object"):asString()
								item:setText(escapeForCEGUI(modelItem.predicate .. " : " .. modelItem.subject .. " : ".. modelItem.object))

								item:subscribeEvent("SelectionChanged", function(args)
									if item:isSelected() then
										local predicate = self.widget:getWindow("NewKnowledgePredicate")
										local subject = self.widget:getWindow("NewKnowledgeSubject")
										local knowledge = self.widget:getWindow("NewKnowledgeKnowledge")

										predicate:setText(modelItem.predicate)
										subject:setText(modelItem.subject)
										knowledge:setText(modelItem.object)

										self:handleKnowledgeSelected(modelItem)
									end

									return true
								end)

								item:setID(#self.instance.knowledge.model)
								table.insert(self.instance.knowledge.model, modelItem)
								self.knowledgelistbox:addItem(item)
							end
						else
							--Handle goals specially

							local verb = modelItem.subject
							local _, _, singleVerb = string.find(modelItem.subject, "'(%a*)'.*")
							if singleVerb then
								verb = singleVerb
							end

							local addGoal = function(object)

								local goalItem = CEGUI.toItemEntry(windowManager:createWindow("EmberLook/ItemEntry"))
								goalItem:setText(escapeForCEGUI(verb .. " : " .. object))
								self.goallistbox:addItem(goalItem)

								goalItem:subscribeEvent("SelectionChanged", function(args)
									self.goalInfo:setText("")
									if goalItem:isSelected() then
										local goalVerb = self.widget:getWindow("GoalVerb")
										local goalDef = self.widget:getWindow("GoalDefinition")

										goalVerb:setText(verb)
										goalVerb.verb = verb

										goalDef:setText(object)

										self.instance.helper:getGoalInfo(modelItem.subject, object)
									end

									return true
								end)
								return goalItem
							end

							local goals = {}

							if object:isString() then
								addGoal(object:asString())

								table.insert(goals, object:asString())
							elseif object:isList() then
								local goalsList = object:asList()
								for i = 0, goalsList:size() - 1 do
									local anObject = goalsList[i]
									if anObject:isString() then
										addGoal(anObject:asString()):setID(i + 1)
										table.insert(goals, anObject:asString())
									end
								end
							end
							self.instance.goals[verb] = goals
						end
					end
				end
			end
		end
	end)

	self:knowledgeRefresh()


	createConnector(self.instance.helper.EventGotGoalInfo):connect(function(element)
		if element:isMap() then
			local goalMap = element:asMap()
			local reportElem = goalMap:get("report")
			if reportElem and reportElem:isMap() then
				local goalString = Ember.OgreView.Gui.EntityEditor:parseElementMap(reportElem:asMap())
				self.goalInfo:setText(escapeForCEGUI(goalString))
			end
		end
	end)


end

function EntityEditor:createAdapter(attributeName, element)
	local prototype = self:getPrototype(attributeName, element)
	return self:createAdapterFromPrototype(element, prototype)
end

function EntityEditor:createAdapterFromPrototype(element, prototype)
	local adapterWrapper = nil
	if prototype.adapter then
		adapterWrapper = prototype.adapter.createAdapter(self, element, prototype)
		if adapterWrapper then
			if prototype.suggestions then
				for index,value in ipairs(prototype.suggestions) do
					adapterWrapper.adapter:addSuggestion(value)
				end
			end
			adapterWrapper.prototype = prototype
		end
	end
	return adapterWrapper
end

function EntityEditor:getPrototype(attributeName, element)
	local prototype = {}
	if self.prototypes[attributeName] then
		prototype = self.prototypes[attributeName]
	else
		if element:isString() then
			prototype.adapter = self.adapters.string
		elseif element:isNum() then
			prototype.adapter = self.adapters.number
		elseif element:isMap() then
			prototype.adapter = self.adapters.map
		elseif element:isList() then
			prototype.adapter = self.adapters.list
		end
	end
	return prototype
end


function EntityEditor:addUnNamedAdapterContainer(adapter, container, parentContainer, prototype)
	local outercontainer = guiManager:createWindow("DefaultWindow")
	outercontainer:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))

	local deleteButton = nil
	local deleteButtonWidth = 0
	if prototype.nodelete == nil then
		deleteButton = self:createDeleteButton("list")
		deleteButton:setProperty("UnifiedPosition", "{{0,0},{0,2}}")
		deleteButton:setProperty("Tooltip", "Delete list item");
		deleteButtonWidth = 16

		function removeAdapter(args)
			adapter:remove()
			outercontainer:setAlpha(0.2)
		end
		deleteButton:subscribeEvent("Clicked", removeAdapter)
	end

	local width = container:getWidth()
	--increase with delete button width
	width = width + CEGUI.UDim(0, deleteButtonWidth)
	outercontainer:setWidth(width)

	outercontainer:setHeight(container:getHeight())

	container:setXPosition(CEGUI.UDim(0, deleteButtonWidth))

	--make sure that the outer container has the same height as the inner container (so that when we add new child adapters it's updated)
	function syncWindowHeights(args)
		outercontainer:setHeight(container:getHeight())
	end
	local SizedConnection = container:subscribeEvent("Sized", syncWindowHeights)

	if deleteButton then
		outercontainer:addChildWindow(deleteButton)
	end
	outercontainer:addChildWindow(container)

	parentContainer:addChildWindow(outercontainer)
	return outercontainer
end

function EntityEditor:addNamedAdapterContainer(attributeName, adapter, container, parentContainer, prototype)
	local textWidth = 75
	local outercontainer = guiManager:createWindow("DefaultWindow")
	outercontainer:setMaxSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(0,6000)))
	--outercontainer:setRiseOnClickEnabled(false)
	local label = guiManager:createWindow("EmberLook/StaticText")

	local tooltip = attributeName
	if prototype and prototype.help then
		tooltip = attributeName .. ": " .. prototype.help
	end


	label:setText(attributeName)
	label:setWidth(CEGUI.UDim(0, textWidth))
	label:setProperty("FrameEnabled", "false");
	label:setProperty("BackgroundEnabled", "false");
	label:setProperty("VertFormatting", "TopAligned");
	label:setProperty("Tooltip", tooltip);

	local width = container:getWidth()
	width = width + CEGUI.UDim(0, textWidth)
	outercontainer:setWidth(width)
	container:setXPosition(CEGUI.UDim(0, textWidth))
	container:setProperty("Tooltip", tooltip);

	outercontainer:setHeight(container:getHeight())

	--make sure that the outer container has the same height as the inner container (so that when we add new child adapters it's updated)
	function syncWindowHeights(args)
		outercontainer:setHeight(container:getHeight())
	end
	local SizedConnection = container:subscribeEvent("Sized", syncWindowHeights)

	if prototype.nodelete == nil then

		local deleteButton = self:createDeleteButton(attributeName)
		deleteButton:setProperty("UnifiedPosition", "{{1,-16},{0,2}}")
		deleteButton:setProperty("Tooltip", "Delete '" .. attributeName .. "'");

		-- 	function showDeleteButton(args)
		-- 		console:pushMessage("wee")
		-- 		deleteButton:setVisible(true)
		-- 	end
		-- 	function hideDeleteButton(args)
		-- 		console:pushMessage("waa")
		-- 		deleteButton:setVisible(false)
		-- 	end
		-- 	outercontainer:subscribeEvent("MouseEnter", showDeleteButton)
		-- 	outercontainer:subscribeEvent("MouseLeave", hideDeleteButton)

		function removeAdapter(args)
			adapter:remove()
			outercontainer:setAlpha(0.2)
		end
		deleteButton:subscribeEvent("Clicked", removeAdapter)

		label:addChildWindow(deleteButton)
	end

	outercontainer:addChildWindow(label)
	outercontainer:addChildWindow(container)

	parentContainer:addChildWindow(outercontainer)
	return outercontainer
end

function EntityEditor:createDeleteButton(attributeName)
	local deleteButton = guiManager:createWindow("EmberLook/SystemButton")
	deleteButton:setProperty("NormalImage", "set:EmberLook image:CloseButtonNormal")
	deleteButton:setProperty("HoverImage", "set:EmberLook image:CloseButtonHover")
	deleteButton:setProperty("PushedImage", "set:EmberLook image:CloseButtonPushed")
	deleteButton:setProperty("UnifiedSize", "{{0,16},{0,16}}")
	deleteButton:setAlpha(0.5)
	return deleteButton
end

function EntityEditor:fillNewElementCombobox(combobox, elementName, outerElement)

	combobox:resetList()
	local newAdapters = {}

	local possibleProto = self.prototypes[elementName]
	--Only add prototype adapter type if it's available and marked for suggestion for this element
	if possibleProto and possibleProto.adapter and (possibleProto.shouldAddSuggestion == nil or possibleProto.shouldAddSuggestion(outerElement, self.instance.entity)) then
		local itemIndex = table.maxn(newAdapters) + 1

		local item = Ember.OgreView.Gui.ColouredListItem:new(possibleProto.adapter.name, itemIndex)
		table.insert(newAdapters, possibleProto.adapter)
		combobox:addItem(item)
	else
		--Use the default adapters

		for index,value in pairsByKeys(self.defaultPrototypes) do
			local itemIndex = table.maxn(newAdapters) + 1
			local item = Ember.OgreView.Gui.ColouredListItem:new(value.adapter.name, itemIndex)
			table.insert(newAdapters, value.adapter)
			combobox:addItem(item)
		end
	end

	--check that our previous selection is still available
	local selectedItem = combobox:findItemWithText(combobox:getText(), nil)
	if selectedItem == nil then
		if combobox:getItemCount() == 1 then
			combobox:getListboxItemFromIndex(0):setSelected(true)
			combobox:setText(combobox:getSelectedItem():getText())
		else
			combobox:clearAllSelections()
			combobox:setText("")
		end
	else
		selectedItem:setSelected(true)
		combobox:setText(combobox:getSelectedItem():getText())
	end

	combobox:setProperty("ReadOnly", "true")
	return newAdapters
end



function EntityEditor:Submit_Clicked(args)
	self.instance.helper:submitChanges()
	--we want to update the next time a change comes from the server
	self.listenForChanges = true
	--self:editEntity(self.instance.entity)
	return true
end

function EntityEditor:DeleteButton_Clicked(args)
	if self.instance then
		local entity = self.instance.entity
		if entity then
			-- 	self:clearEditing()
			emberServices:getServerService():deleteEntity(entity)
		end
	end
	return true
end

function EntityEditor:RefreshAtlas_Clicked(args)
	if self.instance then
		local entity = self.instance.entity
		if entity then
			local ss = std.stringstream:new_local()
			local ss_log = std.stringstream:new_local()
			entity:dumpAttributes(ss, ss_log.__std__ostream__)
			self.widget:getWindow("AtlasTextbox"):setText(ss:str())
		end
	end
	return true

end

function EntityEditor:handleKnowledgeSelected(modelItem)
	if modelItem.predicate == "location" then
		_, _, entityid, x, y, z = string.find(modelItem.object, "%('(%d*)',%s*%(([%d%-%.]*),%s*([%d%-%.]*),%s*([%d%-%.]*)%)%)")

		if (entityid and x and y and z) then
			local point = Ember.OgreView.Gui.EntityEditor:createPoint(tonumber(x), tonumber(y), tonumber(z))
			self.instance.helper:addMarker(entityid, point)
		else
			self.instance.helper:removeMarker()
		end
	else
		self.instance.helper:removeMarker()
	end
end

function EntityEditor:knowledgeRefresh()
	self.instance.clearThoughts = true
	self.instance.helper:getThoughts()
end

function EntityEditor:RefreshKnowledge_Clicked(args)
	self:knowledgeRefresh()
	return true
end

function EntityEditor:NewKnowledge_Clicked(args)
	local predicate = self.widget:getWindow("NewKnowledgePredicate")
	local subject = self.widget:getWindow("NewKnowledgeSubject")
	local knowledge = self.widget:getWindow("NewKnowledgeKnowledge")
	self.instance.helper:addKnowledge(predicate:getText(), subject:getText(), knowledge:getText())
	self:knowledgeRefresh()
	return true
end

function EntityEditor:RefreshGoals_Clicked(args)
	self:knowledgeRefresh()
	return true
end

function EntityEditor:GoalAdd_Clicked(args)
	local goalVerb = self.widget:getWindow("GoalVerb")
	local goalDef = self.widget:getWindow("GoalDefinition")

	local goals = std.vector_std__string_:new_local()
	local existingGoals = self.instance.goals[goalVerb:getText()]
	if existingGoals then
		for i, v in ipairs(existingGoals) do
			goals:push_back(v)
		end
	end
	goals:push_back(goalDef:getText())

	self.instance.helper:setGoals(goalVerb:getText(), goals)
	self:knowledgeRefresh()
	return true
end

function EntityEditor:GoalUpdate_Clicked(args)
	local goalVerb = self.widget:getWindow("GoalVerb")
	local goalDef = self.widget:getWindow("GoalDefinition")

	local goals = std.vector_std__string_:new_local()


	local selectedItem = self.goallistbox:getFirstSelectedItem()
	if selectedItem then
		local editingIndex = selectedItem:getID()
		local existingGoals = self.instance.goals[goalVerb:getText()]
		if existingGoals then
			for i, v in ipairs(existingGoals) do
				if i == editingIndex then
					goals:push_back(goalDef:getText())
				else
					goals:push_back(v)
				end
			end
		else
			--this should not really be possible; if we've selected a goal in the list it should exist in the list of goals
			--we'll keep it anyway just as a precaution
			goals:push_back(goalDef:getText())
		end

		self.instance.helper:setGoals(goalVerb:getText(), goals)
		self:knowledgeRefresh()
	end

	return true
end

--[[
Removes a goal. This is done by sending an updated list of all the goal with the same verb as the selected, minus the selected.
]]--
function EntityEditor:GoalRemove_Clicked(args)

	local goalVerb = self.widget:getWindow("GoalVerb")

	local goals = std.vector_std__string_:new_local()

	local selectedItem = self.goallistbox:getFirstSelectedItem()
	if selectedItem then
		editingIndex = selectedItem:getID()

		--we've set the 'verb' property of the goalVerb textbox when the goal item was selected
		local existingGoals = self.instance.goals[goalVerb.verb]
		if existingGoals then
			for i, v in ipairs(existingGoals) do
				if i ~= editingIndex then
					goals:push_back(v)
				end
			end
			self.instance.helper:setGoals(goalVerb.verb, goals)
			self:knowledgeRefresh()
		end
	end

	return true
end

function EntityEditor:RefreshButton_Clicked(args)
	if self.instance.entity then
		self:editEntity(self.instance.entity)
	end
	return true
end

function EntityEditor:ShowOgreBbox_CheckStateChanged(args)
	if self.instance.entity then
		self.instance.entity:setVisualize("OgreBBox", self.modelTab.showOgreBbox:isSelected())
	end
	return true
end

function EntityEditor:ShowErisBbox_CheckStateChanged(args)
	if self.instance.entity then
		if self.modelTab.showErisBbox:isSelected() then
			self.world:getAuthoringManager():displaySimpleEntityVisualization(self.instance.entity)
		else
			self.world:getAuthoringManager():hideSimpleEntityVisualization(self.instance.entity)
		end
	end
	return true
end

function EntityEditor:ChildList_MouseDoubleClick(args)
	local entityId = self.childlistbox:getFirstSelectedItem():getID()
	editEntity(entityId)
	return true
end



function EntityEditor:handleAction(action, entity)

	if action == "edit" then
		self:editEntity(entity)
	end
end

function EntityEditor:refreshChildren(entity)
	self.childListholder:resetList()
	local numContained = entity:numContained()
	if numContained ~= 0 then
		for i = 0, numContained - 1 do
			local childEntity = entity:getContained(i)
			local label = childEntity:getName()

			local item = Ember.OgreView.Gui.ColouredListItem:new(label, childEntity:getId(), childEntity)
			self.childListholder:addItem(item)
		end
	end
end

function EntityEditor:refreshModelInfo(entity)
	local showOgreBbox = entity:getVisualize("OgreBBox")
	self.modelTab.showOgreBbox:setSelected(showOgreBbox)
	local showErisBbox = self.world:getAuthoringManager():hasSimpleEntityVisualization(self.instance.entity)
	self.modelTab.showErisBbox:setSelected(showErisBbox)
end

function EntityEditor:Entity_Changed(attributes)
	--only update if we're actively listening (for example right after an update)
	if self.listenForChanges then
		self.listenForChanges = false
		self:editEntity(self.instance.entity)
	end
end

--we need to clean up when the entity is deleted, so we don't cause segfaults when trying to access a null ref
function EntityEditor:Entity_BeingDeleted()
	self:clearEditing()
end


function EntityEditor:buildWidget()
	self.widget = guiManager:createWidget()
	local setup = function()

		self.factory = Ember.OgreView.Gui.Adapters.Atlas.AdapterFactory:new("EntityEditor")

		self.attributesContainer = self.widget:getWindow("AttributesContainer")
		self.infoWindow = self.widget:getWindow("EntityInfo")

		self.childlistbox = CEGUI.toListbox(self.widget:getWindow("ChildList"))
		--EntityBrowser.childlistbox:subscribeEvent("ItemSelectionChanged", "EntityBrowser.EntityList_SelectionChanged")

		self.childlistFilter = CEGUI.toEditbox(self.widget:getWindow("FilterChildren"))
		self.childListholder = Ember.OgreView.Gui.ListHolder:new(self.childlistbox, self.childlistFilter)

		self.goallistbox = CEGUI.toItemListbox(self.widget:getWindow("GoalList"))

		self.knowledgelistbox = CEGUI.toItemListbox(self.widget:getWindow("KnowledgeList"))

		--[[	self.modelTab.stackableWindow = self.widget:getWindow("ModelPanelStackable")
		self.modelTab.stackableContainer = Ember.OgreView.Gui.StackableContainer:new_local(self.modelTab.stackableWindow)
		self.modelTab.stackableContainer:setInnerContainerWindow(self.modelTab.stackableWindow)]]
		self.modelTab.showOgreBbox = CEGUI.toCheckbox(self.widget:getWindow("ShowOgreBbox"))
		self.modelTab.showErisBbox = CEGUI.toCheckbox(self.widget:getWindow("ShowErisBbox"))
		self.modelTab.modelInfo = self.widget:getWindow("ModelInfo")



		self.widget:getWindow("ChildList"):subscribeEvent("DoubleClick", self.ChildList_MouseDoubleClick, self)
		self.widget:getWindow("ShowOgreBbox"):subscribeEvent("CheckStateChanged", self.ShowOgreBbox_CheckStateChanged, self)
		self.widget:getWindow("ShowErisBbox"):subscribeEvent("CheckStateChanged", self.ShowErisBbox_CheckStateChanged, self)
		self.widget:getWindow("RefreshAtlas"):subscribeEvent("Clicked", self.RefreshAtlas_Clicked, self)
		self.widget:getWindow("RefreshKnowledge"):subscribeEvent("Clicked", self.RefreshKnowledge_Clicked, self)
		self.widget:getWindow("NewKnowledgeAdd"):subscribeEvent("Clicked", self.NewKnowledge_Clicked, self)
		self.widget:getWindow("RefreshGoals"):subscribeEvent("Clicked", self.RefreshGoals_Clicked, self)
		
		local knowledgePredicate = CEGUI.toCombobox(self.widget:getWindow("NewKnowledgePredicate"))
		local knowledgeHelp = self.widget:getWindow("KnowledgeHelp")
		
		for k, v in pairsByKeys(EntityEditor.knowledge.predicates.mason) do
			local item = Ember.OgreView.Gui.ColouredListItem:new(k)
			knowledgePredicate:addItem(item)
		end
		knowledgePredicate:subscribeEvent("ListSelectionAccepted", function(args)
			knowledgeHelp:setVisible(false)
			return true
		end)
		knowledgePredicate:subscribeEvent("MouseLeavesArea", function(args)
			knowledgeHelp:setVisible(false)
			return true
		end)		
		knowledgePredicate:subscribeEvent("ListSelectionChanged", function(args)
			local selectedItem = knowledgePredicate:getSelectedItem()
			if selectedItem then
				local prototype = EntityEditor.knowledge.predicates.mason[selectedItem:getText()]
				if prototype and prototype.help then
					knowledgeHelp:setText(prototype.help)
					knowledgeHelp:setVisible(true)
				else
					knowledgeHelp:setVisible(false)
				end
			end
			return true
		end)
		
		
		
--		knowledgePredicate:subscribeEvent("ListSelectionAccepted", function(args)
--			local selectedItem = knowledgePredicate:getSelectedItem()
--			if selectedItem then
--				local prototype = EntityEditor.knowledge.predicates.mason[selectedItem:getText()]
--				if prototype then
--					self.goalDefinition:setText(prototype.proto)
--				end
--			end
--
--			return true
--		end)




		self.goalVerb = CEGUI.toCombobox(self.widget:getWindow("GoalVerb"))
		self.goalDefinition = self.widget:getWindow("GoalDefinition")


		local goalHelp = self.widget:getWindow("GoalHelp")
		self.goalInfo = self.widget:getWindow("GoalInfo")
		self.goalAdd = CEGUI.toPushButton(self.widget:getWindow("GoalAdd"))
		self.goalUpdate = CEGUI.toPushButton(self.widget:getWindow("GoalUpdate"))
		self.goalRemove = CEGUI.toPushButton(self.widget:getWindow("GoalRemove"))
		self.goalAdd:subscribeEvent("Clicked", self.GoalAdd_Clicked, self)
		self.goalUpdate:subscribeEvent("Clicked", self.GoalUpdate_Clicked, self)
		self.goalRemove:subscribeEvent("Clicked", self.GoalRemove_Clicked, self)

		local goalListUpdateFunction = function(args)
			if self.goallistbox:getSelectedCount() > 0 then
				self.goalUpdate:setEnabled(true)
				self.goalRemove:setEnabled(true)
			else
				self.goalInfo:setText("")
				self.goalUpdate:setEnabled(false)
				self.goalRemove:setEnabled(false)
			end
			return true
		end

		self.goallistbox:subscribeEvent("SelectionChanged", goalListUpdateFunction)
		self.goallistbox:subscribeEvent("ListItemsChanged", goalListUpdateFunction)


		self.goalUpdate:setEnabled(false)
		self.goalRemove:setEnabled(false)

		local goalPrototypes = self.goalPrototypes.mason
		for k, v in pairsByKeys(goalPrototypes) do
			local item = Ember.OgreView.Gui.ColouredListItem:new(k)
			self.goalVerb:addItem(item)
		end
		self.goalVerb:subscribeEvent("ListSelectionAccepted", function(args)
			local selectedItem = self.goalVerb:getSelectedItem()
			if selectedItem then
				self.goallistbox:clearAllSelections()
				local prototype = goalPrototypes[selectedItem:getText()]
				if prototype then
					self.goalDefinition:setText(prototype.proto)
				end
			end
			return true
		end)
		self.goalVerb:subscribeEvent("ListSelectionChanged", function(args)
			local selectedItem = self.goalVerb:getSelectedItem()
			if selectedItem then
				local prototype = goalPrototypes[selectedItem:getText()]
				if prototype and prototype.help then
					goalHelp:setVisible(true)
					goalHelp:setText(prototype.help)
				else
					goalHelp:setVisible(false)
				end
			end
			return true
		end)
		
		self.goalDefinition:subscribeEvent("MouseEntersArea", function(args)
			local verb = self.goalVerb:getText()
			if verb then
				local prototype = goalPrototypes[verb]
				if prototype and prototype.help then
					goalHelp:setVisible(true)
					goalHelp:setText(prototype.help)
				else
					goalHelp:setVisible(false)
				end
			end
			return true
		end)
		self.goalDefinition:subscribeEvent("MouseLeavesArea", function(args)
			goalHelp:setVisible(false)
			return true
		end)
		self.goalVerb:subscribeEvent("MouseLeavesArea", function(args)
			goalHelp:setVisible(false)
			return true
		end)



		self.widget:getWindow("Submit"):subscribeEvent("Clicked", self.Submit_Clicked, self)
		self.widget:getWindow("DeleteButton"):subscribeEvent("Clicked", self.DeleteButton_Clicked, self)
		self.widget:getWindow("RefreshButton"):subscribeEvent("Clicked", self.RefreshButton_Clicked, self)


		self.exportFilenameWindow = self.widget:getWindow("ExportFileName")
		self.exportNameWindow = self.widget:getWindow("ExportName")
		self.exportDescriptionWindow = self.widget:getWindow("ExportDescription")

		local worldDumper = function()

			local exportsOverlay = self.widget:getWindow("ExportsOverlay")

			local cancelButton = self.widget:getWindow("DumpCancel")
			cancelButton:subscribeEvent("Clicked", function(args)
				if cancelButton.method then
					cancelButton.method()
					return true
				end
			end)

			self.widget:getWindow("DumpWorld"):subscribeEvent("Clicked", function(args)

				local filename = self.exportFilenameWindow:getText()

				if filename ~= "" then
					local worldDumper = Ember.EntityExporter:new(emberServices:getServerService():getAccount())
					worldDumper:setDescription(self.exportDescriptionWindow:getText())
					worldDumper:setName(self.exportNameWindow:getText())
					self.widget:getWindow("DumpStatus"):setText("Dumping...")
					createConnector(worldDumper.EventCompleted):connect(function()
						self.widget:getWindow("DumpStatus"):setText("Done dumping.")
						exportsOverlay:setVisible(false)
						cancelButton.method = nil
						worldDumper:delete()
					end)
					createConnector(worldDumper.EventProgress):connect(function(entitiesDumped)
						self.widget:getWindow("DumpStatus"):setText("Dumping, " .. entitiesDumped .. " dumped")
					end)
					cancelButton.method = function()
						worldDumper:cancel()
						self.widget:getWindow("DumpStatus"):setText("Cancelled")
					end
					exportsOverlay:setVisible(true)
					worldDumper:start(emberServices:getConfigService():getHomeDirectory() .. "/entityexport/" .. filename, self.instance.entity:getId())

				end
				return true
			end)

		end
		worldDumper()
		self.widget:enableCloseButton()
	end

	connect(self.connectors, self.widget.EventFirstTimeShown, setup)
	connect(self.connectors, guiManager.EventEntityAction, self.handleAction, self)


	self.widget:loadMainSheet("EntityEditor.layout", "EntityEditor/")
	self.widget:registerConsoleVisibilityToggleCommand("entityEditor")
	self.widget:hide()

end

function EntityEditor:shutdown()
	disconnectAll(self.connectors)
	deleteSafe(self.factory)
	deleteSafe(self.childListholder)
	guiManager:destroyWidget(self.widget)
end


EntityEditor.createdWorldConnector = createConnector(emberOgre.EventWorldCreated):connect(function(world)
	entityEditor = {connectors={},
		instance = {
			stackableContainers = {},
			entity = nil,
			rootMapAdapter = nil,
			helper = nil,
			newElements = {},
			deleteListener = nil,
			model = {}
		},
		factory = nil,
		attributesContainer = nil,
		modelTab = {},
		world = world
	}
	setmetatable(entityEditor, {__index = EntityEditor})

	entityEditor:buildWidget()
	connect(entityEditor.connectors, emberOgre.EventWorldDestroyed, function()
		entityEditor:shutdown()
		entityEditor = nil
	end
	)
end
)

