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

#include "SoundGeneral.h"
#include "SoundSample.h"
#include "SoundDefinition.h"

namespace Ember
{
class SoundGroup;
class SoundInstance;
	enum playOrder
	{
		PLAY_LINEAR,
		PLAY_INVERSE,
		PLAY_RANDOM
	};
	
class SoundGroupBinding
: public SoundBinding
{
public:
	SoundGroupBinding(SoundSource& source, SoundGroup& soundGroup);
	virtual ~SoundGroupBinding();
	
protected:
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
			typedef std::list<BaseSoundSample*> SampleStore;
		private:
			/**
			 * A list of the Samples allocated within the group
			 */
			SampleStore mSamples;
			BaseSoundSample* mLastPlayed;

			/**
			 * The frequency (Hz) in wich the sounds should be played.
			 */
			unsigned int mFrequency;

			/**
			 * How the buffers should be played, specified by playOrder.
			 */
			unsigned int mPlayOrder;

			/**
			 * This is an internal member to detect what is the next buffer
			 * to be played based on mPlayOrder
			 */
			unsigned int mNextToPlay;

			/**
			 * If this Group is playing.
			 */
			bool mIsPlaying;

			/**
			 * Based on mPlayOrder this function
			 * tells mNextToPlay the next buffer
			 * to be played.
			 */
			void getNextToPlay();

			/**
			 * This is the group timer, used to verify
			 * when buffers should be played (based on frequency).
			 */
			struct timeval start;

			/**
			 * Reset internal group timer.
			 */
			void resetClock();

			/**
			 * Returns the amount of time that passed since
			 * the start of the second.
			 */
			float getTime();

		public:
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
			
			bool bindToInstance(SoundInstance* instance);
			
			const SampleStore& getSamples() const;
	};
}

#endif

