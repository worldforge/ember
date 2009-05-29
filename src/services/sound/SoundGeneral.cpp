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

#include "SoundGeneral.h"
#include "framework/LoggingInstance.h"

#ifdef __APPLE__
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif

#ifndef __WIN32__
#include <AL/alut.h>
#else
#include <ALUT/alut.h>
#endif


namespace Ember
{
	bool SoundGeneral::checkAlError(const std::string& description)
	{
		ALenum error = alGetError();
		if (error == AL_NO_ERROR) {
			return true;
		} else {
			if (description == "") {
				S_LOG_FAILURE("OpenAl error: " << alutGetErrorString(error));
			} else {
				S_LOG_FAILURE("OpenAl error: " << alutGetErrorString(error)
					      << "\nDescription: " << description);
			}
			return false;
		}
	}
}
