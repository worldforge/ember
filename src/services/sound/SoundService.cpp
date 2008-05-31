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
	SoundSample::SoundSample(const std::string &name)
	: filename(name)
	{
	}

	SoundSample::~SoundSample()
	{
		alDeleteBuffers(1, &buffer);
		alDeleteSources(1, &source);
	}

	void SoundSample::setPosition(WFMath::Point<3> &pos)
	{
		alSource3f(source, AL_POSITION, pos.x(), pos.y(), pos.z());
	}

	void SoundSample::setVelocity(WFMath::Point<3> &vel)
	{
		alSource3f(source, AL_VELOCITY, vel.x(), vel.y(), vel.z());
	}

	void SoundSample::setBuffer(ALuint buf)
	{
		buffer = buf;
	}

	void SoundSample::setSource(ALuint src)
	{
		source = src;
	}

	const std::string SoundSample::getFilename()
	{
		return filename;
	}

	ALuint SoundSample::getSource()
	{
		return source;
	}

	ALuint* SoundSample::getSourcePtr()
	{
		return &source;
	}

	ALuint SoundSample::getBuffer()
	{
		return buffer;
	}

	ALuint* SoundSample::getBufferPtr()
	{
		return &buffer;
	}

	/* ctor */
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
		
		if(int error = alGetError() != AL_NO_ERROR)
		{
			S_LOG_FAILURE("Sound Service failed to start: " + 
					std::string(alutGetErrorString(error))); 

			return Service::FAILURE;
		}
		
		samples.clear();
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
		std::list<SoundSample*>::iterator it;
		for (it = samples.begin(); it != samples.end(); it++)
		{
			if ((*it)->getFilename() == filename)
				alSourcePlay((*it)->getSource());
		}
	}

	void SoundService::stopSound(const std::string &filename)
	{
		std::list<SoundSample*>::iterator it;
		for (it = samples.begin(); it != samples.end(); it++)
		{
			if ((*it)->getFilename() == filename)
				alSourceStop((*it)->getSource());
		}
	}

	bool SoundService::registerSound(const std::string &filename, const SoundSampleType type)
	{
		std::list<SoundSample*>::iterator it;
		for (it = samples.begin(); it != samples.end(); it++)
		{
			if ((*it)->getFilename() == filename)
			{
				S_LOG_INFO("Sound Sample (" + filename + ") already allocated.");
				return false;
			}
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
							return allocateWAVPCM(filename);
						else
						if (extension == "ogg")
							return allocateOGG(filename);
					}
					return false;
				}
				break;
			case SAMPLE_PCM:
			case SAMPLE_WAV:
				return allocateWAVPCM(filename);
			case SAMPLE_OGG:
				return allocateOGG(filename);
		};
	}

	bool SoundService::unRegisterSound(const std::string &filename)
	{
		std::list<SoundSample*>::iterator it;
		for (it = samples.begin(); it != samples.end(); )
		{
			if ((*it)->getFilename() == filename)
			{
				SoundSample* sample = dynamic_cast<SoundSample*>(*it);
				if (!sample)
					return false;

				it = samples.erase(it);
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

		// TODO: Convert the quaternion to forward/up vectors
		// alListener3f(AL_ORIENTATION, ListenerOri);
	}

	bool SoundService::allocateWAVPCM(const std::string &filename)
	{
		SoundSample* newSample = new SoundSample(filename);
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
		ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
		ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
	
		newSample->setBuffer(alutCreateBufferFromFile(filename.c_str()));

		if (newSample->getBuffer() == AL_NONE)
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
		alSourcefv(newSample->getSource(), AL_POSITION, SourcePos);
		alSourcefv(newSample->getSource(), AL_VELOCITY, SourceVel);
		alSourcei (newSample->getSource(), AL_LOOPING, loop);

		if (alGetError() != AL_NO_ERROR)
		{
			S_LOG_FAILURE("Failed to set sound sample attributes.");
			delete newSample;

			return false;
		}

		samples.push_back(newSample);
		return true;
	}

	bool SoundService::allocateOGG(const std::string &filename)
	{
		//TODO
	}

} // namespace Ember

