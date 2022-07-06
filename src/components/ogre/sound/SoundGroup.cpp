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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SoundGroup.h"
#include "SoundDefinition.h"

#include "framework/LoggingInstance.h"


#include "services/EmberServices.h"
#include "services/sound/SoundService.h"
#include "services/sound/SoundInstance.h"
#include "services/sound/SoundSource.h"
#include "services/sound/SoundSample.h"

namespace Ember {
namespace OgreView {

SoundGroupBinding::SoundGroupBinding(SoundSource& source, SoundGroup& soundGroup)
		: SoundBinding(source), mSoundGroup(soundGroup) {
	auto& samples = mSoundGroup.getSamples();
	mBufferBindings.resize(samples.size());
	ALsizei i = 0;
	//get the buffers and bind the source to them
	for (auto& sample : samples) {
		auto sampleBuffers = sample->getBuffers();
		mBufferBindings[i] = *(sampleBuffers.begin());
		++i;
	}
	alSourceQueueBuffers(source.getALSource(), i, mBufferBindings.data());
// 	alSourcei(source.getALSource(), AL_BUFFER, sample.getBuffer());

}

SoundGroupBinding::~SoundGroupBinding() = default;

void SoundGroupBinding::update() {
	//TODO: add support for streaming sounds
}


SoundGroup::SoundGroup()
		: mIsPlaying(false) {
}

SoundGroup::~SoundGroup() = default;

// 	void SoundGroup::setFrequency(const unsigned int freq)
// 	{
// 		mFrequency = freq;
// 	}

void SoundGroup::setPlayOrder(const unsigned int playO) {
	mPlayOrder = playO;
// 		switch(mPlayOrder)
// 		{
// 			case PLAY_LINEAR:
// 				mNextToPlay = 0;
// 				break;
// 			case PLAY_INVERSE:
// 				mNextToPlay = mSamples.size() - 1; 
// 				break;
// 			case PLAY_RANDOM:
// 				getNextToPlay();
// 				break;
// 		};
}

// 	void SoundGroup::getNextToPlay()
// 	{
// 		if (!mSamples.size())
// 		{
// 			S_LOG_INFO("Group has no samples to play");
// 			return;
// 		}
// 
// 		switch(mPlayOrder)
// 		{
// 			case PLAY_LINEAR:
// 				mNextToPlay++; 
// 				break;
// 			case PLAY_INVERSE:
// 				mNextToPlay--; 
// 				break;
// 			case PLAY_RANDOM:
// 				srand(time(nullptr));
// 				mNextToPlay = rand() % mSamples.size();
// 				break;
// 		};
// 	}
// 
// 	void SoundGroup::resetClock()
// 	{
// 		gettimeofday(&start, nullptr);
// 	}
// 
// 	float SoundGroup::getTime()
// 	{
// 		struct timeval end;
// 		gettimeofday(&end, nullptr);
// 
// 		return (end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;
// 	}

// 	void SoundGroup::update()
// 	{
// 		if (getTime() >= 1000.0f/mFrequency)
// 		{
// 			play();
// 			resetClock();
// 		}
// 	}

void SoundGroup::addSound(const SoundDefinition& soundDef) {
	BaseSoundSample* soundSample = EmberServices::getSingleton().getSoundService().createOrRetrieveSoundSample(soundDef.mFilename);
	if (soundSample) {
		mSamples.push_back(soundSample);
	}
}

bool SoundGroup::bindToInstance(SoundInstance* instance) {
	auto binding = std::make_unique<SoundGroupBinding>(instance->getSource(), *this);
	instance->bind(std::move(binding));
	return true;
}


const std::list<BaseSoundSample*>& SoundGroup::getSamples() const {
	return mSamples;
}


}
}
