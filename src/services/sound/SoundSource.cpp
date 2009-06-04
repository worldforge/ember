//
// C++ Implementation: SoundSource
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SoundSource.h"

#include "SoundGeneral.h"

#include "framework/LoggingInstance.h"
#include "framework/Exception.h"

namespace Ember {

SoundSource::SoundSource()
: mALSource(0)
{
	alGetError();
	// Bind the buffer with the source.
	alGenSources(1, &mALSource);
	if (!SoundGeneral::checkAlError("Creating new sound source."))
	{
		alDeleteSources(1, &mALSource);
		throw Exception("Failed to generate a new sound source.");
	}
	
	alSourcef(mALSource, AL_PITCH, 1.0f);
	SoundGeneral::checkAlError("Setting sound source pitch.");
	alSourcef(mALSource, AL_GAIN, 1.0f);
	SoundGeneral::checkAlError("Setting sound source gain.");
	alSource3f(mALSource, AL_POSITION, 0, 0, 0);
	SoundGeneral::checkAlError("Setting sound source position.");
	alSource3f(mALSource, AL_VELOCITY, 0, 0, 0);
	SoundGeneral::checkAlError("Setting sound source velocity.");
	alSourcei(mALSource, AL_LOOPING, true);
	SoundGeneral::checkAlError("Setting sound source looping.");

}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &mALSource);
	SoundGeneral::checkAlError("Deleting sound source.");
}

void SoundSource::setPosition(const WFMath::Point<3>& pos)
{
	alSource3f(mALSource, AL_POSITION, pos.x(), pos.y(), pos.z());
	SoundGeneral::checkAlError("Setting sound source position.");
}

void SoundSource::setVelocity(const WFMath::Vector<3>& vel)
{
	alSource3f(mALSource, AL_VELOCITY, vel.x(), vel.y(), vel.z());
	SoundGeneral::checkAlError("Setting sound source velocity.");
}

void SoundSource::setOrientation(const WFMath::Quaternion& orientation)
{
	///TODO: implement this
}


}
