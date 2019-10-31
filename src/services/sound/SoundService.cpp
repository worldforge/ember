/*
    Copyright (C) 2008 Romulo Fernandes Machado (nightz)
    Copyright (C) 2008 Erik Ogenvik <erik@ogenvik.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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

#include <cstring>

#include <AL/alut.h>

namespace Ember {
/* Constructor */
SoundService::SoundService()
		: Service("Sound")
#ifdef _MSC_VER
		, mContext(0), mDevice(0), mResourceProvider(0)
#else
		, mResourceProvider(nullptr)
#endif
		, mEnabled(false) {
}

SoundService::~SoundService() = default;

/* Method for starting this service */
bool SoundService::start() {
	S_LOG_INFO("Sound Service starting");

	if (EmberServices::getSingleton().getConfigService().hasItem("audio", "enabled")
		&& !static_cast<bool>(EmberServices::getSingleton().getConfigService().getValue("audio", "enabled"))) {
		S_LOG_INFO("Sound disabled.");
	} else {

		if (isEnabled()) {
			S_LOG_FAILURE("Can't start the sound system if it's already started.");
		} else {

#ifndef _MSC_VER
			mEnabled = alutInit(nullptr, nullptr) == ALC_TRUE;
#else
			mDevice = alcOpenDevice("DirectSound3D");
	
			if (!mDevice) {
				mEnabled = false;
				S_LOG_FAILURE("Sound Service failed to start, sound device not found 'DirectSound3D'");
				return false;
			}
	
			mContext = alcCreateContext(mDevice, nullptr);
			if (!mContext) {
				mEnabled = false;
				S_LOG_FAILURE("Sound Service failed to start, sound device not found 'DirectSound3D'");
				return false;
			}
			mEnabled = alcMakeContextCurrent(mContext) == ALC_TRUE;
#endif

			SoundGeneral::checkAlError();
		}
	}

	setRunning(true);
	return true;
}

/* Interface method for stopping this service */
void SoundService::stop() {
	if (!mInstances.empty()) {
		S_LOG_WARNING("Found a still registered SoundInstance when shutting down sound service. This shouldn't normally happen, since all instances should be handled by their proper owners and removed well in advance of the SoundService shutting down. We'll now delete the instance, which might lead to a segfault or similar problem as the instance owner might still expect it to be existing.");
	}
	mInstances.clear();

	mBaseSamples.clear();

	if (isEnabled()) {
#ifndef __WIN32__
		alutExit();
#else
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(mContext);
		alcCloseDevice(mDevice);
		mDevice = 0;
		mContext = 0;
#endif
	}
	mEnabled = false;
	Service::stop();
}

bool SoundService::isEnabled() const {
	return mEnabled;
}

void SoundService::runCommand(const std::string& command, const std::string& args) {
}

void SoundService::registerStream(StreamedSoundSample* copy) {
/*		#ifdef THREAD_SAFE
	pthread_mutex_lock(&mSamplesMutex);
	#endif

	mSamples.push_back(copy);

	#ifdef THREAD_SAFE
	pthread_mutex_unlock(&mSamplesMutex);
	#endif*/
}

bool SoundService::unregisterStream(const StreamedSoundSample* sample) {
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

void SoundService::updateListenerPosition(const WFMath::Point<3>& pos, const WFMath::Vector<3>& direction, const WFMath::Vector<3>& up) {
	if (!isEnabled()) {
		return;
	}

	alListener3f(AL_POSITION, pos.x(), pos.y(), pos.z());
	SoundGeneral::checkAlError("Setting the listener position.");

	//Set the direction of the listener.

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

void SoundService::cycle() {
	for (auto I = mInstances.begin(); I != mInstances.end();) {
		//We do the iteration this way to allow for instances to be removed inside the iteration.
		//A typical example would be a sound instance that has played to its completion and thus should be destroyed. The signal for this is emitted as a result of calling SoundInstance::update().
		auto instance = I->get();
		++I;
		instance->update();
	}
}

BaseSoundSample* SoundService::createOrRetrieveSoundSample(const std::string& soundPath) {
	auto I = mBaseSamples.find(soundPath);
	if (I != mBaseSamples.end()) {
		return I->second.get();
	}
	if (mResourceProvider) {
		ResourceWrapper resWrapper = mResourceProvider->getResource(soundPath);
		if (resWrapper.hasData()) {
			auto* sample = new StaticSoundSample(resWrapper, false, 1.0);
			mBaseSamples.emplace(soundPath, std::unique_ptr<BaseSoundSample>(sample));
			return sample;
		}
	}
	return nullptr;
}

bool SoundService::destroySoundSample(const std::string& soundPath) {
	auto I = mBaseSamples.find(soundPath);
	if (I != mBaseSamples.end()) {
		mBaseSamples.erase(I);
		return true;
	}
	return false;
}

SoundInstance* SoundService::createInstance() {
	if (!isEnabled()) {
		return nullptr;
	}
	auto* instance = new SoundInstance();
	mInstances.emplace_back(instance);
	return instance;
}

bool SoundService::destroyInstance(SoundInstance* instance) {
	auto I = std::find_if(mInstances.begin(), mInstances.end(), [instance](const std::unique_ptr<SoundInstance>& entry) { return entry.get() == instance; });
	if (I != mInstances.end()) {
		mInstances.erase(I);
		return true;
	}
	return false;
}

IResourceProvider* SoundService::getResourceProvider() {
	return mResourceProvider;
}

void SoundService::setResourceProvider(IResourceProvider* resourceProvider) {
	mResourceProvider = resourceProvider;
}

} // namespace Ember

