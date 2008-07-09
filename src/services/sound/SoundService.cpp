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

namespace Ember
{
	SoundObject* SoundService::getSoundObject(const std::string &name)
	{
		// Can Return NULL
		SoundObject* obj = mObjects[name];
		return obj;
	}

	/* Constructor */
	SoundService::SoundService()
	{
		setName("Sound Service");
		setDescription("Service for reproduction of sound effects and background music");

		#ifdef THREAD_SAFE
		pthread_mutex_init(&mSamplesMutex, NULL);
		pthread_mutex_init(&mObjectsMutex, NULL);
		#endif
	}

	/* dtor */
	SoundService::~SoundService()
	{
	}
	
	void SoundService::onAction(const Atlas::Objects::Operation::RootOperation& act)
	{
		S_LOG_INFO("Romulo, actions working!");
	}

	/* Method for starting this service */
	Service::Status SoundService::start()
	{
		S_LOG_INFO("Sound Service starting");
		
		#ifndef __WIN32__
			alutInit(NULL, NULL);
		#else
			ALCcontext *Context;
			ALCdevice *Device;
			Device = alcOpenDevice("DirectSound3D");

			if (Device == NULL)
			{
				S_LOG_FAILURE("Sound Service failed to start, sound device not found 'DirectSound3D'");
				return Service::FAILURE;
			}

			Context = alcCreateContext(Device, NULL);
			alcMakeContextCurrent(Context);
		#endif
		
		checkAlError();
		
		mSamples.clear();
		mObjects.clear();
		return Service::OK;
	}

	/* Interface method for stopping this service */
	void SoundService::stop(int code)
	{
    	Service::stop(code);
		setStatus(Service::OK);
	}

	void SoundService::runCommand(const std::string &command, const std::string &args)
	{
		// Test Suite
		if (command == "alloc")
		{
			Ember::Tokeniser tokeniser;
			tokeniser.initTokens(args);

			std::string filename = tokeniser.nextToken();
			registerSound(filename);
		}
		else
		if (command == "play")
		{
			Ember::Tokeniser tokeniser;
			tokeniser.initTokens(args);

			std::string filename = tokeniser.nextToken();
			playSound(filename);
		}
		else
		if (command == "dealloc")
		{
			Ember::Tokeniser tokeniser;
			tokeniser.initTokens(args);

			std::string filename = tokeniser.nextToken();
			unRegisterSound(filename);
		}
	}

	void SoundService::playSound(const std::string &filename)
	{
		BaseSoundSample* sound = mSamples[filename];
		if (sound)
		{
			sound->play();	
		}
		else S_LOG_INFO("Sound with filename " + filename + " not found.");
	}

	void SoundService::stopSound(const std::string &filename)
	{
		BaseSoundSample* sound = mSamples[filename];
		if (sound)
		{
			alSourceStop(sound->getSource());
			checkAlError();
		}
	}

	bool SoundService::registerSound(const std::string &filename, bool playLocally, const SoundSampleType type)
	{
		#ifdef THREAD_SAFE
		pthread_mutex_lock(&mSamplesMutex);
		#endif

		if (getSoundSample(filename))
		{
			S_LOG_INFO("Sound Sample (" + filename + ") already allocated.");
			return false;
		}

		switch (type)
		{
			default:
			case SAMPLE_NONE:
				{
					std::string extension;

					// Try to guess by extension
					if (filename.size() > 4)
					{
						extension = filename.substr(filename.size()-3, 3);
					
						if (extension == "wav" || extension == "pcm")
						{
							return allocateWAVPCM(filename, playLocally);
						}
						else
						if (extension == "ogg")
						{
							return allocateOGG(filename, playLocally);
						}
					}
					return false;
				}
				break;
			case SAMPLE_PCM:
			case SAMPLE_WAV:
				return allocateWAVPCM(filename, playLocally);
			case SAMPLE_OGG:
				return allocateOGG(filename, playLocally);
		};

		#ifdef THREAD_SAFE
		pthread_mutex_unlock(&mSamplesMutex);
		#endif
	}

	bool SoundService::unRegisterSound(const std::string &filename)
	{
		#ifdef THREAD_SAFE
		pthread_mutex_lock(&mSamplesMutex);
		#endif

		BaseSoundSample* sound = mSamples[filename];
		if (sound)
		{
			mSamples.erase(filename);
			delete sound;
			return true;
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

	SoundObject* SoundService::registerObject(const std::string &name)
	{
		#ifdef THREAD_SAFE
		pthread_mutex_lock(&mObjectsMutex);
		#endif

		if (getSoundObject(name))
		{
			S_LOG_INFO("Sound Object (" + name + ") already allocated.");
			return NULL;
		}

		SoundObject* newObject = new SoundObject();
		if (!newObject)
		{
			S_LOG_FAILURE("Failed to allocate new sound object.");
			return NULL;
		}

		mObjects[name] = newObject;

		#ifdef THREAD_SAFE
		pthread_mutex_unlock(&mObjectsMutex);
		#endif

		return newObject;
	}
		
	bool SoundService::allocateWAVPCM(const std::string &filename, bool playsLocally)
	{
		StaticSoundSample* newSample = new StaticSoundSample();
		if (!newSample)
		{
			S_LOG_FAILURE("Failed to allocate a new sound sample.");
			return false;
		}

		// Generate a new Buffer
		alGenBuffers(1, newSample->getBufferPtr());
	
		if (alGetError() != AL_NO_ERROR)
		{
			S_LOG_FAILURE("Failed to generate a new sound buffer.");
			delete newSample;

			return false;
		}

		ALboolean loop;
		newSample->setBuffer(alutCreateBufferFromFile(filename.c_str()));

		if (*(newSample->getBufferPtr()) == AL_NONE)
		{
			S_LOG_FAILURE("Failed to set buffer with file ("+ filename +") data.");
			delete newSample;

			return false;
		}

		// Bind the buffer with the source.
		alGenSources(1, newSample->getSourcePtr());

		if (alGetError() != AL_NO_ERROR)
		{
			S_LOG_FAILURE("Failed to generate a new sound source.");
			delete newSample;

			return false;
		}
	
		alSourcei (newSample->getSource(), AL_BUFFER, newSample->getBuffer());
		alSourcef (newSample->getSource(), AL_PITCH, 1.0);
		alSourcef (newSample->getSource(), AL_GAIN, 1.0);
		alSource3f(newSample->getSource(), AL_POSITION, 0, 0, 0);
		alSource3f(newSample->getSource(), AL_VELOCITY, 0, 0, 0);
		alSourcei (newSample->getSource(), AL_LOOPING, loop);

		if (playsLocally == PLAY_LOCAL)
			alSourcei(newSample->getSource(), AL_SOURCE_RELATIVE, true);

		if (alGetError() != AL_NO_ERROR)
		{
			S_LOG_FAILURE("Failed to set sound sample attributes.");
			delete newSample;

			return false;
		}

		mSamples[filename] = newSample;
		return true;
	}

	bool SoundService::allocateOGG(const std::string &filename, bool playsLocally)
	{
		StreamedSoundSample* newSample = new StreamedSoundSample();
		if (!newSample)
		{
			S_LOG_FAILURE("Failed to allocate memory for a new sound source.");
			return false;
		}

		// Should we handle this in Ogre or any other Resource Manager?
		FILE* newFile = NULL;
		if (!(newFile = fopen(filename.c_str(), "rb")))
		{
			S_LOG_FAILURE("Failed to open file.");
			delete newSample;

			return false;
		}

		newSample->setFile(newFile);

		if (ov_open(newFile, newSample->getStreamPtr(), NULL, 0) < 0)
		{
Error0:
			S_LOG_FAILURE("Failed to bind ogg stream to sound sample.");

			fclose(newFile);
			delete newSample;

			return false;
		}

		vorbis_info* oggInfo = ov_info(newSample->getStreamPtr(), -1);
		if (oggInfo->channels == 1)
		{
			newSample->setFormat(AL_FORMAT_MONO16);
		}
		else
		{
			newSample->setFormat(AL_FORMAT_STEREO16);
		}

		newSample->setRate(oggInfo->rate);

		alGenBuffers(2, newSample->getBufferPtr());
		if (alGetError() != AL_NO_ERROR)
		{
			goto Error0;
		}

		alGenSources(1, newSample->getSourcePtr());
		if (alGetError() != AL_NO_ERROR)
		{
			goto Error0;
		}

		alSourcef (newSample->getSource(), AL_PITCH, 1.0);
		alSourcef (newSample->getSource(), AL_GAIN, 1.0);
		alSource3f(newSample->getSource(), AL_POSITION, 0, 0, 0);
		alSource3f(newSample->getSource(), AL_VELOCITY, 0, 0, 0);
		alSourcei (newSample->getSource(), AL_LOOPING, false);

		if (playsLocally == PLAY_LOCAL)
			alSourcei(newSample->getSource(), AL_SOURCE_RELATIVE, true);

		mSamples[filename] = newSample;
		return true;
	}

	BaseSoundSample* SoundService::getSoundSample(const std::string &filename)
	{
		// Can return NULL;
		BaseSoundSample* sound = mSamples[filename];
		return sound;
	}

	void SoundService::cycle()
	{
		#ifdef THREAD_SAFE
		pthread_mutex_lock(&mSamplesMutex);
		#endif

		for (std::map<std::string, BaseSoundSample*>::iterator it = mSamples.begin(); 
				it != mSamples.end(); it++)
		{
			BaseSoundSample* sample = (*it).second;
			if (sample && sample->getType() == SAMPLE_OGG)
			{
				StreamedSoundSample* ogg = dynamic_cast<StreamedSoundSample*>(sample);
				if (ogg->isPlaying())
				{
					ogg->cycle();
				}
			}
		}

		#ifdef THREAD_SAFE
		pthread_mutex_unlock(&mSamplesMutex);
		#endif

		#ifdef THREAD_SAFE
		pthread_mutex_lock(&mObjectsMutex);
		#endif

		for (std::map<std::string, SoundObject*>::iterator it = mObjects.begin();
				it != mObjects.end(); it++)
		{
			SoundObject* object = (*it).second;
			if (object)
			{
				object->playQueued();
			}
		}

		#ifdef THREAD_SAFE
		pthread_mutex_lock(&mObjectsMutex);
		#endif
	}

} // namespace Ember

