//
// C++ Interface: SoundInstance
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBERSOUNDINSTANCE_H
#define EMBERSOUNDINSTANCE_H

#include <memory>

namespace Ember {

class SoundSource;
class BaseSoundSample;
class SoundBinding;
class SoundService;

/**
@brief Provides motion updates each frame for a certain SoundInstance.

This interface allows a SoundInstance to get motion updates each frame. If an instance of this is attached to an SoundInstance, it will get queried each frame for motion updates. A typical implementation would be a class that provides updates from an ingame entity.

@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class ISoundMotionProvider
{
public:
	virtual ~ISoundMotionProvider() {}
	virtual void update(SoundSource& soundSource) = 0;
};

/**
@brief Represents one instance in the world of a sound being played at a certain location.
This is the basic class for all sounds that are played. Whenever another component in Ember wants a sound to be played, it should ask the SoundService for a new SoundInstance instance, and use that to play the sound. Once the sound has completed the instance should be destroyed.
The idea is that there shouldn't be that many sounds being played at any one momement, and thus not that many live instances of this class.

An instane of this encapsulates a SoundSource instance which is automatically created and destroyed together with the SoundInstance. The actual binding to sound data is however handled by an instance of SoundBinding. An instance of SoundBinding can normally be obtained either directly from a BaseSoundSample, or from a SoundGroup. After you've obtained a SoundBinding you are required to bind it to this class through a call to bind().

If you want to provide motion updates for the sound instance (such as with a sound eminating from within the 3d world) you need to register an instance of ISoundMotionProvider through setMotionProvider(). Note that this isn't required, for example with ambient or gui sounds.

@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class SoundInstance
{
friend class SoundService;
public:

	/**
	 * @brief Start to play the sound.
	 * If this is called for a sound that is already playing, it will restart at the beginning.
	 * @return True if we could successfully start playing the sound.
	 */
	bool play();
	
	/**
	 * @brief Stops playing the sound
	 * If called on an already stopped sound nothing will happen.
	 * @return True if we could successfully stop playing the sound.
	 */
	bool stop();
	
	/**
	 * @brief Pauses playing of the sound
	 * @return True if we could successfully pause playing the sound.
	 */
	bool pause();
	
	/**
	 * @brief Gets the sound source which this instance holds.
	 * Each SoundInstance is connected to one SoundSource (and as a result to one OpenAL source).
	 * @return The SoundSource which this instance holds.
	 */
	SoundSource& getSource();
	
	/**
	 * @brief Binds this sound instance to sound data.
	 * Before a sound can be played it must be bound to one or many sound data buffers. The SoundBinding class is responsible for this binding.
	 * In addition, every frame the SoundBinding instance will be asked to update itself. This is to handle those cases where the sound buffers are streaming, and should be filled with new data.
	 * Upon calling this ownership of the SoundBinding will be transferred to this class, and it will be deleted when this instance is deleted.
	 * @param binding The instance which binds the SoundSource held by this class to one or many sound data buffers.
	 */
	void bind(SoundBinding* binding);
	
	/**
	 * @brief Registers a motion provider for this sound instance.
	 * Some sounds, for example those that represents world entities, needs to have their position, velocity and orientation updated continously. Those sounds should therefore register an instance of ISoundMotionProvider through this method. The ISoundMotionProvider will be asked every frame for updates.
	 * Other sounds, such as ambient or gui ones, shouldn't need to have their motion updated and don't need to call this.
	 * The instance registered here won't be owned by this class, and it's therefore up to the calling party to make sure that it's properly destroyed after the SoundInstance has been destroyed and there's no more use for it.
	 * @param motionProvider A pointer to a sound motion provider, or null.
	 */
	inline void setMotionProvider(ISoundMotionProvider* motionProvider);
	
protected:
    /**
     * @brief Ctor. This is protected to allow only the SoundService to create instances.
     * An instance of SoundSource will automatically be created at construction.
     */
    SoundInstance();

    /**
     * @brief Dtor. This is protected to allow only the SoundService to delete instances.
     * The SoundSource instance held by this class will be automatically deleted.
     * Note that you don't need to call stop() before destroying the object.
     */
    ~SoundInstance();

	/**
	 * @brief This is called each frame by the SoundService.
	 * Through a call to this, which should happen each frame, both the ISoundMotionProvider and the SoundBinding instances attached to this class will be asked to update themselves.
	 */
	void update();
	
	/**
	 * @brief The sound source held by this class.
	 * Each instance of this class holds onto a SoundSource (and thus also an OpenAL source).
	 * It's created and destroyed together with the instance.
	 */
	std::auto_ptr<SoundSource> mSource;
	
	/**
	 * @brief The sound binding, which binds one or many sound data buffers to the sound source.
	 * The actual binding of sound data buffers aren't handled by this class, but by an instance of SoundBinding which is responsible for setting up the buffers and connecting them to the sound source.
	 * In addition, every frame the SoundBinding::update() method will be called through this class.
	 * The sound binding will be owned by this instance once it's been registered, and thus deleted when this instance is deleted.
	 * @see update()
	 */
	SoundBinding* mBinding;
	
	/**
	 * @brief An optional sound motion provider.
	 * Some sounds needs to have their motion in the 3d space continously updated. This instance will take care of that.
	 * Ownership won't be transferred to this class, and it won't be deleted when an instance of this is deleted.
	 */
	ISoundMotionProvider* mMotionProvider;

};

void SoundInstance::setMotionProvider(ISoundMotionProvider* motionProvider)
{
	mMotionProvider = motionProvider;
}

}

#endif
