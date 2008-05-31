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

#include <AL/al.h>
#include <AL/alc.h>

#ifndef __WIN32__
#include <AL/alut.h>
#else
#include <ALUT/alut.h>
#endif

#include <wfmath/point.h>
#include <wfmath/vector.h>
#include <wfmath/quaternion.h>

namespace Ember {

typedef enum
{
	SAMPLE_NONE, // Try To guess by file extension
	SAMPLE_PCM,
	SAMPLE_WAV,
	SAMPLE_OGG
} SoundSampleType;

/**
 * Sound Sample 
 *
 * Defines general properties of sound data
 */
class SoundSample
{
	private:
		std::string filename;
		
		ALuint source;
		ALuint buffer;
		WFMath::Point<3> position;
		WFMath::Point<3> velocity;

	public:
		SoundSample(const std::string &name);
		~SoundSample();

		// Data Writing
		void setPosition(WFMath::Point<3> &pos);
		void setVelocity(WFMath::Point<3> &vel);
		void setBuffer(ALuint buf);
		void setSource(ALuint src);

		// Retrieval of Data
		const std::string getFilename();
		ALuint getSource();
		ALuint* getSourcePtr();
		ALuint getBuffer();
		ALuint* getBufferPtr();
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
		std::list<SoundSample*> samples;

		// Allocation Functions
		bool allocateWAVPCM(const std::string &filename); 
		bool allocateOGG(const std::string &filename); 

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
		bool registerSound(const std::string &filename, const SoundSampleType type = SAMPLE_NONE);
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

}; //SoundService

} // namespace Ember

#endif
