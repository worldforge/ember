/*
    Copyright (C) 2008 Romulo Fernandes Machado (nightz)
    Copyright (C) 2008 Erik Ogenvik <erik@ogenvik.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef SOUNDMODEL_H
#define SOUNDMODEL_H

#include "services/sound/SoundGeneral.h"
#include <string>

namespace Ember {
namespace OgreView {
/**
 * @brief Represent each buffer described inside specific sound groups.
 */
struct SoundDefinition {

	/**
	* Filename WITH path
	*/
	std::string mFilename;

	/**
	* Defines the sound Format (WAV/OGG/PCM)
	*/
	SoundGeneral::SoundSampleType mSampleType;

	/**
	* The volume the sound is going to be played with
	* ranges from 0.0f to 1.0f
	*/
	float mVolume;

};

}
}

#endif
