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
#include <sstream>

#include "SoundEntity.h"
#include "SoundGroup.h"
#include "SoundEntityManager.h"

template<> Ember::SoundEntityManager* Ember::Singleton<Ember::SoundEntityManager>::ms_Singleton = 0;

namespace Ember
{
	SoundEntityManager::SoundEntityManager()
	{
		mEntities.clear();
		mGroups.clear();

		mGroupSeed = 0;
		mEntitySeed = 0;

		S_LOG_INFO("Sound Entity Manager Started.");
	}

	SoundEntityManager::~SoundEntityManager()
	{
		std::map<std::string, SoundEntity*>::iterator it = mEntities.begin();
		for (; it != mEntities.end(); it++)
		{
			SoundEntity* toBeDeleted = (*it).second;
			delete toBeDeleted;
		}

		S_LOG_INFO("Sound Entity Manager Stopped.");
	}

	SoundEntity* SoundEntityManager::allocateEntity(const std::string& name)
	{
		SoundEntity* newEntity = getEntity(name);
		if (newEntity)
		{
			S_LOG_INFO(std::string("Entity ") + name + std::string(" already exists."));
			return newEntity;
		}
		else
		{
			newEntity = new SoundEntity();
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
		std::map<std::string, SoundEntity*>::iterator it(mEntities.find(name));
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

		std::map<std::string, SoundEntity*>::iterator it(mEntities.find(name));
		if (it != mEntities.end())
		{
			return (*it).second;
		}

		return NULL;
	}

	SoundGroup* SoundEntityManager::getGroup(const std::string& name)
	{
		if (mGroups.empty())
		{
			return NULL;
		}

		std::map<std::string, SoundGroup*>::iterator it(mGroups.find(name));
		if (it != mGroups.end())
		{
			return (*it).second;
		}

		return NULL;
	}

	SoundGroup* SoundEntityManager::allocateGroup(const std::string& name)
	{
		if (getGroup(name))
		{
			S_LOG_INFO(std::string("Group ") + name + std::string(" already exists."));
			return NULL;
		}
		else
		{
			SoundGroup* newGroup = new SoundGroup();
			if (newGroup)
			{
				mGroups[name] = newGroup;
				return newGroup;
			}

			return NULL;
		}
	}
		
	SoundGroup* SoundEntityManager::instantiateGroup(const std::string& name)
	{
		SoundGroup* entTypeGroup = getGroup(name);
		if (!entTypeGroup)
		{
			S_LOG_FAILURE("Cannot instantiate type " + name + " because type root doesnt exist");
			return NULL;
		}

		// Generate a seeded number for instance
		std::stringstream instanceName;
		instanceName << name << "_" << mGroupSeed++;
	}

	void SoundEntityManager::deallocateGroup(const std::string& name)
	{
		std::map<std::string, SoundGroup*>::iterator it(mGroups.find(name));
		if (it != mGroups.end())
		{
			SoundGroup* toBeDeleted = (*it).second;
			mGroups.erase(it);

			delete toBeDeleted;
		}
	}

	void SoundEntityManager::cycle()
	{
		std::map<std::string, SoundGroup*>::iterator it = mGroups.begin();
		for (; it != mGroups.end(); it++)
		{
			Ember::SoundGroup* group = it->second;
			if (group)
			{
				group->update();
			}
		}
	}
}

