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
#include "components/ogre/WorldEmberEntity.h"
#include "components/ogre/Avatar.h"


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


#ifdef WIN32
#include <tchar.h>
#define snprintf _snprintf
#include <io.h> // for _access, Win32 version of stat()
#include <direct.h> // for _mkdir
//	#include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <ostream>
#else
#include <dirent.h>
#endif
using namespace Ember::EntityMapping;
namespace EmberOgre
{

EmberEntityFactory::EmberEntityFactory(Eris::View& view, Scene& scene) :
	ShowModels("showmodels", this, "Show or hide models."), DumpAttributes("dump_attributes", this, "Dumps the attributes of a supplied entity to a file. If no entity id is supplied the current avatar will be used."), mView(view), mTypeService(*view.getAvatar()->getConnection()->getTypeService()), mScene(scene), mTerrainType(0), mWorldEntity(0)
{
	mTerrainType = mTypeService.getTypeByName("world");
}

EmberEntityFactory::~EmberEntityFactory()
{
	EventBeingDeleted();
}

/// create whatever entity the client desires
Eris::Entity* EmberEntityFactory::instantiate(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type, Eris::View* w)
{

	Eris::Entity* emberEntity(0);

	if (type->isA(mTerrainType)) {
		emberEntity = createWorld(ge, type, w);
	} else {
		emberEntity = new EmberEntity(ge->getId(), type, w, mScene);
	}

	S_LOG_VERBOSE("Entity added to game view.");
	return emberEntity;
}

bool EmberEntityFactory::accept(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type)
{
	return true;
}

Eris::Entity* EmberEntityFactory::createWorld(const Atlas::Objects::Entity::RootEntity & ge, Eris::TypeInfo* type, Eris::View *world)
{
	assert(!mWorldEntity);
	mWorldEntity = new WorldEmberEntity(ge->getId(), type, world, mScene);
	return mWorldEntity;
}

WorldEmberEntity* EmberEntityFactory::getWorld() const
{
	return mWorldEntity;
}

int EmberEntityFactory::priority()
{
	return 10;
}

void EmberEntityFactory::dumpAttributesOfEntity(const std::string& entityId) const
{
	EmberEntity* entity = static_cast<EmberEntity*>(mView.getEntity(entityId));
	if (entity) {
		///make sure the directory exists
		std::string dir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "/entityexport/");

		if (!oslink::directory(dir).isExisting()) {
			S_LOG_INFO("Creating directory " << dir);
#ifdef __WIN32__
			mkdir(dir.c_str());
#else
			mkdir(dir.c_str(), S_IRWXU);
#endif
		}

		const std::string fileName(dir + entityId + ".atlas");
		std::fstream exportFile(fileName.c_str(), std::fstream::out);

		S_LOG_INFO("Dumping attributes to " << fileName);
		entity->dumpAttributes(exportFile, std::cout);
		Ember::ConsoleBackend::getSingletonPtr()->pushMessage(std::string("Dumped attributes to ") + fileName);
	}
}

void EmberEntityFactory::runCommand(const std::string &command, const std::string &args)
{
	if (command == ShowModels.getCommand()) {
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string value = tokeniser.nextToken();
		if (value == "true") {
			S_LOG_INFO("Showing models.");
			Model::ModelDefinitionManager::getSingleton().setShowModels(true);
		} else if (value == "false") {
			S_LOG_INFO("Hiding models.");
			Model::ModelDefinitionManager::getSingleton().setShowModels(false);
		}
	} else if (DumpAttributes == command) {
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string value = tokeniser.nextToken();
		if (value == "") {
			dumpAttributesOfEntity(value);
		}
	}
}


}

