/*
 Copyright (C) 2004  Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberEntityFactory.h"

#include "domain/EmberEntity.h"
#include "components/ogre/EmberEntityActionCreator.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#include <Eris/EntityRef.h>
#include <Eris/View.h>
#include <Eris/EventService.h>

#ifdef _WIN32
#include "platform/platform_windows.h"
#else
#endif
using namespace Ember::EntityMapping;
namespace Ember {
namespace OgreView {

EmberEntityFactory::EmberEntityFactory(Eris::View& view, Scene& scene, EntityMappingManager& mappingManager) :
		mView(view),
		mTypeService(view.getTypeService()),
		mScene(scene),
		mMappingManager(mappingManager) {
}

EmberEntityFactory::~EmberEntityFactory() = default;

// create whatever entity the client desires
Eris::ViewEntity* EmberEntityFactory::instantiate(const Atlas::Objects::Entity::RootEntity& ge, Eris::TypeInfo* type, Eris::View& w) {

	auto entity = new EmberEntity(ge->getId(), type, w);
	S_LOG_VERBOSE("Entity " << entity->getId() << " (" << type->getName() << ") added to game view.");
	return entity;
}


bool EmberEntityFactory::accept(const Atlas::Objects::Entity::RootEntity& ge, Eris::TypeInfo* type) {
	return true;
}

int EmberEntityFactory::priority() {
	return 10;
}

}
}

