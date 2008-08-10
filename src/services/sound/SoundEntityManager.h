/*
    Copyright (C) 2008 Romulo Fernandes Machado (nightz)

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

#ifndef SOUND_ENTITY_MANAGER_H
#define SOUND_ENTITY_MANAGER_H

#include <map>

#include "SoundEntity.h"
#include "SoundGroup.h"
#include "framework/Singleton.h"

namespace Ember
{
	/**
	 * The SoundEntityManager class is responsible
	 * to return instances for each entity type and to 
	 * keep track of SoundGroups, allowing then to be updated
	 * when needed.
	 */
	class SoundEntityManager
	: public Ember::Singleton<SoundEntityManager>
	{
		private:
			/**
			 * Hold the information about all entities allocated.
			 */
			std::map<std::string, SoundEntity*> mEntities;

			/**
			 * Keep a list of Groups just to keep the
			 * update on the sound cycle.
			 */
			std::map<std::string, SoundGroup*> mGroups;

			/**
			 * Seeds for instances
			 */
			unsigned int mGroupSeed;
			unsigned int mEntitySeed;

		public:
			SoundEntityManager();
			~SoundEntityManager();

			/**
			 * Allocate an Entity type, referenced by name.
			 *
			 * @param name The Entity name
			 * @return A pointer to the allocated entity, if it fails, returns NULL
			 */
			SoundEntity* allocateEntity(const std::string& name);

			/**
			 * Instantiate an Entity Type
			 *
			 * @param name The Entity name
			 * @return A pointer to the instance entity, if it fails, returns NULL
			 */
			SoundEntity* instantiateEntity(const std::string& name);

			/**
			 * Deallocate entity from its name.
			 *
			 * @param name The Entities Name
			 */
			void deallocateEntity(const std::string& name);

			/**
			 * Ember::Entity use those methods to communicate to
			 * the soundservice indirectly
			 *
			 * @param name The Entity name
			 * @return A pointer to the entity, if it fails, returns NULL
			 */
			SoundEntity* getEntity(const std::string& name);

			/**
			 * Allocate a SoundGroup.
			 *
			 * @param name The Entities name.
			 * @return A pointer to the allocated group, if it fails, returns NULL
			 */
			SoundGroup* allocateGroup(const std::string& name);

			/**
			 * Instantiate a SoundGroup type.
			 *
			 * @param name The Group Name to instantiate
			 * @return A pointer to the instantiated group, if it fails, returns NULL
			 */
			SoundGroup* instantiateGroup(const std::string& name);

			/**
			 * Return a pointer to the SoundGroup from its name
			 *
			 * @param name The Group Name to search for.
			 * @return A pointer to the group, if it cant be found, returns NULL
			 */
			SoundGroup* getGroup(const std::string& name);

			/**
			 * Deallocate a sound group from its name
			 *
			 * @param name The Sound Group name to be deallocated.
			 */
			void deallocateGroup(const std::string& name);

			/**
			 * This will update all the timers on 
			 * SoundGroups
			 */
			void cycle();
	};
}

#endif

