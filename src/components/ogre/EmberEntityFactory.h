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

#include "EmberOgrePrerequisites.h"
#include "framework/ConsoleObject.h"

#include <Eris/Factory.h>

#include <Atlas/Objects/Entity.h>

#include <sigc++/trackable.h>
#include <set>

namespace Eris
{
class Entity;
class View;
class TypeService;
class TypeInfo;
class Avatar;
}

namespace EmberOgre
{

class EmberEntity;
class ViewEmberEntity;
class WorldEmberEntity;

namespace Authoring
{
class AuthoringManager;
}

/**
 * Creates the EmberEntities required.
 * Basically this attaches to Eris and creates Entites on demand.
 * @see Eris::Factory
 *
 */
class EmberEntityFactory: public Eris::Factory, public sigc::trackable, public Ember::ConsoleObject
{
public:

	typedef std::set<std::string> StringSet;

	/**
	 Default constructor. This should be instantiated by EmberOgre or similiar high level object. Note that Eris upon shutdown will delete all registered factories, so don't delete an instance of this yourself.
	 */
	EmberEntityFactory(Eris::View& view, Eris::TypeService& typeService);
	virtual ~EmberEntityFactory();

	/**
	 Will always return true.
	 */
	virtual bool accept(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type);

	/**
	 Creates instances of EmberEntity and its subclasses.
	 */
	virtual Eris::Entity* instantiate(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type, Eris::View* w);

	/** retrieve this factory's priority level; higher priority factories
	 get first chance to process a recieved Atlas entity. The default implementation
	 returns one. */
	virtual int priority();

	/**
	 Returns the main world entity.
	 */
	WorldEmberEntity* getWorld() const;

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	virtual void runCommand(const std::string &command, const std::string &args);

	/**
	 Command for setting whether models should be shown or not.
	 */
	const Ember::ConsoleCommandWrapper ShowModels;

	/**
	 Dumps the attributes of a supplied entity to the std::out.
	 */
	const Ember::ConsoleCommandWrapper DumpAttributes;

	/**
	 *    Dumps the attributes of the entity with the supplied id to the std::out.
	 * @param entityId
	 * @return
	 */
	void dumpAttributesOfEntity(const std::string& entityId) const;

protected:

	/**
	 Creates a WorldEmberEntity instance.
	 */
	Eris::Entity* createWorld(const Atlas::Objects::Entity::RootEntity & ge, Eris::TypeInfo* type, Eris::View *world);
	/**
	 Creates a avatar instance.
	 */
	EmberEntity* createAvatarEntity(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type, Eris::View *world);

	void gotAvatarCharacter(Eris::Entity* entity);

	Eris::TypeService& mTypeService;
	Eris::TypeInfo* mTerrainType;

	Eris::Avatar* getErisAvatar();

	WorldEmberEntity *mWorldEntity;

	Eris::View& mView;

	Authoring::AuthoringManager* mAuthoringManager;
};

}

#endif // DIMEENTITYFACTORY_H
