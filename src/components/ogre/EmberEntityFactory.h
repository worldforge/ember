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

#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#include <Eris/TypeInfo.h>
#include <Eris/Factory.h>
#include <wfmath/point.h>

#include <Eris/Connection.h>

#include <Atlas/Objects/Entity.h>

#include <Ogre.h>

#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif

#include <OgreTerrainSceneManager.h>

namespace DimeOgre {

class AvatarDimeEntity;
class DimeTerrainPageSource;
class DimePhysicalEntity;
class DimeEntity;
class ViewDimeEntity;
class TerrainGenerator;
class WorldDimeEntity;

/*
 * Creates the DimeEntities required.
 * Basically this attaches to Eris and creates Entites on demand.
 * @see Eris::Factory
 * 
 */
class DimeEntityFactory : public Eris::Factory, virtual public SigC::Object
{
public:
	

	typedef std::set<Ogre::String> StringSet;
	

	DimeEntityFactory(Ogre::TerrainSceneManager* sceneManager, TerrainGenerator* terrainGenerator, Eris::TypeService* typeService);
	virtual ~DimeEntityFactory();
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
	DimePhysicalEntity* createPhysicalEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type, Eris::View *world);
	AvatarDimeEntity* createAvatarEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type, Eris::View *world);

	/*
	 * loads data about the different entity types, such as which ones are persons
	 */
	void loadTypeInfo();
	
	void setAvatar(Eris::Avatar* avatar);
	void gotAvatarCharacter(Eris::Entity* entity);



	Ogre::TerrainSceneManager* mSceneManager;
	//DimeTerrainPageSource* mTerrainSource;
	TerrainGenerator* mTerrainGenerator;
	Eris::TypeService* mTypeService;
	Eris::TypeInfo* mTerrainType;
	
	Eris::Avatar* mAvatar;
	
	StringSet mPersonSet;
	
	WorldDimeEntity *mWorldEntity;

	
};

}

#endif // DIMEENTITYFACTORY_H
