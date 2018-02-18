//
// C++ Implementation: EntityMappingManager
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityMappingManager.h"

#include "EntityMappingCreator.h"

namespace Ember
{

namespace EntityMapping
{

using namespace Definitions;

EntityMappingManager::EntityMappingManager() :
		mTypeService(nullptr)
{
}

EntityMappingManager::~EntityMappingManager()
{
	for (auto& entry : mDefinitions) {
		delete entry.second;
	}
}

void EntityMappingManager::addDefinition(EntityMappingDefinition* definition)
{
	std::pair<EntityMappingDefinitionStore::iterator, bool> result = mDefinitions.insert(EntityMappingDefinitionStore::value_type(definition->getName(), definition));

	//If it was already added, delete the definition now.
	if (!result.second) {
		delete definition;
	} else {
		for (auto& matchEntry : definition->getRoot().getMatches()) {
			if (matchEntry.getType() == "entitytype") {
				for (auto& aCase : matchEntry.getCases()) {
					for (auto& paramEntry : aCase.getCaseParameters()) {
						if (paramEntry.first == "equals") {
							mEntityTypeMappings[paramEntry.second] = definition;
						}
					}
				}
			}
		}


			/*		const std::string& entityName = I->getProperties()["equals"];
			 std::vector<std::string> splitNames = splitString(entityName, "|", 100);
			 for (std::vector<std::string>::const_iterator I = splitNames.begin(); I != splitNames.end(); ++I) {
			 mEntityTypeMappings[*I] = definition;
			 }*/

	}
}

EntityMappingDefinition* EntityMappingManager::getDefinitionForType(Eris::TypeInfo* typeInfo)
{
	bool noneThere = false;

	while (!noneThere) {
		auto I = mEntityTypeMappings.find(typeInfo->getName());
		if (I != mEntityTypeMappings.end()) {
			return I->second;
		} else {
			if (!typeInfo->getParent()) {
				noneThere = true;
			} else {
				typeInfo = typeInfo->getParent();
			}
		}
	}
	return nullptr;
}

EntityMapping* EntityMappingManager::createMapping(Eris::Entity& entity, IActionCreator& actionCreator, Eris::View* view)
{
	if (mTypeService) {
		EntityMappingDefinition* definition = nullptr;
		if (entity.hasAttr("present")) {
			auto mappingElement = entity.valueOfAttr("present");
			if (mappingElement.isString() && !mappingElement.String().empty()) {
				auto I = mDefinitions.find(mappingElement.String());
				if (I != mDefinitions.end()) {
					definition = I->second;
				}
			}
		}

		if (!definition) {
			definition = getDefinitionForType(entity.getType());
		}

		if (definition) {
			EntityMappingCreator creator(*definition, entity, actionCreator, *mTypeService, view);
			return creator.create();
		}
	}
	return nullptr;
}

}

}
