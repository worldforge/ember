/*
 * Copyright (C) 2008 Romulo Fernandes Machado (nightz)
 * Copyright (C) 2008 Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * Copyright (C) 2009 by Manuel A. Fernandez Montecelo <manuel.montezelo@gmail.com>
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

#ifndef SOUNDSERVICE_H
#define SOUNDSERVICE_H

#include "framework/Service.h"
#include "framework/ConsoleObject.h"
#include "SoundGeneral.h"

#include <wfmath/vector.h>
#include <wfmath/quaternion.h>
#include <wfmath/point.h>

#include <vector>
#include <map>

namespace Ember {

class IResourceProvider;
class StreamedSoundSample;
class SoundInstance;
class SoundGroup;
class BaseSoundSample;

/**
 * @brief A service responsible for playing and managing sounds.
 *
 * In normal operations, the only way to play a sound is to first request a new
 * instance of SoundInstance throug createInstance(), binding that instance to
 * one or many sound samples and then asking the SoundInstance to start
 * playing. Once the SoundInstance is done playing it should be returned through
 * destroyInstance(). Since it's expected that not too many sounds should be
 * playing at one time it's not expected to be too many live instances of
 * SoundInstance at any time.
 *
 * Before you can start requesting sound instances and binding them to samples
 * you must however set up the service. The first thing that needs to be set up
 * is a resource provider through the IResourceProvider interface. The resource
 * provider is responsible for providing any resource when so asked, and is the
 * main interface into the actual sound data.
 *
 * @author Romulo Fernandes Machado (nightz)
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class SoundService: public Service, public ConsoleObject
{
/**
 * @note This is a list because we want to allow removal or insertion in the
 * list while we're iterating over it (which isn't allowed with a vector).
 */
typedef std::list<SoundInstance*> SoundInstanceStore;
typedef std::map<std::string, BaseSoundSample*> SoundSampleStore;

public:
	/**
	 * @brief Ctor.
	 */
	SoundService();

	/**
	 * @brief Dtor.
	 */
	virtual ~SoundService();

	/**
	 * @copydoc Service::start()
	 */
	Service::Status start();

	/**
	 * @copydoc Service::stop()
	 */
	void stop(int code);

	/**
	 * @copydoc ConsoleObject::runCommand()
	 */
	void runCommand(const std::string &command, const std::string &args);
	
	
	/**
	 * @brief Attempts to retrieve, or create if not already existing, the sound sample with the supplied identifier.
	 * Each sound sample is identified through the path to it, within the Ember resource system. This method will first look within the already allocated sound samples, and if the sought after sound sample is found there it will be returned.
	 * If not, it will try to create a new sound sample and return it. If no sound sample could be created (for example if no resource could be found) a null ref will be returned.
	 * @param soundPath The path to the sound data within the resource system.
	 * @return A sound sample, or null if none could be created.
	 */
	BaseSoundSample* createOrRetrieveSoundSample(const std::string& soundPath);
	
	/**
	 * @brief Destroys the specified sound sample.
	 *
	 * Call this to destroy a specified sound sample. If no sound sample
	 * with the specified path can be found nothing will happen.
	 *
	 * Normally you would never call this since all sound samples will be
	 * destroyed automatically when the service shuts down.
	 *
	 * \note Caution! If you destroy a sound sample that is in use by a
	 * SoundInstance you will probably get memory corruption and crashes as
	 * a result!
	 *
	 * @param soundPath The path to the sound data.
	 *
	 * @return True if the sound sample could be destroyed, false if it for
	 * some reason couldn't be destroyed, or if there was no such sound
	 * sample registered.
	 */
	bool destroySoundSample(const std::string& soundPath);

	/**
	 * Register individual StreamedSamples to keep updated on the cycle
	 * calls
	 *
	 * @param copy The StreamedSoundSample to be registered
	 */
	void registerStream(StreamedSoundSample* copy);

	/**
	 * Unregister streamed allocate sound buffers.
	 *
	 * This will only remove it from the service list it will not deallocate
	 * the data.
	 *
	 * @param sample A pointer to the sample to be unregistered
	 *
	 * @return The status of the unregistration.
	 */
	bool unregisterStream(const StreamedSoundSample* sample);

	/**
	 * @brief Update the position (in world coordinates) of the listener
	 *
	 * @param position The new listener position.
	 *
	 * @param direction The direction vector of the listener.
	 *
	 * @param up The up vector of the listener.
	 */
	void updateListenerPosition(const WFMath::Point<3>& pos, const WFMath::Vector<3>& direction, const WFMath::Vector<3> up);
	
	/**
	 * @brief Call this each frame to update the sound samples.
	 * Through a call of this all registered and active SoundInstance instances will be asked to update themselves. Such an update could involve updating streaming buffers in the case of a streaming sound, or update the position of the sound if it's positioned within the 3d world.
	 */
	void cycle();
	
	
	/**
	 * @brief Creates a new SoundInstance.
	 * Every time you want to play a sound you must create a SoundInstance and use that to play it. The only way to (normally) create such an instance is through this method. The sound service will keep track of all SoundInstance instances that are created, and will call SoundInstance::update() each frame, granted that SoundService::cycle() is called.
	 * Ownership of the SoundInstance is held by the sound service, and as soon as you're finished with it you should immediately return it to the sound service through destroyInstance(). Under normal operations it's expected that there will only be a few SoundInstances in play at once.
	 * @note If the sound system is disabled this will always return null, so make sure to check what you recieve when calling this.
	 * @return A new SoundInstance instance, or null if no instance could be created or the sound system is disabled. Before you can play it, through SoundInstance::play(), you must bind it to a SoundSample.
	 */
	SoundInstance* createInstance();
	
	/**
	 * @brief Destroys a SoundInstance.
	 * Once you're done with a sound instance, for example when the sound has completed, you are expected to return it to the sound service.
	 * This method will take care of releasing the resources allocated by the SoundInstance (either destroying them or returning them to a pool, depending on the implementation).
	 * @param instance The instance which shuold be destroyed. If null, nothing will happen.
	 * @return True if the instance could be properly destroyed, else false. The behaviour if the destruction fails is undefined, and you should probably throw some kind of exception, or just mark the SoundInstance as a memory/resource leak and carry on.
	 */
	bool destroyInstance(SoundInstance* instance);
	
	/**
	 * @brief Gets the resource provider for this service.
	 * @return The resource provider registered for this service, or null if none has been registered.
	 */
	IResourceProvider* getResourceProvider();

	/**
	 * @brief Sets the resource provider which should be used for this service.
	 * It's through the resource provider that all sound data is loaded, so this must be called in order to have a properly functioning service.
	 * @param resourceProvider A pointer to the resource provider to use. Ownership will not be transferred to this service.
	 */
	void setResourceProvider(Ember::IResourceProvider* resourceProvider);
	
	/**
	 * @brief Returns true if the sound system is enabled.
	 * @return True if the sound is enabled.
	 */
	bool isEnabled() const;

private:
	
	/**
	 * @brief All the samples registered with the service are stored here.
	 *
	 * These are owned by the service and should be destroyed when the
	 * service is stopped.
	 */
	SoundSampleStore mBaseSamples;

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
	/**
	 * @brief The main OpenAL context.
	 */
	ALCcontext* mContext;

	/**
	 * @brief The main OpenAL device.
	 */
	ALCdevice* mDevice;
	#endif
	
	/**
	 * @brief Stores all SoundInstances.
	 *
	 * These are owned by the service and should be destroyed when the
	 * service is stopped.
	 */
	SoundInstanceStore mInstances;

	/**
	 * @brief The resource provider used for loading resources.
	 *
	 * This is not owned by the service and won't be destroyed when the
	 * service shuts down.
	 */
	IResourceProvider* mResourceProvider;
	
	/**
	 * @brief True if the sound system is enabled.
	 * @see isEnabled()
	 */
	bool mEnabled;
}; //SoundService

} // namespace Ember

#endif
