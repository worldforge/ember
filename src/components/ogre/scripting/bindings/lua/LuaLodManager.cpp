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
#include "RegisterLua.h"
#include "components/ogre/lod/LodDefinition.h"
#include "components/ogre/lod/LodDefinitionManager.h"
#include "components/ogre/lod/LodManager.h"
#include "components/ogre/lod/PMInjectorSignaler.h"

using namespace Ember::OgreView::Lod;
using namespace Ember::Lua;

template <>
void registerLua<LodManager>(sol::table& space) {
	auto lodDistance = space.new_usertype<LodDistance>("LodDistance", sol::no_constructor);
	lodDistance["meshName"] = &LodDistance::meshName;
	lodDistance["reductionMethod"] = &LodDistance::reductionMethod;
	lodDistance["reductionValue"] = &LodDistance::reductionValue;

	auto lodDefinition = space.new_usertype<LodDefinition>("LodDefinition", sol::no_constructor,
														 sol::base_classes, sol::bases<Ogre::Resource>()
	);
	lodDefinition["getUseAutomaticLod"] = &LodDefinition::getUseAutomaticLod;
	lodDefinition["getUseAutomaticLod"] = &LodDefinition::getUseAutomaticLod;
	lodDefinition["setUseAutomaticLod"] = &LodDefinition::setUseAutomaticLod;
	lodDefinition["getType"] = &LodDefinition::getType;
	lodDefinition["setType"] = &LodDefinition::setType;
	lodDefinition["getStrategy"] = &LodDefinition::getStrategy;
	lodDefinition["setStrategy"] = &LodDefinition::setStrategy;
	lodDefinition["addLodDistance"] = &LodDefinition::addLodDistance;
	lodDefinition["hasLodDistance"] = &LodDefinition::hasLodDistance;
	lodDefinition["getLodDistance"] = &LodDefinition::getLodDistance;
	lodDefinition["getLodDistanceCount"] = &LodDefinition::getLodDistanceCount;
	lodDefinition["createListOfDistances"] = &LodDefinition::createListOfDistances;
	lodDefinition["createDistance"] = &LodDefinition::createDistance;
	lodDefinition["removeLodDistance"] = &LodDefinition::removeLodDistance;


	auto lodType = sol::state_view(space.lua_state()).create_table();
	lodType["LT_AUTOMATIC_VERTEX_REDUCTION"] = Ember::OgreView::Lod::LodDefinition::LT_AUTOMATIC_VERTEX_REDUCTION;
	lodType["LT_USER_CREATED_MESH"] = Ember::OgreView::Lod::LodDefinition::LT_USER_CREATED_MESH;
	lodDefinition["LodType"] = lodType;

	auto lodStrategy = sol::state_view(space.lua_state()).create_table();
	lodStrategy["LS_DISTANCE"] = Ember::OgreView::Lod::LodDefinition::LS_DISTANCE;
	lodStrategy["LS_PIXEL_COUNT"] = Ember::OgreView::Lod::LodDefinition::LS_PIXEL_COUNT;
	lodDefinition["LodStrategy"] = lodStrategy;

	auto lodDefinitionManager = space.new_usertype<LodDefinitionManager>("LodDefinitionManager", sol::no_constructor,
																	   sol::base_classes, sol::bases<Ogre::ResourceManager>()
	);
	lodDefinitionManager["getSingleton"] = &LodDefinitionManager::getSingleton;
	lodDefinitionManager["exportScript"] = &LodDefinitionManager::exportScript;
	lodDefinitionManager["create"] = [](LodDefinitionManager* self, const Ogre::String& name, const Ogre::String& group) { return std::shared_ptr(self->create(name, group)); };
	lodDefinitionManager["getByName"] = [](LodDefinitionManager* self, const Ogre::String& name) { return std::shared_ptr(self->getByName(name)); };


	auto lodManager = space.new_usertype<LodManager>("LodManager", sol::no_constructor);
	lodManager["getSingleton"] = &LodManager::getSingleton;
	lodManager["loadLod"] = [](LodManager* self, const Ogre::MeshPtr& mesh, const LodDefinition& def) { self->loadLod(mesh, def); };
//	lodManager["loadLod"] = sol::resolve(&LodManager::loadLod;


	auto pmInjectorSignaler = space.new_usertype<PMInjectorSignaler>("PMInjectorSignaler", sol::no_constructor);
	pmInjectorSignaler["LodInjected"] = LuaConnector::make_property(&PMInjectorSignaler::LodInjected);
}