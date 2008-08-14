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
		/**
		 * Keep a list of the samples we are allocating.
		 * That way, we can cycle and update the streamed ones.
		 */
		std::list<StreamedSoundSample*> mCopySamples;
		std::map<std::string, BaseSoundSample*> mSamples;

		/**
		 * All entities we allocate, keep a reference of them
		 */
		std::map<std::string, SoundEntity*> mEntities;

		/**
		 * Used to allocate a sound of each type. We treat WAV and PCM as the same sample types
		 * since they dont require any special parsing stuff.
		 */
		bool allocateWAVPCM(const std::string &filename, bool playsLocally, float soundVolume); 
		bool allocateOGG(const std::string &filename, bool playsLocally, float soundVolume); 

		#ifdef THREAD_SAFE
		/**
		 * In case we are using threads, we must lock the mutexes to 
		 * prevent incorrect writing to the sample lists
		 */
		pthread_mutex_t mCopyMutex;
		pthread_mutex_t mSamplesMutex;
		pthread_mutex_t mEntitiesMutex;
		#endif

	public:
		/**
		 * Constructor
		 */
		SoundService();
		~SoundService();

		/**
		 * Start the Sound Service
		 */
		Service::Status start();

		/**
		 * Stop the Sound Service
		 */
		void stop(int code);

		/**
		 * From console you can call functions to the Sound Service
		 */
		void runCommand(const std::string &command, const std::string &args);

		/**
		 * Register a new Sound Entity
		 *
		 * @param name The new Entity name
		 * @return A pointer to the new entity or NULL if it fails.
		 */
		SoundEntity* registerEntity(const std::string &name);

		/**
		 * Ask SoundService to instantiate a Sound Sample
		 *
		 * @param base A base to get the informations from.
		 * @param copy The destination sample.
		 * @return The status of the instantiation.
		 */
		bool registerInstance(BaseSoundSample* base, BaseSoundSample** copy);

		/**
		 * Ask SoundService to register a copy of this streamed instance
		 * to keep it on the update cycle (because of ogg feeding)
		 *
		 * @param copy The sample to be registered
		 */
		void registerStreamedCopy(BaseSoundSample* copy);

		/**
		 * This function registers individual sound buffers, used on sound samples
		 *
		 * @param filename The file full path to be allocated.
		 * @param playLocally If the sound is 2D or 3D.
		 * @param type The type of the sound.
		 * @param soundVolume The desired volume to play this sound (0 to 1.0f)
		 * @return The status of the sound registration.
		 */
		bool registerSound(const std::string &filename, bool playLocally = PLAY_WORLD,
				const SoundSampleType type = SAMPLE_NONE, float soundVolume = 1.0f);

		/**
		 * Unregister sound buffers from their names
		 *
		 * @param filename The sound filename used to allocate the sample.
		 * @return The status of the unregistration.
		 */
		bool unRegisterSound(const std::string &filename);

		/**
		 * Play an individual sound sample (for things like GUI elements)
		 *
		 * @param filename The sample filename to be played.
		 */
		void playSound(const std::string &filename);

		/**
		 * Stop a playing sound sample
		 *
		 * @param filename The sample filename to be stopped.
		 */
		void stopSound(const std::string &filename);

		/**
		 * Update the position (in world coordinates)
		 * of the listener
		 *
		 * @param position The new listener position.
		 * @param orientation The listener orientation.
		 */
		void updateListenerPosition(
		const WFMath::Point<3>& position,
		const WFMath::Quaternion& orientation);

		/**
		 * Streaming update
		 */
		void cycle();
		
		/**
		 * Get a Sound Sample from its filename.
		 *
		 * @param filename The sample filename.
		 * @return A pointer to the sample and if it doesnt find, NULL.
		 */
		BaseSoundSample* getSoundSample(const std::string &filename);

		/**
		 * Get a Sound Entity from its name.
		 *
		 * @param name The entity name.
		 * @return A pointer to the entity and if it doesnt find, NULL.
		 */
		SoundEntity* getSoundEntity(const std::string &name);

}; //SoundService

} // namespace Ember

#endif
