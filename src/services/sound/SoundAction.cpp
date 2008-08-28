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

#include "framework/Service.h"
#include "framework/ConsoleObject.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/logging/LoggingService.h"
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

#include "SoundService.h"
#include "SoundGeneral.h"
#include "SoundModel.h"
#include "SoundAction.h"
#include "SoundGroup.h"
#include "SoundInstance.h"

#include <map>
#include <cstring>
#include <sstream>

namespace Ember
{
	SoundAction::SoundAction()
	: mGroup(0), mInstance(0)
	{
	}

	SoundGroup* SoundAction::getGroup()
	{
		return mGroup;
	}

	SoundGroup* SoundAction::setGroup(const std::string& name)
	{
		if (mGroup) {
			return 0;
		}
	
		SoundGroupDefinition* groupModel = EmberServices::getSingleton().getSoundService()->getSoundGroupDefinition(name);

		if (!groupModel)
		{
			S_LOG_FAILURE("A template to the group " << name << " could not be found.");
			return NULL;
		}

		SoundGroup* newGroup = new SoundGroup();

		std::list<SoundDefinition*>::const_iterator beg = groupModel->getSamplesBegin();
		std::list<SoundDefinition*>::const_iterator end = groupModel->getSamplesEnd();
		for (beg; beg != end; beg++)
		{
			// Register Individual samples
			SoundDefinition* thisModel = *beg;
			newGroup->addSound(thisModel);
		}

		mGroup = newGroup;
		return newGroup;
	}

	void SoundAction::play()
	{
		if (mGroup) {
			if (!mInstance) {
				mInstance = EmberServices::getSingleton().getSoundService()->createInstance();
				mGroup->bindToInstance(mInstance);
			}
			mInstance->play();
		}
	}

	void SoundAction::stop()
	{
		if (mInstance) {
			mInstance->stop();
		}
	}
	
	SoundInstance* SoundAction::getInstance() const
	{
		return mInstance;
	}
	
}

