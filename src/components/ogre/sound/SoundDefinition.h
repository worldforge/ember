/*
    Copyright (C) 2008 Romulo Fernandes Machado (nightz)
    Copyright (C) 2008 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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
#include "services/sound/SoundGeneral.h"
#include <string>

namespace EmberOgre 
{
/**
 * @brief Represent each buffer described inside specific sound groups.
 */
class SoundDefinition
{

public:
	/**
	* Initialize this SoundDefinition
	*
	* @param filename The filename (with path) of the sample.
	* @param type The Format of the sample (OGG/WAV/PCM)
	* @param playLocal Defines if the sound is 2D or 3D
	* @param volume The sample volume to be played
	*/
	void setup(const std::string& filename, Ember::SoundGeneral::SoundSampleType type, float volume);

	/**
	* Return filename
	*/
	const std::string& getFilename() const;

	/**
	* Return the format
	*/
	Ember::SoundGeneral::SoundSampleType getFormat() const;

	/**
	* Return the sound Volume
	*/
	float getVolume() const;

protected:
	
	/**
	* Filename WITH path
	*/
	std::string mFilename;

	/**
	* Defines the sound Format (WAV/OGG/PCM)
	*/
	Ember::SoundGeneral::SoundSampleType mSampleType;

	/**
	* The volume the sound is going to be played with
	* ranges from 0.0f to 1.0f
	*/
	float mVolume;

};


}

#endif

