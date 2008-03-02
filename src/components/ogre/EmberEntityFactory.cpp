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

#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/TypeInfo.h>
#include <Eris/Avatar.h>

#include "services/server/ServerService.h"
#include "services/EmberServices.h"


#include "EmberEntity.h"
#include "WorldEmberEntity.h"
#include "EmberPhysicalEntity.h"
#include "AvatarEmberEntity.h"
#include "EmberOgre.h"


#include "model/Model.h"
#include "model/ModelDefinition.h"
#include "model/ModelDefinitionManager.h"
#include "model/mapping/EmberModelMappingManager.h"



#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"
#include "terrain/TerrainGenerator.h"

#include "Avatar.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "framework/osdir.h"

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

namespace EmberOgre {


EmberEntityFactory::EmberEntityFactory(Eris::View* view, Terrain::TerrainGenerator* terrainGenerator, Eris::TypeService* typeService)
: ShowModels("showmodels", this, "Show or hide models.")
, DumpAttributes("dump_attributes", this, "Dumps the attributes of a supplied entity to a file. If no entity id is supplied the current avatar will be used.")
, mTerrainGenerator(terrainGenerator)
, mTypeService(typeService)
, mTerrainType(0)
, mWorldEntity(0)
, mView(view)
{
	mView->registerFactory(this);
	
	mTerrainType = mTypeService->getTypeByName("world");
	
	getErisAvatar()->GotCharacterEntity.connect(sigc::mem_fun(*this, &EmberEntityFactory::gotAvatarCharacter));
	
}



EmberEntityFactory::~EmberEntityFactory()
{
/// there is no way to deregister the factory from the View, instead the View will delete the factory when deleted
// 	mView->deregisterFactory(this);
}

/// create whatever entity the client desires
Eris::Entity* EmberEntityFactory::instantiate(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type, Eris::View* w)
{
	
	Eris::Entity* emberEntity(0);

 	bool isPhysical = Model::Mapping::EmberModelMappingManager::getSingleton().getManager().getDefinitionForType(type) != 0;
    
    if (ge->getId() == getErisAvatar()->getId()) {
   	
    	AvatarEmberEntity* avatarEntity = createAvatarEntity(ge, type,  w);
    	emberEntity = avatarEntity;
 
    } else if (type->isA(mTerrainType)) {

    	emberEntity = createWorld(ge, type, w);

    } else if (!isPhysical) {
    	S_LOG_VERBOSE("Creating immaterial entity.");

    	///we don't want this to have any Ogre::Entity
		emberEntity = new EmberEntity(ge->getId(), type, w, EmberOgre::getSingleton().getSceneManager());

    } else {

    	emberEntity = createPhysicalEntity(ge, type, w);
 
    }

	S_LOG_VERBOSE("Entity added to game view.");
	return emberEntity;
}

bool EmberEntityFactory::accept(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type)
{
	return true;
}


Eris::Entity* EmberEntityFactory::createWorld(const Atlas::Objects::Entity::RootEntity & ge, Eris::TypeInfo* type, Eris::View *world) {
	assert(!mWorldEntity);
	mWorldEntity = new WorldEmberEntity(ge->getId(), type, world, EmberOgre::getSingleton().getSceneManager(), mTerrainGenerator);
    return mWorldEntity;
}

WorldEmberEntity* EmberEntityFactory::getWorld() const
{
	return mWorldEntity;
}

void EmberEntityFactory::gotAvatarCharacter(Eris::Entity* entity)
{
	AvatarEmberEntity* avatarEntity = static_cast<AvatarEmberEntity*>(entity);
	EmberOgre::getSingleton().getAvatar()->createdAvatarEmberEntity(avatarEntity);
   	EmberOgre::getSingleton().EventCreatedAvatarEntity.emit(avatarEntity);
}
	





EmberPhysicalEntity* EmberEntityFactory::createPhysicalEntity(const Atlas::Objects::Entity::RootEntity &ge,Eris::TypeInfo* type, Eris::View *world) {
	
	EmberPhysicalEntity* entity = new EmberPhysicalEntity(ge->getId(), type, world, EmberOgre::getSingleton().getSceneManager());
	
	return entity;
	
}

AvatarEmberEntity* EmberEntityFactory::createAvatarEntity(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type, Eris::View *world)
{
    return new AvatarEmberEntity(ge->getId(), type, world,EmberOgre::getSingleton().getSceneManager(), getErisAvatar());
}

int EmberEntityFactory::priority() {
	return 10;
}

Eris::Avatar* EmberEntityFactory::getErisAvatar()
{
	return mView->getAvatar();
}

void EmberEntityFactory::dumpAttributesOfEntity(const std::string& entityId) const
{
	EmberEntity* entity = EmberOgre::getSingleton().getEmberEntity(entityId);
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
	if(command == ShowModels.getCommand())
	{
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
			if (getErisAvatar()) {
				dumpAttributesOfEntity(getErisAvatar()->getEntity()->getId());
			}
		} else {
			dumpAttributesOfEntity(value);
		}
	}
}




}

