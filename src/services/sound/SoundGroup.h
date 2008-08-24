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
#include "SoundModel.h"

namespace Ember
{
	enum playOrder
	{
		PLAY_LINEAR,
		PLAY_INVERSE,
		PLAY_RANDOM
	};

	/**
	 * The class SoundGroup is responsible to keep
	 * sound buffers together and play them in a determinated
	 * way (specified in the sounddefs)
	 */
	class SoundGroup
	{
		private:
			/**
			 * A list of the Samples allocated within the group
			 */
			std::list<BaseSoundSample*> mSamples;
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
			void createBuffer(SoundModel* model);

			/**
			 * Update the Sample Position, to better define sounds
			 * in space.
			 *
			 * @param pos The new sample position.
			 */
			void updateSamplesPosition(const WFMath::Point<3> &pos);

			/**
			 * Update the Sample Velocity, to better define sounds
			 * in space.
			 *
			 * @param vel The new sample velocity.
			 */
			void updateSamplesVelocity(const WFMath::Vector<3> &vel);

			/**
			 * Insert a new Sound Sample to group samples list.
			 *
			 * @param sample A pointer to the sound sample.
			 */
			void pushSample(BaseSoundSample* sample);

			/**
			 * Return the number of samples allocated
			 * on this SoundGroup
			 */
			unsigned int getBuffersCount();

			/**
			 * Returns a sound sample from a index.
			 * 
			 * @param index An index to buffer, in order from they were allocated
			 * @return A pointer to the sample, if it doesnt exists, returns NULL
			 */
			BaseSoundSample* getBuffer(unsigned int index);

			/**
			 * Set Group Frequency
			 *
			 * @param freq The play frequency in hertz
			 */
			void setFrequency(const unsigned int freq);

			/**
			 * Set the group play order.
			 *
			 * @param playO The new play order, from playOrder structure.
			 */
			void setPlayOrder(const unsigned int playO);

			/**
			 * Update the timers and buffers within this group
			 */
			void update();

			/**
			 * Play the next buffer defined on mNextToPlay.
			 */
			void play();

			/**
			 * Stop all playing buffers on this group
			 */
			void stop();
	};
}

#endif

