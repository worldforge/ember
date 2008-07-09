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

#ifndef SOUND_SAMPLE_H
#define SOUND_SAMPLE_H

#include "SoundGeneral.h"

namespace Ember
{

/**
 * Sound Sample 
 *
 * Defines general properties of sound data
 */
class BaseSoundSample
{
	protected:
		std::string			mAction;
		
		ALuint 				mSource;
		SoundSampleType	mType;
		bool					mPlayPosition;

	public:
		virtual ~BaseSoundSample() {}

		// Set Variables
		void setSource(ALuint src);
		void setPosition(const WFMath::Point<3> &pos);
		void setVelocity(const WFMath::Vector<3> &vel);
		void setAction(const std::string &act);

		// Get Variables
		const std::string getAction();

		ALuint getSource();
		ALuint* getSourcePtr();
		SoundSampleType getType();
		virtual ALuint* getBufferPtr() = 0;
		virtual unsigned int getNumberOfBuffers() = 0;

		// Common Methods
		virtual void play();
};

class StaticSoundSample : public BaseSoundSample
{
	private:
		ALuint mBuffer;
	
	public:
		StaticSoundSample();
		~StaticSoundSample();

		// Set Variables
		void setBuffer(ALuint buf);

		// Get Variables
		ALuint	getBuffer();
		ALuint*	getBufferPtr();
		unsigned int getNumberOfBuffers();

		// Common methods
		void play();
};

class StreamedSoundSample : public BaseSoundSample
{
	private:
		FILE*				mFile;
		OggVorbis_File mStream;
		ALuint			mBuffers[2];
		ALenum			mFormat;
		ALuint			mRate;
		bool				mPlaying;

		bool stream(ALuint buffer);

	public:
		StreamedSoundSample();
		~StreamedSoundSample();

		// Set Variables
		void setFile(FILE* ptr);
		void setFormat(ALenum fmt);
		void setRate(ALuint rate);
		void setPlaying(bool play);

		// Get Variables
		ALuint*				getBufferPtr();
		OggVorbis_File*	getStreamPtr();
		bool					isPlaying();
		unsigned int		getNumberOfBuffers();

		// Common methods
		void play();	
		void cycle();				
};

} // namespace Ember

#endif

