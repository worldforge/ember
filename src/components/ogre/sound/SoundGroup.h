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

#ifndef SOUND_GROUP_H
#define SOUND_GROUP_H

#include "services/sound/SoundBinding.h"
#include <list>

namespace Ember
{
class SoundInstance;
class SoundSource;
class BaseSoundSample;
}
namespace EmberOgre {

class SoundGroup;
class SoundDefinition;
	
/**
@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
@brief Provides sound binding functionality for a SoundGroup.
What makes this differs a little from normal sound binding is that the sound group is made up of many different sounds. If all sounds are static there's no problem, as the sound buffers then can be queued as they are. If however any sample is streaming it becomes a little more complex, since we then must update the buffers etc. as long as we're playing the streaming sample.
*/
class SoundGroupBinding
: public Ember::SoundBinding
{
public:
	/**
	 * @brief Ctor.
	 * @param source The sound source to which we should bind ourselves.
	 * @param soundGroup The soundgroup which we should bind to the source.
	 */
	SoundGroupBinding(Ember::SoundSource& source, SoundGroup& soundGroup);
	/**
	 * @brief Dtor.
	 */
	virtual ~SoundGroupBinding();
	
	/**
	 * @brief If we have any streaming sounds we should update the buffers accordingly here.
	 */
	virtual void update();
	
protected:
	/**
	 * @brief The sound group which contains the definitions used by this binding.
	 */
	SoundGroup& mSoundGroup;
};

/**
* The class SoundGroup is responsible to keep
* sound buffers together and play them in a determinated
* way (specified in the sounddefs)
*/
class SoundGroup
{
public: 
	enum PlayOrder
	{
		PLAY_LINEAR,
		PLAY_INVERSE,
		PLAY_RANDOM
	};
	typedef std::list<Ember::BaseSoundSample*> SampleStore;
	
	SoundGroup();
	~SoundGroup();

	/**
	* Allocate a sound sample/buffer in this group. 
	*
	* @param model A sound model containing definitions for the sample.
	*/
	void addSound(SoundDefinition* soundDef);

	/**
	* Set the group play order.
	*
	* @param playO The new play order, from playOrder structure.
	*/
	void setPlayOrder(const unsigned int playO);
	
	bool bindToInstance(Ember::SoundInstance* instance);
	
	const SampleStore& getSamples() const;
protected:
	/**
	* A list of the Samples allocated within the group
	*/
	SampleStore mSamples;

	/**
	* The frequency (Hz) in wich the sounds should be played.
	*/
	unsigned int mFrequency;

	/**
	* How the buffers should be played, specified by playOrder.
	*/
	unsigned int mPlayOrder;

	/**
	* If this Group is playing.
	*/
	bool mIsPlaying;

	/**
	* This is the group timer, used to verify
	* when buffers should be played (based on frequency).
	*/
// 	struct timeval start;

	/**
	* Reset internal group timer.
	*/
// 	void resetClock();

	/**
	* Returns the amount of time that passed since
	* the start of the second.
	*/
// 	float getTime();

};
}

#endif

