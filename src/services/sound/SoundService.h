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
#include "SoundDefinition.h"
#include "SoundSample.h" 

namespace Ember {

class IResourceProvider;
class StreamedSoundSample;
class SoundInstance;
class SoundGroup;

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

	typedef std::vector<SoundInstance*> SoundInstanceStore;
	typedef std::map<std::string, BaseSoundSample*> SoundSampleStore;
	friend class IScriptingProvider;

	private:
		/**
		 * Keep a list of the samples we are allocating.
		 * That way, we can cycle and update the streamed ones.
		 */
		std::list<StreamedSoundSample*> mSamples;
		
		SoundSampleStore mBaseSamples;

		/**
		 * Keep a list of the sound groups we are allocating.
		 * That way, we can cycle and update their timers/playability
		 */
		std::list<SoundGroup*> mGroups;

		/**
		 * Thats the list of the sound groups parsed in
		 * sounddefs
		 */
		std::map<std::string, SoundGroupDefinition*> mSoundGroupDefinitions;

		#ifdef THREAD_SAFE
		/**
		 * In case we are using threads, we must lock the mutexes to 
		 * prevent incorrect writing to the sample lists
		 */
		pthread_mutex_t mGroupModelsMutex;
		pthread_mutex_t mGroupsMutex;
		pthread_mutex_t mSamplesMutex;
		#endif
		
		#ifdef __WIN32__
			ALCcontext *mContext;
			ALCdevice *mDevice;
		#endif
		
		SoundInstanceStore mInstances;

	IResourceProvider* mResourceProvider;
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
		
		
		BaseSoundSample* createOrRetrieveSoundSample(const std::string& soundPath);
		
		bool destroySoundSample(const std::string& soundPath);

		/**
		 * Register a new SoundDefinition used to define soundgroups
		 *
		 * @return A pointer to the new created SoundDefinition, if it fails, returns NULL
		 */
		SoundGroupDefinition* createSoundGroupDefinition(const std::string& name);

		/**
		 * Returns the SoundDefinition from its name
		 *
		 * @param name The desired SoundDefinition name
		 * @return A pointer to the SoundDefinition or NULL if it can't be found
		 */
		SoundGroupDefinition* getSoundGroupDefinition(const std::string& name);

		/**
		 * Register individual StreamedSamples to keep updated
		 * on the cycle calls
		 *
		 * @param copy The StreamedSoundSample to be registered
		 */
		void registerStream(StreamedSoundSample* copy);

		/**
		 * Unregister streamed allocate sound buffers
		 * This will only remove it from the service list
		 * it will not deallocate the data.
		 *
		 * @param sample A pointer to the sample to be unregistered
		 * @return The status of the unregistration.
		 */
		bool unregisterStream(const StreamedSoundSample* sample);

		/**
		 * Register individual SoundGroups to keep updated
		 * on the cycle calls
		 *
		 * @param copy The sound group to be registered
		 */
		void registerSoundGroup(SoundGroup* copy);

		/**
		 * Unregister SoundGroups allocated on SoundActions.
		 * This will only remove it from the service list
		 * it will not deallocate the data.
		 *
		 * @param sample A pointer to the group to be unregistered
		 * @return The status of the unregistration.
		 */
		bool unregisterSoundGroup(const SoundGroup* sample);

		/**
		 * Update the position (in world coordinates)
		 * of the listener
		 *
		 * @param position The new listener position.
		 * @param orientation The listener orientation.
		 */
		void updateListenerPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation);

		/**
		 * Streaming update
		 */
		void cycle();
		
		
		SoundInstance* createInstance();
		bool destroyInstance(SoundInstance* instance);
		
	IResourceProvider* getResourceProvider();
	
	void setResourceProvider(Ember::IResourceProvider* resourceProvider);
		

}; //SoundService

} // namespace Ember

#endif
