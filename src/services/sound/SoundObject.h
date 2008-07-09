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

#ifndef SOUND_OBJECT_H
#define SOUND_OBJECT_H

#include "SoundGeneral.h"
#include "SoundSample.h"

namespace Ember
{

/**
 * Sound Object
 *
 * Objects contains samples that are played from 
 * their position with their velocity
 */
class SoundObject
{
	private:
		WFMath::Point<3> mPosition;
		WFMath::Vector<3> mVelocity;

		std::map<std::string, BaseSoundSample*> mSamples;

		// Allocation Functions
		bool allocateWAVPCM(const std::string &filename, const std::string &action, bool playsLocally); 
		bool allocateOGG(const std::string &filename, const std::string &action, bool playsLocally); 

	public:
		SoundObject();
		~SoundObject();

		// Set Variables
		void setPosition(const WFMath::Point<3> &pos);
		void setVelocity(const WFMath::Vector<3> &vel);

		// Get Variables
		const WFMath::Point<3> getPosition();
		const WFMath::Vector<3> getVelocity();

		// Register new Buffers
		bool registerSound(const std::string &filename, const std::string &action, bool playLocally,
				const SoundSampleType type = SAMPLE_NONE);

		bool unRegisterSound(const std::string &filename);

		// Common methods
		void playQueued();
		void playAction(const std::string &name);
};

} // namespace Ember

#endif

