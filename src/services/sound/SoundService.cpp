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

#include "services/config/ConfigService.h"
#include "framework/LoggingInstance.h"

#include "SoundSample.h"
#include "SoundInstance.h"


#include <algorithm>

namespace Ember {
SoundService::SoundService(ConfigService& configService)
		: Service("Sound")
		, mContext(nullptr), mDevice(nullptr), mResourceProvider(nullptr)
		, mEnabled(false) {
	S_LOG_INFO("Sound Service starting");

	if (configService.hasItem("audio", "enabled")
		&& !static_cast<bool>(configService.getValue("audio", "enabled"))) {
		S_LOG_INFO("Sound disabled.");
	} else {

		if (isEnabled()) {
			S_LOG_FAILURE("Can't start the sound system if it's already started.");
		} else {

			mDevice = alcOpenDevice(nullptr);
			//Seems like alcOpenDevice in some cases alters the locale. We don't want that, so we'll revert back to "C".
			setlocale(0, "C");
			setlocale(1, "C");
			setlocale(2, "C");
			setlocale(3, "C");
			setlocale(4, "C");
			setlocale(5, "C");
			setlocale(6, "C");
			setlocale(7, "C");
			setlocale(8, "C");
			setlocale(9, "C");
			setlocale(10, "C");
			setlocale(11, "C");
			setlocale(12, "C");

			if (!mDevice) {
				mEnabled = false;
				S_LOG_FAILURE("Sound Service failed to start, sound device not found 'DirectSound3D'");
			} else {

				mContext = alcCreateContext(mDevice, nullptr);
				if (!mContext) {
					mEnabled = false;
					S_LOG_FAILURE("Sound Service failed to start, sound device not found 'DirectSound3D'");
				} else {
					mEnabled = alcMakeContextCurrent(mContext) == ALC_TRUE;
				}
			}


			SoundGeneral::checkAlError();
		}
	}
}

SoundService::~SoundService() {
	if (!mInstances.empty()) {
		S_LOG_WARNING("Found a still registered SoundInstance when shutting down sound service. This shouldn't normally happen, since all instances should be handled by their proper owners and removed well in advance of the SoundService shutting down. We'll now delete the instance, which might lead to a segfault or similar problem as the instance owner might still expect it to be existing.");
	}
	mInstances.clear();

	mBaseSamples.clear();

	if (isEnabled()) {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(mContext);
		alcCloseDevice(mDevice);
	}
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
			auto sample = std::make_unique<StaticSoundSample>(resWrapper, false, 1.0f);
			auto result = mBaseSamples.emplace(soundPath, std::move(sample));
			if (result.second) {
				return result.first->second.get();
			}
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
	auto instance = std::make_unique<SoundInstance>();
	mInstances.emplace_back(std::move(instance));
	return mInstances.back().get();
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

