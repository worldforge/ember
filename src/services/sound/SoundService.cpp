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

#include "SoundService.h"
#include "SoundEntity.h"
#include "SoundInstance.h"

namespace Ember
{
	/* Constructor */
	SoundService::SoundService()
	
	#ifdef WIN32
	 : mContext(0), mDevice(0), mResourceProvider(0)
	#else
	 : mResourceProvider(0)
	#endif
	
	{
		setName("Sound Service");
		setDescription("Service for reproduction of sound effects and background music");

		#ifdef THREAD_SAFE
		pthread_mutex_init(&mGroupsMutex, NULL);
		pthread_mutex_init(&mSamplesMutex, NULL);
		pthread_mutex_init(&mGroupModelsMutex, NULL);
		#endif
	}

	/* dtor */
	SoundService::~SoundService()
	{
	}
	
	/* Method for starting this service */
	Service::Status SoundService::start()
	{
		S_LOG_INFO("Sound Service starting");
		
		#ifndef __WIN32__
			alutInit(NULL, NULL);
		#else
			mDevice = alcOpenDevice("DirectSound3D");

			if (!mDevice)
			{
				S_LOG_FAILURE("Sound Service failed to start, sound device not found 'DirectSound3D'");
				return Service::FAILURE;
			}

			mContext = alcCreateContext(mDevice, NULL);
			alcMakeContextCurrent(mContext);
		#endif
		
		checkAlError();
		
		mSamples.clear();
		mSoundGroupModels.clear();

		return Service::OK;
	}
		
	SoundGroupModel* SoundService::getSoundGroupModel(const std::string& name)
	{
		std::map<std::string, SoundGroupModel*>::iterator it(mSoundGroupModels.find(name));
		if (it != mSoundGroupModels.end())
		{
			return it->second;
		}

		return NULL;
	}

	SoundGroupModel* SoundService::createSoundGroupModel(const std::string& name)
	{
		SoundGroupModel* newModel = getSoundGroupModel(name);
		if (!newModel)
		{
			newModel = new SoundGroupModel();
			if (newModel)
			{
				#ifdef THREAD_SAFE
				pthread_mutex_lock(&mGroupModelsMutex);
				#endif

				mSoundGroupModels[name] = newModel;

				#ifdef THREAD_SAFE
				pthread_mutex_unlock(&mGroupModelsMutex);
				#endif

				return newModel;
			}
			else
			{
				S_LOG_FAILURE("Failed to allocate sound group model " + name);
				return NULL;
			}
		}
		else
		{
			S_LOG_INFO("Sound Group Model " + name + " already exists.");
			return NULL;
		}
	}

	/* Interface method for stopping this service */
	void SoundService::stop(int code)
	{
	
	///this hangs, perhaps because we don't clean up properly after us, so we'll deactivate it for now
// 		#ifndef __WIN32__
// 			alutExit();
// 		#else
// 			alcMakeContextCurrent(NULL);
// 			alcDestroyContext(mContext);
// 			alcCloseDevice(mDevice);
// 		#endif
    	Service::stop(code);
		setStatus(Service::OK);
	}

	void SoundService::runCommand(const std::string &command, const std::string &args)
	{
	}

	void SoundService::registerSoundGroup(SoundGroup* copy)
	{
		#ifdef THREAD_SAFE
		pthread_mutex_lock(&mGroupsMutex);
		#endif

		mGroups.push_back(copy);

		#ifdef THREAD_SAFE
		pthread_mutex_unlock(&mGroupsMutex);
		#endif
	}

	bool SoundService::unregisterSoundGroup(const SoundGroup* sample)
	{
		#ifdef THREAD_SAFE
		pthread_mutex_lock(&mGroupsMutex);
		#endif
	
		std::list<SoundGroup*>::iterator it;
		for (it = mGroups.begin(); it != mGroups.end(); )
		{
			if ((*it) == sample)
			{
				it = mGroups.erase(it);
				return true;
			}
			else
			{
				++it;
			}
		}

		#ifdef THREAD_SAFE
		pthread_mutex_unlock(&mGroupsMutex);
		#endif

		return false;
	}

	void SoundService::registerStream(StreamedSoundSample* copy)
	{
		#ifdef THREAD_SAFE
		pthread_mutex_lock(&mSamplesMutex);
		#endif

		mSamples.push_back(copy);

		#ifdef THREAD_SAFE
		pthread_mutex_unlock(&mSamplesMutex);
		#endif
	}

	bool SoundService::unregisterStream(const StreamedSoundSample* sample)
	{
		#ifdef THREAD_SAFE
		pthread_mutex_lock(&mSamplesMutex);
		#endif
	
		std::list<StreamedSoundSample*>::iterator it;
		for (it = mSamples.begin(); it != mSamples.end(); )
		{
			if ((*it) == sample)
			{
				it = mSamples.erase(it);
				return true;
			}
			else
			{
				++it;
			}
		}

		#ifdef THREAD_SAFE
		pthread_mutex_unlock(&mSamplesMutex);
		#endif

		return false;
	}

	void SoundService::updateListenerPosition(const WFMath::Point<3>& pos, 
			const WFMath::Quaternion& ori)
	{
		alListener3f(AL_POSITION, pos.x(), pos.y(), pos.z());
		checkAlError();

		// TODO: Convert the quaternion to forward/up vectors
		// alListener3f(AL_ORIENTATION, ListenerOri);
	}
		
	void SoundService::cycle()
	{
// 		// Groups
// 		std::list<SoundGroup*>::const_iterator git = mGroups.begin();
// 		for (; git != mGroups.end(); git++)
// 		{
// 			SoundGroup* group = (*git);
// 			if (group)
// 			{
// 				group->update();
// 			}
// 		}
// 
// 		// Streams
// 		std::list<StreamedSoundSample*>::const_iterator it = mSamples.begin();
// 		for (; it != mSamples.end(); it++)
// 		{
// 			StreamedSoundSample* sample = (*it);
// 			if (sample && sample->isPlaying())
// 			{
// 				sample->cycle();
// 			}
// 		}
	}
	
BaseSoundSample* SoundService::createOrRetrieveSoundSample(const std::string& soundPath)
{
	SoundSampleStore::iterator I = mBaseSamples.find(soundPath);
	if (I != mBaseSamples.end()) {
		return I->second;
	}
	
}

bool SoundService::destroySoundSample(BaseSoundSample* soundSample)
{
}
	
	
SoundInstance* SoundService::createInstance()
{
	SoundInstance* instance = new SoundInstance();
	mInstances.push_back(instance);
	return instance;
}
bool SoundService::destroyInstance(SoundInstance* instance)
{
	SoundInstanceStore::iterator I = std::find(mInstances.begin(), mInstances.end(), instance);
	if (I != mInstances.end()) {
		mInstances.erase(I);
		return true;
	}
	return false;
}
	
Ember::IResourceProvider* SoundService::getResourceProvider()
{
	return mResourceProvider;
}
	
void SoundService::setResourceProvider(Ember::IResourceProvider* resourceProvider)
{
	mResourceProvider = resourceProvider;
}	

} // namespace Ember

