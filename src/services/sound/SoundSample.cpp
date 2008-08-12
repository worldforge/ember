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

#include "SoundSample.h"
#include "SoundGeneral.h"

namespace Ember
{
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
	
	void BaseSoundSample::play()
	{
	}

	void BaseSoundSample::stop()
	{
	}

	// Static Sounds (PCM, WAV)
	StaticSoundSample::StaticSoundSample()
	{
		mType = SAMPLE_WAV;
	}

	StaticSoundSample::~StaticSoundSample()
	{
		if (alIsBuffer(mBuffer))
		{
			alDeleteBuffers(1, &mBuffer);
			checkAlError();
		}

		if (alIsSource(mSource))
		{
			alDeleteSources(1, &mSource);
			checkAlError();
		}
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

	BaseSoundSample* StaticSoundSample::instantiate()
	{
		StaticSoundSample* newSample = new StaticSoundSample();
		if (!newSample)
		{
			S_LOG_FAILURE("Failed to allocate a new sound sample.");
			return NULL;
		}

		// Bind the buffer with the source.
		alGenSources(1, newSample->getSourcePtr());

		if (alGetError() != AL_NO_ERROR)
		{
			S_LOG_FAILURE("Failed to generate a new sound source.");
			delete newSample;

			return NULL;
		}
	
		alSourcei (newSample->getSource(), AL_BUFFER, getBuffer());
		alSourcef (newSample->getSource(), AL_PITCH, 1.0);
		alSourcef (newSample->getSource(), AL_GAIN, 1.0);
		alSource3f(newSample->getSource(), AL_POSITION, 0, 0, 0);
		alSource3f(newSample->getSource(), AL_VELOCITY, 0, 0, 0);

		ALboolean loop = false;
		alSourcei (newSample->getSource(), AL_LOOPING, loop);

		int playsLocally = 0;
		alGetSourcei(getSource(), AL_SOURCE_RELATIVE, &playsLocally);
		alSourcei(newSample->getSource(), AL_SOURCE_RELATIVE, playsLocally);

		if (alGetError() != AL_NO_ERROR)
		{
			S_LOG_FAILURE("Failed to set sound sample attributes.");
			delete newSample;

			return NULL;
		}

		return newSample;
	}

	void StaticSoundSample::play()
	{
		alSourcePlay(mSource);
		checkAlError();
	}

	void StaticSoundSample::stop()
	{
		alSourceStop(mSource);
		checkAlError();
	}

	// Streamed (OGG)
	StreamedSoundSample::StreamedSoundSample()
	{
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

	const std::string& StreamedSoundSample::getFilename()
	{
		return mFilename;
	}

	void StreamedSoundSample::setFile(FILE* ptr, const std::string& filename)
	{
		if (!ptr)
		{
			S_LOG_FAILURE("Invalid OGG file pointer");
			return;
		}

		mFile = ptr;
		mFilename = filename;
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

	BaseSoundSample* StreamedSoundSample::instantiate()
	{
		StreamedSoundSample* newSample = new StreamedSoundSample();
		if (!newSample)
		{
			S_LOG_FAILURE("Failed to allocate memory for a new sound source.");
			return NULL;
		}

		// Should we handle this in Ogre or any other Resource Manager?
		FILE* newFile = NULL;
		if (!(newFile = fopen(getFilename().c_str(), "rb")))
		{
			S_LOG_FAILURE("Failed to open file.");
			delete newSample;

			return false;
		}

		newSample->setFile(newFile, getFilename());

		if (ov_open(newFile, newSample->getStreamPtr(), NULL, 0) < 0)
		{
Error0:
			S_LOG_FAILURE("Failed to bind ogg stream to sound sample.");

			fclose(newFile);
			delete newSample;

			return NULL;
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

		int playsLocally = 0;
		alGetSourcei(getSource(), AL_SOURCE_RELATIVE, &playsLocally);
		alSourcei(newSample->getSource(), AL_SOURCE_RELATIVE, playsLocally);

		return newSample;
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

	void StreamedSoundSample::stop()
	{
		mPlaying = false;
		alSourceStop(mSource);
		checkAlError();
	}

}

