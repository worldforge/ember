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

#ifndef SOUNDMODEL_H
#define SOUNDMODEL_H

#include "SoundGeneral.h"

namespace Ember 
{
	/**
	 * The SoundDefinition class represent each buffer described inside
	 * especific sound groups.
	 */
	class SoundDefinition
	{
		private:
			/**
			 * Filename WITH path
			 */
			std::string mFilename;

			/**
			 * Defines the sound Format (WAV/OGG/PCM)
			 */
			SoundSampleType mSampleType;

			/**
			 * If the sample plays in local (always audible) coordinates
			 * or in the world coordinates
			 */
			bool mPlaysLocally;

			/**
			 * The volume the sound is going to be played with
			 * ranges from 0.0f to 1.0f
			 */
			float mVolume;

		public:
			/**
			 * Initialize this SoundDefinition
			 *
			 * @param filename The filename (with path) of the sample.
			 * @param type The Format of the sample (OGG/WAV/PCM)
			 * @param playLocal Defines if the sound is 2D or 3D
			 * @param volume The sample volume to be played
			 */
			void setup(const std::string& filename, SoundSampleType type, 
					bool playLocal, float volume);

			/**
			 * Return filename
			 */
			const std::string& getFilename();

			/**
			 * Return the format
			 */
			SoundSampleType getFormat();

			/**
			 * Return if the sound is 2D/3D
			 */
			bool getPlayLocally();

			/**
			 * Return the sound Volume
			 */
			float getVolume();
	};

	/**
	 * The SoundGroupDefinition class describes a set
	 * of sound samples (SoundDefinitions)
	 */
	class SoundGroupDefinition
	{
		private:
			std::list<SoundDefinition*> mSamples;

		public:
			SoundGroupDefinition();
			
			/**
			 * Insert a sound sample into this group definition
			 */
			void insertSample(const std::string& name, SoundSampleType type, 
					bool playsLocally, float volume);

			/**
			 * Return an iterator to the first member of the list
			 */
			std::list<SoundDefinition*>::const_iterator getSamplesBegin();

			/**
			 * Return an iterator to the last member of the list
			 */
			std::list<SoundDefinition*>::const_iterator getSamplesEnd();
	};
}

#endif

