/*
 * Copyright (C) 2008 Romulo Fernandes Machado (nightz)
 * Copyright (C) 2008 Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * Copyright (C) 2009 by Manuel A. Fernandez Montecelo <manuel.montezelo@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 675 Mass
 * Ave, Cambridge, MA 02139, USA.
 */

#ifndef SOUND_GENERAL_H
#define SOUND_GENERAL_H

#include <string>


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
	 *
	 * If an error has occurred, it's logged and this method returns false.
	 *
	 * Make sure to call this after you've interacted with OpenAL, so the
	 * internal error state is reset before the next call to OpenAL
	 * (otherwise the errors will carry over).
	 *
	 * @param description When an error occurs, an entry will be written to
	 * the log; and this "description" will be appended, thus helping to
	 * identify each entry.
	 *
	 * @return False if an error has occurred, true otherwise.
	 */
	static bool checkAlError(const std::string& description = "");
};
}


#endif

