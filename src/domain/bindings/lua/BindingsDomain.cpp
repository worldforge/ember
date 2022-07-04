/*
 Copyright (C) 2022 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "BindingsDomain.h"
#include "components/lua/Connector.h"
#include "domain/EmberEntity.h"
#include "domain/EntityTalk.h"
#include "domain/IGraphicalRepresentation.h"

using namespace Ember;
using namespace Ember::Lua;


void registerBindingsDomain(sol::state_view& lua) {
	auto Ember = lua["Ember"].get_or_create<sol::table>();


	auto emberEntity = Ember.new_usertype<EmberEntity>("EmberEntity", sol::no_constructor,
													   "getUsages",
													   [](EmberEntity* self) {
														   std::vector <std::string> usagesNames;
														   for (auto& entry: self->getUsages()) {
															   usagesNames.emplace_back(entry.first);
														   }
														   return usagesNames;
													   },
													   "getUsage",
													   [](EmberEntity* self, const std::string& usage) -> const Eris::Usage* {
														   auto I = self->getUsages().find(usage);
														   if (I != self->getUsages().end()) {
															   return &I->second;
														   }
														   return nullptr;
													   },
													   "getUsagesProtected",
													   [](EmberEntity* self) {
														   std::vector <std::string> usagesNames;
														   for (auto& entry: self->getUsagesProtected()) {
															   usagesNames.emplace_back(entry.first);
														   }
														   return usagesNames;
													   },
													   "getUsageProtected",
													   [](EmberEntity* self, const std::string& usage) -> const Eris::Usage* {
														   auto I = self->getUsagesProtected().find(usage);
														   if (I != self->getUsages().end()) {
															   return &I->second;
														   }
														   return nullptr;
													   },
													   "setVisualize", &EmberEntity::setVisualize,
													   "getVisualize", &EmberEntity::getVisualize,
													   "dumpAttributes", [](EmberEntity* self) {
				std::stringstream ss;
				std::stringstream ss_log;
				self->dumpAttributes(ss, ss_log);
				return ss.str();
			},
													   "getAttachedEntity", &EmberEntity::getAttachedEntity,
													   "setVisualize", &EmberEntity::setVisualize,
													   "setVisualize", &EmberEntity::setVisualize,
													   sol::base_classes, sol::bases<Eris::Entity>()
	);

	emberEntity["EventChangedGraphicalRepresentation"] = LuaConnector::make_property(&EmberEntity::EventChangedGraphicalRepresentation);
	emberEntity["getEmberLocation"] = &EmberEntity::getEmberLocation;
	emberEntity["getEmberContained"] = &EmberEntity::getEmberContained;


	Ember.new_usertype<EntityTalk>("EntityTalk", sol::no_constructor,
								   "getMessage", &EntityTalk::getMessage,
								   "isAddressedToNone", &EntityTalk::isAddressedToNone,
								   "isAddressedToEntity", &EntityTalk::isAddressedToEntity,
								   "isAddressedToEntityOrNone", &EntityTalk::isAddressedToEntityOrNone,
								   "getAddressedEntityIds", &EntityTalk::getAddressedEntityIds,
								   "getSuggestedResponses", &EntityTalk::getSuggestedResponses
	);

	Ember.new_usertype<IGraphicalRepresentation>("IGraphicalRepresentation", sol::no_constructor,
												 "getType", &IGraphicalRepresentation::getType,
												 "setVisualize", &IGraphicalRepresentation::setVisualize,
												 "getVisualize", &IGraphicalRepresentation::getVisualize
	);
}
