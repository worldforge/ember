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

#ifndef DIMEENTITYFACTORY_H
#define DIMEENTITYFACTORY_H
#include <OgreNoMemoryMacros.h>

#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#include <Eris/TypeInfo.h>
#include <Eris/Factory.h>
#include <wfmath/point.h>

#include <Eris/Connection.h>

#include <Atlas/Objects/Entity.h>

#include "EmberOgrePrerequisites.h"

#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif

#include "EmberSceneManager/include/EmberTerrainSceneManager.h"

namespace EmberOgre {

class AvatarEmberEntity;
class EmberTerrainPageSource;
class EmberPhysicalEntity;
class EmberEntity;
class ViewEmberEntity;
class TerrainGenerator;
class WorldEmberEntity;

/*
 * Creates the EmberEntities required.
 * Basically this attaches to Eris and creates Entites on demand.
 * @see Eris::Factory
 * 
 */
class EmberEntityFactory : public Eris::Factory, virtual public SigC::Object
{
public:
	

	typedef std::set<Ogre::String> StringSet;
	

	EmberEntityFactory(EmberTerrainSceneManager* sceneManager, TerrainGenerator* terrainGenerator, Eris::TypeService* typeService);
	virtual ~EmberEntityFactory();
/*
 * eris 1.3	
	/// Accept is called by the world to test if this factory can instantiate the specified object
    /** Accept is called when an entity must be constructed; this will be called every time
    an object is created, so avoid lengthy processing if possible. */

    virtual bool accept(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type);

    /// create whatever entity the client desires
    virtual Eris::Entity* instantiate(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type, Eris::View* w);
    
    /** retrieve this factory's priority level; higher priority factories
    get first chance to process a recieved Atlas entity. The default implementation
    returns one. */
    virtual int priority();
    


/*	//virtual bool Accept(const Atlas::Message::Element &o) = 0;
	virtual bool accept(const Atlas::Objects::Entity::GameEntity &ge, Eris::View *world);

	/// create whatever entity the client desires
	virtual Eris::Entity* instantiate(const Atlas::Objects::Entity::GameEntity &ge, Eris::View *world);*/



    
protected:

	void buildTerrainAroundAvatar();

	Eris::Entity* createWorld(const Atlas::Objects::Entity::GameEntity & ge,Eris::TypeInfo* type, Eris::View *world);
	EmberPhysicalEntity* createPhysicalEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type, Eris::View *world);
	AvatarEmberEntity* createAvatarEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type, Eris::View *world);

	/*
	 * loads data about the different entity types, such as which ones are persons
	 */
	void loadTypeInfo();
	
	void setAvatar(Eris::Avatar* avatar);
	void gotAvatarCharacter(Eris::Entity* entity);



	EmberTerrainSceneManager* mSceneManager;
	//EmberTerrainPageSource* mTerrainSource;
	TerrainGenerator* mTerrainGenerator;
	Eris::TypeService* mTypeService;
	Eris::TypeInfo* mTerrainType;
	
	Eris::Avatar* mAvatar;
	
	StringSet mPersonSet;
	
	WorldEmberEntity *mWorldEntity;

	
};

}

#endif // DIMEENTITYFACTORY_H
