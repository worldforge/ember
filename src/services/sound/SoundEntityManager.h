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
#include "framework/Singleton.h"

namespace Ember
{
	class SoundEntityManager
	: public Ember::Singleton<SoundEntityManager>
	{
		private:
			std::map<std::string, SoundEntity*> mEntities;

		public:
			SoundEntityManager();
			~SoundEntityManager();

			/**
			 * mEntities modifiers
			 */
			SoundEntity* allocateEntity(const std::string& name);
			void deallocateEntity(const std::string& name);

			/**
			 * Ember::Entity use those methods to communicate to
			 * the soundservice indirectly
			 */
			SoundEntity* getEntity(const std::string& name);
	};
}

#endif

