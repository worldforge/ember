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

#include "SoundSample.h"

#include "framework/LoggingInstance.h"

#include "SoundSource.h"

#ifndef _MSC_VER
#include <AL/alut.h>
#else
#include <ALUT/alut.h>
#endif


namespace Ember
{

StaticSoundBinding::StaticSoundBinding(SoundSource& source, StaticSoundSample& sample)
: SoundBinding(source), mSample(sample)
{
	// Bind it to the buffer.
	alSourcei(source.getALSource(), AL_BUFFER, sample.getBuffer());
	SoundGeneral::checkAlError("Binding sound source to static sound buffer.");
}

SoundGeneral::SoundSampleType BaseSoundSample::getType() const
{
	return mType;
}


StaticSoundSample::StaticSoundSample(const ResourceWrapper& resource, bool playsLocal, float volume)
: mBuffer(0), mResource(resource)
{
	mType = SoundGeneral::SAMPLE_WAV;
	mBuffer = alutCreateBufferFromFileImage(mResource.getDataPtr(), mResource.getSize());
	
	if (!SoundGeneral::checkAlError("Generated buffer for static sample.")) {
		alDeleteBuffers(1, &mBuffer);
	}
}

StaticSoundSample::~StaticSoundSample()
{
	if (alIsBuffer(mBuffer)) {
		alDeleteBuffers(1, &mBuffer);
		SoundGeneral::checkAlError("Deleting static sound buffers.");
	}
}

ALuint StaticSoundSample::getBuffer() const
{
	return mBuffer;
}
	
BaseSoundSample::BufferStore StaticSoundSample::getBuffers() const
{
	BaseSoundSample::BufferStore buffers;
	buffers.push_back(mBuffer);
	return buffers;
}


SoundBinding* StaticSoundSample::createBinding(SoundSource& source)
{
	return new StaticSoundBinding(source, *this);
}

unsigned int StaticSoundSample::getNumberOfBuffers() const
{
	return 1;
}

	// Streamed (OGG)
// 	StreamedSoundSample::StreamedSoundSample(const std::string& filename, bool playsLocal, float volume)
// 	{
// 		mPlaying = false;
// 		mType = SAMPLE_OGG;
// 
// 		FILE* newFile = fopen(filename.c_str(), "rb");
// 		if (!newFile)
// 		{
// 			S_LOG_FAILURE("Failed to open file(" + filename + ") to stream.");
// 			return;
// 		}
// 
// 		mFile = newFile;
// 		mFilename = filename;
// 
// 		if (ov_open(newFile, &mStream, NULL, 0) < 0)
// 		{
// 			S_LOG_FAILURE("Failed to bind ogg stream to sound sample.");
// 
// 			fclose(newFile);
// 			return;
// 		}
// 
// 		vorbis_info* oggInfo = ov_info(&mStream, -1);
// 		if (oggInfo->channels == 1)
// 		{
// 			mFormat = AL_FORMAT_MONO16;
// 		}
// 		else
// 		{
// 			mFormat = AL_FORMAT_STEREO16;
// 		}
// 
// 		mRate = oggInfo->rate;
// 
// 		alGenBuffers(2, mBuffers);
// 		if (alGetError() != AL_NO_ERROR)
// 		{
// 			S_LOG_FAILURE("Failed to bind ogg stream to sound sample.");
// 
// 			fclose(newFile);
// 			return;
// 		}
// 
// 		alGenSources(1, &mSource);
// 		if (alGetError() != AL_NO_ERROR)
// 		{
// 			S_LOG_FAILURE("Failed to bind ogg stream to sound sample.");
// 
// 			alDeleteBuffers(2, mBuffers);
// 			fclose(newFile);
// 
// 			return;
// 		}
// 
// 		alSourcef(mSource, AL_PITCH, 1.0);
// 		alSourcef(mSource, AL_GAIN, volume);
// 		alSource3f(mSource, AL_POSITION, 0, 0, 0);
// 		alSource3f(mSource, AL_VELOCITY, 0, 0, 0);
// 		alSourcei(mSource, AL_LOOPING, false);
// 
// 		if (playsLocal == PLAY_LOCAL)
// 			alSourcei(mSource, AL_SOURCE_RELATIVE, true);
// 
// 		// Ask the Sound Service to register us
// 		// that we way we keep playing =]
// 		EmberServices::getSingleton().getSoundService()->registerStream(this);
// 	}
// 
// 	StreamedSoundSample::~StreamedSoundSample()
// 	{
// 		EmberServices::getSingleton().getSoundService()->unregisterStream(this);
// 
// 		alSourceStop(mSource);
// 		checkAlError();
// 
// 		alDeleteSources(1, &mSource);
// 		checkAlError();
// 
// 		alDeleteBuffers(2, mBuffers);
// 		checkAlError();
// 
// 		ov_clear(&mStream);
// 	}
// 
// 	const std::string& StreamedSoundSample::getFilename()
// 	{
// 		return mFilename;
// 	}
// 
// 	void StreamedSoundSample::setPlaying(bool play)
// 	{
// 		mPlaying = play;
// 	}
// 
// 	OggVorbis_File* StreamedSoundSample::getStreamPtr()
// 	{
// 		return &mStream;
// 	}
// 
// 	ALuint* StreamedSoundSample::getBufferPtr()
// 	{
// 		return mBuffers;
// 	}
// 
// 	bool StreamedSoundSample::isPlaying()
// 	{
// 		return mPlaying;
// 	}
// 
// 	unsigned int StreamedSoundSample::getNumberOfBuffers()
// 	{
// 		return 2;
// 	}
// 
// 	void StreamedSoundSample::cycle()
// 	{
// 		ALint processed;
// 		bool active = true;
// 		
// 		alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);
// 		checkAlError();
// 		while (processed > 0)
// 		{
// 			ALuint buffer;
// 
// 			alSourceUnqueueBuffers(mSource, 1, &buffer);
// 			checkAlError();
// 
// 			active = stream(buffer);
// 			if (!active)
// 				break;
// 
// 			alSourceQueueBuffers(mSource, 1, &buffer);
// 			checkAlError();
// 
// 			alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);
// 			checkAlError();
// 		}
// 	}
// 
// 	bool StreamedSoundSample::stream(ALuint buffer)
// 	{
// 		char data[OGG_BUFFER_SIZE];
// 		unsigned int size = 0;
// 		int section;
// 		int result;
// 
// 		while (size < OGG_BUFFER_SIZE)
// 		{
// 			#ifndef LITTLE_ENDIAN
// 				result = ov_read(&mStream, data + size, OGG_BUFFER_SIZE - size, 1, 2, 1, &section);
// 			#else
// 				result = ov_read(&mStream, data + size, OGG_BUFFER_SIZE - size, 0, 2, 1, &section);
// 			#endif
// 
// 			if (result > 0)
// 			{
// 				size += result;
// 			}
// 			else
// 			{
// 				if (result < 0)
// 				{
// 					S_LOG_FAILURE("Failed to read from ogg stream.");
// 					return false;
// 				}
// 				else break;
// 			}
// 		}
// 
// 		// Stream is over
// 		if (!size)
// 		{
// 			mPlaying = false;
// 			return false;
// 		}
// 
// 		alBufferData(buffer, mFormat, data, size, mRate);
// 		checkAlError();
// 
// 		return true;
// 	}
// 		
// 	void StreamedSoundSample::play()
// 	{
// 		// Already Playing
// 		if (isPlaying())
// 		{
// 			return;
// 		}
// 
// 		if (!stream(mBuffers[0]))
// 		{
// 			return;
// 		}
// 
// 		if (!stream(mBuffers[1]))
// 		{
// 			return;
// 		}
// 
// 		mPlaying = true;
// 		alSourceQueueBuffers(mSource, 2, mBuffers);
// 		checkAlError();
// 
// 		alSourcePlay(mSource);
// 		checkAlError();
// 	}
// 
// 	void StreamedSoundSample::stop()
// 	{
// 		mPlaying = false;
// 		alSourceStop(mSource);
// 		checkAlError();
// 	}

}

