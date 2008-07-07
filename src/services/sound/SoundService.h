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

#ifndef SOUNDSERVICE_H
#define SOUNDSERVICE_H

class Service;
class ISoundProvider;

#include "framework/ConsoleObject.h"
#include "framework/ISoundProvider.h"

#include <map>
#include <AL/al.h>
#include <AL/alc.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#ifdef THREAD_SAFE
#include <pthread.h>
#endif

#ifndef __WIN32__
#include <AL/alut.h>
#else
#include <ALUT/alut.h>
#endif

#include <wfmath/point.h>
#include <wfmath/vector.h>
#include <wfmath/quaternion.h>
			 
#include <Atlas/Objects/ObjectsFwd.h>
#include <sigc++/trackable.h>
#include <sigc++/signal.h>

#define OGG_BUFFER_SIZE (4096 * 8)

namespace Ember {

typedef enum
{
	SAMPLE_NONE, // Try To guess by file extension
	SAMPLE_PCM,
	SAMPLE_WAV,
	SAMPLE_OGG
} SoundSampleType;

#define PLAY_LOCAL false
#define PLAY_WORLD true

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

/**
 * Sound Object
 *
 * Objects contains samples that are played from 
 * their position with their velocity
 */
class SoundObject
{
	private:
		WFMath::Point<3> mPosition;
		WFMath::Vector<3> mVelocity;

		std::map<std::string, BaseSoundSample*> mSamples;

		// Allocation Functions
		bool allocateWAVPCM(const std::string &filename, const std::string &action, bool playsLocally); 
		bool allocateOGG(const std::string &filename, const std::string &action, bool playsLocally); 

	public:
		SoundObject();
		~SoundObject();

		// Set Variables
		void setPosition(const WFMath::Point<3> &pos);
		void setVelocity(const WFMath::Vector<3> &vel);

		// Get Variables
		const WFMath::Point<3> getPosition();
		const WFMath::Vector<3> getVelocity();

		// Register new Buffers
		bool registerSound(const std::string &filename, const std::string &action, bool playLocally,
				const SoundSampleType type = SAMPLE_NONE);

		bool unRegisterSound(const std::string &filename);

		// Common methods
		void playQueued();
		void playAction(const std::string &name);
};

/**
 * Ember Sound Service
 *
 * @author Romulo Fernandes Machado (nightz)
 *
 * @see Ember::Service
 * @see Ember::ConsoleObject
 */
class SoundService: public Service, public ConsoleObject
{
	friend class IScriptingProvider;

	private:
		// All Allocated buffers
		std::map<std::string, BaseSoundSample*> mSamples;
		std::map<std::string, SoundObject*> mObjects;
		
		// Allocation Functions
		bool allocateWAVPCM(const std::string &filename, bool playsLocally); 
		bool allocateOGG(const std::string &filename, bool playsLocally); 

		#ifdef THREAD_SAFE
		pthread_mutex_t mSamplesMutex;
		pthread_mutex_t mObjectsMutex;
		#endif

	public:
		SoundService();
		~SoundService();

		// Services
		Service::Status start();
		void stop(int code);

		// ConsoleObject
		void runCommand(const std::string &command, const std::string &args);

		/**
		 * Register(allocate) a new sound buffer
		 */
		SoundObject* registerObject(const std::string &name);
		bool registerSound(const std::string &filename, bool playLocally = PLAY_WORLD,
				const SoundSampleType type = SAMPLE_NONE);
		bool unRegisterSound(const std::string &filename);

		/**
		 * Play Functions
		 */
		void playSound(const std::string &filename);
		void stopSound(const std::string &filename);

		/**
		 * Update the position (in world coordinates)
		 * of the listener
		 */
		void updateListenerPosition(
		const WFMath::Point<3>& position,
		const WFMath::Quaternion& orientation);

		/**
		 * Streaming update
		 */
		void cycle();
		
		/**
		 * From server
		 */
		void onAction(const Atlas::Objects::Operation::RootOperation& act);

		/**
		 * Allocated Entities
		 */
		BaseSoundSample* getSoundSample(const std::string &filename);
		SoundObject* getSoundObject(const std::string &name);

}; //SoundService

} // namespace Ember

#endif
