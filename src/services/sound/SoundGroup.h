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

namespace Ember
{
	class SoundGroup
	{
		private:
			std::string mName;
			std::list<BaseSoundSample*> mSamples;

		public:
			SoundGroup(const std::string& name);
			~SoundGroup();

			// Activity
			void allocateBuffer(const std::string& filename, 
					bool playsReal, const SoundSampleType& type);

			void updateSamplesPosition(const WFMath::Point<3> &pos);
			void updateSamplesVelocity(const WFMath::Vector<3> &pos);
			void update();
	};
}

#endif

