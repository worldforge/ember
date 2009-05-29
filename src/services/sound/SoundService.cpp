/*
    Copyright (C) 2008 Romulo Fernandes Machado (nightz)
    Copyright (C) 2008 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "SoundService.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "framework/LoggingInstance.h"

#include "SoundSample.h"
#include "SoundInstance.h"

#include <map>
#include <cstring>
#include <algorithm>

#ifndef __WIN32__
#include <AL/alut.h>
#else
#include <ALUT/alut.h>
#endif


namespace Ember
{
/* Constructor */
SoundService::SoundService()
#ifdef WIN32
	: mContext(0), mDevice(0), mResourceProvider(0)
#else
	: mResourceProvider(0)
#endif
, mEnabled(false)
{
	setName("Sound Service");
	setDescription("Service for reproduction of sound effects and background music");
}

/* dtor */
SoundService::~SoundService()
{
}

/* Method for starting this service */
Service::Status SoundService::start()
{
	S_LOG_INFO("Sound Service starting");
	
	if (Ember::EmberServices::getSingleton().getConfigService()->hasItem("audio", "enabled")
	    && static_cast<bool>(Ember::EmberServices::getSingleton().getConfigService()->getValue("audio", "enabled")) == false) {
		S_LOG_INFO("Sound disabled.");
	} else {
	
		if (isEnabled()) {
			S_LOG_FAILURE("Can't start the sound system if it's already started.");
		} else {

		#ifndef __WIN32__
			mEnabled = alutInit(NULL, NULL) == ALC_TRUE;
		#else
			mDevice = alcOpenDevice("DirectSound3D");
	
			if (!mDevice) {
				mEnabled = false;
				S_LOG_FAILURE("Sound Service failed to start, sound device not found 'DirectSound3D'");
				return Service::FAILURE;
			}
	
			mContext = alcCreateContext(mDevice, NULL);
			if (!mContext) {
				mEnabled = false;
				S_LOG_FAILURE("Sound Service failed to start, sound device not found 'DirectSound3D'");
				return Service::FAILURE;
			}
			mEnabled = alcMakeContextCurrent(mContext) == ALC_TRUE;
		#endif
		
			SoundGeneral::checkAlError();
		}
	}
	
	return Service::OK;
}

/* Interface method for stopping this service */
void SoundService::stop(int code)
{
	for (SoundInstanceStore::iterator I = mInstances.begin(); I != mInstances.end(); ++I) {
		S_LOG_WARNING("Found a still registered SoundInstance when shutting down sound service. This shouldn't normally happen, since all instances should be handled by their proper owners and removed well in advance of the SoundService shutting down. We'll now delete the instance, which might lead to a segfault or similiar problem as the instance owner might still expect it to be existing.");
		delete *I;
	}
	mInstances.clear();
	
	for (SoundSampleStore::iterator I = mBaseSamples.begin(); I != mBaseSamples.end(); ++I) {
		delete I->second;
	}
	mBaseSamples.clear();
	
///this hangs, perhaps because we don't clean up properly after us, so we'll deactivate it for now
// 	if (isEnabled()) {
// 		#ifndef __WIN32__
// 			alutExit();
// 		#else
// 			alcMakeContextCurrent(NULL);
// 			alcDestroyContext(mContext);
// 			alcCloseDevice(mDevice);
// 			mDevice = 0;
// 			mContext = 0;
// 		#endif
// 	}
	mEnabled = false;
	Service::stop(code);
	setStatus(Service::OK);
}

bool SoundService::isEnabled() const
{
	return mEnabled;
}

void SoundService::runCommand(const std::string &command, const std::string &args)
{
}

void SoundService::registerStream(StreamedSoundSample* copy)
{
/*		#ifdef THREAD_SAFE
	pthread_mutex_lock(&mSamplesMutex);
	#endif

	mSamples.push_back(copy);

	#ifdef THREAD_SAFE
	pthread_mutex_unlock(&mSamplesMutex);
	#endif*/
}

bool SoundService::unregisterStream(const StreamedSoundSample* sample)
{
// 		#ifdef THREAD_SAFE
// 		pthread_mutex_lock(&mSamplesMutex);
// 		#endif
// 	
// 		std::list<StreamedSoundSample*>::iterator it;
// 		for (it = mSamples.begin(); it != mSamples.end(); )
// 		{
// 			if ((*it) == sample)
// 			{
// 				it = mSamples.erase(it);
// 				return true;
// 			}
// 			else
// 			{
// 				++it;
// 			}
// 		}
// 
// 		#ifdef THREAD_SAFE
// 		pthread_mutex_unlock(&mSamplesMutex);
// 		#endif

	return false;
}

void SoundService::updateListenerPosition(const WFMath::Point<3>& pos, const WFMath::Vector<3>& direction, const WFMath::Vector<3> up)
{
	if (!isEnabled()) {
		return;
	}

	alListener3f(AL_POSITION, pos.x(), pos.y(), pos.z());
	SoundGeneral::checkAlError("Setting the listener position.");

	///Set the direction of the listener.

	ALfloat aluVectors[6];
	aluVectors[0] = direction.x();
	aluVectors[1] = direction.y();
	aluVectors[2] = direction.z();
	aluVectors[3] = up.x();
	aluVectors[4] = up.y();
	aluVectors[5] = up.z();

	alListenerfv(AL_ORIENTATION, aluVectors);
	SoundGeneral::checkAlError("Setting the listener orientation.");
}

void SoundService::cycle()
{
	for (SoundInstanceStore::iterator I = mInstances.begin(); I != mInstances.end(); ) {
		///We do the iteration this way to allow for instances to be removed inside the iteration.
		///A typical example would be a sound instance that has played to its completion and thus should be destroyed. The signal for this is emitted as a result of calling SoundInstance::update().
		SoundInstance* instance(*I);
		++I;
		instance->update();
	}
}
	
BaseSoundSample* SoundService::createOrRetrieveSoundSample(const std::string& soundPath)
{
	SoundSampleStore::iterator I = mBaseSamples.find(soundPath);
	if (I != mBaseSamples.end()) {
		return I->second;
	}
	if (mResourceProvider) {
		ResourceWrapper resWrapper = mResourceProvider->getResource(soundPath);
		if (resWrapper.hasData()) {
			StaticSoundSample* sample = new StaticSoundSample(resWrapper, false, 1.0);
			mBaseSamples.insert(SoundSampleStore::value_type(soundPath, sample));
			return sample;
		}
	}
	return 0;
}

bool SoundService::destroySoundSample(const std::string& soundPath)
{
	SoundSampleStore::iterator I = mBaseSamples.find(soundPath);
	if (I != mBaseSamples.end()) {
		delete I->second;
		mBaseSamples.erase(I);
		return true;
	}
	return false;
}

SoundInstance* SoundService::createInstance()
{
	if (!isEnabled()) {
		return 0;
	}
	SoundInstance* instance = new SoundInstance();
	mInstances.push_back(instance);
	return instance;
}

bool SoundService::destroyInstance(SoundInstance* instance)
{
	SoundInstanceStore::iterator I = std::find(mInstances.begin(), mInstances.end(), instance);
	if (I != mInstances.end()) {
		mInstances.erase(I);
		delete instance;
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

