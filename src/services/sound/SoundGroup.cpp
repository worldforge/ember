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

#include <list>
#include <cstring>

#include "SoundService.h"
#include "SoundGeneral.h"
#include "SoundSample.h"
#include "SoundGroup.h"

namespace Ember
{
	SoundGroup::SoundGroup(const std::string& name)
	{
		mName = name;
		mSamples.clear();
	}

	SoundGroup::~SoundGroup()
	{
		// TODO
	}

	void SoundGroup::allocateBuffer(const std::string& filename, 
					bool playsReal, const SoundSampleType& type)
	{	
		#define sSoundService Ember::EmberServices::getSingleton().getSoundService() 
		if (sSoundService->registerSound(filename, playsReal, type))
		{
			BaseSoundSample* newSample = sSoundService->getSoundSample(filename);
			if (newSample)
				mSamples.push_back(newSample);
		}
		else 
		{
			S_LOG_INFO(std::string("Failed to register SoundGroup buffer") + filename);
		}
		#undef sSoundService
	}

	void SoundGroup::updateSamplesPosition(const WFMath::Point<3> &pos)
	{
		std::list<BaseSoundSample*>::iterator it = mSamples.begin();
		for (it; it != mSamples.end(); it++)
		{
			(*it)->setPosition(pos);
		}
	}

	void SoundGroup::updateSamplesVelocity(const WFMath::Vector<3> &vel)
	{
		std::list<BaseSoundSample*>::iterator it = mSamples.begin();
		for (it; it != mSamples.end(); it++)
		{
			(*it)->setVelocity(vel);
		}
	}

	void SoundGroup::update()
	{
		// TODO
	}
} // namespace Ember

