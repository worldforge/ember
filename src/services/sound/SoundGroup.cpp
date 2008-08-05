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

// Need to find a cross platform solution
// to this
#include <sys/time.h>

#include "SoundService.h"
#include "SoundGeneral.h"
#include "SoundSample.h"
#include "SoundGroup.h"

namespace Ember
{
	SoundGroup::SoundGroup()
	{
		mSamples.clear();
		mLastPlayed = NULL;
		mIsPlaying = false;
	}

	SoundGroup::~SoundGroup()
	{
		// If this Group is an instance, all
		// its static samples must be deallocated by it
		if (mIsInstance)
		{
			std::list<BaseSoundSample*>::iterator it = mSamples.begin();
			for (; it != mSamples.end(); )
			{
				BaseSoundSample* sample = *it;
				it = mSamples.erase(it);

				delete sample;
			}
		}
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

	void SoundGroup::setInstance(bool isInstance)
	{
		mIsInstance = isInstance;
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

	void SoundGroup::setFrequency(const unsigned int freq)
	{
		mFrequency = freq;
	}

	void SoundGroup::setPlayOrder(const unsigned int playO)
	{
		mPlayOrder = playO;
		switch(mPlayOrder)
		{
			case PLAY_LINEAR:
				mNextToPlay = 0;
				break;
			case PLAY_INVERSE:
				mNextToPlay = mSamples.size() - 1; 
				break;
			case PLAY_RANDOM:
				getNextToPlay();
				break;
		};
	}

	void SoundGroup::getNextToPlay()
	{
		if (!mSamples.size())
		{
			S_LOG_INFO("Group has no samples to play");
			return;
		}

		switch(mPlayOrder)
		{
			case PLAY_LINEAR:
				mNextToPlay++; 
				break;
			case PLAY_INVERSE:
				mNextToPlay--; 
				break;
			case PLAY_RANDOM:
				srand(time(NULL));
				mNextToPlay = rand() % mSamples.size();
				break;
		};
	}

	void SoundGroup::resetClock()
	{
		gettimeofday(&start, NULL);
	}

	float SoundGroup::getTime()
	{
		struct timeval end;
		gettimeofday(&end, NULL);

		return (end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;
	}

	void SoundGroup::update()
	{
		if (getTime() >= 1000.0f/mFrequency)
		{
			play();
			resetClock();
		}
	}

	void SoundGroup::play()
	{
		unsigned int index = 0;

		std::list<BaseSoundSample*>::iterator it = mSamples.begin();
		for (it; it != mSamples.end(); it++)
		{
			if (index == mNextToPlay)
			{
				if (mLastPlayed)
				{
					mLastPlayed->stop();
				}

				(*it)->play();
				mLastPlayed = dynamic_cast<BaseSoundSample*>(*it);
				mIsPlaying = true;
				resetClock();

				break;
			}

			index++;
		}
	}

	void SoundGroup::stop()
	{
		if (mLastPlayed)
		{
			// Should be the active buffer
			mLastPlayed->stop();
			mIsPlaying = false;
		}
	}
	
	void SoundGroup::pushSample(BaseSoundSample* sample)
	{
		if (sample)
		{
			mSamples.push_back(sample);
		}
	}

	bool SoundGroup::instantiate(SoundGroup* instance)
	{
		if (!instance)
		{
			S_LOG_FAILURE("Invalid SoundGroup instance, NULL pointer.");
			return false;
		}

		BaseSoundSample* copy;
		BaseSoundSample* base;

		#define sSoundService Ember::EmberServices::getSingleton().getSoundService() 
		instance->setInstance(true);
		std::list<BaseSoundSample*>::iterator it = mSamples.begin();
		for (; it != mSamples.end(); it++)
		{
			sSoundService->registerInstance(*it, copy);
			if (!copy)
			{
				S_LOG_FAILURE("Failed to set copy instance.");
				return false;
			}
			else
			{
				instance->pushSample(copy);
			}
		}
		#undef sSoundService

		return true;
	}

} // namespace Ember

