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

	void StaticSoundSample::play()
	{
		alSourcePlay(mSource);
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

	void StreamedSoundSample::setFile(FILE* ptr)
	{
		if (!ptr)
		{
			S_LOG_FAILURE("Invalid OGG file pointer");
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

}

