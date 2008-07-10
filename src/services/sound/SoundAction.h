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

#ifndef SOUND_ACTION_H
#define SOUND_ACTION_H

#include "SoundGeneral.h"
#include "SoundService.h"

namespace Ember
{
	enum orderOfPlay
	{
		PLAY_LINEAR,
		PLAY_INVERSE,
		PLAY_RANDOM
	};

	/**
	 * Class SoundAction
	 *
	 * Each SoundEntity will have a set of sound actions
	 * attached to it. For each SoundAction we have a list
	 * of buffers available for it and how they play.
	 *
	 * @mName : The name of the action that triggers 
	 * @mTimeStep : The frequency (Hz) in wich sounds should be repeated (0 for no loop)
	 * @mActive : If the SoundAction is active (like idleing, walking...)
	 * @mPlayOrder : How the buffers should be played
	 * @mSamples : All the buffers belonging to this SoundAction. Take a note that those
	 * buffers will be allocated by the SoundService and deallocated by the same.
	 */
	class SoundAction
	{
		private:
			std::string mName;
			float mTimeStep;
			bool mActive;

			orderOfPlay mPlayOrder;
			int lastPlayedIndex;

			std::list<BaseSoundSample*> mSamples;

			void defineNextBuffer(); 

		public:
			SoundAction(const std::string& name, const orderOfPlay playorder, 
					const float timestep);

			~SoundAction();

			// Activity
			void setActivity(bool act);			
			bool isActive();

			void allocateBuffer(const std::string& filename, 
					bool playsReal, const SoundSampleType& type);

			void deallocateBuffer(const std::string& filename);

			void update(double timeSinceLastFrame);
	};
}

#endif

