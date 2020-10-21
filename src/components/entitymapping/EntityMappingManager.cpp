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

#include <components/entitymapping/Matches/SingleAttributeMatch.h>
#include "Matches/AttributeMatch.h"
#include "Cases/AttributeComparers/StringComparerWrapper.h"
#include "EntityMappingManager.h"
#include "IActionCreator.h"

#include "EntityMappingCreator.h"
#include <Eris/View.h>

namespace Ember {

namespace EntityMapping {

using namespace Definitions;

EntityMappingManager::EntityMappingManager() = default;

EntityMappingManager::~EntityMappingManager() = default;

void EntityMappingManager::addDefinition(std::unique_ptr<EntityMappingDefinition> definition) {
	auto name = definition->Name;
	//Overwrite any existing definition.
	mDefinitions[name] = std::move(definition);
}

std::unique_ptr<EntityMapping> EntityMappingManager::createMapping(Eris::Entity& entity, IActionCreator& actionCreator, Eris::TypeService& typeService, Eris::View* view) {
	EntityMappingDefinition* definition = nullptr;
	if (entity.hasProperty("present")) {
		auto mappingElement = entity.valueOfProperty("present");
		if (mappingElement.isString()) {
			auto I = mDefinitions.find(mappingElement.String());
			if (I != mDefinitions.end()) {
				definition = I->second.get();
			}
		}
	}

	if (definition) {
		EntityMappingCreator creator(*definition, entity, actionCreator, typeService, view);
		return creator.create();
	} else {
		auto mapping = std::make_unique<EntityMapping>(entity);

		auto attributeMatch = std::make_unique<Matches::SingleAttributeMatch>("present");
		auto attributeCase = std::make_unique<Cases::AttributeCase>(std::make_unique<Cases::AttributeComparers::StringComparerWrapper>(std::make_unique<Cases::AttributeComparers::StringNotEmptyComparer>()));
		auto observer = std::make_unique<Matches::Observers::MatchAttributeObserver>(*attributeMatch, "present");
		attributeMatch->setMatchAttributeObserver(std::move(observer));

		CaseDefinition caseDefinition;
		ActionDefinition actionDefinition;
		actionDefinition.Type = "present";
		caseDefinition.Actions.emplace_back(std::move(actionDefinition));
		actionCreator.createActions(*mapping, *attributeCase, caseDefinition);
		attributeMatch->addCase(std::move(attributeCase));
		mapping->getBaseCase().addMatch(std::move(attributeMatch));

		mapping->getBaseCase().setEntity(&entity);

		return mapping;
	}
}

}

}
