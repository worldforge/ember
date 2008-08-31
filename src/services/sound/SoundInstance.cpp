//
// C++ Implementation: SoundInstance
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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

#include "SoundInstance.h"
#include "SoundSample.h"
#include "SoundSource.h"

namespace Ember {

SoundInstance::SoundInstance()
: mSource(new SoundSource()), mBinding(0), mMotionProvider(0)
{
}


SoundInstance::~SoundInstance()
{
	stop();
	delete mBinding;
}

void SoundInstance::bind(SoundBinding* binding)
{
	if (mBinding) {
		///TODO: handle calling this when there's already a binder
	}
	mBinding = binding;

}

SoundSource& SoundInstance::getSource()
{
	return *mSource;
}

bool SoundInstance::play()
{
	alGetError();
	alSourcePlay(mSource->getALSource());
	///TODO: add error handling
	return true;
}

bool SoundInstance::stop()
{
	alGetError();
	alSourceStop(mSource->getALSource());
	///TODO: add error handling
	return true;
}

bool SoundInstance::pause()
{
	alGetError();
	alSourcePause(mSource->getALSource());
	///TODO: add error handling
	return true;
}

void SoundInstance::update()
{
	if (mMotionProvider) {
		mMotionProvider->update(*mSource);
	}
	if (mBinding) {
		mBinding->update();
	}
}


}
