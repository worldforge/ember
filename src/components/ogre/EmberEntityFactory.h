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

#include <Ogre.h>
#include <Eris/Entity.h>
#include <Eris/World.h>
#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#include <Eris/TypeInfo.h>
#include <Eris/Factory.h>


/*
#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif
*/


class DimeEntityFactory : public Eris::Factory
{
public:

	DimeEntityFactory(Ogre::SceneManager* sceneManager);
	virtual ~DimeEntityFactory();
/*
 * eris 1.3	
	/// Accept is called by the world to test if this factory can instantiate the specified object
    /** Accept is called when an entity must be constructed; this will be called every time
    an object is created, so avoid lengthy processing if possible. */
/*
    virtual bool accept(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type);

    /// create whatever entity the client desires
    virtual Entity* instantiate(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type, Eris::World* w);
    
    /** retrieve this factory's priority level; higher priority factories
    get first chance to process a recieved Atlas entity. The default implementation
    returns one. */
/*    virtual int priority();
    
*/

	//virtual bool Accept(const Atlas::Message::Element &o) = 0;
	virtual bool accept(const Atlas::Objects::Entity::GameEntity &ge, Eris::World *world);

	/// create whatever entity the client desires
	virtual Eris::Entity* instantiate(const Atlas::Objects::Entity::GameEntity &ge, Eris::World *world);

    
private:
	Ogre::SceneManager* mSceneManager;
};

#endif // DIMEENTITYFACTORY_H
