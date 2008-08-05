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

#include "SoundGeneral.h"
#include "SoundSample.h" 
#include "SoundEntity.h"

namespace Ember {

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
		std::map<std::string, SoundEntity*> mEntities;

		// Instances
		StaticSoundSample* instStaticSample(StaticSoundSample* base);
		StreamedSoundSample* instStreamedSample(StreamedSoundSample* base);
		
		// Allocation Functions
		bool allocateWAVPCM(const std::string &filename, bool playsLocally); 
		bool allocateOGG(const std::string &filename, bool playsLocally); 

		#ifdef THREAD_SAFE
		pthread_mutex_t mSamplesMutex;
		pthread_mutex_t mEntitiesMutex;
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
		SoundEntity* registerEntity(const std::string &name);
		bool registerInstance(BaseSoundSample* base, BaseSoundSample* copy);
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
		 * Allocated Entities
		 */
		BaseSoundSample* getSoundSample(const std::string &filename);
		SoundEntity* getSoundEntity(const std::string &name);

}; //SoundService

} // namespace Ember

#endif
