/*
 Copyright (C) 2004  Erik Hjortsberg

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberEntityFactory.h"

#include "components/ogre/EmberEntity.h"
#include "components/ogre/Avatar.h"
#include "components/ogre/EmberEntityActionCreator.h"

#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinition.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/server/ServerService.h"

#include "framework/ConsoleBackend.h"
#include "framework/osdir.h"
#include "framework/Tokeniser.h"

#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/TypeInfo.h>
#include <Eris/Avatar.h>
#include <Eris/Connection.h>

#include <sigc++/bind.h>

#ifdef _WIN32
#include "platform/platform_windows.h"
#else
#include <dirent.h>
#endif
using namespace Ember::EntityMapping;
namespace Ember
{
namespace OgreView
{

EmberEntityFactory::EmberEntityFactory(Eris::View& view, Scene& scene) :
		ShowModels("showmodels", this, "Show or hide models."), mView(view), mTypeService(*view.getAvatar()->getConnection()->getTypeService()), mScene(scene)
{
}

EmberEntityFactory::~EmberEntityFactory()
{
	EventBeingDeleted();
}

// create whatever entity the client desires
Eris::Entity* EmberEntityFactory::instantiate(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type, Eris::View* w)
{

	EmberEntity* entity = new EmberEntity(ge->getId(), type, w);
	//the creator binds the model mapping and this instance together by creating instance of EmberEntityModelAction and EmberEntityPartAction which in turn calls the setModel(..) and show/hideModelPart(...) methods.
	EmberEntityActionCreator creator(*entity, mScene);
	EntityMapping::EntityMapping* mapping = Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(*entity, creator, &mView);
	if (!mapping) {
	    entity->BeingDeleted.connect(sigc::bind(sigc::mem_fun(*this, &EmberEntityFactory::deleteMapping), mapping));
	    mapping->initialize();
	}
	S_LOG_VERBOSE("Entity " << entity->getId() << " (" << type->getName() << ") added to game view.");
	return entity;
}

void EmberEntityFactory::deleteMapping(EntityMapping::EntityMapping* mapping)
{
	delete mapping;
}

bool EmberEntityFactory::accept(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type)
{
	return true;
}

int EmberEntityFactory::priority()
{
	return 10;
}

void EmberEntityFactory::runCommand(const std::string &command, const std::string &args)
{
	if (command == ShowModels.getCommand()) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string value = tokeniser.nextToken();
		if (value == "true") {
			S_LOG_INFO("Showing models.");
			Model::ModelDefinitionManager::getSingleton().setShowModels(true);
		} else if (value == "false") {
			S_LOG_INFO("Hiding models.");
			Model::ModelDefinitionManager::getSingleton().setShowModels(false);
		}
	}
}

}
}

