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

namespace Ember
{
	void checkAlError()
	{
		ALenum error = alGetError();
		if (error == AL_NO_ERROR)
			return;

		S_LOG_FAILURE(std::string("openAl error: ") + 
				std::string(alutGetErrorString(error)));
	}

	void BaseSoundSample::setPosition(const WFMath::Point<3> &pos)
	{
		if (mPlayPosition == PLAY_LOCAL)
			return;

		alSource3f(mSource, AL_POSITION, pos.x(), pos.y(), pos.z());
		checkAlError();
	}

	void BaseSoundSample::setVelocity(const WFMath::Vector<3> &vel)
	{
		alSource3f(mSource, AL_VELOCITY, vel.x(), vel.y(), vel.z());
		checkAlError();
	}

	void BaseSoundSample::setSource(ALuint src)
	{
		mSource = src;
	}

	const std::string BaseSoundSample::getFilename()
	{
		return mFilename;
	}
		
	const std::string BaseSoundSample::getAction()
	{
		return mAction;
	}

	ALuint BaseSoundSample::getSource()
	{
		return mSource;
	}

	ALuint* BaseSoundSample::getSourcePtr()
	{
		return &mSource;
	}

	SoundSampleType BaseSoundSample::getType()
	{
		return mType;
	}
	
	void BaseSoundSample::setAction(const std::string &act)
	{
		mAction = act;
	}

	void BaseSoundSample::play()
	{
	}

	// Static Sounds (PCM, WAV)
	StaticSoundSample::StaticSoundSample(const std::string &name)
	{
		mFilename = name;
		mType = SAMPLE_WAV;
	}

	StaticSoundSample::~StaticSoundSample()
	{
		alDeleteBuffers(1, &mBuffer);
		checkAlError();

		alDeleteSources(1, &mSource);
		checkAlError();
	}

	ALuint StaticSoundSample::getBuffer()
	{
		return mBuffer;
	}

	ALuint* StaticSoundSample::getBufferPtr()
	{
		return &mBuffer;
	}

	void StaticSoundSample::setBuffer(ALuint buf)
	{
		mBuffer = buf;
	}

	unsigned int StaticSoundSample::getNumberOfBuffers()
	{
		return 1;
	}

	void StaticSoundSample::play()
	{
		alSourcePlay(mSource);
		checkAlError();
	}

	// Streamed (OGG)
	StreamedSoundSample::StreamedSoundSample(const std::string& name)
	{
		mFilename = name;
		mPlaying = false;
		mType = SAMPLE_OGG;
	}

	StreamedSoundSample::~StreamedSoundSample()
	{
		alSourceStop(mSource);
		checkAlError();

		alDeleteSources(1, &mSource);
		checkAlError();

		alDeleteBuffers(2, mBuffers);
		checkAlError();

		ov_clear(&mStream);
	}

	void StreamedSoundSample::setFile(FILE* ptr)
	{
		if (!ptr)
		{
			return;
		}

		mFile = ptr;
	}

	void StreamedSoundSample::setFormat(ALenum fmt)
	{
		mFormat = fmt;
	}

	void StreamedSoundSample::setRate(ALuint rate)
	{
		mRate = rate;
	}

	void StreamedSoundSample::setPlaying(bool play)
	{
		mPlaying = play;
	}

	OggVorbis_File* StreamedSoundSample::getStreamPtr()
	{
		return &mStream;
	}

	ALuint* StreamedSoundSample::getBufferPtr()
	{
		return mBuffers;
	}

	bool StreamedSoundSample::isPlaying()
	{
		return mPlaying;
	}

	unsigned int StreamedSoundSample::getNumberOfBuffers()
	{
		return 2;
	}

	void StreamedSoundSample::cycle()
	{
		ALint processed;
		bool active = true;
		
		alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);
		checkAlError();
		while (processed > 0)
		{
			ALuint buffer;

			alSourceUnqueueBuffers(mSource, 1, &buffer);
			checkAlError();

			active = stream(buffer);
			if (!active)
				break;

			alSourceQueueBuffers(mSource, 1, &buffer);
			checkAlError();

			alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);
			checkAlError();
		}
	}

	bool StreamedSoundSample::stream(ALuint buffer)
	{
		char data[OGG_BUFFER_SIZE];
		unsigned int size = 0;
		int section;
		int result;

		while (size < OGG_BUFFER_SIZE)
		{
			#ifndef LITTLE_ENDIAN
				result = ov_read(&mStream, data + size, OGG_BUFFER_SIZE - size, 1, 2, 1, &section);
			#else
				result = ov_read(&mStream, data + size, OGG_BUFFER_SIZE - size, 0, 2, 1, &section);
			#endif

			if (result > 0)
			{
				size += result;
			}
			else
			{
				if (result < 0)
				{
					S_LOG_FAILURE("Failed to read from ogg stream.");
					return false;
				}
				else break;
			}
		}

		// Stream is over
		if (!size)
		{
			mPlaying = false;
			return false;
		}

		alBufferData(buffer, mFormat, data, size, mRate);
		checkAlError();

		return true;
	}
		
	void StreamedSoundSample::play()
	{
		// Already Playing
		if (isPlaying())
		{
			return;
		}

		if (!stream(mBuffers[0]))
		{
			return;
		}

		if (!stream(mBuffers[1]))
		{
			return;
		}

		mPlaying = true;
		alSourceQueueBuffers(mSource, 2, mBuffers);
		checkAlError();

		alSourcePlay(mSource);
		checkAlError();
	}

	/* Constructor */
	SoundService::SoundService()
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
		
		#ifndef __WIN32__
			alutInit(NULL, 0);
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
		std::list<BaseSoundSample*>::iterator it;
		for (it = mSamples.begin(); it != mSamples.end(); it++)
		{
			BaseSoundSample* sound = *it;
			if (sound->getFilename() == filename)
			{
				sound->play();	
			}
		}
	}

	void SoundService::stopSound(const std::string &filename)
	{
		std::list<BaseSoundSample*>::iterator it;
		for (it = mSamples.begin(); it != mSamples.end(); it++)
		{
			if ((*it)->getFilename() == filename)
			{
				alSourceStop((*it)->getSource());
				checkAlError();
			}
		}
	}

	bool SoundService::registerSound(const std::string &filename, bool playLocally, const SoundSampleType type)
	{
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
	}

	bool SoundService::unRegisterSound(const std::string &filename)
	{
		std::list<BaseSoundSample*>::iterator it;
		for (it = mSamples.begin(); it != mSamples.end(); )
		{
			if ((*it)->getFilename() == filename)
			{
				BaseSoundSample* sample = *it;
				if (!sample)
					return false;

				it = mSamples.erase(it);
				delete sample;

				return true;
			}
			else ++it;
		}

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
		if (getSoundObject(name))
		{
			S_LOG_INFO("Sound Object (" + name + ") already allocated.");
			return NULL;
		}

		SoundObject* newObject = new SoundObject(name);
		if (!newObject)
		{
			S_LOG_FAILURE("Failed to allocate new sound object.");
			return NULL;
		}

		mObjects.push_back(newObject);
		return newObject;
	}
		
	bool SoundService::allocateWAVPCM(const std::string &filename, bool playsLocally)
	{
		StaticSoundSample* newSample = new StaticSoundSample(filename);
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

		mSamples.push_back(newSample);
		return true;
	}

	bool SoundService::allocateOGG(const std::string &filename, bool playsLocally)
	{
		StreamedSoundSample* newSample = new StreamedSoundSample(filename);
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

		mSamples.push_back(newSample);
		return true;
	}

	BaseSoundSample* SoundService::getSoundSample(const std::string &filename)
	{
		std::list<BaseSoundSample*>::iterator it;
		for (it = mSamples.begin(); it != mSamples.end(); it++)
		{
			BaseSoundSample* sample = *it;
			if (sample && sample->getFilename() == filename)
			{
				return sample;
			}
		}

		return NULL;
	}

	void SoundService::cycle()
	{
		for (std::list<BaseSoundSample*>::iterator it = mSamples.begin(); 
				it != mSamples.end(); it++)
		{
			BaseSoundSample* sample = *it;
			if (sample->getType() == SAMPLE_OGG)
			{
				StreamedSoundSample* ogg = dynamic_cast<StreamedSoundSample*>(*it);
				if (ogg->isPlaying())
				{
					ogg->cycle();
				}
			}
		}

		for (std::list<SoundObject*>::iterator it = mObjects.begin();
				it != mObjects.end(); it++)
		{
			SoundObject* object = *it;
			object->playQueued();
		}
	}

	SoundObject* SoundService::getSoundObject(const std::string &name)
	{
		std::list<SoundObject*>::iterator it;
		for (it = mObjects.begin(); it != mObjects.end(); it++)
		{
			SoundObject* obj = *it;
			if (obj && obj->getName() == name)
			{
				return obj;
			}
		}

		return NULL;
	}

	SoundObject::SoundObject(const std::string &name)
	{
		mName = name;
		mSamples.clear();
	}

	void SoundObject::setPosition(const WFMath::Point<3> &pos)
	{
		mPosition = pos;
	}

	void SoundObject::setVelocity(const WFMath::Vector<3> &vel)
	{
		mVelocity = vel;
	}

	const std::string SoundObject::getName()
	{
		return mName;
	}

	const WFMath::Point<3> SoundObject::getPosition()
	{
		return mPosition;
	}

	const WFMath::Vector<3> SoundObject::getVelocity()
	{
		return mVelocity;
	}

	void SoundObject::playQueued()
	{
		for (std::list<BaseSoundSample*>::iterator it = mSamples.begin(); 
				it != mSamples.end(); it++)
		{
			BaseSoundSample* sample = *it;
			if (sample->getType() == SAMPLE_OGG)
			{
				StreamedSoundSample* ogg = dynamic_cast<StreamedSoundSample*>(*it);
				if (ogg->isPlaying())
				{
					ogg->cycle();
				}
			}
		}
	}

	void SoundObject::playAction(const std::string &name)
	{
		for (std::list<BaseSoundSample*>::iterator it = mSamples.begin(); 
				it != mSamples.end(); it++)
		{
			BaseSoundSample* sample = *it;

			// TODO
		}
	}

	bool SoundObject::allocateWAVPCM(const std::string &filename, const std::string &action,
			bool playsLocally)
	{
		StaticSoundSample* newSample = new StaticSoundSample(filename);
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

		newSample->setAction(action);
		mSamples.push_back(newSample);
		return true;
	}

	bool SoundObject::allocateOGG(const std::string &filename, const std::string &action,
			bool playsLocally)
	{
		StreamedSoundSample* newSample = new StreamedSoundSample(filename);
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

		newSample->setAction(action);

		mSamples.push_back(newSample);
		return true;
	}

	bool SoundObject::registerSound(const std::string &filename, const std::string &action, 
			bool playLocally, const SoundSampleType type)
	{
		std::list<BaseSoundSample*>::iterator it;
		for (it = mSamples.begin(); it != mSamples.end(); it++)
		{
			BaseSoundSample* sound = *it;
			if (sound->getFilename() == filename) 
			{
				S_LOG_INFO("Sound Sample (" + filename + ") already allocated.");
				return false;
			}
			else
			if (sound->getAction() == action)
			{
				S_LOG_INFO("Sound Object Action (" + action + ") already has a buffer allocated.");
				return false;
			}
			else
			{
				// Can be allocated
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
								return allocateWAVPCM(filename, action, playLocally);
							}
							else
							if (extension == "ogg")
							{
								return allocateOGG(filename, action, playLocally);
							}
						}
						return false;
					}
					break;
				case SAMPLE_PCM:
				case SAMPLE_WAV:
					return allocateWAVPCM(filename, action, playLocally);
				case SAMPLE_OGG:
					return allocateOGG(filename, action, playLocally);
				};
			}
		}
	}

	bool SoundObject::unRegisterSound(const std::string &filename)
	{
		std::list<BaseSoundSample*>::iterator it;
		for (it = mSamples.begin(); it != mSamples.end(); )
		{
			BaseSoundSample* sound = *it;
			if (sound->getFilename() == filename) 
			{
				it = mSamples.erase(it);
				delete sound;
			}
			else ++it;
		}
	}

} // namespace Ember

