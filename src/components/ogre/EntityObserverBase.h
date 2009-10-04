/*
 Copyright (C) 2009 Erik Hjortsberg

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

#ifndef ENTITYOBSERVERBASE_H_
#define ENTITYOBSERVERBASE_H_

#include <sigc++/trackable.h>

namespace Eris
{
class Entity;
}

namespace EmberOgre
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Utility class providing entity deletion observation functionality.
 * Often features in the client needs to attach to entities. However, all entities can at any time be deleted, and this requires that these features provide functionality for listening to deletion signals and acting accordingly, often cleaning up.
 * By inheriting from this class such features can simplify the setup needed for detecting entity deletions. At entity deletion time, the method cleanup() will be called automatically.
 * Depending on whether mDeleteOurselvesWhenDone is true of false the instance can then take care of deleting itself.
 * Any subclass should want to implement cleanup(), to perform cleanup functionality when the entity is deleted.
 */
class EntityObserverBase: public virtual sigc::trackable
{
public:
	/**
	 * @brief Ctor.
	 * @param entity The entity to observe.
	 * @param deleteOurselvesWhenDone Whether the class should delete itself when done.
	 */
	EntityObserverBase(Eris::Entity& entity, bool deleteOurselvesWhenDone);

	/**
	 * @brief Dtor.
	 */
	virtual ~EntityObserverBase();

	/**
	 * @brief Deletes this instance. This is called if mDeleteOurselvesWhenDone is set to true.
	 */
	void deleteOurselves();

protected:

	/**
	 * @brief This is called when the entity is deleting.
	 * Default implementation does nothing.
	 * It's expected that subclasses should override this to perform their own cleanup.
	 */
	virtual void cleanup();

private:

	/**
	 * @brief It true, the instance will delete itself when the entity it's observing is deleted.
	 */
	bool mDeleteOurselvesWhenDone;

	/**
	 * @brief Hooked up to Eris::Entity::BeingDeleted.
	 * At deletion time cleanup will be called, and if mDeleteOurselvesWhenDone is true deleteOurselves() will also be called.
	 */
	void entity_BeingDeleted();


};

}

#endif /* ENTITYOBSERVERBASE_H_ */
