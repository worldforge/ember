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

#include "SoundObject.h"
#include "SoundGeneral.h"

namespace Ember
{
	SoundObject::SoundObject()
	{
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
	}

	void SoundObject::playAction(const std::string &name)
	{
		BaseSoundSample* sample = mSamples[name];
		// TODO
	}

	bool SoundObject::allocateWAVPCM(const std::string &filename, const std::string &action,
			bool playsLocally)
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

		newSample->setAction(action);
		mSamples[filename] = newSample;
		return true;
	}

	bool SoundObject::allocateOGG(const std::string &filename, const std::string &action,
			bool playsLocally)
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

		newSample->setAction(action);

		mSamples[filename] = newSample;
		return true;
	}

	bool SoundObject::registerSound(const std::string &filename, const std::string &action, 
			bool playLocally, const SoundSampleType type)
	{
		BaseSoundSample* sound = mSamples[filename];
		if (sound) 
		{
			S_LOG_INFO("Sound Sample (" + filename + ") already allocated.");
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

	bool SoundObject::unRegisterSound(const std::string &filename)
	{
		BaseSoundSample* sound = mSamples[filename];
		if (sound)
		{
			mSamples.erase(filename);
			delete sound;

			return true;
		}

		return false;
	}
}

