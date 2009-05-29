/*
 * Copyright (C) 2008 Romulo Fernandes Machado (nightz)
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 675 Mass
 * Ave, Cambridge, MA 02139, USA.
 */

#ifndef SOUND_SAMPLE_H
#define SOUND_SAMPLE_H

#include "SoundGeneral.h"
#include "SoundBinding.h"
#include "framework/IResourceProvider.h"
#include <vector>

#ifdef __APPLE__
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif


namespace Ember
{
class SoundSource;


/**
 * Sound Sample 
 *
 * Defines general properties of sound data
 */
class BaseSoundSample
{
public:
	
	typedef std::vector<ALuint> BufferStore;

	/**
	 * Dtor.
	 */
	virtual ~BaseSoundSample() { }
	
	/**
	 * @return entity type
	 */
	SoundGeneral::SoundSampleType getType() const;

	/**
	 * @brief Returns the number of buffers stored for this sample.
	 *
	 * @return The number of buffers.
	 */
	virtual unsigned int getNumberOfBuffers() const = 0;
	
	/**
	 * @brief Returns a store of the sound data buffers stored by this sample.
	 *
	 * The buffers will be returned as ALuint which is the internal buffer
	 * reference within OpenAL. Any further operation on the buffer must
	 * therefore go through OpenAL (i.e. the values returned are _not_
	 * memory pointers).
	 *
	 * @return A store of OpenAL buffer identifiers.
	 */
	virtual BufferStore getBuffers() const = 0;
	
	/**
	 * @brief Creates a new binding to this buffer, to be used together with
	 * an instance of SoundInstance.
	 *
	 * If you want the sound held by this buffer to be played, one way would
	 * be to call this to create a binding which you then feed to an
	 * instance of SoundInstance.
	 *
	 * Note that ownership of the created binding is transferred to the
	 * caller, and thus it's the caller's responsibility to make sure it's
	 * properly deleted. Under normal circumstances that will be taken care
	 * of by SoundInstance however.
	 *
	 * @see SoundInstance::bind()
	 *
	 * @param source The sound source to which we should bind this sound
	 * sample.
	 *
	 * @return A new sound binding instance.
	 */
	virtual SoundBinding* createBinding(SoundSource& source) = 0;

protected:

	/**
	 * @brief Ctor. This is protected to disallow direct creation of this class except by subclasses.
	 */
	BaseSoundSample() { }

	/**
	 * Type of the sample
	 */
	SoundGeneral::SoundSampleType	mType;
};

/**
 * The class StaticSoundSample is responsible to keep track of samples that
 * doesnt need often updates and only have one buffer
 */
class StaticSoundSample : public BaseSoundSample
{
public:
	/**
	 * Ctor.
	 *
	 * @param resource Resource associated with this sample
	 *
	 * @param playsLocal ??? (not used)
	 *
	 * @param volume Volume for the sample (not used)
	 */
	StaticSoundSample(const ResourceWrapper& resource, bool playsLocal, float volume);

	/**
	 * Dtor.
	 */
	~StaticSoundSample();

	/**
	 * @return the unique buffer this sample has.
	 */
	ALuint getBuffer() const;

	/**
	 * Within this class, this is always 1.
	 */
	unsigned int getNumberOfBuffers() const;

	/**
	 * @copydoc BaseSoundSample::createBinding()
	 */
	virtual SoundBinding* createBinding(SoundSource& source);

	/**
	 * @copydoc BaseSoundSample::getBuffers()
	 */
	virtual BaseSoundSample::BufferStore getBuffers() const;

private:
	/**
	 * Sample buffer
	 */
	ALuint mBuffer;
	
	/**
	 * @brief The resource wrapper instance which holds the actual data.
	 */
	ResourceWrapper mResource;
};


/**
 * @brief A binding to a "static" sound source, i.e. a sound source which
 * doesn't have to be updated.
 *
 * A "static" sound is one that is small enough to fit into one continous
 * buffer, and thus doesn't need to be dynamically updated as is the case with
 * "streaming" sounds. As a result, this binding is very simple and will just
 * bind the sound data to the source in the constructor, without having to
 * provide any functionality in the update() method.
 *
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class StaticSoundBinding : public SoundBinding
{
public:

	/**
	 * @brief Ctor. All bindings between the buffer and the sound source will occur here.
	 *
	 * @param source The sound source.
	 *
	 * @param sample The static sound sample to bind to the source.
	 */
	StaticSoundBinding(SoundSource& source, StaticSoundSample& sample);

	/**
	 * @copydoc SoundBinding::update()
	 */
	virtual void update() {
		// Since it's a static sound we don't need to update anything.
	}

protected:

	/**
	 * @brief The static sound samle used for binding.
	 *
	 * There's really no need to keep this around here, since the binding
	 * will occur in the constructor, but hey, someday we might provide some
	 * kind of dynamic unloading/reloading...
	 */
	StaticSoundSample& mSample;
};


/**
 * The class StreamedSoundSample is responsible to keep track of samples that
 * often need updates and requires more than a buffer to stream data.
 *
 * \todo not implemented
 */
// class StreamedSoundSample : public BaseSoundSample
// {
// 	private:
// 		/**
// 		 * Filename with full path to the data.
// 		 */
// 		std::string		mFilename;
// 
// 		/**
// 		 * A pointer to the file specified in mFilename
// 		 */
// 		FILE*				mFile;
// 
// 		/**
// 		 * VORBIS Internal Stream 
// 		 */
// 		OggVorbis_File mStream;
// 
// 		/**
// 		 * Front and back buffers for openAl
// 		 */
// 		ALuint			mBuffers[2];
// 
// 		/**
// 		 * Format of the stream (checked from ogg/vorbis)
// 		 */
// 		ALenum			mFormat;
// 
// 		/**
// 		 * Rate of the stream (checked from ogg/vorbis)
// 		 */
// 		ALuint			mRate;
// 
// 		/**
// 		 * If this stream is playing
// 		 */
// 		bool				mPlaying;
// 
// 		/**
// 		 * This function is responsible to fill
// 		 * buffers from stream data
// 		 *
// 		 * @param buffer The destination openAl buffer
// 		 * @return Status of the streaming
// 		 */
// 		bool stream(ALuint buffer);
// 
// 	public:
// 		StreamedSoundSample(const std::string& filename, bool playsLocal, float volume);
// 		~StreamedSoundSample();
// 
// 		/**
// 		 * Set the file to be used in stream proccess.
// 		 *
// 		 * @param ptr A pointer to the file.
// 		 * @param filename The file name with full path.
// 		 */
// 		void setFile(FILE* ptr, const std::string& filename);
// 
// 		/**
// 		 * Set stream format
// 		 */
// 		void setFormat(ALenum fmt);
// 
// 		/**
// 		 * Set stream rate
// 		 */
// 		void setRate(ALuint rate);
// 
// 		/**
// 		 * Set the stream status (if playing or not)
// 		 */
// 		void setPlaying(bool play);
// 
// 		/**
// 		 * Returns a pointer to the buffers array
// 		 */
// 		ALuint*				getBufferPtr();
// 
// 		/**
// 		 * Returns a pointer to the stream information (vorbis internals).
// 		 */
// 		OggVorbis_File*	getStreamPtr();
// 
// 		/**
// 		 * Return the state of the stream.
// 		 */
// 		bool					isPlaying();
// 
// 		/**
// 		 * Return the number of buffers in this stream. In this case this is 2.
// 		 */
// 		unsigned int		getNumberOfBuffers();
// 
// 		/**
// 		 * Return the full filename of the stream file.
// 		 */
// 		const std::string& getFilename();
// 
// 		// Common methods
// 		void play();	
// 		void stop();
// 		void cycle();				
// };

} // namespace Ember

#endif

