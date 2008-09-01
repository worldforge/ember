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
		
		mBaseSamples.clear();
		mSoundGroupDefinitions.clear();

		return Service::OK;
	}
		
	SoundGroupDefinition* SoundService::getSoundGroupDefinition(const std::string& name)
	{
		std::map<std::string, SoundGroupDefinition*>::iterator it(mSoundGroupDefinitions.find(name));
		if (it != mSoundGroupDefinitions.end())
		{
			return it->second;
		}

		return NULL;
	}

	SoundGroupDefinition* SoundService::createSoundGroupDefinition(const std::string& name)
	{
		SoundGroupDefinition* newModel = getSoundGroupDefinition(name);
		if (!newModel)
		{
			newModel = new SoundGroupDefinition();
			if (newModel)
			{
				#ifdef THREAD_SAFE
				pthread_mutex_lock(&mGroupModelsMutex);
				#endif

				mSoundGroupDefinitions[name] = newModel;

				#ifdef THREAD_SAFE
				pthread_mutex_unlock(&mGroupModelsMutex);
				#endif

				return newModel;
			}
			else
			{
				S_LOG_FAILURE("Failed to allocate sound group model " << name);
				return NULL;
			}
		}
		else
		{
			S_LOG_INFO("Sound Group definition " << name << " already exists.");
			return NULL;
		}
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
		
		for (SoundGroupDefinitionStore::iterator I = mSoundGroupDefinitions.begin(); I != mSoundGroupDefinitions.end(); ++I) {
			delete I->second;
		}
		mSoundGroupDefinitions.clear();
	
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

	void SoundService::updateListenerPosition(const WFMath::Point<3>& pos, const WFMath::Quaternion& orientation)
	{
		alListener3f(AL_POSITION, pos.x(), pos.y(), pos.z());
		checkAlError();

		///Set the direction of the listener.
		///These conversions are however incorrect, we'll have to fix it. /ehj
		WFMath::RotMatrix<3> rotMatrix(orientation);
		WFMath::Vector<3> vectors[3];
		
        for (size_t iCol = 0; iCol < 3; iCol++)
        {
            vectors[iCol].x() = rotMatrix.elem(0, iCol);
            vectors[iCol].y() = rotMatrix.elem(1, iCol);
            vectors[iCol].z() = rotMatrix.elem(2, iCol);
        }
        
        ALfloat aluVectors[6];
        aluVectors[0] = vectors[1].x();
        aluVectors[1] = vectors[1].y();
        aluVectors[2] = vectors[1].z();
        aluVectors[3] = vectors[2].x();
        aluVectors[4] = vectors[2].y();
        aluVectors[5] = vectors[2].z();
        
/*        ALfloat aluVectors[6];
        aluVectors[0] = rotMatrix.elem(0, 1);
        aluVectors[1] = rotMatrix.elem(1, 1);
        aluVectors[2] = rotMatrix.elem(2, 1);
        aluVectors[3] = rotMatrix.elem(0, 2);
        aluVectors[4] = rotMatrix.elem(1, 2);
        aluVectors[5] = rotMatrix.elem(2, 2);    */    
		
		// TODO: Convert the quaternion to forward/up vectors
		alListenerfv(AL_ORIENTATION, aluVectors);
	}
		
	void SoundService::cycle()
	{
		for (SoundInstanceStore::iterator I = mInstances.begin(); I != mInstances.end(); ++I) {
			(*I)->update();
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

