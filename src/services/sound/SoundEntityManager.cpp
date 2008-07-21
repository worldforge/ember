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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "framework/Singleton.h"
#include "framework/Service.h"
#include "framework/ConsoleObject.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/logging/LoggingService.h"
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

#include <map>
#include <cstring>

#include "SoundEntity.h"
#include "SoundEntityManager.h"

template<> Ember::SoundEntityManager* Ember::Singleton<Ember::SoundEntityManager>::ms_Singleton = 0;

namespace Ember
{
	#define entityIt std::map<std::string, SoundEntity*>::iterator
	SoundEntityManager::SoundEntityManager()
	{
		mEntities.clear();
		S_LOG_INFO("Sound Entity Manager Started.");
	}

	SoundEntityManager::~SoundEntityManager()
	{
		for (entityIt it = mEntities.begin(); it != mEntities.end(); it++)
		{
			SoundEntity* toBeDeleted = (*it).second;
			delete toBeDeleted;
		}

		S_LOG_INFO("Sound Entity Manager Stopped.");
	}

	SoundEntity* SoundEntityManager::allocateEntity(const std::string& name)
	{
		if (getEntity(name))
		{
			S_LOG_INFO(std::string("Entity ") + name + std::string(" already exists."));
			return NULL;
		}
		else
		{
			SoundEntity* newEntity = new SoundEntity();
			if (newEntity)
			{
				mEntities[name] = newEntity;
				return newEntity;
			}

			return NULL;
		}
	}

	void SoundEntityManager::deallocateEntity(const std::string& name)
	{
		entityIt it(mEntities.find(name));
		if (it != mEntities.end())
		{
			SoundEntity* toBeDeleted = (*it).second;
			mEntities.erase(it);

			delete toBeDeleted;
		}
	}

	SoundEntity* SoundEntityManager::getEntity(const std::string& name)
	{
		if (mEntities.empty())
		{
			return NULL;
		}

		/* TODO - ITS CRASHING!!!!
		entityIt it(mEntities.find(name));
		if (it != mEntities.end())
		{
			return (*it).second;
		}

		return NULL;
		*/
		return mEntities[name];
	}
	#undef entityIt
}

