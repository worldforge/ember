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

#include "services/server/ServerService.h"
#include "services/DimeServices.h"

#include "framework/ConsoleBackend.h"
//#include "MathConverter.h"

#include "DimeEntity.h"
#include "DimeEntityFactory.h"

#include "TerrainGenerator.h"
#include "DimeTerrainSceneManager.h"

DimeEntityFactory::DimeEntityFactory(Ogre::SceneManager* sceneManager, Eris::TypeService* typeService ) : mSceneManager(sceneManager), mTypeService(typeService)
{
	mTerrainType = typeService->getTypeByName("world");
}
DimeEntityFactory::~DimeEntityFactory()
{}

/// create whatever entity the client desires
/*eris 1.3
Entity* DimeEntityFactory::instantiate(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type, Eris::World* w)
*/
Eris::Entity* DimeEntityFactory::instantiate(const Atlas::Objects::Entity::GameEntity &ge, Eris::World *world)
{
	
//	dime::ConsoleBackend::getMainConsole()->pushMessage("Adding entity...");

    Eris::TypeInfoPtr type = world->getConnection()->getTypeService()->getTypeForAtlas(ge);
    if (!type->safeIsA(mTerrainType)) {
		//perhaps we should store this somewhere so we can do proper memory handling?
		DimeEntity* dimeEntity = new DimeEntity(ge, world, mSceneManager);
	
		fprintf(stderr, "TRACE - ENTITY ADDED TO THE GAMEVIEW\n");
		return dimeEntity;
    } else {
    	return createWorld(ge, world);
    }
	
}

/// Accept is called by the world to test if this factory can instantiate the specified object
/** Accept is called when an entity must be constructed; this will be called every time
an object is created, so avoid lengthy processing if possible. */
bool DimeEntityFactory::accept(const Atlas::Objects::Entity::GameEntity &ge, Eris::World *world)
{
	return true;
}


Eris::Entity* DimeEntityFactory::createWorld(const Atlas::Objects::Entity::GameEntity & ge, Eris::World *world) {
    Eris::Entity *we = new Eris::Entity(ge, world);
      // Extract base points and send to terrain        
      //TerrainEntity * te = new TerrainEntity(ge,w);
	TerrainGenerator::getSingleton().initTerrain(we, world);
	DimeTerrainSceneManager::getSingleton().buildTerrainAroundAvatar();
    return we;
}

/* namespace Sear */


/* eris 1.3
bool DimeEntityFactory::accept(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type)
{
	return true;
}
*/





/** retrieve this factory's priority level; higher priority factories
get first chance to process a recieved Atlas entity. The default implementation
returns one. */
/*eris 1.3
int DimeEntityFactory::DimeEntityFactory::priority() {
	return 10;
}
*/
