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

#include "SoundEntityManager.h"
#include "SoundGeneral.h"
#include "SoundAction.h"
#include "SoundGroup.h"

#include "framework/Service.h"
#include "framework/ConsoleObject.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/logging/LoggingService.h"
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

#include <map>
#include <cstring>

namespace Ember
{
	SoundAction::SoundAction()
	{
		mGroups.clear();
	}

	SoundGroup* SoundAction::getGroup(const std::string& name)
	{
		std::map<std::string, SoundGroup*>::iterator it(mGroups.find(name));
		if (it != mGroups.end())
		{
			return (*it).second;
		}

		return NULL;
	}

	#define soundMgr SoundEntityManager::getSingleton()
	SoundGroup* SoundAction::registerGroup(const std::string& name)
	{
		SoundGroup* newGroup = getGroup(name);
		if (!newGroup)
		{
			newGroup = soundMgr.getGroup(name);
		}

		if (!newGroup)
		{
			S_LOG_FAILURE("Failed to register Sound Group " + name);
			return NULL;
		}

		mGroups[name] = newGroup;
		return newGroup;
	}

	void SoundAction::play()
	{
		std::map<std::string, SoundGroup*>::iterator it;
		for (it = mGroups.begin(); it != mGroups.end(); it++)
		{
			SoundGroup* group = it->second;
			group->play();
		}
	}

	void SoundAction::stop()
	{
		std::map<std::string, SoundGroup*>::iterator it;
		for (it = mGroups.begin(); it != mGroups.end(); it++)
		{
			SoundGroup* group = it->second;
			group->stop();
		}
	}
}

