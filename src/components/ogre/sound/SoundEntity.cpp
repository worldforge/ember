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
#include "SoundInstance.h"
#include "SoundSource.h"
namespace Ember
{
	SoundEntity::SoundEntity()
	: mActiveAction(0)
	{
		mActions.clear();
	}

	SoundEntity::~SoundEntity()
	{
		// TODO: Deallocate groups
	}

	void SoundEntity::setPosition(const WFMath::Point<3> &pos)
	{
		if (mPosition != pos)
		{
			if (mActiveAction) {
				mActiveAction->getInstance()->getSource().setPosition(pos);
			}

			mPosition = pos;
		}
	}
			
	void SoundEntity::setVelocity(const WFMath::Vector<3> &vel)
	{
		if (mVelocity != vel)
		{
			std::map<std::string, SoundAction*>::iterator it;
			for (it = mActions.begin(); it != mActions.end(); it++)
			{
				// TODO
			}

			mVelocity = vel;
		}
	}

	const WFMath::Point<3> SoundEntity::getPosition()
	{
		return mPosition;
	}

	const WFMath::Vector<3> SoundEntity::getVelocity()
	{
		return mVelocity;
	}

	SoundAction* SoundEntity::createAction(const std::string& name)
	{
		SoundAction* newAction = actionExists(name);
		if (newAction)
			return newAction;

		newAction = new SoundAction();
		if (!newAction)
		{
			S_LOG_FAILURE("Failed to allocate Entity Action " + name);
			return NULL;
		}

		S_LOG_INFO("Sound Entity Action " + name + " allocated.");
		mActions[name] = newAction;

		return newAction;
	}

	SoundAction* SoundEntity::actionExists(const std::string& name)
	{
		std::map<std::string, SoundAction*>::iterator it(mActions.find(name));
		if (it != mActions.end())
		{
			return (*it).second;
		}

		return NULL;
	}

	void SoundEntity::playAction(const std::string& name)
	{
		SoundAction* theAction = actionExists(name);
		if (!theAction)
		{
			S_LOG_INFO("Sound Action " + name + " doesn't exist.");
			return;
		}

		if (theAction != mActiveAction) {
			if (mActiveAction)
			{
				mActiveAction->stop();
			}
	
			S_LOG_INFO("Playing Sound Action " + name);
	
			theAction->play();
			mActiveAction = theAction;
		}
	}

	void SoundEntity::update()
	{
		// TODO
	}

} // namespace Ember

