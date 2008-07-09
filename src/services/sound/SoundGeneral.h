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

#ifndef SOUND_GENERAL_H
#define SOUND_GENERAL_H

#include "framework/ConsoleObject.h"
#include "framework/ISoundProvider.h"

#include <map>
#include <AL/al.h>
#include <AL/alc.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#ifdef THREAD_SAFE
#include <pthread.h>
#endif

#ifndef __WIN32__
#include <AL/alut.h>
#else
#include <ALUT/alut.h>
#endif

#include <wfmath/point.h>
#include <wfmath/vector.h>
#include <wfmath/quaternion.h>
			 
#include <Atlas/Objects/ObjectsFwd.h>
#include <sigc++/trackable.h>
#include <sigc++/signal.h>

#define OGG_BUFFER_SIZE (4096 * 8)

namespace Ember 
{
	typedef enum
	{
		SAMPLE_NONE, // Try To guess by file extension
		SAMPLE_PCM,
		SAMPLE_WAV,
		SAMPLE_OGG
	} SoundSampleType;

	void checkAlError();
}

#define PLAY_LOCAL false
#define PLAY_WORLD true

#endif

