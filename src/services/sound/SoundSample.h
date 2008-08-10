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
		/**
		 * openAl internal
		 */
		ALuint 				mSource;

		/**
		 * Type of the sample
		 */
		SoundSampleType	mType;

		/**
		 * If the sound should be play locally(2D) or not(3D).
		 */
		bool					mPlayPosition;

	public:
		virtual ~BaseSoundSample() {}

		/**
		 * Set sample openAl source
		 *
		 * @param src The new source.
		 */
		void setSource(ALuint src);

		/**
		 * Set sample position
		 *
		 * @param pos The new sound position
		 */
		void setPosition(const WFMath::Point<3> &pos);

		/**
		 * Set sample velocity 
		 *
		 * @param vel The new sound velocity
		 */
		void setVelocity(const WFMath::Vector<3> &vel);

		/**
		 * Return openAl source within this sample
		 */
		ALuint getSource();

		/**
		 * Return openAl pointer to source within this sample
		 */
		ALuint* getSourcePtr();

		/**
		 * Returns entity type
		 */
		SoundSampleType getType();

		virtual ALuint* getBufferPtr() = 0;
		virtual unsigned int getNumberOfBuffers() = 0;
		virtual void play();
		virtual void stop();
};

/**
 * The class StaticSoundSample is responsible
 * to keep track of samples that doesnt need
 * often updates and only have one buffer
 */
class StaticSoundSample : public BaseSoundSample
{
	private:
		/**
		 * Sample buffer
		 */
		ALuint mBuffer;
	
	public:
		StaticSoundSample();
		~StaticSoundSample();

		/**
		 * Set this sample buffer
		 *
		 * @param buf The new sample buffer
		 */
		void setBuffer(ALuint buf);

		/**
		 * Returns the unique buffer this sample has.
		 */
		ALuint	getBuffer();

		/**
		 * Returns an pointer to the unique buffer this sample has.
		 */
		ALuint*	getBufferPtr();

		/**
		 * Within this class, this is always 1.
		 */
		unsigned int getNumberOfBuffers();

		// Common methods
		void play();
		void stop();
};

/**
 * The class StreamedSoundSample is responsible
 * to keep track of samples that often need
 * updates and requires more than a buffer to stream
 * data.
 */
class StreamedSoundSample : public BaseSoundSample
{
	private:
		/**
		 * Filename with full path to the data.
		 */
		std::string		mFilename;

		/**
		 * A pointer to the file specified in mFilename
		 */
		FILE*				mFile;

		/**
		 * VORBIS Internal Stream 
		 */
		OggVorbis_File mStream;

		/**
		 * Front and back buffers for openAl
		 */
		ALuint			mBuffers[2];

		/**
		 * Format of the stream (checked from ogg/vorbis)
		 */
		ALenum			mFormat;

		/**
		 * Rate of the stream (checked from ogg/vorbis)
		 */
		ALuint			mRate;

		/**
		 * If this stream is playing
		 */
		bool				mPlaying;

		/**
		 * This function is responsible to fill
		 * buffers from stream data
		 *
		 * @param buffer The destination openAl buffer
		 * @return Status of the streaming
		 */
		bool stream(ALuint buffer);

	public:
		StreamedSoundSample();
		~StreamedSoundSample();

		/**
		 * Set the file to be used in stream proccess.
		 *
		 * @param ptr A pointer to the file.
		 * @param filename The file name with full path.
		 */
		void setFile(FILE* ptr, const std::string& filename);

		/**
		 * Set stream format
		 */
		void setFormat(ALenum fmt);

		/**
		 * Set stream rate
		 */
		void setRate(ALuint rate);

		/**
		 * Set the stream status (if playing or not)
		 */
		void setPlaying(bool play);

		/**
		 * Returns a pointer to the buffers array
		 */
		ALuint*				getBufferPtr();

		/**
		 * Returns a pointer to the stream information (vorbis internals).
		 */
		OggVorbis_File*	getStreamPtr();

		/**
		 * Return the state of the stream.
		 */
		bool					isPlaying();

		/**
		 * Return the number of buffers in this stream. In this case this is 2.
		 */
		unsigned int		getNumberOfBuffers();

		/**
		 * Return the full filename of the stream file.
		 */
		const std::string& getFilename();

		// Common methods
		void play();	
		void stop();
		void cycle();				
};

} // namespace Ember

#endif

