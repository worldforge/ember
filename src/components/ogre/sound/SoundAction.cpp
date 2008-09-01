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

#include "SoundAction.h"
#include "SoundEntity.h"
#include "SoundGroup.h"
#include "SoundDefinition.h"
#include "SoundGroupDefinition.h"
#include "SoundDefinitionManager.h"

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"

#include "services/sound/SoundService.h"
#include "services/sound/SoundGeneral.h"
#include "services/sound/SoundInstance.h"
#include "services/sound/SoundSource.h"

#include <map>
#include <cstring>
#include <sstream>

using namespace Ember;

namespace EmberOgre
{
	SoundAction::SoundAction(SoundEntity& soundEntity)
	: mSoundEntity(soundEntity), mGroup(0), mInstance(0)
	{
	}
	
	SoundAction::~SoundAction()
	{
		if (mInstance) {
			EmberServices::getSingleton().getSoundService()->destroyInstance(mInstance);
		}
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
	
		SoundGroupDefinition* groupModel = SoundDefinitionManager::getSingleton().getSoundGroupDefinition(name);

		if (!groupModel)
		{
			S_LOG_FAILURE("A template to the group " << name << " could not be found.");
			return NULL;
		}

		SoundGroup* newGroup = new SoundGroup();

		std::list<SoundDefinition*>::const_iterator beg = groupModel->getSamplesBegin();
		std::list<SoundDefinition*>::const_iterator end = groupModel->getSamplesEnd();
		for (; beg != end; beg++)
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
				mInstance->setMotionProvider(this);
				mGroup->bindToInstance(mInstance);
			}
			mInstance->play();
		}
	}

	void SoundAction::stop()
	{
		if (mInstance) {
			mInstance->stop();
			EmberServices::getSingleton().getSoundService()->destroyInstance(mInstance);
			mInstance = 0;
		}
	}
	
	SoundInstance* SoundAction::getInstance() const
	{
		return mInstance;
	}
	
	void SoundAction::update(SoundSource& soundSource)
	{
		soundSource.setPosition(mSoundEntity.getPosition());
// 		soundSource.setVelocity(mSoundEntity.getVelocity());
	}
	
}

