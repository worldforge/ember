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

#ifndef SOUND_GENERAL_H
#define SOUND_GENERAL_H

#include <string>


// #define OGG_BUFFER_SIZE (4096 * 8)

namespace Ember 
{
class SoundGeneral
{
public:
	enum SoundSampleType
	{
		SAMPLE_NONE, // Try To guess by file extension
		SAMPLE_PCM,
		SAMPLE_WAV,
		SAMPLE_OGG
	};

	/**
	 * @brief Checks for any errors in the OpenAL context.
	 * If an error has occurred, it's logged and this method returns false.
	 * Make sure to call this after you've interacted with OpenAL to make sure that the internal error state is reset in time for the next call to OpenAL. If not, the errors will carry over.
	 * @param  description If an error occurs, an entry will be written to the log. If you provide a description this will be added too, which might be good when looking through the log and figuring out what's wrong.
	 * @return False if an error has occurred, else true.
	 */
	static bool checkAlError(const std::string& description = "");
};
}


#endif

